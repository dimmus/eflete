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

#include "live_view_prop.h"
#include "live_widgets.h"

static void
_on_swallow_check(void *data,
                  Evas_Object *obj,
                  void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *check = NULL, *ch;
   Eina_List *item_list = NULL;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        if (edje_object_part_swallow_get(object, part_name))
          return;
        rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, RECT_COLOR);
        edje_object_part_swallow(object, part_name, rect);
        item_list = elm_box_children_get(pd->prop_swallow.swallows);
        EINA_LIST_FREE(item_list, ch)
          {
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
     }
   else
     {
        rect = edje_object_part_swallow_get(object, part_name);
        if (!rect) return;
        edje_object_part_unswallow(object, rect);
        evas_object_del(rect);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

static void
_on_text_check(void *data,
               Evas_Object *obj,
               void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *ch;
   Eina_List *item_list = NULL;
   Eina_Bool all_checks = true;
   const char *default_text;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        default_text = edje_object_part_text_get(object, part_name);
        if (default_text)
          eina_hash_add(pd->prop_text.default_text, part_name, eina_stringshare_add(default_text));
        edje_object_part_text_set(object, part_name,
                                  _("Look at it! This is absolutely and totally text"));
        item_list = elm_box_children_get(pd->prop_text.texts);

        EINA_LIST_FREE(item_list, ch)
          {
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
     }
   else
     {
        default_text = eina_hash_find(pd->prop_text.default_text, part_name);
        eina_hash_del(pd->prop_text.default_text, part_name, NULL);
        edje_object_part_text_set(object, part_name, default_text);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

static void
_on_swallow_clean(const char *part_name, Evas_Object *object)
{
   Evas_Object *rect = NULL;

   assert(part_name != NULL);
   assert(object != NULL);

   rect = edje_object_part_swallow_get(object, part_name);
   edje_object_part_unswallow(object, rect);
   evas_object_del(rect);
}

Evas_Object *
layout_prog_edit_create(Evas_Object *parent)
{
   assert(parent != NULL);

   Evas_Object *object = edje_edit_object_add(parent);

   assert(object != NULL);

   evas_object_data_set(object, SWALLOW_FUNC, _on_swallow_check);
   evas_object_data_set(object, SWALLOW_CLEAN_FUNC, _on_swallow_clean);
   evas_object_data_set(object, TEXT_FUNC, _on_text_check);

   return object;
}
