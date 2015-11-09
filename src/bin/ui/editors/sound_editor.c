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

#include "sound_editor.h"
#include "main_window.h"
#include "editor.h"

TODO("Rename this file to sound_manager")

#ifdef HAVE_AUDIO
TODO("Check pulse_audio on configure and add COREAUDIO support")
   #define HAVE_PULSE 1
   #include <Ecore_Audio.h>
#endif

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define UPDATE_FREQUENCY 1.0 / 30.0
#define TONE_PLAYING_DURATION 2.0
#define SND_EDIT_KEY "sound_editor_key"

typedef struct _Sound_Editor Sound_Editor;
typedef struct _Search_Data Search_Data;
typedef struct _Item Item;
typedef struct _Sound Sound;

struct _Sound
{
   const char *name;
   const char *src;
   int tone_frq;
};

struct _Item
{
   Eina_Stringshare *sound_name;
   Eina_Stringshare *src;
   Eina_Stringshare *format;
   Edje_Edit_Sound_Comp comp;
   double rate;
   int tone_frq;
};

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Sound_Editor
{
   Sound_Editor_Mode mode;
   Evas_Object *popup;
   Evas_Object *popup_btn_add;
   Evas_Object *add_cmb;
   Evas_Object *tone_entry, *frq_entry;
   Elm_Validator_Regexp *tone_validator, *frq_validator;
   Evas_Object *gengrid;
   Evas_Object *sample_box;
   Evas_Object *tone_box;
   Evas_Object *markup;
   Evas_Object *player_markup;
   Elm_Object_Item *tone;
   Search_Data sound_search_data;
   Evas_Object *check;
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
      Evas_Object *teg;
      Evas_Object *tone_name;
      Evas_Object *tone_frq;
      Evas_Object *tone_duration;
      Evas_Object *file_name;
      Evas_Object *duration;
      Evas_Object *type;
      Evas_Object *size;
      Evas_Object *comp;
      Evas_Object *quality;
   } snd_data;

   Ecore_Timer *timer;
   Evas_Object *rewind;
   Evas_Object *play;
   Evas_Object *pause;
   Evas_Object *fs_win;
   Eina_Bool decoration : 1;
   Eina_Bool playing : 1;
   Eina_Bool stopped : 1;
   Eina_Bool switched : 1;
   Eina_Bool added : 1;
};

static Elm_Gengrid_Item_Class *gic = NULL, *ggic = NULL;

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part)
{
   const Item *it = data;
   if (!part)
     return NULL;

   if (!strcmp(part, "elm.label.type"))
     return strdup(it->format);
   else
     return strdup(it->sound_name);
}

static char *
_grid_group_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char  *part)
{
   const Item *it = data;
   if (!part)
     return NULL;

   if (!strcmp(part, "elm.text"))
     return strdup(it->sound_name);

   return NULL;
}

static void
_sound_editor_del(Sound_Editor *edit)
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

#ifdef HAVE_AUDIO
static int
_snd_file_seek(void *data, Eo *eo_obj EINA_UNUSED, int offset, int whence)
{
   Sound_Editor *vf = data;

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
   Sound_Editor *vf = data;

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
   Sound_Editor *vf = data;

   assert(vf != NULL);

   return vf->io.length;
}

static int
_snd_file_tell(void *data, Eo *eo_obj EINA_UNUSED)
{
   Sound_Editor *vf = data;

   assert(vf != NULL);

   return vf->io.offset;
}

static Eina_Bool
_play_finished_cb(void *data,
                  Eo *in EINA_UNUSED,
                  const Eo_Event_Description *desc EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   Sound_Editor *edit = (Sound_Editor *) data;

   assert(edit != NULL);

   eo_del(edit->io.in);
   edit->io.in = NULL;
   edit->playing = false;
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
   Sound_Editor *edit = (Sound_Editor *)data;

   value = elm_slider_value_get(edit->rewind);
   elm_slider_min_max_get(edit->rewind, NULL, &max);

   if (max == value)
     {
        elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
        evas_object_hide(edit->pause);
        elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
        evas_object_show(edit->play);
        elm_slider_value_set(edit->rewind, 0.0);
        return ECORE_CALLBACK_CANCEL;
     }

   elm_slider_value_set(edit->rewind, value + UPDATE_FREQUENCY);
   return ECORE_CALLBACK_RENEW;
}

static void
_create_io_stream(Sound_Editor *edit)
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
_initialize_io_data(Sound_Editor *edit)
{
   edit->io.offset = 0;

   assert(edit != NULL);

   _create_io_stream(edit);

   eo_do(edit->io.in, ecore_audio_obj_name_set(edit->selected));
   eo_do(edit->io.in, ecore_audio_obj_vio_set(&edit->io.vio, edit, NULL));
}

