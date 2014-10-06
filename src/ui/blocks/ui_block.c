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
   if ((!block) || (!content)) return EINA_FALSE;
   elm_object_part_content_set(block, "elm.swallow.content", content);
   return EINA_TRUE;

}

static Evas_Object *
ui_block_content_get(Evas_Object *block)
{
   if (!block) return NULL;
   return elm_object_part_content_get(block, "elm.swallow.content");
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
   elm_layout_file_set(block, EFLETE_EDJ, "eflete/block/layout");

   return block;
}

Eina_Bool
ui_block_title_visible(Evas_Object *block, Eina_Bool vis)
{
   if (!block)
     {
        ERR("Could not show/hide a title, because a block is NULL.");
        return false;
     }
   if (vis) elm_object_signal_emit(block, "title,show", "eflete");
   else elm_object_signal_emit(block, "title,hide", "eflete");
   return true;
}

Eina_Bool
ui_block_content_visible(Evas_Object *block, Eina_Bool vis)
{
   if (!block)
     {
        ERR("Could not show/hide content, because a block is NULL.");
        return false;
     }
   if (vis) elm_object_signal_emit(block, "content,show", "eflete");
   else elm_object_signal_emit(block, "content,hide", "eflete");
   return true;
}

Eina_Bool
ui_block_title_text_set(Evas_Object *block, const char *title)
{
   if (!block)
     {
        ERR("Could not set title text, because a block is NULL.");
        return false;
     }
   elm_object_part_text_set(block, "elm.text.title", title);
   ui_block_title_visible(block, EINA_TRUE);
   return true;
}

const char *
ui_block_title_text_get(Evas_Object *block)
{
   if (!block)
     {
        ERR("Could not get title text, becouse a block is NULL");
        return NULL;
     }

   return elm_object_part_text_get(block, "elm.text.title");
}

Evas_Object *
ui_block_widget_list_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.left_top);
}

Evas_Object *
ui_block_ws_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.canvas);
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
ui_block_live_view_get(App_Data *ap)
{
   return ui_block_content_get(ap->block.bottom_right);
}

Eina_Bool
ui_block_widget_list_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.left_top, content);
}

Eina_Bool
ui_block_ws_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.canvas, content);
}

Eina_Bool
ui_block_signal_list_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.left_bottom, content);
}

Eina_Bool
ui_block_state_list_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.bottom_left, content);
}

Eina_Bool
ui_block_property_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.right_bottom, content);
}

Eina_Bool
ui_block_graph_vision_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.right_top, content);
}

Eina_Bool
ui_block_live_view_set(App_Data *ap, Evas_Object *content)
{
   return ui_block_content_set(ap->block.bottom_right, content);
}
