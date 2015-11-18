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
   Evas_Object *sample_box;
   Evas_Object *tone_box;

   struct {
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
};
typedef struct _Sound_Prop_Data Sound_Prop_Data;

#define INFO_ADD(PARENT, ITEM, TEXT, STYLE) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_theme_set(ITEM, "layout", "sound_editor", STYLE); \
   elm_object_part_text_set(ITEM, "elm.text", TEXT); \
   evas_object_show(ITEM);

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
_sample_info_create(Evas_Object *parent, Sound_Prop_Data *edit)
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
_tone_info_create(Evas_Object *parent, Sound_Prop_Data *edit)
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

#undef INFO_ADD

static void
_sound_info_create(Evas_Object *parent, Sound_Prop_Data *edit)
{
   assert(parent != NULL);
   assert(edit != NULL);

   _sample_info_create(parent, edit);
   _tone_info_create(parent, edit);
}

static void
_on_sound_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Sound_Type sound_type = (Sound_Type)event_info;

   Evas_Object *property = data;
   SOUND_PROP_DATA_GET()

   switch (sound_type)
     {
      case SOUND_TYPE_SAMPLE:
        {
           evas_object_show(pd->sample_box);
           evas_object_hide(pd->tone_box);

           elm_box_unpack(pd->box, pd->tone_box);
           elm_box_pack_end(pd->box, pd->sample_box);
        }
      break;
      case SOUND_TYPE_TONE:
        {
           evas_object_hide(pd->sample_box);
           evas_object_show(pd->tone_box);

           elm_box_unpack(pd->box, pd->sample_box);
           elm_box_pack_end(pd->box, pd->tone_box);
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

   pd->sound_player = elm_layout_add(parent);
   elm_layout_theme_set(pd->sound_player, "layout", "sound_editor", "player");
   elm_box_pack_end(pd->box, pd->sound_player);
   evas_object_size_hint_weight_set(pd->sound_player, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->sound_player, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(pd->sound_player);

   _sound_info_create(parent, pd);

   evas_object_data_set(pd->box, SOUND_PROP_DATA, pd);

   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_SELECTED, _on_sound_selected, pd->box);

   return pd->box;
}