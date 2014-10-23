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

#define EFL_BETA_API_SUPPORT
#include <Ecore_Audio.h>

#include "sound_editor.h"
#include "main_window.h"

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define UPDATE_FREQUENCY 1.0 / 30.0

typedef struct _Sound_Editor Sound_Editor;
typedef struct _Search_Data Search_Data;
typedef struct _Item Item;

static Elm_Entry_Filter_Accept_Set accept_value = {
   .accepted = "0123456789.",
   .rejected = NULL
};

struct _Item
{
   const char *sound_name;
   Edje_Edit_Sound_Comp comp;
   int tone_frq;
   double rate;
};

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Sound_Editor
{
   Project *pr;
   Evas_Object *popup;
   Evas_Object *cmb;
   Evas_Object *win;
   Evas_Object *tone_entry, *frq_entry;
   Evas_Object *gengrid;
   Evas_Object *sample_box;
   Evas_Object *tone_box;
   Evas_Object *markup;
   Evas_Object *player_markup;
   Elm_Object_Item *tone;
   Search_Data sound_search_data;
   Evas_Object *check;
   const char  *selected;

   struct {
        int offset, length;
        const void *data;
        Ecore_Audio_Vio vio;
        Eo *in, *out;
        Eina_Binbuf *buf;
   } io;

   struct {
      Evas_Object *teg;
      Evas_Object *tone_name;
      Evas_Object *tone_frq;
      Evas_Object *file_name;
      Evas_Object *location;
      Evas_Object *type;
      Evas_Object *size;
      Evas_Object *comp;
      Evas_Object *quality;
   } snd_data;

   Ecore_Timer *timer;
   Evas_Object *rewind;
   Evas_Object *play;
   Evas_Object *pause;
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
                const char  *part __UNUSED__)
{
   const Item *it = data;
   return strdup(it->sound_name);
}

static void
_sound_editor_del(Sound_Editor *edit)
{
   ecore_audio_shutdown();
   edit->pr = NULL;
   elm_gengrid_item_class_free(gic);
   elm_gengrid_item_class_free(ggic);
   evas_object_del(edit->win);
   evas_object_del(edit->markup);
   eina_stringshare_del(edit->selected);
   free(edit);
}

static void
_on_ok_cb(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *edje_edit_obj;
   Sound_Editor *edit = (Sound_Editor *)data;
   GET_OBJ(edit->pr, edje_edit_obj);
   edje_edit_without_source_save(edje_edit_obj, true);
}

static int
_snd_file_seek(void *data, Eo *eo_obj EINA_UNUSED, int offset, int whence)
{
   Sound_Editor *vf = data;

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
   return vf->io.length;
}

static int
_snd_file_tell(void *data, Eo *eo_obj EINA_UNUSED)
{
   Sound_Editor *vf = data;
   return vf->io.offset;
}

static Eina_Bool
_play_finished_cb(void *data,
                  Eo *in EINA_UNUSED,
                  const Eo_Event_Description *desc EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   Sound_Editor *edit = (Sound_Editor *) data;

   eo_del(edit->io.in);
   edit->io.in = NULL;
   edit->playing = false;
   edit->added = false;
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
   if (!edit->io.in)
     {
        ERR("Input stream was't create!");
        return;
     }

   edit->io.out = eo_add(ECORE_AUDIO_OUT_PULSE_CLASS, NULL);
   if (!edit->io.out)
     {
        ERR("Output stream was't create!");
        eo_del(edit->io.in);
        edit->io.in = NULL;
        return;
     }

   eo_do(edit->io.out, eo_event_callback_add(ECORE_AUDIO_OUT_PULSE_EVENT_CONTEXT_FAIL,
                                             _out_fail, NULL));

   eo_do(edit->io.in, eo_event_callback_add(ECORE_AUDIO_IN_EVENT_IN_STOPPED,
                                            _play_finished_cb, edit));
}

static void
_initialize_io_data(Sound_Editor *edit)
{
   edit->io.offset = 0;
   _create_io_stream(edit);

   eo_do(edit->io.in, ecore_audio_obj_name_set(edit->selected));
   eo_do(edit->io.in, ecore_audio_obj_vio_set(&edit->io.vio, edit, NULL));
}

