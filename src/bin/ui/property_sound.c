/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "string_common.h"
#include "new_history.h"
#include "editor.h"
#include "sound_editor.h"
#include "validator.h"

#include "signals.h"

#define UPDATE_FREQUENCY 1.0 / 30.0
#define TONE_PLAYING_DURATION 2.0
#define SND_EDIT_KEY "sound_editor_key"
#define SOUND_PROP_DATA "sound_prop_data"

#define SOUND_PROP_DATA_GET() \
   assert(property != NULL); \
   Sound_Prop_Data *pd = evas_object_data_get(property, SOUND_PROP_DATA); \
   assert(pd != NULL);

/*
 * Callback is added for frames at property box to correct scroller
 * work while each frame would be expanded/collapsed
 */
#define FRAME_PROPERTY_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

struct _Sound_Prop_Data
{
   Evas_Object *markup;
   Evas_Object *gengrid;

   Evas_Object *box;
   Evas_Object *sound_player;
   Evas_Object *preview_sound_player;
   Evas_Object *sound_player_frame;

   Evas_Object *sample_box;
   Evas_Object *tone_box;
   Evas_Object *info_frame;

   Elm_Object_Item *tone;
   const char  *selected;
   const char  *snd_src;

#ifdef HAVE_AUDIO
   struct {
      int offset, length;
      const void *data;
      Ecore_Audio_Vio vio;
      Eo *in, *out;
      Eina_Binbuf *buf;
   } io;
#endif

   struct {
      Ecore_Timer *timer;
      Evas_Object *check;
      Evas_Object *rewind;
      Evas_Object *play;
      Evas_Object *pause;
      Eina_Bool decoration : 1;
      Eina_Bool playing : 1;
      Eina_Bool stopped : 1;
      Eina_Bool switched : 1;
      Eina_Bool added : 1;
   } player_data;

   struct {
      Evas_Object *teg;
      Evas_Object *tone_name;
      Evas_Object *tone_frq;
      Evas_Object *tone_duration;
      Evas_Object *file_name;
      Evas_Object *duration;
      Evas_Object *type;
      Evas_Object *size;
      Evas_Object *compression_type;
      Evas_Object *compression_quality;
   } snd_data;
};
typedef struct _Sound_Prop_Data Sound_Prop_Data;

static Elm_Gengrid_Item_Class *gic = NULL, *ggic = NULL;

/* accroding to Edje_Edit.h */
static const char *edje_sound_compression[] = { N_("RAW"),
                                                N_("COMP"),
                                                N_("LOSSY"),
                                                N_("AS_IS"),
                                                NULL};

static Evas_Object *
prop_sound_editor_compression_type_add(Evas_Object *property, Sound_Prop_Data *pd);

static void
_interrupt_playing(Sound_Prop_Data *edit);

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   assert(parent != NULL);
   assert(label != NULL);

   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

static Evas_Object *
prop_sound_editor_compression_quality_add(Evas_Object *parent,
                                          Sound_Prop_Data *pd)
{
   PROPERTY_ITEM_ADD(parent, _("compression quality/rate"), "2swallow")
   SPINNER_ADD(item, pd->snd_data.compression_quality, 0, 100, 1, false)
   elm_layout_content_set(item, "swallow.content1", pd->snd_data.compression_quality);
   elm_object_disabled_set(pd->snd_data.compression_quality, true);
   elm_spinner_value_set(pd->snd_data.compression_quality, 0);
   return item;
}

static Evas_Object *
prop_sound_editor_tone_frequency_add(Evas_Object *parent,
                                     Sound_Prop_Data *pd)
{
   PROPERTY_ITEM_ADD(parent, _("tone frequency"), "2swallow")
   SPINNER_ADD(item, pd->snd_data.tone_frq, 200, 20000, 10, false)
   elm_layout_content_set(item, "swallow.content1", pd->snd_data.tone_frq);
   elm_object_disabled_set(pd->snd_data.tone_frq, true);
   elm_spinner_value_set(pd->snd_data.tone_frq, 200);
   return item;
}

