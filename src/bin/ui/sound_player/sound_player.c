/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "sound_player.h"

#define TONE_PLAYING_DURATION 2.0
#define UPDATE_FREQUENCY 1.0 / 30.0

static Sound_Data *snd = NULL;

#ifdef HAVE_AUDIO
static Eina_Bool playing = false;

static int offset, length;
static const void *data_sound;
static Ecore_Audio_Vio vio;
static Eo *in, *out;

static Ecore_Timer *timer;
static Evas_Object *rewin;
static Evas_Object *play;

static int
_snd_file_seek(void *data __UNUSED__, Eo *eo_obj __UNUSED__, int ofset, int whence)
{
   switch (whence)
     {
      case SEEK_SET:
         offset = ofset;
         break;
      case SEEK_CUR:
         offset += ofset;
         break;
      case SEEK_END:
         offset = length + ofset;
         break;
      default:
         break;
     }
   return offset;
}

static int
_snd_file_read(void *data __UNUSED__, Eo *eo_obj __UNUSED__, void *buffer, int len)
{
   if ((offset + len) > length)
     len = length - offset;
   memcpy(buffer, data_sound + offset, len);
   offset += len;
   return len;
}

static int
_snd_file_get_length(void *data __UNUSED__, Eo *eo_obj __UNUSED__)
{
   return length;
}

static int
_snd_file_tell(void *data __UNUSED__, Eo *eo_obj __UNUSED__)
{
   return offset;
}

static char *
_player_units_format(double val)
{
   char *units = mem_malloc(sizeof(char) * 16);
   int tmp = (int)val;
   snprintf(units, 16, "%02.0f:%02.0f", (double)(tmp / 60), (double)(tmp % 60));
   return units;
}

static void
_player_units_free(char *str)
{
   free(str);
}

#define BT_ADD(PARENT, OBJ, ICON, ICON_STYLE) \
   OBJ = elm_button_add(PARENT); \
   evas_object_size_hint_align_set(OBJ, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_style_set(OBJ, "anchor"); \
   ICON_STANDARD_ADD(OBJ, ICON, false, ICON_STYLE) \
   evas_object_show(OBJ); \
   elm_object_part_content_set(OBJ, NULL, ICON);

#define INFO_ADD(PARENT, ITEM, TEXT, STYLE) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_theme_set(ITEM, "layout", "sound_editor", STYLE); \
   elm_object_part_text_set(ITEM, "elm.text", TEXT); \
   evas_object_show(ITEM);

static Eina_Bool
_play_finished_cb(void *data __UNUSED__, const Eo_Event *event __UNUSED__)
{
   eo_unref(in);
   in = NULL;
   eo_unref(out);
   out = NULL;
   return true;
}

static Eina_Bool
_out_fail(void *data __UNUSED__, const Eo_Event *event)
{
   eo_unref(event->object);
   return true;
}

static Eina_Bool
_rewind_cb(void *data __UNUSED__)
{
   double value, max;

   value = elm_slider_value_get(rewin);
   elm_slider_min_max_get(rewin, NULL, &max);

   if (max == value)
     {
        Evas_Object *icon = elm_object_part_content_get(play, NULL);
        elm_icon_standard_set(icon, "media_player/play");
        playing = false;

        elm_slider_value_set(rewin, 0.0);
        return ECORE_CALLBACK_CANCEL;
     }

   elm_slider_value_set(rewin, value + UPDATE_FREQUENCY);
   return ECORE_CALLBACK_RENEW;
}

static void
_on_rewin_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   double value = elm_slider_value_get(rewin);
   value = ecore_audio_obj_in_seek(in, value, SEEK_SET);
}
static void
_create_io_stream()
{
   in = eo_add(ECORE_AUDIO_IN_SNDFILE_CLASS, NULL);
   assert(in != NULL);
   out = eo_add(ECORE_AUDIO_OUT_PULSE_CLASS, NULL);
   assert(out != NULL);
   eo_event_callback_add(out, ECORE_AUDIO_OUT_PULSE_EVENT_CONTEXT_FAIL,
                         _out_fail, NULL);
   eo_event_callback_add(in, ECORE_AUDIO_IN_EVENT_IN_STOPPED,
                         _play_finished_cb, NULL);
}
static void
_tone_play()
{
   double value;
   Eina_Bool ret = false;
   Tone_Resource *tone;

   tone = (Tone_Resource *)snd->resource;

   if (!in)
     {
        in = eo_add(ECORE_AUDIO_IN_TONE_CLASS, NULL);
        ecore_audio_obj_name_set(in, tone->name);
        eo_key_data_set(in, ECORE_AUDIO_ATTR_TONE_FREQ, &tone->freq);
        ecore_audio_obj_in_length_set(in, TONE_PLAYING_DURATION);
        eo_event_callback_add(in, ECORE_AUDIO_IN_EVENT_IN_STOPPED,
                              _play_finished_cb, NULL);
     }

   if (!out)
     out = eo_add(ECORE_AUDIO_OUT_PULSE_CLASS, NULL,
                           eo_event_callback_add(eo_self, ECORE_AUDIO_OUT_PULSE_EVENT_CONTEXT_FAIL,
                                                 _out_fail, NULL));

   ret = ecore_audio_obj_out_input_attach(out, in);
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        abort();
     }

   value = elm_slider_value_get(rewin);
   if (value)
     value = ecore_audio_obj_in_seek(in, value, SEEK_SET);

   timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, NULL);
}
static void
_sample_play()
{
   double value;
   Eina_Bool ret = false;
   External_Resource *sample;

   if (!in)
     {
        sample = (External_Resource *)snd->resource;
        _create_io_stream();
        ecore_audio_obj_name_set(in, sample->source);
        ret = ecore_audio_obj_source_set(in, sample->source);
        if (!ret)
          {
             ERR("Can not set source obj for added sample");
             return;
          }
     }

   ret = ecore_audio_obj_out_input_attach(out, in);
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        return;
     }

   value = elm_slider_value_get(rewin);
   if (value)
     value = ecore_audio_obj_in_seek(in, value, SEEK_SET);

   timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, NULL);
}
static void
_on_play_cb(void *data __UNUSED__,
           Evas_Object *obj,
           void *event_info __UNUSED__)
{
   Evas_Object *icon = elm_object_part_content_get(obj, NULL);
   Eina_Bool paused;

   if (!playing)
     {
        elm_icon_standard_set(icon, "media_player/pause");
        playing = true;

        if (in)
          {
             paused = ecore_audio_obj_paused_get(in);
             if (paused)
               {
                  ecore_audio_obj_paused_set(in, false);
                  ecore_timer_thaw(timer);
                  return;
               }
          }

        switch (snd->type)
          {
           case SOUND_TYPE_SAMPLE:
              _sample_play();
              break;
           case SOUND_TYPE_TONE:
              _tone_play();
              break;
          }
     }
   else
     {
        elm_icon_standard_set(icon, "media_player/play");
        playing = false;

        ecore_audio_obj_paused_set(in, true);
        ecore_timer_freeze(timer);
     }
}

