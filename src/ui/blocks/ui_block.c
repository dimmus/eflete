/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#include "ui_block.h"

static void
ui_block_content_set (Evas_Object *block, Evas_Object *content)
{
   if (!block || !content) return;
   elm_object_part_content_set (block, "elm.block.swallow", content);
}

static Evas_Object *
ui_block_content_get (Evas_Object *block)
{
   if (!block) return NULL;
   return elm_object_part_content_get (block, "elm.block.swallow");
}

Evas_Object *
ui_block_add(Evas_Object *parent)
{
   Evas_Object *block;

   if (!parent)
     {
        ERR("Could not create 'ui_block', because parent object is NULL.");
        return NULL;
     }
   block = elm_layout_add(parent);
   elm_layout_file_set(block, TET_THEME, "tet/block/layout");

   return block;
}

void
ui_block_title_visible(Evas_Object *block, Eina_Bool vis)
{
   if (!block)
     {
        ERR("Could not show/hide a title, because a block is NULL.");
        return;
     }
   if (vis) elm_object_signal_emit(block, "title,show", "");
   else elm_object_signal_emit(block, "title,hide", "");
}

void
ui_block_title_text_set(Evas_Object *block, const char *title)
{
   if (!block)
     {
        ERR("Could not set title text, because a block is NULL.");
        return;
     }
   elm_object_part_text_set(block, "text.header", title);
   ui_block_title_visible(block, EINA_TRUE);
}

const char *
ui_block_title_text_get(Evas_Object *block)
{
   if(!block)
     {
        ERR("Could not get title text, becouse a block is NULL");
        return NULL;
     }

   return elm_object_part_text_get(block, "text.header");
}

Evas_Object *
ui_block_widget_list_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.left_top);
}

Evas_Object *
ui_block_signal_list_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.left_bottom);
}

Evas_Object *
ui_block_state_list_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.bottom_left);
}

Evas_Object *
ui_block_property_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.right_bottom);
}

Evas_Object *
ui_block_graph_vision_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.right_top);
}

Evas_Object *
ui_block_demo_view_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.bottom_right);
}

void
ui_block_widget_list_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.left_top, content);
}

void
ui_block_signal_list_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.left_bottom, content);
}

void
ui_block_state_list_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.bottom_left, content);
}

void
ui_block_property_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.right_bottom, content);
}

void
ui_block_graph_vision_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.right_top, content);
}

void
ui_block_demo_view_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.bottom_right, content);
}