static void
_tone_play(Sound_Editor *edit, int tone_frq)
{
   double value;
   Eina_Bool ret = false;

   assert(edit != NULL);

   elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
   evas_object_hide(edit->play);
   elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->pause);
   evas_object_show(edit->pause);

   if (edit->stopped)
     {
        eo_do(edit->io.in, ecore_audio_obj_paused_set(false));
        ecore_timer_thaw(edit->timer);
        edit->stopped = false;
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

   value = elm_slider_value_get(edit->rewind);
   if (value)
     eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));

   edit->playing = true;
   edit->timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, edit);
   ecore_main_loop_begin();
}

static void
_add_sound_play(Sound_Editor *edit)
{
   double value;
   Eina_Bool ret = false;

   assert(edit != NULL);

   elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
   evas_object_hide(edit->play);
   elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->pause);
   evas_object_show(edit->pause);

   if (edit->stopped)
     {
        eo_do(edit->io.in, ecore_audio_obj_paused_set(false));
        ecore_timer_thaw(edit->timer);
        edit->stopped = false;
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
             elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
             evas_object_hide(edit->pause);
             elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
             evas_object_show(edit->play);
             return;
          }
     }

   eo_do(edit->io.out, ret = ecore_audio_obj_out_input_attach(edit->io.in));
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
        evas_object_hide(edit->pause);
        elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
        evas_object_show(edit->play);
        return;
     }

   value = elm_slider_value_get(edit->rewind);
   if (value)
     eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));

   edit->playing = true;
   edit->timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, edit);
   ecore_main_loop_begin();
}

static void
_play_sound(Sound_Editor *edit)
{
   double value;
   Eina_Bool ret = false;
   Elm_Object_Item *g_item;
   const Item *item;

   assert(edit != NULL);

   g_item = elm_gengrid_selected_item_get(edit->gengrid);
   if (!g_item)
     return;

   if (edit->added)
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

   elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
   evas_object_hide(edit->play);
   elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->pause);
   evas_object_show(edit->pause);

   if (edit->stopped)
     {
        eo_do(edit->io.in, ecore_audio_obj_paused_set(false));
        ecore_timer_thaw(edit->timer);
        edit->stopped = false;
        return;
     }

   if (!edit->io.in)
     _initialize_io_data(edit);

   eo_do(edit->io.out, ret = ecore_audio_obj_out_input_attach(edit->io.in));
   if (!ret)
     {
        ERR("Couldn't attach input and output!");
        elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
        evas_object_hide(edit->pause);
        elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
        evas_object_show(edit->play);
        return;
     }
   edit->playing = true;

   value = elm_slider_value_get(edit->rewind);
   if (value)
     eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));

   edit->timer = ecore_timer_add(UPDATE_FREQUENCY, _rewind_cb, edit);
   ecore_main_loop_begin();
}

static void
_interrupt_playing(Sound_Editor *edit)
{
   assert(edit != NULL);

   Eina_Bool ret;
   if (edit->playing)
     {
        eo_do(edit->io.out, ret = ecore_audio_obj_out_input_detach(edit->io.in));
        if (!ret)
          ERR("Could not detach input");
        if (!edit->stopped)
          {
             ecore_timer_del(edit->timer);
             edit->timer = NULL;
          }
        ecore_main_loop_quit();
        elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
        evas_object_hide(edit->pause);
        elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
        evas_object_show(edit->play);
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

   edit->stopped = false;
   edit->playing = false;
   edit->added = false;
}

static void
_on_play_cb(void *data,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   _play_sound(data);
}

static void
_on_pause_cb(void *data,
             Evas_Object *obj EINA_UNUSED,
             void *event_info EINA_UNUSED)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   edit->stopped = true;
   eo_do(edit->io.in, ecore_audio_obj_paused_set(true));

   ecore_timer_freeze(edit->timer);
   elm_object_part_content_unset(edit->player_markup, "swallow.button.play");
   evas_object_hide(edit->pause);
   elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
   evas_object_show(edit->play);
}

static void
_on_next_cb(void *data,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   Elm_Object_Item *it, *first, *last;
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   edit->switched = true;
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
_on_prev_cb(void *data,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   Elm_Object_Item *it, *first, *last;
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   edit->switched = true;
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
_on_rewind_cb(void *data,
              Evas_Object *obj EINA_UNUSED,
              void *event_info EINA_UNUSED)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   double value = elm_slider_value_get(edit->rewind);
   eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));
}
#endif