static void
_on_sound_compression_type_change(void *data __UNUSED__,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info __UNUSED__)
{
}

static Evas_Object *
prop_sound_editor_compression_type_add(Evas_Object *parent, Sound_Prop_Data *pd)
{
   int i;
   PROPERTY_ITEM_ADD(parent, _("compression type"), "1swallow");
   EWE_COMBOBOX_ADD(parent, pd->snd_data.compression_type);
   /* disable for now */
   elm_object_disabled_set(pd->snd_data.compression_type, true);
   for (i = 0; edje_sound_compression[i]; ewe_combobox_item_add(pd->snd_data.compression_type, edje_sound_compression[i]), i++);

   evas_object_smart_callback_add(pd->snd_data.compression_type, "selected", _on_sound_compression_type_change, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->snd_data.compression_type);

   return item;
}

static void
_sound_editor_del(Sound_Prop_Data *edit)
{
   assert(edit != NULL);

#ifdef HAVE_AUDIO
   ecore_audio_shutdown();
#endif
   elm_gengrid_item_class_free(gic);
   elm_gengrid_item_class_free(ggic);
   evas_object_data_del(edit->markup, SND_EDIT_KEY);
   eina_stringshare_del(edit->selected);
   free(edit);
}

static void
_sound_editor_quit(Sound_Prop_Data *edit)
{
#ifdef HAVE_AUDIO
   if (edit->player_data.playing)
     _interrupt_playing(edit);
   else if (edit->io.in)
     {
        eo_del(edit->io.in);
        eo_del(edit->io.out);
        eina_binbuf_free(edit->io.buf);
     }
#endif
   _sound_editor_del(edit);
}

static void
_on_sound_editor_del(void * data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   assert(edit != NULL);

   _sound_editor_quit(edit);
}

#ifdef HAVE_AUDIO
static int
_snd_file_seek(void *data, Eo *eo_obj EINA_UNUSED, int offset, int whence)
{
   Sound_Prop_Data *vf = data;

   assert(vf != NULL);

   switch (whence)
     {
      case SEEK_SET:
         vf->io.offset = offset;
         break;
      case SEEK_CUR:
         vf->io.offset += offset;
         break;
      case SEEK_END:
         vf->io.offset = vf->io.length + offset;
         break;
      default:
         break;
     }
   return vf->io.offset;
}

static int
_snd_file_read(void *data, Eo *eo_obj EINA_UNUSED, void *buffer, int len)
{
   Sound_Prop_Data *vf = data;

   assert(vf != NULL);

   if ((vf->io.offset + len) > vf->io.length)
     len = vf->io.length - vf->io.offset;
   memcpy(buffer, vf->io.data + vf->io.offset, len);
   vf->io.offset += len;
   return len;
}

static int
_snd_file_get_length(void *data, Eo *eo_obj EINA_UNUSED)
{
   Sound_Prop_Data *vf = data;

   assert(vf != NULL);

   return vf->io.length;
}

static int
_snd_file_tell(void *data, Eo *eo_obj EINA_UNUSED)
{
   Sound_Prop_Data *vf = data;

   assert(vf != NULL);

   return vf->io.offset;
}
#endif

static Eina_Bool
_play_finished_cb(void *data,
                  Eo *in EINA_UNUSED,
                  const Eo_Event_Description *desc EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   assert(edit != NULL);

   eo_del(edit->io.in);
   edit->io.in = NULL;
   edit->player_data.playing = false;
   ecore_main_loop_quit();
   return true;
}

static Eina_Bool
_out_fail(void *data EINA_UNUSED,
          Eo *output,
          const Eo_Event_Description *desc EINA_UNUSED,
          void *event_info EINA_UNUSED)
{
   eo_del(output);
   ecore_main_loop_quit();
   return true;
}

