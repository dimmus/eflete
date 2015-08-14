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

#include "config.h"
#include "ui_block.h"
#include "ui_signal_list.h"
#include "ui_states_list.h"

struct _Window_Attributes
{
   int width;
   int height;
};
typedef struct _Window_Attributes Window_Attributes;

/*
   Store main panes in global pointer. This need for have access to all panes.
 */
Evas_Object *panes;

static void
_double_click_up_cb(void * data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info __UNUSED__)
{
   static volatile double _size = 0.0;
   if (elm_panes_content_left_size_get(obj) > 0.0)
     {
        _size = elm_panes_content_left_size_get(obj);
        elm_panes_content_left_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.3;
        elm_panes_content_left_size_set(obj, _size);
     }

}

static void
_double_click_left_panes_down_cb(void * data __UNUSED__,
                                 Evas_Object *obj,
                                 void *event_info __UNUSED__)
{
   static volatile double _size = 0.0;
   if (elm_panes_content_right_size_get(obj) > 0.0)
     {
        _size = elm_panes_content_right_size_get(obj);
        elm_panes_content_right_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.4;
        elm_panes_content_right_size_set(obj, _size);
     }
}

static void
_double_click_center_panes_down_cb(void * data __UNUSED__,
                                   Evas_Object *obj,
                                   void *event_info __UNUSED__)
{
   static volatile double _size = 0.0;
   if (elm_panes_content_right_size_get(obj) > 0.0)
     {
        _size=elm_panes_content_right_size_get(obj);
        elm_panes_content_right_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.2;
        elm_panes_content_right_size_set(obj, _size);
     }
}

#define PANES_MIN_SIZE_LEFT  300
#define PANES_MIN_SIZE_RIGHT 385

Eina_Bool
ui_panes_left_panes_min_size_toggle(Eina_Bool is_on)
{
   assert(ap != NULL);
   assert(ap->panes.left != NULL);

   elm_panes_content_left_min_size_set(ap->panes.left,
                                       is_on ? PANES_MIN_SIZE_LEFT : 0);
   elm_panes_content_right_min_size_set(ap->panes.left,
                                        is_on ? PANES_MIN_SIZE_RIGHT : 0);
   return true;
}

#undef PANES_MIN_SIZE_LEFT
#undef PANES_MIN_SIZE_RIGHT

static void
_on_discard_changes_selected(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   assert(ap != NULL);
   assert(ap->project != NULL);
   assert(ap->project->current_style != NULL);

   if (history_diff_count_get(ap->project->current_style->obj) > 0)
     history_undo(ap->project->current_style->obj,
                  history_diff_count_get(ap->project->current_style->obj));
}


Eina_Bool
ui_panes_add(void)
{
   Config *config;
   Evas_Object *block;
   Evas_Object *panes_left, *panes_left_hor, *panes_right;
   Evas_Object *panes_center, *panes_right_hor;
   Ewe_Tabs_Item *tab_item;

   assert(ap != NULL);
   assert(ap->win_layout != NULL);

   config = config_get();

   panes_left = elm_panes_add(ap->win_layout);
   evas_object_size_hint_weight_set(panes_left, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left, config->panes.left);
   elm_object_part_content_set(ap->win_layout, "eflete.swallow.panes", panes_left);
   panes = panes_left;

   panes_left_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_left_hor, true);
   evas_object_size_hint_weight_set(panes_left_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left_hor, config->panes.left_hor);
   elm_object_part_content_set(panes_left, "left", panes_left_hor);

   panes_right = elm_panes_add(ap->win_layout);
   evas_object_size_hint_weight_set(panes_right, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_right, config->panes.right);
   elm_panes_content_right_min_size_set(panes_right, 385);
   elm_object_part_content_set(panes_left, "right", panes_right);

   panes_right_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_right_hor, true);
   evas_object_size_hint_weight_set(panes_right_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_right_hor, config->panes.right_hor);
   elm_object_part_content_set(panes_right, "right", panes_right_hor);

   panes_center = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_center, true);
   evas_object_size_hint_weight_set(panes_center, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_center, config->panes.center);
   elm_object_part_content_set(panes_right, "left", panes_center);

   ap->panes.left = panes_left;
   ap->panes.right = panes_right;
   ap->panes.left_hor = panes_left_hor;
   ap->panes.right_hor = panes_right_hor;
   ap->panes.center = panes_center;

   ui_panes_left_panes_min_size_toggle(true);

   evas_object_smart_callback_add(panes_right_hor, "clicked,double",
                                  _double_click_up_cb, NULL);
   evas_object_smart_callback_add(panes_left_hor, "clicked,double",
                                  _double_click_left_panes_down_cb, NULL);
   evas_object_smart_callback_add(panes_center, "clicked,double",
                                  _double_click_center_panes_down_cb, NULL);

   block = ui_block_add(ap->win_layout);
   elm_layout_text_set(block, "elm.text", _("Live View"));
   elm_object_part_content_set(panes_center, "right", block);
   evas_object_show(block);
   ap->block.bottom_right = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_visible(block, false);
   elm_object_part_content_set(panes_center, "left", block);
   evas_object_show(block);
   ap->block.canvas = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_visible(block, false);
   elm_object_part_content_set(panes_left_hor, "left", block);
   evas_object_show(block);
   ap->block.left_top = block;

   /* add tab with the signal and signal lists */
   ap->block.left_bottom = ewe_tabs_add(ap->win_layout);
   tab_item = ewe_tabs_item_append(ap->block.left_bottom, NULL, _("Part states"), NULL);
   ap->block.state_list = ui_states_list_add(ap->win_layout);
   ewe_tabs_item_content_set(ap->block.left_bottom, tab_item, ap->block.state_list);

   tab_item = ewe_tabs_item_append(ap->block.left_bottom, NULL, _("Using signals"), NULL);
   ap->block.signal_list = ui_signal_list_add(ap->win_layout);
   ewe_tabs_item_content_set(ap->block.left_bottom, tab_item, ap->block.signal_list);
   elm_layout_content_set(ap->panes.left_hor, "right", ap->block.left_bottom);
   evas_object_show(ap->block.left_bottom);

   TODO("STOP! it's last changes in this archaic module. All this module must be refactored, and merget to main window!!!!")
   block = ui_block_add(ap->win_layout);
   elm_layout_text_set(block, "elm.text", _("History"));
   elm_object_part_content_set(panes_right_hor, "left", block);
   ap->block.right_top_btn = elm_button_add(block);
   elm_object_text_set(ap->block.right_top_btn, _("Discard"));
   elm_object_disabled_set(ap->block.right_top_btn, true);
   evas_object_smart_callback_add(ap->block.right_top_btn, "clicked", _on_discard_changes_selected, (void*)ap);
   evas_object_show(ap->block.right_top_btn);
   elm_object_part_content_set(block, "elm.swallow.title", ap->block.right_top_btn);
   elm_object_signal_emit(block, "title,content,show", "eflete");
   evas_object_show(block);
   ap->block.right_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_visible(block, false);
   elm_object_part_content_set(panes_right_hor, "right", block);
   evas_object_show(block);
   ap->block.right_bottom = block;

   return true;
}

Eina_Bool
ui_panes_show(void)
{
   assert(ap != NULL);
   assert(ap->win_layout != NULL);

   elm_object_signal_emit(ap->win_layout, "window,panes,show", "eflete");
   return true;
}

Eina_Bool
ui_panes_hide(void)
{
   assert(ap != NULL);
   assert(ap->win_layout != NULL);

   elm_object_signal_emit(ap->win_layout, "window,panes,hide", "eflete");
   return true;
}