static void
_sound_editor_quit(Sound_Editor *edit)
{
#ifdef HAVE_AUDIO
   if (edit->playing)
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
                     Evas *e __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   _sound_editor_quit(edit);
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

#ifdef HAVE_AUDIO
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
_sound_player_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *bt, *icon, *item;

   assert(parent != NULL);
   assert(edit != NULL);

   edit->player_markup = elm_layout_add(parent);
   elm_layout_theme_set(edit->player_markup, "layout", "sound_editor", "player");
   evas_object_size_hint_weight_set(edit->player_markup,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->player_markup);
   elm_object_part_content_set(parent, "player", edit->player_markup);

   edit->snd_data.teg = elm_icon_add(edit->player_markup);
   evas_object_image_smooth_scale_set(edit->snd_data.teg, false);
   evas_object_show(edit->snd_data.teg);
   evas_object_size_hint_fill_set(edit->snd_data.teg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(edit->snd_data.teg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(edit->player_markup, "eflete.swallow.teg",
                               edit->snd_data.teg);

   INFO_ADD(parent, item, _("Play on select:"), "item");
   CHECK_ADD(item, edit->check);
   elm_object_part_content_set(item, "swallow.second", edit->check);
   elm_object_part_content_set(edit->player_markup, "eflete.swallow.check", item);

   edit->rewind = elm_slider_add(edit->player_markup);
   elm_slider_unit_format_set(edit->rewind, "%2.0f");
   elm_slider_units_format_function_set(edit->rewind, _player_units_format,
                                        _player_units_free);
   evas_object_size_hint_align_set(edit->rewind, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(edit->rewind, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->rewind);
   elm_object_part_content_set(edit->player_markup, "eflete.swallow.fast", edit->rewind);
   evas_object_smart_callback_add(edit->rewind, "changed", _on_rewind_cb, edit);

   BT_ADD(edit->player_markup, bt, icon, "media_player/prev");
   elm_object_part_content_set(edit->player_markup, "swallow.button.prev", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_prev_cb, edit);

   BT_ADD(edit->player_markup, edit->play, icon, "media_player/play");
   elm_object_part_content_set(edit->player_markup, "swallow.button.play", edit->play);
   evas_object_smart_callback_add(edit->play, "clicked", _on_play_cb, edit);

   BT_ADD(edit->player_markup, bt, icon, "media_player/next");
   elm_object_part_content_set(edit->player_markup, "swallow.button.next", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_next_cb, edit);

   edit->pause = elm_button_add(edit->player_markup);
   elm_object_style_set(edit->pause, "anchor");
   evas_object_size_hint_align_set(edit->pause, EVAS_HINT_FILL, EVAS_HINT_FILL);
   ICON_STANDARD_ADD(edit->pause, icon, false, "media_player/pause")
   elm_object_part_content_set(edit->pause, NULL, icon);
   evas_object_smart_callback_add(edit->pause, "clicked", _on_pause_cb, edit);

   edit->io.vio.get_length = _snd_file_get_length;
   edit->io.vio.seek = _snd_file_seek;
   edit->io.vio.read = _snd_file_read;
   edit->io.vio.tell = _snd_file_tell;
}
#endif

#undef BT_ADD

static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;
   eina_stringshare_del(it->sound_name);
   eina_stringshare_del(it->format);
   free(it);
}

static Evas_Object *
_sound_info_label_add(Evas_Object *box,
                      const char *label)
{
   Evas_Object *item;
   INFO_ADD(box, item, label, "item");
   elm_box_pack_end(box, item);
   return item;
}

static void
_sample_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *item;

   assert(parent != NULL);
   assert(edit != NULL);

   BOX_ADD(parent, edit->sample_box, false, false);
   elm_box_align_set(edit->sample_box, 0.5, 0.5);

   edit->snd_data.file_name = _sound_info_label_add(edit->sample_box, _("file name:"));
   edit->snd_data.duration = _sound_info_label_add(edit->sample_box, _("duration:"));
   edit->snd_data.type = _sound_info_label_add(edit->sample_box, _("type:"));
   edit->snd_data.size = _sound_info_label_add(edit->sample_box, _("size:"));

   INFO_ADD(edit->sample_box, item, _("compression:"), "item");

   EWE_COMBOBOX_ADD(item, edit->snd_data.comp);
   ewe_combobox_item_add(edit->snd_data.comp, "NONE");
   ewe_combobox_item_add(edit->snd_data.comp, "RAW");
   ewe_combobox_item_add(edit->snd_data.comp, "COMP");
   ewe_combobox_item_add(edit->snd_data.comp, "LOSSY");
   ewe_combobox_item_add(edit->snd_data.comp, "AS_IS");
   elm_object_disabled_set(edit->snd_data.comp, true);

   elm_object_part_text_set(item, "label.property", _("quality:"));
   SPINNER_ADD(item, edit->snd_data.quality, 45, 1000, 1, false);
   elm_object_disabled_set(edit->snd_data.quality, true);
   elm_object_part_content_set(item, "swallow.first", edit->snd_data.comp);
   elm_object_part_content_set(item, "swallow.second", edit->snd_data.quality);

   elm_box_pack_end(edit->sample_box, item);
   evas_object_hide(edit->sample_box);
}

static void
_tone_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *item;

   assert(parent != NULL);
   assert(edit != NULL);

   BOX_ADD(parent, edit->tone_box, false, false);
   elm_box_align_set(edit->tone_box, 0.5, 0.5);

   edit->snd_data.tone_name = _sound_info_label_add(edit->tone_box, _("name:"));

   INFO_ADD(edit->tone_box, item, "frequency:", "item");
   SPINNER_ADD(edit->tone_box, edit->snd_data.tone_frq, 20, 20000, 10, false);
   elm_object_disabled_set(edit->snd_data.tone_frq, true);
   elm_object_part_content_set(item, "swallow.first", edit->snd_data.tone_frq);

   edit->snd_data.tone_duration = _sound_info_label_add(edit->tone_box, _("duration:"));
   elm_box_pack_end(edit->tone_box, item);
   evas_object_hide(edit->tone_box);
}