static Eina_Bool
_rewind_cb(void *data)
{
   double value, max;
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   value = elm_slider_value_get(edit->player_data.rewind);
   elm_slider_min_max_get(edit->player_data.rewind, NULL, &max);

   if (max == value)
     {
        printf("3\n");
        elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
        evas_object_hide(edit->player_data.pause);
        elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
        evas_object_show(edit->player_data.play);
        elm_slider_value_set(edit->player_data.rewind, 0.0);
        return ECORE_CALLBACK_CANCEL;
     }

   elm_slider_value_set(edit->player_data.rewind, value + UPDATE_FREQUENCY);
   return ECORE_CALLBACK_RENEW;
}

static void
_create_io_stream(Sound_Prop_Data *edit)
{
   edit->io.in = eo_add(ECORE_AUDIO_IN_SNDFILE_CLASS, NULL);

   assert(edit->io.in != NULL);

   edit->io.out = eo_add(ECORE_AUDIO_OUT_PULSE_CLASS, NULL);

   assert(edit->io.out != NULL);

   eo_do(edit->io.out, eo_event_callback_add(ECORE_AUDIO_OUT_PULSE_EVENT_CONTEXT_FAIL,
                                             _out_fail, NULL));

   eo_do(edit->io.in, eo_event_callback_add(ECORE_AUDIO_IN_EVENT_IN_STOPPED,
                                            _play_finished_cb, edit));
}

static void
_initialize_io_data(Sound_Prop_Data *edit)
{
   edit->io.offset = 0;

   assert(edit != NULL);

   _create_io_stream(edit);

   eo_do(edit->io.in, ecore_audio_obj_name_set(edit->selected));
   eo_do(edit->io.in, ecore_audio_obj_vio_set(&edit->io.vio, edit, NULL));
}

static void
_tone_play(Sound_Prop_Data *edit, int tone_frq)
{
   double value;
   Eina_Bool ret = false;

   assert(edit != NULL);

   elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
   evas_object_hide(edit->player_data.play);
   elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.pause);
   evas_object_show(edit->player_data.pause);

   if (edit->player_data.stopped)
     {
        eo_do(edit->io.in, ecore_audio_obj_paused_set(false));
        ecore_timer_thaw(edit->player_data.timer);
        edit->player_data.stopped = false;
        return;
     }

   if (!edit->io.in)
     {
        edit->io.in = eo_add(ECORE_AUDIO_IN_TONE_CLASS, NULL);
        eo_do(edit->io.in, ecore_audio_obj_name_set(edit->selected));
        eo_do(edit->io.in, eo_key_data_set(ECORE_AUDIO_ATTR_TONE_FREQ, &tone_frq));
        eo_do(edit->io.in, ecore_audio_obj_in_length_set(TONE_PLAYING_DURATION));
        eo_do(edit->io.in, eo_event_callback_add(ECORE_AUDIO_IN_EVENT_IN_STOPPED,
                                                 _play_finished_cb, edit));
     }

   if (!edit->io.out)
     edit->io.out = eo_add(ECORE_AUDIO_OUT_PULSE_CLASS, NULL,
                           eo_event_callback_add(ECORE_AUDIO_OUT_PULSE_EVENT_CONTEXT_FAIL,
                                                 _out_fail, NULL));

   eo_do(edit->io.out, ret = ecore_audio_obj_out_input_attach(edit->io.in));
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        abort();
     }

   value = elm_slider_value_get(edit->player_data.rewind);
   if (value)
     eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));

   edit->player_data.playing = true;
   edit->player_data.timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, edit);
   ecore_main_loop_begin();
}

