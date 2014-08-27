/**
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

Eina_Bool
ui_panes_add(App_Data *ap)
{
   Config *config;
   Evas_Object *block;
   Evas_Object *panes_left, *panes_left_hor, *panes_right;
   Evas_Object *panes_center, *panes_center_down, *panes_right_hor;
   if ((!ap) || (!ap->win_layout))
     {
        ERR("Can't create the panes. Application Data is NULL");
        return false;
     }

   config = config_get();

   panes_left = elm_panes_add(ap->win_layout);
   elm_object_style_set(panes_left, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_left, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left, config->panes.left);
   elm_panes_content_left_min_size_set(panes_left, 300);
   elm_panes_content_right_min_size_set(panes_left, 385);
   elm_object_part_content_set(ap->win_layout, "eflete.swallow.panes", panes_left);
   panes = panes_left;

   panes_left_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_left_hor, true);
   elm_object_style_set(panes_left_hor, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_left_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left_hor, config->panes.left_hor);
   elm_object_part_content_set(panes_left, "left", panes_left_hor);

   panes_right = elm_panes_add(ap->win_layout);
   elm_object_style_set(panes_right, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_right, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_right, config->panes.right);
   elm_panes_content_right_min_size_set(panes_right, 385);
   elm_object_part_content_set(panes_left, "right", panes_right);

   panes_right_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_right_hor, true);
   elm_object_style_set(panes_right_hor, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_right_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_right_hor, config->panes.right_hor);
   elm_object_part_content_set(panes_right, "right", panes_right_hor);

   panes_center = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(panes_center, true);
   elm_object_style_set(panes_center, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_center, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_center, config->panes.center);
   elm_object_part_content_set(panes_right, "left", panes_center);

   panes_center_down = elm_panes_add(ap->win_layout);
   elm_object_style_set(panes_center_down, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes_center_down, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center_down, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_center_down, config->panes.center_down);
   elm_object_part_content_set(panes_center, "right", panes_center_down);

   ap->panes.left = panes_left;
   ap->panes.right = panes_right;
   ap->panes.left_hor = panes_left_hor;
   ap->panes.right_hor = panes_right_hor;
   ap->panes.center = panes_center;
   ap->panes.center_down = panes_center_down;

   evas_object_smart_callback_add(panes_right_hor, "clicked,double",
                                  _double_click_up_cb, NULL);
   evas_object_smart_callback_add(panes_left_hor, "clicked,double",
                                  _double_click_left_panes_down_cb, NULL);
   evas_object_smart_callback_add(panes_center, "clicked,double",
                                  _double_click_center_panes_down_cb, NULL);


   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Part States"));
   elm_object_part_content_set(panes_center_down, "left", block);

   evas_object_show(block);
   ap->block.bottom_left = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Live View"));
   elm_object_part_content_set(panes_center_down, "right", block);
   evas_object_show(block);
   ap->block.bottom_right = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_center, "left", block);
   evas_object_show(block);
   ap->block.canvas = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_left_hor, "left", block);
   evas_object_show(block);
   ap->block.left_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Using Signals"));
   elm_object_part_content_set(panes_left_hor, "right", block);
   evas_object_show(block);
   ap->block.left_bottom = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_right_hor, "left", block);
   evas_object_show(block);
   ap->block.right_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, _("Property"));
   elm_object_part_content_set(panes_right_hor, "right", block);
   evas_object_show(block);
   ap->block.right_bottom = block;

   return true;
}

Eina_Bool
ui_panes_show(App_Data *ap)
{
   if ((!ap) || (!ap->win_layout))
     {
        ERR("Application Data is missing! Can't emit the signal to win_layout.");
        return false;
     }
   elm_object_signal_emit(ap->win_layout, "window,panes,show", "eflete");
   return true;
}

Eina_Bool
ui_panes_hide(App_Data *ap)
{
   if ((!ap) || (!ap->win_layout))
     {
        ERR("Application Data is missing! Can't emit the signal to win_layout.");
        return false;
     }
   elm_object_signal_emit(ap->win_layout, "window,panes,hide", "eflete");
   return true;
}