static void
_sound_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   assert(parent != NULL);
   assert(edit != NULL);

   _sample_info_create(parent, edit);
   _tone_info_create(parent, edit);
}

Eina_Stringshare *
_sound_format_get(Eina_Stringshare *snd_src)
{
   Eina_Stringshare *type;
   char *dot, *up;

   assert(snd_src != NULL);

   dot = strrchr(snd_src, '.');
   if (!dot)
     return eina_stringshare_add(_("UNKNOWN"));

   dot++;
   up = strdup(dot);
   eina_str_toupper(&up);
   type = eina_stringshare_add((const char *)up);
   free(up);
   return type;
}

static void
_sample_info_setup(Sound_Editor *edit,
                   const Item *it,
                   Eina_Stringshare *snd_src,
                   double len)
{
   Eina_Stringshare *duration, *type, *type_show;
   Evas_Object *content;

   assert(edit != NULL);
   assert(it != NULL);
   assert(snd_src != NULL);

   content = elm_object_part_content_unset(edit->markup, "sound_info");
   evas_object_hide(content);

   duration = eina_stringshare_printf("%.2f s", len);
   elm_icon_standard_set(edit->snd_data.teg, "sound");
   elm_object_part_content_set(edit->markup, "sound_info", edit->sample_box);

   type = _sound_format_get(snd_src);
   type_show = eina_stringshare_printf(_("%s Format Sound (.%s)"), it->format, type);

   elm_object_part_text_set(edit->snd_data.file_name, "label.value", it->sound_name);
   elm_object_part_text_set(edit->snd_data.duration, "label.value", duration);
   elm_object_part_text_set(edit->snd_data.type, "label.value", type_show);
#ifdef HAVE_AUDIO
   Eina_Stringshare *size = eina_stringshare_printf("%.2f KB", edit->io.length / 1024.0);
   elm_object_part_text_set(edit->snd_data.size, "label.value", size);
   eina_stringshare_del(size);
#endif
   ewe_combobox_select_item_set(edit->snd_data.comp, it->comp);
   elm_spinner_value_set(edit->snd_data.quality, it->rate);
   evas_object_show(edit->sample_box);

   eina_stringshare_del(duration);
   eina_stringshare_del(type);
   eina_stringshare_del(type_show);
}

static void
_tone_info_setup(Sound_Editor *edit, const Item *it)
{
   Evas_Object *content;
   Eina_Stringshare *duration;

   assert(edit != NULL);
   assert(it != NULL);

   content = elm_object_part_content_unset(edit->markup, "sound_info");
   evas_object_hide(content);

   duration = eina_stringshare_printf("%.1f s", TONE_PLAYING_DURATION);
   elm_icon_standard_set(edit->snd_data.teg, "sound");
   elm_object_part_content_set(edit->markup, "sound_info", edit->tone_box);

   elm_object_part_text_set(edit->snd_data.tone_name, "label.value", it->sound_name);
   elm_spinner_value_set(edit->snd_data.tone_frq, it->tone_frq);
   elm_object_part_text_set(edit->snd_data.tone_duration, "label.value", duration);
   evas_object_show(edit->tone_box);
   eina_stringshare_del(duration);
}
#ifdef HAVE_AUDIO

static void
_added_sample_src_info_setup(Sound_Editor *edit,
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
   elm_slider_min_max_set(edit->rewind, 0, *len);
   elm_slider_value_set(edit->rewind, 0.0);
   edit->io.length = ecore_file_size(edit->snd_src);
}
#endif

static void
_grid_sel_sample(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   double len = 0.0;
   const Item *item;
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);
#ifdef HAVE_AUDIO
   Eina_Bool auto_play = elm_check_state_get(edit->check);
#endif

   if (!edit->decoration)
     {
        elm_layout_signal_emit(edit->markup, "eflete,sound,clicked", "eflete");
        edit->decoration = true;
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
        _sample_info_setup(edit, item, item->src, len);
        edit->added = true;
#ifdef HAVE_AUDIO
        if ((edit->switched) || (auto_play))
          {
             edit->switched = false;
             _add_sound_play(edit);
          }
#endif
     }
}

static void
_grid_sel_tone(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   const Item *item;
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);

   if (!edit->decoration)
     {
        elm_layout_signal_emit(edit->markup, "eflete,sound,clicked", "eflete");
        edit->decoration = true;
     }

#ifdef HAVE_AUDIO
   _interrupt_playing(edit);
#endif

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        edit->selected = eina_stringshare_add(item->sound_name);

        elm_slider_min_max_set(edit->rewind, 0.0, TONE_PLAYING_DURATION);
        elm_slider_value_set(edit->rewind, 0.0);

        _tone_info_setup(edit, item);
