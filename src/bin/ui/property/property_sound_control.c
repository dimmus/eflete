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

#include "property.h"
#include "property_private.h"
#include "main_window.h"

static Eina_Bool playing = false;

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

static void
_on_rewind_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
//   double value = elm_slider_value_get(edit->player_data.rewind);
//   value = ecore_audio_obj_in_seek(edit->io.in, value, SEEK_SET);
}
static void
_on_play_cb(void *data __UNUSED__,
           Evas_Object *obj,
           void *event_info __UNUSED__)
{
   Evas_Object *icon = elm_object_part_content_get(obj, NULL);

   if (!playing)
     {
        elm_icon_standard_set(icon, "media_player/pause");
        playing = true;
     }
   else
     {
        elm_icon_standard_set(icon, "media_player/play");
        playing = false;
     }
}

Evas_Object *
property_player_control_add(Evas_Object *parent)
{
   Evas_Object *control, *rewind, *play, *icon;

   assert(parent != NULL);

   control = elm_layout_add(parent);
   elm_layout_theme_set(control, "layout", "sound_editor", "player");
   evas_object_size_hint_weight_set(control,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(control, EVAS_HINT_FILL, EVAS_HINT_FILL);

   rewind = elm_slider_add(control);
   elm_slider_unit_format_set(rewind, "%2.0f");
   elm_slider_units_format_function_set(rewind, _player_units_format,
                                        _player_units_free);
   evas_object_size_hint_align_set(rewind, EVAS_HINT_FILL, 0.5);
   evas_object_size_hint_weight_set(rewind, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(rewind);
   elm_object_part_content_set(control, "eflete.swallow.fast", rewind);
   evas_object_smart_callback_add(rewind, "changed", _on_rewind_cb, NULL);

   BT_ADD(control, play, icon, "media_player/play");
   elm_object_part_content_set(rewind, NULL, play);
   evas_object_smart_callback_add(play, "clicked", _on_play_cb, NULL);

   return control;
}