static void
_interrupt_playing()
{
   Eina_Bool ret;
   Evas_Object *icon;

   if (!in) return; /* case when previous sound playing is finished */
   ret = ecore_audio_obj_paused_get(in);
   if (ret)
     {
        ret = ecore_audio_obj_out_input_detach(out, in);
        if (!ret) ERR("Could not detach input");

        ecore_timer_del(timer);
        timer = NULL;


        icon = elm_object_part_content_get(play, NULL);
        elm_icon_standard_set(icon, "media_player/play");
        playing = false;

        eo_unref(in);
        in = NULL;
        eo_unref(out);
        out = NULL;
     }
}
#endif

void
sound_player_sound_set(Sound_Data *sound)
{
#ifdef HAVE_AUDIO
   External_Resource *sample;
   Eina_Bool ret;
   double len = 0.0;
#endif

   snd = sound;

#ifdef HAVE_AUDIO
   if (!snd)
     {
         elm_object_disabled_set(play, true);
     }
   else if (snd->type == SOUND_TYPE_SAMPLE)
     {
         elm_object_disabled_set(play, false);
        _interrupt_playing();

        sample = (External_Resource *)snd->resource;
        _create_io_stream();
        ecore_audio_obj_name_set(in, sample->source);
        ret = ecore_audio_obj_source_set(in, sample->source);
        if (!ret) ERR("Can not set source '%s' to obj sample", sample->source)
          len = ecore_audio_obj_in_length_get(in);
        elm_slider_min_max_set(rewin, 0, len);
        elm_slider_value_set(rewin, 0.0);
        length = ecore_file_size(sample->source);
     }
   else if (snd->type == SOUND_TYPE_TONE)
     {
         elm_object_disabled_set(play, false);
        _interrupt_playing();

        elm_slider_min_max_set(rewin, 0.0, TONE_PLAYING_DURATION);
        elm_slider_value_set(rewin, 0.0);
     }
#endif
}

Evas_Object *
sound_player_add(Evas_Object *parent)
{
   Evas_Object *control;
#ifdef HAVE_AUDIO
   Evas_Object *icon;
#endif

   assert(parent != NULL);

   control = elm_layout_add(parent);
   elm_layout_theme_set(control, "layout", "sound_editor", "player");
   evas_object_size_hint_weight_set(control,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(control, EVAS_HINT_FILL, EVAS_HINT_FILL);

#ifdef HAVE_AUDIO
   rewin = elm_slider_add(control);
   elm_slider_unit_format_set(rewin, "%2.0f");
   elm_slider_units_format_function_set(rewin, _player_units_format,
                                        _player_units_free);
   evas_object_size_hint_align_set(rewin, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(rewin, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(rewin);
   elm_object_part_content_set(control, "eflete.swallow.fast", rewin);
   evas_object_smart_callback_add(rewin, "changed", _on_rewin_cb, NULL);

   BT_ADD(control, play, icon, "media_player/play");
   elm_object_part_content_set(rewin, NULL, play);
   evas_object_smart_callback_add(play, "clicked", _on_play_cb, NULL);
   elm_object_disabled_set(play, true);

   vio.get_length = _snd_file_get_length;
   vio.seek = _snd_file_seek;
   vio.read = _snd_file_read;
   vio.tell = _snd_file_tell;
#endif

   return control;
}