#ifdef HAVE_AUDIO
        if (edit->switched)
          {
             edit->switched = false;
             _tone_play(edit, item->tone_frq);
             return;
          }
        if (elm_check_state_get(edit->check))
          _tone_play(edit, item->tone_frq);
#endif
     }
}

static void
_gengrid_content_fill(Sound_Editor *edit)
{
   Eina_List *sounds, *tones, *l;
   Item *it;
   External_Resource *res;

   assert(edit != NULL);
   assert(ap.project != NULL);

   sounds = ap.project->sounds;
   tones = ap.project->tones;

   if (edit->mode != SOUND_EDITOR_TONE_SELECT)
     {
        it = (Item *)mem_calloc(1, sizeof(Item));

        it->sound_name = eina_stringshare_add(_("<b>Sound Samples</b>"));
        elm_gengrid_item_append(edit->gengrid, ggic, it, NULL, NULL);

        if (sounds)
          {
             EINA_LIST_FOREACH(sounds, l, res)
               {
                  it = (Item *)mem_calloc(1, sizeof(Item));
                  it->sound_name = eina_stringshare_add(res->name);
                  it->format = _sound_format_get(res->source);
                  it->comp = edje_edit_sound_compression_type_get(ap.project->global_object, it->sound_name);
                  it->rate = edje_edit_sound_compression_rate_get(ap.project->global_object, it->sound_name);
                  it->src = eina_stringshare_add(res->source);
                  elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_sample, edit);
               }
          }
     }

   if (edit->mode != SOUND_EDITOR_SAMPLE_SELECT)
     {
        it = (Item *)mem_calloc(1, sizeof(Item));
        it->sound_name = eina_stringshare_add(_("<b>Sound Tones</b>"));
        edit->tone = elm_gengrid_item_append(edit->gengrid, ggic, it, NULL, NULL);

        if (tones)
          {
             EINA_LIST_FOREACH(tones, l, res)
               {
                  it = (Item *)mem_calloc(1, sizeof(Item));
                  it->sound_name = eina_stringshare_add(res->name);
                  it->tone_frq = edje_edit_sound_tone_frequency_get(ap.project->global_object, it->sound_name);
                  it->format = eina_stringshare_printf("%d", it->tone_frq);
                  elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_tone, edit);
               }
          }
     }
}

