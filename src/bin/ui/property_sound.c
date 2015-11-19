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

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "string_common.h"
#include "new_history.h"
#include "editor.h"
#include "sound_editor.h"
#include "validator.h"

#include "signals.h"

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
   Evas_Object *box;
   Evas_Object *sound_player;
   Evas_Object *preview_sound_player;
   Evas_Object *sound_player_frame;

   Evas_Object *sample_box;
   Evas_Object *tone_box;
   Evas_Object *info_frame;

   struct {
      Evas_Object *check;
      Evas_Object *rewind;
      Evas_Object *play;
      Evas_Object *pause;
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

/* accroding to Edje_Edit.h */
static const char *edje_sound_compression[] = { N_("RAW"),
                                                N_("COMP"),
                                                N_("LOSSY"),
                                                N_("AS_IS"),
                                                NULL};

static Evas_Object *
prop_sound_editor_compression_type_add(Evas_Object *property, Sound_Prop_Data *pd);

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
_play_sound(Sound_Prop_Data *edit EINA_UNUSED)
{
}

static void
_on_play_cb(void *data,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   _play_sound(data);
}

static void
_on_prev_cb(void *data EINA_UNUSED,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
}

static void
_on_next_cb(void *data EINA_UNUSED,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
}

static void
_on_pause_cb(void *data EINA_UNUSED,
             Evas_Object *obj EINA_UNUSED,
             void *event_info EINA_UNUSED)
{
}

static void
_on_rewind_cb(void *data EINA_UNUSED,
              Evas_Object *obj EINA_UNUSED,
              void *event_info EINA_UNUSED)
{
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
_on_sound_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Selected_Sound_Data *snd_data = (Selected_Sound_Data *)event_info;

   Evas_Object *property = data;
   SOUND_PROP_DATA_GET()

   switch (snd_data->sound_type)
     {
      case SOUND_TYPE_SAMPLE:
        {
           _sample_info_update(pd, snd_data);
        }
      break;
      case SOUND_TYPE_TONE:
        {
           _tone_info_update(pd, snd_data);
        }
      break;
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

   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_SELECTED, _on_sound_selected, pd->box);

   return pd->box;
}