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

#include "live_elementary_widgets.h"

static void
_on_scroller_swallow_check(void *data,
                        Evas_Object *obj,
                        void *ei __UNUSED__)
{
   Evas_Object *content = NULL;

   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     {
        if (!strcmp(part_name, "elm.swallow.content"))
          {
             content = elm_table_add(object);
             Evas_Object *bt;
             int i, j;

             for (j = 0; j < ELEMENTS_COUNT; j++)
               {
                  for (i = 0; i < ELEMENTS_COUNT; i++)
                    {
                       bt = elm_button_add(content);
                       elm_table_pack(content, bt, i, j, 1, 1);
                       elm_object_text_set(bt, _("Both"));
                       evas_object_show(bt);
                    }
               }
             elm_object_content_set(object, content);
             evas_object_show(content);
          }
        else
          {
             content = evas_object_rectangle_add(object);
             evas_object_color_set(content, HIGHLIGHT_COLOR);
             elm_object_part_content_set(object, part_name, content);
          }
     }
   else
     {
        if (!strcmp(part_name, "elm.swallow.content"))
          content =  elm_object_content_unset(object);
        else
          content = elm_object_part_content_unset(object, part_name);
        evas_object_del(content);
     }
}

Evas_Object *
widget_scroller_create(Evas_Object *parent, const char *class, const char *style __UNUSED__)
{
   Evas_Object *object;

   if ((strcmp(class, "entry") == 0) || (strcmp(class, "entry_single") == 0))
     {
        object = elm_entry_add(parent);
        evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
     }
   else
     {
        object = elm_scroller_add(parent);
        evas_object_data_set(object, SWALLOW_FUNC, _on_scroller_swallow_check);
     }

   elm_entry_scrollable_set(object, true);
   elm_scroller_policy_set(object, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);

   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   return object;
}