static void
_create_gengrid(Evas_Object *parent,
                Sound_Editor *editor)
{
   assert(parent != NULL);
   assert(editor != NULL);

   editor->gengrid = elm_gengrid_add(parent);
   elm_gengrid_item_size_set(editor->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_group_item_size_set(editor->gengrid, ITEM_HEIGHT/5, ITEM_HEIGHT/5);
   elm_gengrid_align_set(editor->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(editor->gengrid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   elm_gengrid_multi_select_set(editor->gengrid, false);

   evas_object_size_hint_weight_set(editor->gengrid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(editor->gengrid, EVAS_HINT_FILL, EVAS_HINT_FILL);

   gic = elm_gengrid_item_class_new();
   gic->item_style = "sound_editor";
   gic->func.text_get = _grid_label_get;
   gic->func.content_get = NULL;
   gic->func.del = _grid_del;

   ggic = elm_gengrid_item_class_new();
   ggic->item_style = "group_index";
   ggic->func.text_get = _grid_group_label_get;
   ggic->func.content_get = NULL;
   ggic->func.del = _grid_del;

   _gengrid_content_fill(editor);

   evas_object_show(editor->gengrid);
   elm_object_part_content_set(editor->markup, "gengrid", editor->gengrid);
}

static inline Evas_Object *
_sound_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static void
_fs_del(void *data)
{
   Elm_Object_Item *it;
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   if (edit->fs_win)
     {
        evas_object_del(edit->fs_win);
        edit->fs_win = NULL;
        it = elm_gengrid_item_prev_get(edit->tone);
        elm_gengrid_item_selected_set(it, true);
     }
}

static void
_add_sample_done(void *data,
                 Evas_Object *obj,
                 void *event_info)
{
   Item *it;
   Eina_Stringshare *sound_name;
   Eina_List *samples_list, *l;
   Eina_Bool exist = false;
   External_Resource *res;
   const char *file_name;

   const char *selected = event_info;
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   if ((!selected) || (!strcmp(selected, "")))
     goto del;

   samples_list = ap.project->sounds;

   if ((ecore_file_exists(selected)) && (!ecore_file_is_dir(selected)))
     {
        file_name = ecore_file_file_get(selected);
        sound_name = eina_stringshare_add(file_name);
        EINA_LIST_FOREACH(samples_list, l, res)
           if (res->name == sound_name) /* they both are stringshares */
             {
                exist = true;
                break;
             }
        if (exist)
          {
             WIN_NOTIFY_WARNING(obj, _("Sample with this name is already added to project"))
                eina_stringshare_del(sound_name);
             return;
          }

        res = mem_calloc(1, sizeof(External_Resource));
        res->name = eina_stringshare_add(file_name);
        res->source = eina_stringshare_printf("%s/images/%s", ap.project->develop_path, file_name);

        if (!ecore_file_exists(res->source))
          {
             ecore_file_cp(selected, res->source);

             ap.project->sounds = eina_list_sorted_insert(ap.project->sounds, (Eina_Compare_Cb) resource_cmp, res);
          }
        else
          {
             WIN_NOTIFY_ERROR(obj, _("File exist"));
             free(res);
          }

        edje_edit_sound_sample_add(ap.project->global_object, res->name, res->source);

        it = (Item *)mem_calloc(1, sizeof(Item));
        it->sound_name = eina_stringshare_add(sound_name);
        it->format = _sound_format_get(selected);
        it->comp = EDJE_EDIT_SOUND_COMP_RAW;
        it->src = eina_stringshare_add(res->source);
        if (edit->mode != SOUND_EDITOR_SAMPLE_SELECT)
          elm_gengrid_item_insert_before(edit->gengrid, gic, it, edit->tone,
                                         _grid_sel_sample, edit);
        else
          elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_sample, edit);

        editor_save(ap.project->global_object);
        TODO("Remove this line once edje_edit_sound_sample_add would be added into Editor Module and saving would work properly")
        ap.project->changed = true;
     }
   else
     WIN_NOTIFY_ERROR(obj, _("Error while loading file.<br>File is not exist"));

del:
   ecore_job_add(_fs_del, edit);
}

static void
_popup_close(void *data)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);
   assert(edit->tone_validator != NULL);
   assert(edit->frq_validator != NULL);

   evas_object_del(edit->popup);

   elm_validator_regexp_free(edit->tone_validator);
   elm_validator_regexp_free(edit->frq_validator);
   edit->tone_validator = NULL;
   edit->frq_validator = NULL;

   elm_gengrid_item_selected_set(elm_gengrid_last_item_get(edit->gengrid), true);
}

static void
_add_tone_cancel(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   ecore_job_add(_popup_close, edit);
}

static void
_add_tone_done(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Item *it;
   Eina_Stringshare *tone_name;
   const char *str_value;
   int frq;
   Eina_List *tones_list, *l;
   Eina_Bool exist = false;
   Tone_Resource *tone;

   Sound_Editor *edit = (Sound_Editor *)data;

   assert(edit != NULL);

   tone_name = eina_stringshare_add(elm_entry_entry_get(edit->tone_entry));

   tones_list = ap.project->tones;
   EINA_LIST_FOREACH(tones_list, l, tone)
     if (tone->name == tone_name) /* they both are stringshares */
       {
          exist = true;
          break;
       }
   if (exist)
     {
        NOTIFY_WARNING(_("Tone with this name is already added to project"))
        eina_stringshare_del(tone_name);
        return;
     }

   str_value = elm_entry_entry_get(edit->frq_entry);
   frq = atoi(str_value);

   it = (Item *)mem_calloc(1, sizeof(Item));
   it->sound_name = eina_stringshare_add(tone_name);
   it->tone_frq = frq;
   it->format = eina_stringshare_printf("%d", it->tone_frq);
   elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_tone, edit);

   tone = mem_calloc(1, sizeof(External_Resource));
   tone->name = eina_stringshare_add(tone_name);
   tone->freq = frq;
   ap.project->tones = eina_list_sorted_insert(ap.project->tones, (Eina_Compare_Cb) resource_cmp, tone);

   edje_edit_sound_tone_add(ap.project->global_object, tone_name, frq);
   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_image_add would be added into Editor Module and saving would work properly")
   ap.project->changed = true;

   ecore_job_add(_popup_close, edit);
}

static Eina_Bool
_samples_filter(const char *path,
                Eina_Bool dir,
                void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, "wav") || eina_str_has_extension(path, "ogg") ||
       eina_str_has_extension(path, "flac"))
     return true;
   return false;
}

static void
_sample_add_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Evas_Object *fs, *ic;
   Sound_Editor *edit = data;

   assert(edit != NULL);

   edit->fs_win  = mw_add(NULL, NULL, NULL);

   assert(edit->fs_win != NULL);

   mw_title_set(edit->fs_win, "Add sound to the library");
   ic = elm_icon_add(edit->fs_win);
   elm_icon_standard_set(ic, "folder");
   mw_icon_set(edit->fs_win, ic);
   evas_object_show(edit->fs_win);

   FILESELECTOR_ADD(fs, edit->fs_win, _add_sample_done, data);
   elm_fileselector_custom_filter_append(fs, _samples_filter, NULL, _("Sound files"));
   elm_fileselector_mime_types_filter_append(fs, "*", _("All files"));
   elm_win_inwin_content_set(edit->fs_win, fs);
}

static void
_validation(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;

   if ((elm_validator_regexp_status_get(edit->tone_validator) != ELM_REG_NOERROR) ||
       (elm_validator_regexp_status_get(edit->frq_validator) != ELM_REG_NOERROR))
     elm_object_disabled_set(edit->popup_btn_add, true);
   else
     elm_object_disabled_set(edit->popup_btn_add, false);
}