static void
_add_sound_play(Sound_Editor *edit)
{
   double value;
   Eina_Bool ret = false;

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
        eo_do(edit->io.in, ecore_audio_obj_name_set(edit->selected));
        eo_do(edit->io.in, ret = ecore_audio_obj_source_set(edit->selected));
        if (!ret)
          {
             ERR("Could not play the added sound");
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

   if (!elm_gengrid_selected_item_get(edit->gengrid))
     {
        ERR("Can't play sound! Item is not selected.");
        return;
     }

   if (edit->added)
     {
        _add_sound_play(edit);
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
_on_quit_cb(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Sound_Editor *edit = (Sound_Editor *)data;
   if (edit->playing)
     _interrupt_playing(edit);
   else if (edit->io.in)
     {
        eo_del(edit->io.in);
        eo_del(edit->io.out);
        eina_binbuf_free(edit->io.buf);
     }
   _sound_editor_del(edit);
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
   double value = elm_slider_value_get(edit->rewind);
   eo_do(edit->io.in, value = ecore_audio_obj_in_seek(value, SEEK_SET));
}

#define BT_ADD(PARENT, OBJ, ICON, TEXT) \
   OBJ = elm_button_add(PARENT); \
   evas_object_size_hint_align_set(OBJ, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   ICON_ADD(OBJ, ICON, NULL, "icon_"TEXT) \
   elm_object_style_set(OBJ, "eflete/simple"); \
   evas_object_show(OBJ); \
   elm_object_part_content_set(OBJ, NULL, ICON); \
   elm_object_part_content_set(PARENT, "swallow.button."TEXT, OBJ);

static void
_sound_player_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *bt, *icon, *item;

   if (!parent) return;

   edit->player_markup = elm_layout_add(parent);
   elm_layout_file_set(edit->player_markup, EFLETE_EDJ, "eflete/sound_editor/player");
   evas_object_size_hint_weight_set(edit->player_markup,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->player_markup);
   elm_object_part_content_set(parent, "player", edit->player_markup);

   edit->snd_data.teg = elm_image_add(edit->player_markup);
   evas_object_image_smooth_scale_set(edit->snd_data.teg, false);
   evas_object_show(edit->snd_data.teg);
   evas_object_size_hint_fill_set(edit->snd_data.teg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(edit->snd_data.teg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(edit->player_markup, "eflete.swallow.teg",
                               edit->snd_data.teg);

   ITEM_ADD(parent, item, _("Play on select:"), "eflete/sound_editor/item/default");
   CHECK_ADD(item, edit->check, DEFAULT_STYLE);
   elm_object_part_content_set(item, "swallow.second", edit->check);
   elm_object_part_content_set(edit->player_markup, "eflete.swallow.check", item);

   edit->rewind = elm_slider_add(edit->player_markup);
   elm_slider_unit_format_set(edit->rewind, "%1.0f");
   evas_object_size_hint_align_set(edit->rewind, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(edit->rewind, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->rewind);
   elm_object_part_content_set(edit->player_markup, "eflete.swallow.fast", edit->rewind);
   evas_object_smart_callback_add(edit->rewind, "changed", _on_rewind_cb, edit);

   BT_ADD(edit->player_markup, bt, icon, "prev");
   evas_object_smart_callback_add(bt, "clicked", _on_prev_cb, edit);

   BT_ADD(edit->player_markup, edit->play, icon, "play");
   evas_object_smart_callback_add(edit->play, "clicked", _on_play_cb, edit);

   BT_ADD(edit->player_markup, bt, icon, "next");
   evas_object_smart_callback_add(bt, "clicked", _on_next_cb, edit);

   edit->pause = elm_button_add(edit->player_markup);
   evas_object_size_hint_align_set(edit->pause, EVAS_HINT_FILL, EVAS_HINT_FILL);
   ICON_ADD(edit->pause, icon, NULL, "icon_pause")
   elm_object_style_set(edit->pause, "eflete/simple");
   elm_object_part_content_set(edit->pause, NULL, icon);
   evas_object_smart_callback_add(edit->pause, "clicked", _on_pause_cb, edit);

   edit->io.vio.get_length = _snd_file_get_length;
   edit->io.vio.seek = _snd_file_seek;
   edit->io.vio.read = _snd_file_read;
   edit->io.vio.tell = _snd_file_tell;
 }

#undef BT_ADD

static Evas_Object *
_grid_content_get(void *data __UNUSED__,
                  Evas_Object *obj,
                  const char  *part)
{
   Evas_Object *image = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        GET_IMAGE(image, obj, "sound");
        evas_object_show(image);
        return image;
     }
   return NULL;
}

static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;
   eina_stringshare_del(it->sound_name);
   free(it);
}

static Evas_Object *
_sound_info_label_add(Evas_Object *box,
                      const char *label)
{
   Evas_Object *item;
   ITEM_ADD(box, item, label, "eflete/sound_editor/item/default");
   elm_box_pack_end(box, item);
   return item;
}

static void
_sample_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   Evas_Object *item;
   BOX_ADD(parent, edit->sample_box, false, false);
   elm_box_align_set(edit->sample_box, 0.5, 0.5);

   edit->snd_data.file_name = _sound_info_label_add(edit->sample_box, _("file name:"));
   edit->snd_data.location = _sound_info_label_add(edit->sample_box, _("location:"));
   edit->snd_data.type = _sound_info_label_add(edit->sample_box, _("type:"));
   edit->snd_data.size = _sound_info_label_add(edit->sample_box, _("size:"));

   // TODO: Add field with file size info.

   ITEM_ADD(edit->sample_box, item, _("compression:"), "eflete/sound_editor/item/default");

   EWE_COMBOBOX_ADD(item, edit->snd_data.comp);
   ewe_combobox_item_add(edit->snd_data.comp, "NONE");
   ewe_combobox_item_add(edit->snd_data.comp, "RAW");
   ewe_combobox_item_add(edit->snd_data.comp, "COMP");
   ewe_combobox_item_add(edit->snd_data.comp, "LOSSY");
   ewe_combobox_item_add(edit->snd_data.comp, "AS_IS");
   elm_object_disabled_set(edit->snd_data.comp, true);

   elm_object_part_text_set(item, "label.property", _("quality:"));
   SPINNER_ADD(item, edit->snd_data.quality, 45, 1000, 1, false, DEFAULT_STYLE);
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

   BOX_ADD(parent, edit->tone_box, false, false);
   elm_box_align_set(edit->tone_box, 0.5, 0.5);

   edit->snd_data.tone_name = _sound_info_label_add(edit->tone_box, _("name:"));

   ITEM_ADD(edit->tone_box, item, "frequency:", "eflete/sound_editor/item/default");
   SPINNER_ADD(edit->tone_box, edit->snd_data.tone_frq, 20, 20000, 10, false, DEFAULT_STYLE);
   elm_object_disabled_set(edit->snd_data.tone_frq, true);
   elm_object_part_content_set(item, "swallow.first", edit->snd_data.tone_frq);
   elm_box_pack_end(edit->tone_box, item);
   evas_object_hide(edit->tone_box);
}

static void
_sound_info_create(Evas_Object *parent, Sound_Editor *edit)
{
   _sample_info_create(parent, edit);
   _tone_info_create(parent, edit);
}

static void
_sample_info_setup(Sound_Editor *edit, const Item *it)
{
   Eina_Stringshare *size;
   Evas_Object *content;

   content = elm_object_part_content_unset(edit->markup, "sound_info");
   evas_object_hide(content);

   size = eina_stringshare_printf("%.2f KB", edit->io.length / 1024.0);

   evas_object_image_file_set(edit->snd_data.teg, EFLETE_RESOURCES, "sound");

   elm_object_part_content_set(edit->markup, "sound_info", edit->sample_box);

   elm_object_part_text_set(edit->snd_data.file_name, "label.value", it->sound_name);
   elm_object_part_text_set(edit->snd_data.size, "label.value", size);
   ewe_combobox_select_item_set(edit->snd_data.comp, it->comp);
   elm_spinner_value_set(edit->snd_data.quality, it->rate);
   evas_object_show(edit->sample_box);

   eina_stringshare_del(size);
}

static void
_tone_info_setup(Sound_Editor *edit, const Item *it)
{
   Evas_Object *content;

   content = elm_object_part_content_unset(edit->markup, "sound_info");
   evas_object_hide(content);

   evas_object_image_file_set(edit->snd_data.teg, EFLETE_RESOURCES, "sound");

   elm_object_part_content_set(edit->markup, "sound_info", edit->tone_box);

   elm_object_part_text_set(edit->snd_data.tone_name, "label.value", it->sound_name);
   elm_spinner_value_set(edit->snd_data.tone_frq, it->tone_frq);
   evas_object_show(edit->tone_box);
}

static void
_added_sample_src_info_setup(Sound_Editor *edit, const char *snd_src)
{
   Eina_Bool ret;
   double len;

   _create_io_stream(edit);
   eo_do(edit->io.in, ecore_audio_obj_name_set(snd_src));
   eo_do(edit->io.in, ret = ecore_audio_obj_source_set(snd_src));
   if (!ret)
     ERR("Can not set source obj for added sample");
   eo_do(edit->io.in, len = ecore_audio_obj_in_length_get());
   elm_slider_min_max_set(edit->rewind, 0, len);
   elm_slider_value_set(edit->rewind, 0.0);
   edit->io.length = ecore_file_size(snd_src);
}

static void
_grid_sel_sample(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Eina_Bool auto_play;
   Eina_List *l;
   const char *snd_src, *snd;
   Evas_Object *edje_edit_obj;
   double len;
   const Item *item;
   Sound_Editor *edit = (Sound_Editor *)data;

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);
   auto_play = elm_check_state_get(edit->check);

   if (!edit->decoration)
     {
        elm_layout_signal_emit(edit->markup, "eflete,sound,clicked", "eflete");
        edit->decoration = true;
     }

   _interrupt_playing(edit);

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        edit->selected = eina_stringshare_add(item->sound_name);
        GET_OBJ(edit->pr, edje_edit_obj);

        if ((edit->pr->added_sounds))
          {
             EINA_LIST_FOREACH(edit->pr->added_sounds, l, snd)
               {
                  if (!strcmp(snd, edit->selected))
                    {
                       snd_src = edje_edit_sound_samplesource_get(edje_edit_obj, snd);
                       _added_sample_src_info_setup(edit, snd_src);
                       _sample_info_setup(edit, item);
                       edit->added = true;
                       if ((edit->switched) || (auto_play))
                         _add_sound_play(edit);
                       eina_stringshare_del(snd_src);
                       return;
                    }
               }
          }

        edit->io.buf = edje_edit_sound_samplebuffer_get(edje_edit_obj, edit->selected);
        edit->io.data = eina_binbuf_string_get(edit->io.buf);
        edit->io.length = eina_binbuf_length_get(edit->io.buf);
        _initialize_io_data(edit);

        eo_do(edit->io.in, len = ecore_audio_obj_in_length_get());
        elm_slider_min_max_set(edit->rewind, 0, len);
        elm_slider_value_set(edit->rewind, 0.0);

        _sample_info_setup(edit, item);

        if (edit->switched)
          {
             edit->switched = false;
             _play_sound(edit);
             return;
          }

        if (auto_play)
          _play_sound(edit);
     }
}

static void
_grid_sel_tone(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   const Item *item;
   Sound_Editor *edit = (Sound_Editor *)data;

   const Eina_List* sel_list = elm_gengrid_selected_items_get(edit->gengrid);
   int count = eina_list_count(sel_list);

   if (!edit->decoration)
     {
        elm_layout_signal_emit(edit->markup, "eflete,sound,clicked", "eflete");
        edit->decoration = true;
     }

   if (count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        edit->selected = eina_stringshare_add(item->sound_name);
        _tone_info_setup(edit, item);
     }
}

static Eina_Bool
_sound_content_init(Sound_Editor *edit)
{
   Eina_List *sounds = NULL, *tones, *l = NULL;
   Item *it = NULL;
   const char* sound_name = NULL;
   Evas_Object *edje_edit_obj = NULL;

   GET_OBJ(edit->pr, edje_edit_obj);

   sounds = edje_edit_sound_samples_list_get(edje_edit_obj);
   tones = edje_edit_sound_tones_list_get(edje_edit_obj);

   it = (Item *)mem_calloc(1, sizeof(Item));
   it->sound_name = eina_stringshare_add(_("Sound Samples"));
   elm_gengrid_item_append(edit->gengrid, ggic, it, NULL, edit);

   if (sounds)
     {
        EINA_LIST_FOREACH(sounds, l, sound_name)
          {
             it = (Item *)mem_calloc(1, sizeof(Item));
             it->sound_name = eina_stringshare_add(sound_name);
             it->comp = edje_edit_sound_compression_type_get(edje_edit_obj, it->sound_name);
             it->rate = edje_edit_sound_compression_rate_get(edje_edit_obj, it->sound_name);
             elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_sample, edit);
          }
        eina_list_free(sounds);
     }

   it = (Item *)mem_calloc(1, sizeof(Item));
   it->sound_name = eina_stringshare_add(_("Sound Tones"));
   edit->tone = elm_gengrid_item_append(edit->gengrid, ggic, it, NULL, edit);

   if (tones)
     {
        EINA_LIST_FOREACH(tones, l, sound_name)
          {
             it = (Item *)mem_calloc(1, sizeof(Item));
             it->sound_name = eina_stringshare_add(sound_name);
             it->tone_frq = edje_edit_sound_tone_frequency_get(edje_edit_obj, sound_name);
             elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_tone, edit);
          }
        eina_list_free(tones);
     }

   return true;
}