static void
_add_sound_play(Sound_Prop_Data *edit)
{
   double value;
   Eina_Bool ret = false;

   assert(edit != NULL);

   elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
   evas_object_hide(edit->player_data.play);
   elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.pause);
   evas_object_show(edit->player_data.pause);

   if (edit->player_data.stopped)
     {
        eo_do(edit->io.in, ecore_audio_obj_paused_set(false));
        ecore_timer_thaw(edit->player_data.timer);
        edit->player_data.stopped = false;
        return;
     }

   if (!edit->io.in)
     {
        _create_io_stream(edit);
        eo_do(edit->io.in, ecore_audio_obj_name_set(edit->snd_src));
        eo_do(edit->io.in, ret = ecore_audio_obj_source_set(edit->snd_src));
        if (!ret)
          {
             ERR("Can not set source obj for added sample");
             elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
             evas_object_hide(edit->player_data.pause);
             elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
             evas_object_show(edit->player_data.play);
             return;
          }
     }

   eo_do(edit->io.out, ret = ecore_audio_obj_out_input_attach(edit->io.in));
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
        evas_object_hide(edit->player_data.pause);
        elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
        evas_object_show(edit->player_data.play);
        return;
     }

   value = elm_slider_value_get(edit->player_data.rewind);
   if (value)
     eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));

   edit->player_data.playing = true;
   edit->player_data.timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, edit);
   ecore_main_loop_begin();
}

static void
_play_sound(Sound_Prop_Data *edit)
{
   double value;
   Eina_Bool ret = false;
   Elm_Object_Item *g_item;
   const Item *item;

   assert(edit != NULL);

   g_item = elm_gengrid_selected_item_get(edit->gengrid);
   if (!g_item)
     return;

   if (edit->player_data.added)
     {
        _add_sound_play(edit);
        return;
     }

   item = elm_object_item_data_get(g_item);
   if (item->tone_frq)
     {
        _tone_play(edit, item->tone_frq);
        return;
     }

   elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
   evas_object_hide(edit->player_data.play);
   elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.pause);
   evas_object_show(edit->player_data.pause);

   if (edit->player_data.stopped)
     {
        eo_do(edit->io.in, ecore_audio_obj_paused_set(false));
        ecore_timer_thaw(edit->player_data.timer);
        edit->player_data.stopped = false;
        return;
     }

   if (!edit->io.in)
     _initialize_io_data(edit);

   eo_do(edit->io.out, ret = ecore_audio_obj_out_input_attach(edit->io.in));
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
        evas_object_hide(edit->player_data.pause);
        elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
        evas_object_show(edit->player_data.play);
        return;
     }
   edit->player_data.playing = true;

   value = elm_slider_value_get(edit->player_data.rewind);
   if (value)
     eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));

   edit->player_data.timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, edit);
   ecore_main_loop_begin();
}

static void
_interrupt_playing(Sound_Prop_Data *edit)
{
   assert(edit != NULL);

   Eina_Bool ret;
   if (edit->player_data.playing)
     {
        eo_do(edit->io.out, ret = ecore_audio_obj_out_input_detach(edit->io.in));
        if (!ret)
          ERR("Could not detach input");
        if (!edit->player_data.stopped)
          {
             ecore_timer_del(edit->player_data.timer);
             edit->player_data.timer = NULL;
          }
        ecore_main_loop_quit();
        elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
        evas_object_hide(edit->player_data.pause);
        elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
        evas_object_show(edit->player_data.play);
     }
   if (edit->io.in)
     {
        eo_del(edit->io.in);
        edit->io.in = NULL;
        eo_del(edit->io.out);
        edit->io.out = NULL;
     }
   if (edit->io.buf)
     {
        eina_binbuf_free(edit->io.buf);
        edit->io.buf = NULL;
     }

   edit->player_data.stopped = false;
   edit->player_data.playing = false;
   edit->player_data.added = false;
}

static void
_on_play_cb(void *data,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   _play_sound(data);
}