static void
_tone_add_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;

   assert(edit != NULL);

   Evas_Object *popup, *box, *item, *bt_no;
   Eina_Stringshare *title;

   popup = elm_popup_add(ap.win);
   title = eina_stringshare_add(_("Add new tone to the project"));
   elm_object_part_text_set(popup, "title,text", title);
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);
   edit->popup = popup;

   assert(edit->tone_validator == NULL);
   assert(edit->frq_validator == NULL);
   edit->tone_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   edit->frq_validator = elm_validator_regexp_new(FREQUENCY_REGEX, NULL);

   BOX_ADD(popup, box, false, false);
   elm_object_content_set(popup, box);
   LAYOUT_PROP_ADD(box, _("Tone name:"), "property", "1swallow")
   ENTRY_ADD(item, edit->tone_entry, true);
   eo_do(edit->tone_entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, edit->tone_validator));
   evas_object_smart_callback_add(edit->tone_entry, "changed", _validation, edit);
   elm_object_part_text_set(edit->tone_entry, "guide", _("Type a new tone name"));
   /* need to manualy set not valid string for triggered validator */
   elm_entry_entry_set(edit->tone_entry, NULL);
   elm_object_part_content_set(item, "elm.swallow.content", edit->tone_entry);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Frequency:"), "property", "1swallow")
   ENTRY_ADD(item, edit->frq_entry, true);
   eo_do(edit->frq_entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, edit->frq_validator));
   evas_object_smart_callback_add(edit->frq_entry, "changed", _validation, edit);
   elm_object_part_text_set(edit->frq_entry, "guide", _("Type a frequency (20 - 20000)"));
   /* need to manualy set not valid string for triggered validator */
   elm_entry_entry_set(edit->frq_entry, NULL);
   elm_object_part_content_set(item, "elm.swallow.content", edit->frq_entry);
   elm_box_pack_end(box, item);

   BUTTON_ADD(popup, edit->popup_btn_add, _("Add"));
   evas_object_smart_callback_add (edit->popup_btn_add, "clicked", _add_tone_done, edit);
   elm_object_part_content_set(popup, "button1", edit->popup_btn_add);
   elm_object_disabled_set(edit->popup_btn_add, true);

   BUTTON_ADD(popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _add_tone_cancel, edit);
   elm_object_part_content_set(popup, "button2", bt_no);

   evas_object_show(popup);
   eina_stringshare_del(title);
}

#undef INFO_ADD

static void
_on_cmb_sel(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info)
{
   Ewe_Combobox_Item *selected_item = event_info;

   if (!selected_item->index)
     _sample_add_cb(data, NULL, NULL);
   else
     _tone_add_cb(data, NULL, NULL);
}

static void
_on_delete_clicked_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Elm_Object_Item *grid_it;
   Item *item;
   Eina_List *list, *l, *l_next;
   Sound_Editor *edit = (Sound_Editor *)data;
   External_Resource *res;
   char buf[BUFF_MAX];
   int symbs = 0, notdeleted = 0, selected = 0;

   assert(edit != NULL);

   list = (Eina_List *)elm_gengrid_selected_items_get(edit->gengrid);
   selected = eina_list_count(list);

   if ((!list) || (!selected))
     {
        NOTIFY_WARNING("No selected items");
        return;
     }

   snprintf(buf, BUFF_MAX, _("Unable to delete: "));
   symbs = strlen(buf);
   EINA_LIST_FOREACH_SAFE(list, l, l_next, grid_it)
     {
        item = elm_object_item_data_get(grid_it);

        if (!item->tone_frq)
          {
             res = pm_resource_get(ap.project->sounds, item->sound_name);
             if (!res->used_in)
               {
                  edje_edit_sound_sample_del(ap.project->global_object, item->sound_name);
                  ap.project->sounds = pm_resource_del(ap.project->sounds, res);
                  elm_object_item_del(grid_it);
               }
             else if ((res->used_in) && (notdeleted < 4))
               {
                  snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>Sound \"%s\""), res->name);
                  symbs += strlen(res->name);
                  notdeleted++;
               }
          }
        else
          {
             res = pm_resource_get(ap.project->tones, item->sound_name);
             if (!res->used_in)
               {
                  edje_edit_sound_tone_del(ap.project->global_object, item->sound_name);
                  ap.project->tones = pm_resource_del(ap.project->tones, res);
                  elm_object_item_del(grid_it);
               }
             else if ((res->used_in) && (notdeleted < 4))
               {
                  snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>Tone \"%s\""), res->name);
                  symbs += strlen(res->name);
                  notdeleted++;
               }
          }
     }

   if (notdeleted >= 4)
     snprintf(buf + symbs, BUFF_MAX - symbs, "<br>...");

   if (notdeleted != 0)
     NOTIFY_WARNING("%s<br>Used in Programs", buf);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_sound_..._del would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;

}

ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, "elm.label")

static void
_search_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;

   assert(edit != NULL);

   _gengrid_item_search(edit->gengrid, &(edit->sound_search_data),
                        edit->sound_search_data.last_item_found);

}