static void
_create_gengrid(Evas_Object *parent,
                Sound_Editor *editor,
                Sound_Editor_Mode mode)
{
   Evas_Object *bg;

   editor->gengrid = elm_gengrid_add(parent);
   elm_gengrid_item_size_set(editor->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_group_item_size_set(editor->gengrid, ITEM_HEIGHT/5, ITEM_HEIGHT/5);
   elm_gengrid_align_set(editor->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(editor->gengrid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   GET_IMAGE(bg, editor->gengrid, "gallery-bg");
   elm_object_part_content_set(editor->gengrid, "elm.swallow.background", bg);
   evas_object_show(bg);

   if (mode == SOUND_EDITOR_SINGLE)
     elm_gengrid_multi_select_set(editor->gengrid, false);
   else
     elm_gengrid_multi_select_set(editor->gengrid, true);

   evas_object_size_hint_weight_set(editor->gengrid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(editor->gengrid, EVAS_HINT_FILL, EVAS_HINT_FILL);

   gic = elm_gengrid_item_class_new();
   gic->item_style = DEFAULT_STYLE;
   gic->func.text_get = _grid_label_get;
   gic->func.content_get = _grid_content_get;
   gic->func.del = _grid_del;

   ggic = elm_gengrid_item_class_new();
   ggic->item_style = "group_index";
   ggic->func.text_get = _grid_label_get;
   ggic->func.content_get = NULL;
   ggic->func.del = NULL;

   if (!_sound_content_init(editor))
     {
        _sound_editor_del(editor);
        ERR("Filed initialize sound editor");
     }

   evas_object_show(editor->gengrid);
   elm_object_part_content_set(editor->markup, "gengrid", editor->gengrid);
}

static inline Evas_Object *
_sound_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;
   ENTRY_ADD(parent, entry, true, "eflete/search_field");
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_ADD(entry, icon, true, "icon-search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static void
_on_sample_done(void *data,
                Evas_Object *obj,
                void *event_info)
{
   Item *it = NULL;
   Evas_Object *edje_edit_obj = NULL;
   const char *selected = event_info;
   const char *sound_name;
   Elm_Object_Item *new_item;

   if ((!data) || (!selected) || (!strcmp(selected, "")))
     {
        ecore_main_loop_quit();
        return;
     }

   Sound_Editor *edit = (Sound_Editor *)data;

   GET_OBJ(edit->pr, edje_edit_obj);

   if ((ecore_file_exists(selected)) && (!ecore_file_is_dir(selected)))
     {
        sound_name = ecore_file_file_get(selected);
        if (!edje_edit_sound_sample_add(edje_edit_obj, sound_name, selected))
          {
             WIN_NOTIFY_ERROR(obj,
                              _("Error while loading file.<br>"
                                "Please check if file is sound"
                                "or/and file is accessible."));
             return;
          }
        else
          {
             it = (Item *)mem_calloc(1, sizeof(Item));
             it->sound_name = eina_stringshare_add(sound_name);
             it->comp = edje_edit_sound_compression_type_get(edje_edit_obj, it->sound_name);
             new_item = elm_gengrid_item_insert_before(edit->gengrid, gic, it, edit->tone,
                                                       _grid_sel_sample, edit);
             edit->pr->added_sounds = eina_list_append(edit->pr->added_sounds,
                                                       eina_stringshare_add(sound_name));
             elm_gengrid_item_selected_set(new_item, true);
             pm_project_changed(edit->pr);
          }
     }
   else
     {
        WIN_NOTIFY_ERROR(obj, _("Error while loading file.<br>File is not exist"));
        return;
     }

   ecore_main_loop_quit();
}

static void
_cancel_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   evas_object_del(data);
}

static void
_popup_close(void *data)
{
   evas_object_del(data);
}

static void
_add_ok_clicked(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Evas_Object *edje_edit_obj;
   Item *it;
   const char *str_name, *str_value;
   Elm_Object_Item *new_item;
   int frq;

   Sound_Editor *edit = (Sound_Editor *)data;
   if (elm_entry_is_empty(edit->tone_entry))
     {
        NOTIFY_WARNING(_("Tone name can not be empty!"))
        return;
     }
   if (!ewe_entry_regex_check(edit->frq_entry))
     {
        NOTIFY_WARNING(_("Frequency value is not correct!"));
        return;
     }

   str_name = elm_entry_entry_get(edit->tone_entry);
   str_value = elm_entry_entry_get(edit->frq_entry);
   frq = atoi(str_value);

   GET_OBJ(edit->pr, edje_edit_obj);

   if (!edje_edit_sound_tone_add(edje_edit_obj, str_name, frq))
     {
        WIN_NOTIFY_ERROR(obj, _("Error while add new tone<br>"));
        ecore_job_add(_popup_close, edit->popup);
        return;
     }
   else
     {
        it = (Item *)mem_calloc(1, sizeof(Item));
        it->sound_name = eina_stringshare_add(str_name);
        it->tone_frq = frq;
        new_item = elm_gengrid_item_append(edit->gengrid, gic, it, _grid_sel_tone, edit);
        elm_gengrid_item_selected_set(new_item, true);
        pm_project_changed(app_data_get()->project);
     }

   ecore_job_add(_popup_close, edit->popup);
}

static void
_on_cmb_sel(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info)
{
   Sound_Editor *edit = data;
   Ewe_Combobox_Item *selected_item = event_info;

   if (!selected_item->index)
     {
        Evas_Object *bg, *fs, *win;
        MODAL_WINDOW_ADD(win, main_window_get(), _("Add sound to library"),
                         _on_sample_done, NULL);
        bg = elm_bg_add(win);
        evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_show(bg);
        elm_win_resize_object_add(win, bg);

        FILESELECTOR_ADD(fs, win, _on_sample_done, data);
        elm_win_resize_object_add(win, fs);

        ecore_main_loop_begin();
        evas_object_del(win);

        return;
     }
   else
     {
        Evas_Object *popup, *box, *item, *bt_yes, *bt_no;
        Eina_Stringshare *title;
        popup = elm_popup_add(edit->win);
        elm_object_style_set(popup, "eflete");
        title = eina_stringshare_add(_("Add new tone to the project"));
        elm_object_part_text_set(popup, "title,text", title);
        elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);
        edit->popup = popup;

        BOX_ADD(popup, box, false, false);
        elm_object_content_set(popup, box);
        ITEM_ADD(box, item, _("tone name:"), "eflete/image_editor/item/default");
        EWE_ENTRY_ADD(item, edit->tone_entry, true, DEFAULT_STYLE);
        elm_object_part_text_set(edit->tone_entry, "guide", _("Type a new tone name"));
        elm_object_part_content_set(item, "elm.swallow.content", edit->tone_entry);
        elm_box_pack_end(box, item);

        ITEM_ADD(box, item, _("frequency:"), "eflete/image_editor/item/default");
        EWE_ENTRY_ADD(item, edit->frq_entry, true, DEFAULT_STYLE);
        elm_entry_markup_filter_append(edit->frq_entry, elm_entry_filter_accept_set,
                                       &accept_value);
        elm_object_part_text_set(edit->frq_entry, "guide",
                                 _("Type a frequency (20 - 20000)"));
        ewe_entry_regex_set(edit->frq_entry, FLOAT_NUMBER_REGEX, EWE_REG_EXTENDED);
        ewe_entry_regex_autocheck_set(edit->frq_entry, true);
        ewe_entry_regex_glow_set(edit->frq_entry, true);
        elm_object_part_content_set(item, "elm.swallow.content", edit->frq_entry);
        elm_box_pack_end(box, item);

        BUTTON_ADD(popup, bt_yes, _("Add"));
        evas_object_smart_callback_add (bt_yes, "clicked", _add_ok_clicked, edit);
        elm_object_part_content_set(popup, "button1", bt_yes);

        BUTTON_ADD(popup, bt_no, _("Cancel"));
        evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, popup);
        elm_object_part_content_set(popup, "button2", bt_no);

        evas_object_show(popup);
        eina_stringshare_del(title);

        return;
     }
}

static void
_on_delete_clicked_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *edje_edit_obj;
   Elm_Object_Item *grid_it;
   Item *item;
   Eina_List *list, *l, *l_next;
   int selected, deleted = 0;
   Sound_Editor *edit = (Sound_Editor *)data;

   GET_OBJ(edit->pr, edje_edit_obj);

   list = (Eina_List *)elm_gengrid_selected_items_get(edit->gengrid);
   selected = eina_list_count(list);

   if ((!list) || (!selected))
     {
        NOTIFY_WARNING("No selected items");
        return;
     }

   EINA_LIST_FOREACH_SAFE(list, l, l_next, grid_it)
     {
        item = elm_object_item_data_get(grid_it);
        if (!item->tone_frq)
          {
             if (edje_edit_sound_sample_del(edje_edit_obj, item->sound_name))
               {
                  elm_object_item_del(grid_it);
                  deleted++;
               }
          }
        else
          {
             if (edje_edit_sound_tone_del(edje_edit_obj, item->sound_name))
               {
                  elm_object_item_del(grid_it);
                  deleted++;
               }
          }
   }

   if (deleted)
     pm_project_changed(app_data_get()->project);
}

ITEM_SEARCH_FUNC(gengrid)

static void
_search_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Sound_Editor *edit = data;
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
   search_data->last_item_found = NULL;
}