static void
_on_prev_cb(void *data,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   Elm_Object_Item *it, *first, *last;
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   assert(edit != NULL);

   edit->player_data.switched = true;
   first = elm_gengrid_first_item_get(edit->gengrid);
   last = elm_gengrid_last_item_get(edit->gengrid);
   it = elm_gengrid_selected_item_get(edit->gengrid);
   it = elm_gengrid_item_prev_get(it);

   if (it == first)
     {
        it = last;
        if (last == edit->tone)
          it = elm_gengrid_item_prev_get(edit->tone);
     }
   else if (it == edit->tone)
     it = elm_gengrid_item_prev_get(edit->tone);

   elm_gengrid_item_selected_set(it, true);
}

static void
_on_next_cb(void *data EINA_UNUSED,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   Elm_Object_Item *it, *first, *last;
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   assert(edit != NULL);

   edit->player_data.switched = true;
   first = elm_gengrid_first_item_get(edit->gengrid);
   last = elm_gengrid_last_item_get(edit->gengrid);
   it = elm_gengrid_selected_item_get(edit->gengrid);
   it = elm_gengrid_item_next_get(it);

   if (!it)
     it = elm_gengrid_item_next_get(first);
   else if (it == edit->tone)
     {
        if (edit->tone == last)
          it = elm_gengrid_item_next_get(first);
        else
          it = elm_gengrid_item_next_get(edit->tone);
     }

   elm_gengrid_item_selected_set(it, true);
}

static void
_on_pause_cb(void *data EINA_UNUSED,
             Evas_Object *obj EINA_UNUSED,
             void *event_info EINA_UNUSED)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   assert(edit != NULL);

   edit->player_data.stopped = true;
   eo_do(edit->io.in, ecore_audio_obj_paused_set(true));

   ecore_timer_freeze(edit->player_data.timer);
   elm_object_part_content_unset(edit->sound_player, "swallow.button.play");
   evas_object_hide(edit->player_data.pause);
   elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
   evas_object_show(edit->player_data.play);
}

static void
_on_rewind_cb(void *data,
              Evas_Object *obj EINA_UNUSED,
              void *event_info EINA_UNUSED)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;

   assert(edit != NULL);

   double value = elm_slider_value_get(edit->player_data.rewind);
   eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));
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