static void
_search_nxt_gd_item(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;
   Elm_Object_Item *start_from = NULL;

   assert(edit != NULL);

   if (edit->sound_search_data.last_item_found)
     {
        start_from =
           elm_gengrid_item_next_get(edit->sound_search_data.last_item_found);
     }

   _gengrid_item_search(edit->gengrid, &(edit->sound_search_data), start_from);
}

static void
_search_reset_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Search_Data *search_data = data;

   assert(search_data != NULL);

   search_data->last_item_found = NULL;
}

static void
_sound_editor_main_markup_create(Sound_Editor *edit)
{
   Evas_Object *btn, *ic, *search;

   assert(edit != NULL);

   edit->markup = elm_layout_add(ap.win);
   elm_layout_theme_set(edit->markup, "layout", "sound_editor", "default");
   evas_object_size_hint_weight_set(edit->markup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->markup);
   evas_object_data_set(edit->markup, SND_EDIT_KEY, edit);

   evas_object_event_callback_add(edit->markup, EVAS_CALLBACK_DEL, _on_sound_editor_del, edit);

   btn = elm_button_add(edit->markup);
   evas_object_smart_callback_add(btn, "clicked", _on_delete_clicked_cb, edit);
   evas_object_show(btn);
   elm_object_part_content_set(edit->markup, "swallow.btn.del", btn);
   evas_object_show(btn);

   ic = elm_icon_add(btn);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(btn, NULL, ic);

   switch (edit->mode)
     {
      case SOUND_EDITOR_EDIT:
        {
           EWE_COMBOBOX_ADD(edit->markup, edit->add_cmb);
           ewe_combobox_style_set(edit->add_cmb, "small");
           ewe_combobox_item_add(edit->add_cmb, _("Sample"));
           ewe_combobox_item_add(edit->add_cmb, _("Tone"));
           evas_object_smart_callback_add(edit->add_cmb, "selected", _on_cmb_sel, edit);
           elm_object_part_content_set(edit->markup, "swallow.btn.add", edit->add_cmb);
        }
      break;
      case SOUND_EDITOR_SAMPLE_SELECT:
        {
           btn = elm_button_add(edit->markup);
           evas_object_smart_callback_add(btn, "clicked", _sample_add_cb, edit);
           evas_object_show(btn);
           elm_object_part_content_set(edit->markup, "swallow.btn.add", btn);

           ic = elm_icon_add(btn);
           elm_icon_standard_set(ic, "plus");
           elm_object_part_content_set(btn, NULL, ic);
        }
      break;
      case SOUND_EDITOR_TONE_SELECT:
        {
           btn = elm_button_add(edit->markup);
           evas_object_smart_callback_add(btn, "clicked", _tone_add_cb, edit);
           evas_object_show(btn);
           elm_object_part_content_set(edit->markup, "swallow.btn.add", btn);

           ic = elm_icon_add(btn);
           elm_icon_standard_set(ic, "plus");
           elm_object_part_content_set(btn, NULL, ic);
        }
      break;
     }
   search = _sound_editor_search_field_create(edit->markup);
   elm_object_part_content_set(edit->markup, "swallow.search_area", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, edit);
   evas_object_smart_callback_add(edit->gengrid, "pressed", _search_reset_cb,
                                  &(edit->sound_search_data));
   edit->sound_search_data.search_entry = search;
   edit->sound_search_data.last_item_found = NULL;
}

Evas_Object *
sound_editor_window_add(Sound_Editor_Mode mode)
{
   Sound_Editor *edit;

   assert(ap.project != NULL);

#ifdef HAVE_AUDIO
   ecore_audio_init();
#endif

   edit = (Sound_Editor *)mem_calloc(1, sizeof(Sound_Editor));
   edit->mode = mode;

   _sound_editor_main_markup_create(edit);

   _create_gengrid(edit->markup, edit);

   _sound_info_create(edit->markup, edit);

#ifdef HAVE_AUDIO
   _sound_player_create(edit->markup, edit);
#endif

   elm_object_focus_set(edit->sound_search_data.search_entry, true);

   return edit->markup;
}

void sound_editor_added_sounds_free(Eina_List *add_snd)
{
   Sound *data;

   EINA_LIST_FREE(add_snd, data)
     {
        eina_stringshare_del(data->name);
        eina_stringshare_del(data->src);
        free(data);
     }
   add_snd = NULL;
}

Eina_Bool
sound_editor_file_choose(Evas_Object *win, const char *selected)
{
   Elm_Object_Item *grid_item;
   Sound_Editor *snd_edit;

   if (!selected) return false;

   assert(win != NULL);

   snd_edit = evas_object_data_get(win, SND_EDIT_KEY);

   assert(snd_edit != NULL);

   grid_item = elm_gengrid_search_by_text_item_get(snd_edit->gengrid, NULL,
                                                   "elm.text", selected, 0);
   if (grid_item)
     {
        elm_gengrid_item_selected_set(grid_item, true);
        elm_gengrid_item_bring_in(grid_item, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE);
        return true;
     }
   return false;
}

#undef ITEM_WIDTH
#undef ITEM_HEIGHT

