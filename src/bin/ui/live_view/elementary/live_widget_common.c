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

#include "live_elementary_widgets.h"

Eina_Bool
standard_widget_name_parse(const char *full_group_name,
                           Eina_Stringshare **widget,
                           Eina_Stringshare **class,
                           Eina_Stringshare **style_name)
{
   char **c;
   unsigned int count;

   assert(full_group_name != NULL);

   c = eina_str_split_full(full_group_name, "/", 4, &count);

   if (count < 4)
     {
        if (widget) *widget = NULL;
        if (class) *class = NULL;
        if (style_name) *style_name = NULL;

        free(c[0]);
        free(c);
        return false;
     }

   if (widget) *widget = eina_stringshare_add(c[1]);
   if (class) *class = eina_stringshare_add(c[2]);
   if (style_name) *style_name = eina_stringshare_add(c[3]);

   free(c[0]);
   free(c);
   return true;
}

void
on_swallow_check(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *) data;
   Evas_Object *content = NULL;

   int content_type = part->swallow_content;
   int widget_type = part->widget;

   if (part->change)
     {
        /* if NONE - delete object */
        if ((content_type == CONTENT_NONE) && (part->object))
          {
             content = elm_object_part_content_unset(object, part->name);
             evas_object_del(content);
             content = NULL;
             part->object = NULL;
          }

        if (content_type == CONTENT_RECTANGLE) /* create rect */
          {
             content = evas_object_rectangle_add(object);
          }
        else if ((content_type == CONTENT_IMAGE) && (part->image_path)) /* create picture */
          {
             content = elm_image_add(object);
             elm_image_file_set(content, part->image_path, NULL);
          }
        else if (content_type == CONTENT_WIDGET) /* create widget */
          {
             switch (widget_type)
               {
                case WIDGET_BUTTON:
                   content = elm_button_add(object);
                   elm_object_text_set(content, _("User Text"));
                   break;
                case WIDGET_CHECK:
                   content = elm_check_add(object);
                   elm_object_text_set(content, _("User Text"));
                   elm_check_state_set(content, true);
                   break;
                case WIDGET_SLIDER:
                   content = elm_slider_add(object);
                   elm_slider_unit_format_set(content, "%1.2f units");
                   elm_slider_indicator_format_set(content, "%1.2f");
                   elm_slider_min_max_set(content, 0, 10);
                   break;
                case WIDGET_ENTRY:
                   content = elm_entry_add(object);
                   elm_entry_single_line_set(content, false);
                   break;
                case WIDGET_PROGRESSBAR:
                   content = elm_progressbar_add(object);
                   break;
               }
          }

        part->object = content;
        part->change = false;
        elm_object_part_content_set(object, part->name, content);
     }

   if (part->object)
     {
        evas_object_color_set(content,
                              part->r,
                              part->g,
                              part->b,
                              part->a);

        evas_object_size_hint_min_set(content,
                                      part->min_w,
                                      part->min_h);
        evas_object_size_hint_max_set(content,
                                      part->max_w,
                                      part->max_h);
     }
}

void
on_text_check(void *data,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *) data;

   elm_object_part_text_set(object, part->name, part->text_content);
}

void
send_signal(void *data,
             Evas_Object *obj __UNUSED__,
             void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Evas_Object *object = (Evas_Object *)data;

   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   elm_layout_signal_emit(object, sig->sig_name, sig->source_name);
}