static void
_sound_player_create(Evas_Object *parent, Sound_Prop_Data *edit)
{
   Evas_Object *bt, *icon, *item;

   assert(parent != NULL);
   assert(edit != NULL);

   edit->preview_sound_player = elm_layout_add(parent);
   elm_layout_theme_set(edit->preview_sound_player, "layout", "sound_editor", "preview");
   evas_object_size_hint_weight_set(edit->preview_sound_player, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(edit->preview_sound_player, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(edit->preview_sound_player);
   elm_object_content_set(edit->sound_player_frame, edit->preview_sound_player);
   elm_box_pack_end(edit->box, edit->sound_player_frame);

   edit->sound_player = elm_layout_add(edit->preview_sound_player);
   elm_layout_theme_set(edit->sound_player, "layout", "sound_editor", "player");
   evas_object_size_hint_weight_set(edit->sound_player,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(edit->sound_player, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(edit->sound_player);
   elm_object_part_content_set(edit->preview_sound_player, "eflete.swallow.sound_player", edit->sound_player);

   edit->snd_data.teg = elm_icon_add(edit->sound_player);
   evas_object_image_smooth_scale_set(edit->snd_data.teg, false);
   evas_object_show(edit->snd_data.teg);
   evas_object_size_hint_fill_set(edit->snd_data.teg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(edit->snd_data.teg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(edit->sound_player, "eflete.swallow.teg",
                               edit->snd_data.teg);

   INFO_ADD(parent, item, _("Play on select:"), "item");
   CHECK_ADD(item, edit->player_data.check);
   elm_object_part_content_set(item, "swallow.second", edit->player_data.check);
   elm_object_part_content_set(edit->sound_player, "eflete.swallow.check", item);

   edit->player_data.rewind = elm_slider_add(edit->sound_player);
   elm_slider_unit_format_set(edit->player_data.rewind, "%2.0f");
   elm_slider_units_format_function_set(edit->player_data.rewind, _player_units_format,
                                        _player_units_free);
   evas_object_size_hint_align_set(edit->player_data.rewind, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(edit->player_data.rewind, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->player_data.rewind);
   elm_object_part_content_set(edit->sound_player, "eflete.swallow.fast", edit->player_data.rewind);
   evas_object_smart_callback_add(edit->player_data.rewind, "changed", _on_rewind_cb, edit);

   BT_ADD(edit->sound_player, bt, icon, "media_player/prev");
   elm_object_part_content_set(edit->sound_player, "swallow.button.prev", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_prev_cb, edit);

   BT_ADD(edit->sound_player, edit->player_data.play, icon, "media_player/play");
   elm_object_part_content_set(edit->sound_player, "swallow.button.play", edit->player_data.play);
   evas_object_smart_callback_add(edit->player_data.play, "clicked", _on_play_cb, edit);

   BT_ADD(edit->sound_player, bt, icon, "media_player/next");
   elm_object_part_content_set(edit->sound_player, "swallow.button.next", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_next_cb, edit);

   edit->player_data.pause = elm_button_add(edit->sound_player);
   elm_object_style_set(edit->player_data.pause, "anchor");
   evas_object_size_hint_align_set(edit->player_data.pause, EVAS_HINT_FILL, EVAS_HINT_FILL);
   ICON_STANDARD_ADD(edit->player_data.pause, icon, false, "media_player/pause")
   elm_object_part_content_set(edit->player_data.pause, NULL, icon);
   evas_object_smart_callback_add(edit->player_data.pause, "clicked", _on_pause_cb, edit);

   edit->io.vio.get_length = _snd_file_get_length;
   edit->io.vio.seek = _snd_file_seek;
   edit->io.vio.read = _snd_file_read;
   edit->io.vio.tell = _snd_file_tell;
}

static void
_sample_info_update(Sound_Prop_Data *pd, Selected_Sound_Data *snd_data)
{
   Evas_Object *item;
   Eina_Stringshare *duration, *type;

   evas_object_show(pd->sample_box);
   evas_object_hide(pd->tone_box);

   elm_object_content_unset(pd->info_frame);
   elm_object_content_set(pd->info_frame, pd->sample_box);

   duration = eina_stringshare_printf("%.2f s", snd_data->duration);
   type = eina_stringshare_printf(_("%s Format Sound (.%s)"), snd_data->format, snd_data->snd_src);

   item = elm_object_part_content_get(pd->snd_data.file_name, "elm.swallow.content");
   elm_object_text_set(item, snd_data->file_name);

   item = elm_object_part_content_get(pd->snd_data.duration, "elm.swallow.content");
   elm_object_text_set(item, duration);

   item = elm_object_part_content_get(pd->snd_data.type, "elm.swallow.content");
   elm_object_text_set(item, type);

   Eina_Stringshare *size = eina_stringshare_printf("%.2f KB", snd_data->length / 1024.0);
   item = elm_object_part_content_get(pd->snd_data.size, "elm.swallow.content");
   elm_object_text_set(item, size);

   eina_stringshare_del(duration);
   eina_stringshare_del(type);
}

static void
_tone_info_update(Sound_Prop_Data *pd, Selected_Sound_Data *snd_data)
{
   Evas_Object *item;
   Eina_Stringshare *duration;

   evas_object_hide(pd->sample_box);
   evas_object_show(pd->tone_box);

   elm_object_content_unset(pd->info_frame);
   elm_object_content_set(pd->info_frame, pd->tone_box);

   duration = eina_stringshare_printf("%.1f s", snd_data->duration);

   item = elm_object_part_content_get(pd->snd_data.tone_name, "elm.swallow.content");
   elm_object_text_set(item, snd_data->file_name);

   item = elm_object_part_content_get(pd->snd_data.duration, "swallow.content1");
   elm_object_text_set(item, duration);

   eina_stringshare_del(duration);
}

#ifdef HAVE_AUDIO

static void
_added_sample_src_info_setup(Sound_Prop_Data *edit,
                             double *len)
{
   Eina_Bool ret;

   assert(edit != NULL);

   _create_io_stream(edit);
   eo_do(edit->io.in, ecore_audio_obj_name_set(edit->snd_src));
   eo_do(edit->io.in, ret = ecore_audio_obj_source_set(edit->snd_src));
   if (!ret)
     ERR("Can not set source obj for added sample");
   eo_do(edit->io.in, *len = ecore_audio_obj_in_length_get());
   elm_slider_min_max_set(edit->player_data.rewind, 0, *len);
   elm_slider_value_set(edit->player_data.rewind, 0.0);
   edit->io.length = ecore_file_size(edit->snd_src);
}
#endif

static void
_grid_sample_selected(void *data,
                              Evas_Object *obj __UNUSED__,
                              void *event_info)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;
   Selected_Sound_Data *snd_data = (Selected_Sound_Data *)event_info;

   edit->markup = snd_data->markup;
   edit->gengrid = snd_data->gengrid;

   double len = 0.0;
   const Item *item = NULL;
   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);
#ifdef HAVE_AUDIO
   Eina_Bool auto_play = elm_check_state_get(edit->player_data.check);
#endif

   if (!edit->player_data.decoration)
     {
        elm_layout_signal_emit(edit->markup, "eflete,sound,clicked", "eflete");
        edit->player_data.decoration = true;
     }

#ifdef HAVE_AUDIO
   _interrupt_playing(edit);
#endif

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        edit->selected = eina_stringshare_add(item->sound_name);

        edit->snd_src = item->src;
#ifdef HAVE_AUDIO
        _added_sample_src_info_setup(edit, &len);
#endif
        snd_data->file_name = item->sound_name;
        snd_data->duration = len;
        snd_data->format = item->format;
        snd_data->compression_type = item->comp;
        snd_data->quality = item->rate;

        _sample_info_update(edit, snd_data);
        edit->player_data.added = true;
#ifdef HAVE_AUDIO
        if ((edit->player_data.switched) || (auto_play))
          {
             edit->player_data.switched = false;
             _add_sound_play(edit);
          }
#endif
     }
}

static void
_grid_tone_selected(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;
   Selected_Sound_Data *snd_data = (Selected_Sound_Data *)event_info;
   const Item *item;

   assert(edit != NULL);

   edit->markup = snd_data->markup;
   edit->gengrid = snd_data->gengrid;
   edit->tone = snd_data->tone;

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);

   if (!edit->player_data.decoration)
     {
        elm_layout_signal_emit(edit->markup, "eflete,sound,clicked", "eflete");
        edit->player_data.decoration = true;
     }

#ifdef HAVE_AUDIO
   _interrupt_playing(edit);
#endif

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        edit->selected = eina_stringshare_add(item->sound_name);

        elm_slider_min_max_set(edit->player_data.rewind, 0.0, TONE_PLAYING_DURATION);
        elm_slider_value_set(edit->player_data.rewind, 0.0);

        snd_data->file_name = item->sound_name;

        _tone_info_update(edit, snd_data);
#ifdef HAVE_AUDIO
        if (edit->player_data.switched)
          {
             edit->player_data.switched = false;
             _tone_play(edit, item->tone_frq);
             return;
          }
        if (elm_check_state_get(edit->player_data.check))
          _tone_play(edit, item->tone_frq);
#endif
     }
}

static void
_sample_info_create(Evas_Object *parent, Sound_Prop_Data *edit)
{
   Evas_Object *item;

   assert(parent != NULL);
   assert(edit != NULL);

   BOX_ADD(edit->info_frame, edit->sample_box, false, false);
   elm_box_align_set(edit->sample_box, 0.5, 0.0);

   edit->snd_data.file_name = prop_item_label_add(edit->sample_box, &edit->snd_data.file_name, _("file name:"), _(" - "));
   elm_box_pack_end(edit->sample_box, edit->snd_data.file_name);
   edit->snd_data.duration = prop_item_label_add(edit->sample_box, &edit->snd_data.duration, _("duration:"), _(" - "));
   elm_box_pack_end(edit->sample_box, edit->snd_data.duration);
   edit->snd_data.type = prop_item_label_add(edit->sample_box, &edit->snd_data.type, _("type:"), _(" - "));
   elm_box_pack_end(edit->sample_box, edit->snd_data.type);
   edit->snd_data.size = prop_item_label_add(edit->sample_box, &edit->snd_data.size, _("size:"), _(" - "));
   elm_box_pack_end(edit->sample_box, edit->snd_data.size);

   item = prop_sound_editor_compression_type_add(edit->sample_box, edit);
   elm_box_pack_end(edit->sample_box, item);

   item = prop_sound_editor_compression_quality_add(edit->sample_box, edit);
   elm_box_pack_end(edit->sample_box, item);

   evas_object_hide(edit->sample_box);
}

static void
_tone_info_create(Evas_Object *parent, Sound_Prop_Data *edit)
{
   Evas_Object *item;

   assert(parent != NULL);
   assert(edit != NULL);

   BOX_ADD(edit->info_frame, edit->tone_box, false, false);
   elm_box_align_set(edit->tone_box, 0.5, 0.0);

   edit->snd_data.tone_name = prop_item_label_add(edit->tone_box, &edit->snd_data.tone_name, _("name:"), _(" - "));
   elm_box_pack_end(edit->tone_box, edit->snd_data.tone_name);

   item = prop_sound_editor_tone_frequency_add(edit->tone_box, edit);
   elm_box_pack_end(edit->tone_box, item);

   edit->snd_data.tone_duration = prop_item_label_add(edit->tone_box, &edit->snd_data.tone_duration, _("duration:"), _(" - "));
   elm_box_pack_end(edit->tone_box, edit->snd_data.tone_duration);

   evas_object_hide(edit->tone_box);
}

#undef INFO_ADD

static void
_sound_info_create(Evas_Object *parent, Sound_Prop_Data *edit)
{
   assert(parent != NULL);
   assert(edit != NULL);

   _sample_info_create(parent, edit);
   _tone_info_create(parent, edit);

   elm_box_pack_end(edit->box, edit->info_frame);
}

static void
_on_grid_clicked(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Sound_Prop_Data *edit = (Sound_Prop_Data *)data;
   Selected_Sound_Data *snd_data = (Selected_Sound_Data *)event_info;

   assert(edit != NULL);
   assert(snd_data != NULL);

   switch (snd_data->sound_type)
     {
      case SOUND_TYPE_SAMPLE:
        {
           _grid_sample_selected(edit, NULL, snd_data);
           break;
        }
      case SOUND_TYPE_TONE:
        {
           _grid_tone_selected(edit, NULL, snd_data);
           break;
        }
     }
}

Evas_Object *
ui_property_sound_add(Evas_Object *parent)
{
   Sound_Prop_Data *pd;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Sound_Prop_Data));

   /* editors */
   BOX_ADD(parent, pd->box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);
   evas_object_data_set(pd->box, SOUND_PROP_DATA, pd);

   /* Frame with sound player */
   FRAME_PROPERTY_ADD(pd->box, pd->sound_player_frame, true, _("Preview"), pd->box)
   _sound_player_create(parent, pd);

   /* Frame with info */
   FRAME_PROPERTY_ADD(pd->box, pd->info_frame, true, _("Info"), pd->box)
   _sound_info_create(parent, pd);

   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_ADD, _on_grid_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_DEL, _on_sound_editor_del, pd);

   return pd->box;
}