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

#include "live_widget.h"
#include "live_widgets.h"
#include "live_elementary_widgets.h"

struct _live_widget_item {
     const char *name;
     Evas_Object * (*func)(Evas_Object *parent,
                           const Group2 *group);
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
     { "ctxpopup", widget_ctxpopup_create },
     { "colorselector", widget_colorselector_create },
     { "datetime", widget_datetime_create },
     { "entry", widget_entry_create },
     { "frame", widget_frame_create },
     { "gengrid", widget_gengrid_create },
     { "genlist", widget_genlist_create },
     { "label", widget_label_create },
     { "layout", widget_layout_create },
     { "list", widget_list_create },
     { "map", widget_map_create },
     { "multibuttonentry", widget_multibuttonentry_create },
     { "notify", widget_notify_create },
     { "panel", widget_panel_create },
     { "panes", widget_panes_create },
     { "popup", widget_popup_create },
     { "radio", widget_radio_create },
     { "scroller", widget_scroller_create },
     { "segment_control", widget_segment_control_create },
     { "separator", widget_separator_create },
     { "slider", widget_slider_create },
     { "spinner", widget_spinner_create },
     { "toolbar", widget_toolbar_create },
     { "progressbar", widget_progressbar_create },
     { "naviframe", widget_naviframe_create },
     /* do not delete below */
     { NULL, NULL }
};

Evas_Object *
live_widget_create(Group2 *group,
                   Evas_Object *parent)
{
   Evas_Object *object = NULL;
   unsigned int i;

   assert(group != NULL);
   assert(group->widget != NULL);
   assert(parent != NULL);

   for (i = 0; widgets[i].name != NULL; i++)
     {
        if (strcmp(group->widget, widgets[i].name) == 0)
          {
             object = widgets[i].func(parent, group);
             break;
          }
     }

   return object;
}

Eina_Bool
live_widget_del(Evas_Object *widget)
{
   assert(widget != NULL);

   if (!evas_object_data_del(widget, TEXT_FUNC))
     {
        ERR("Given Object doesn't contain any data so it probably wrong object!");
        abort();
     }
   if (!evas_object_data_del(widget, SWALLOW_FUNC))
     {
        ERR("Given Object doesn't contain any data so it probably wrong object!");
        abort();
     }
   evas_object_del(widget);

   /* this one for genlist and gengrid */
   evas_object_data_del(widget, SWALLOW_LIST);
   evas_object_data_del(widget, TEXT_LIST);

   return true;
}