#define ADD_BUTTON(PARENT, OBJ, TEXT, CLB) \
   OBJ = elm_button_add(PARENT); \
   evas_object_size_hint_weight_set(OBJ, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(OBJ, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_style_set(OBJ, "eflete/btn"); \
   elm_object_text_set(OBJ, TEXT); \
   evas_object_show(OBJ); \
   elm_object_part_content_set(PARENT, "swallow.btn."TEXT, OBJ); \
   evas_object_smart_callback_add(OBJ, "clicked", CLB, edit);

static void
_sound_editor_main_markup_create(Sound_Editor *edit)
{
   Evas_Object *btn, *icon, *search;

   edit->markup = elm_layout_add(edit->win);
   elm_layout_file_set(edit->markup, EFLETE_EDJ, "eflete/sound_editor/default");
   evas_object_size_hint_weight_set(edit->markup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(edit->markup);
   elm_win_inwin_content_set(edit->win, edit->markup);

   ADD_BUTTON(edit->markup, btn, "Apply", _on_ok_cb);

   ADD_BUTTON(edit->markup, btn, "Close", _on_quit_cb);

   ADD_BUTTON(edit->markup, btn, "del", _on_delete_clicked_cb);
   ICON_ADD(btn, icon, true, "icon-remove");
   elm_object_content_set(btn, icon);
   elm_object_text_set(btn, NULL);

   EWE_COMBOBOX_ADD(edit->markup, edit->cmb);
   ewe_combobox_style_set(edit->cmb, "small/default");
   ewe_combobox_item_add(edit->cmb, _("Sample"));
   ewe_combobox_item_add(edit->cmb, _("Tone"));
   evas_object_smart_callback_add(edit->cmb, "selected", _on_cmb_sel, edit);
   elm_object_part_content_set(edit->markup, "swallow.btn.add", edit->cmb);

   search = _sound_editor_search_field_create(edit->markup);
   elm_object_part_content_set(edit->markup, "swallow.search_area", search);
   evas_object_smart_callback_add(search, "changed", _search_changed, edit);
   evas_object_smart_callback_add(search, "activated", _search_nxt_gd_item, edit);
   evas_object_smart_callback_add(edit->gengrid, "pressed", _search_reset_cb,
                                  &(edit->sound_search_data));
   edit->sound_search_data.search_entry = search;
   edit->sound_search_data.last_item_found = NULL;
}

#undef ADD_BUTTON

Evas_Object *
sound_editor_window_add(Project *project, Sound_Editor_Mode mode)
{
   Sound_Editor *edit;

   if (!project)
     {
        ERR("Project is not opened");
        return NULL;
     }

   ecore_audio_init();

   edit = (Sound_Editor *)mem_calloc(1, sizeof(Sound_Editor));
   edit->pr = project;
   edit->win = mw_add(_on_quit_cb, edit);
   mw_title_set(edit->win, _("Sound editor"));

   _sound_editor_main_markup_create(edit);

   _create_gengrid(edit->markup, edit, mode);

   _sound_info_create(edit->markup, edit);

   _sound_player_create(edit->markup, edit);

   evas_object_show(edit->win);
   return edit->win;
}

#undef ITEM_WIDTH
#undef ITEM_HEIGHT

