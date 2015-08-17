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

#include "ui_block.h"

static Eina_Bool
ui_block_content_set(Evas_Object *block, Evas_Object *content)
{
   assert(block != NULL);
   assert(content != NULL);

   elm_object_part_content_set(block, "elm.swallow.content", content);
   return EINA_TRUE;

}

static Evas_Object *
ui_block_content_get(Evas_Object *block)
{
   assert(block != NULL);

   return elm_object_part_content_get(block, "elm.swallow.content");
}

Evas_Object *
ui_block_add(Evas_Object *parent)
{
   Evas_Object *block;

   assert(parent != NULL);

   block = elm_layout_add(parent);
   elm_layout_theme_set(block, "layout", "block", "default");

   return block;
}

Eina_Bool
ui_block_title_visible(Evas_Object *block, Eina_Bool vis)
{
   assert(block != NULL);

   if (vis) elm_object_signal_emit(block, "title,show", "eflete");
   else elm_object_signal_emit(block, "title,hide", "eflete");
   return true;
}

Eina_Bool
ui_block_content_visible(Evas_Object *block, Eina_Bool vis)
{
   assert(block != NULL);

   if (vis) elm_object_signal_emit(block, "content,show", "eflete");
   else elm_object_signal_emit(block, "content,hide", "eflete");
   return true;
}

Evas_Object *
ui_block_widget_list_get(void)
{
   assert(ap != NULL);

   return ui_block_content_get(ap->block.left_top);
}

Evas_Object *
ui_block_ws_get(void)
{
   assert(ap != NULL);

   return ui_block_content_get(ap->block.canvas);
}

Evas_Object *
ui_block_signal_list_get(void)
{
   assert(ap != NULL);

   return ui_block_content_get(ap->block.left_bottom);
}

Evas_Object *
ui_block_property_get(void)
{
   assert(ap != NULL);

   //return ui_block_content_get(ap->block.right_bottom);
   return NULL;
}

Evas_Object *
ui_block_history_get(void)
{
   assert(ap != NULL);

   return ui_block_content_get(ap->block.right_top);
}

Evas_Object *
ui_block_live_view_get(void)
{
   assert(ap != NULL);

   return ui_block_content_get(ap->block.bottom_right);
}

Eina_Bool
ui_block_widget_list_set(Evas_Object *content)
{
   assert(ap != NULL);

   return ui_block_content_set(ap->block.left_top, content);
}

Eina_Bool
ui_block_ws_set(Evas_Object *content)
{
   assert(ap != NULL);

   return ui_block_content_set(ap->block.canvas, content);
}

Eina_Bool
ui_block_signal_list_set(Evas_Object *content)
{
   assert(ap != NULL);

   return ui_block_content_set(ap->block.left_bottom, content);
}

Eina_Bool
ui_block_property_set(Evas_Object *content __UNUSED__)
{
   assert(ap != NULL);

   //return ui_block_content_set(ap->block.right_bottom, content);
   return true;
}

Eina_Bool
ui_block_history_set(Evas_Object *content)
{
   assert(ap != NULL);

   return ui_block_content_set(ap->block.right_top, content);
}

Eina_Bool
ui_block_live_view_set(Evas_Object *content)
{
   assert(ap != NULL);

   return ui_block_content_set(ap->block.bottom_right, content);
}
