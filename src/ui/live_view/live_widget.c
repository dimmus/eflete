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

#include "live_widget.h"
#include "live_elementary_widgets.h"

struct _live_widget_item {
     const char *name;
     Evas_Object * (*func)(Evas_Object *parent,
                  const char  *class,
                  const char  *style);
};

static struct _live_widget_item widgets[] =
{
     { "actionslider", widget_actionslider_create },
     { "bg", widget_bg_create },
     { "bubble", widget_bubble_create },
     { "button", widget_button_create },
     { "calendar", widget_calendar_create },
     { "check", widget_check_create },
     { "clock", widget_clock_create },
     { "label", widget_label_create },
     { "panel", widget_panel_create },
     { "panes", widget_panes_create },
     /* do not delete below */
     { NULL, NULL }
};

Evas_Object *
live_widget_create(const char  *widget,
                   const char  *class,
                   const char  *style,
                   Evas_Object *parent)
{
   Evas_Object *object = NULL;
   unsigned int i = 0;

   while (widgets[i].name != NULL)
     {
        if (strcmp(widget, widgets[i].name) == 0)
          {
             object = widgets[i].func(parent, class, style);
             break;
          }
        i++;
     }

   return object;
}

Eina_Bool
live_widget_del(Evas_Object *widget)
{
   if (!evas_object_data_del(widget, TEXT_FUNC))
     {
        WARN("Given Object doesn't contain any data so it probably wrong object!");
        return false;
     }
   if (!evas_object_data_del(widget, SWALLOW_FUNC))
     {
        WARN("Given Object doesn't contain any data so it probably wrong object!");
        return false;
     }
   evas_object_del(widget);

   return true;
}
