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

/*
#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif */
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "property_group.h"
#include "property_macros.h"
#include "main_window.h"
#include "string_common.h"
#include "new_history.h"
#include "editor.h"
#include "validator.h"

#include "syntax_color.h"

#include "signals.h"

#define PROP_DATA "prop_data"

#define PROP_DATA_GET() \
   assert(property != NULL); \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   assert(pd != NULL);

/*
 * Callback is added for frames at property box to correct scroller
 * work while each frame would be expanded/collapsed
 */
#define FRAME_PROPERTY_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

struct _Prop_Data
{
   Evas_Object *group_property;
   Evas_Object *image_property;
   Evas_Object *sound_property;
   Evas_Object *colorclass_property;
   Evas_Object *layout;
};
typedef struct _Prop_Data Prop_Data;

static void
_on_tab_changed(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Evas_Object *property = data;
   PROP_DATA_GET()
   Group *group = event_info;

   assert(pd != NULL);

   if (!group)
     {
        ui_property_group_unset(pd->group_property);
        return;
     }
   ui_property_group_set(pd->group_property, group);

   if (!group->current_part) return;
   ui_property_part_set(pd->group_property, group->current_part);
   ui_property_part_state_set(pd->group_property, group->current_part);
}

Evas_Object *
ui_property_add(Evas_Object *parent)
{
   Prop_Data *pd;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Prop_Data));
   pd->layout = elm_layout_add(parent);
   elm_layout_theme_set(pd->layout, "layout", "property", "default");

   pd->group_property = ui_property_group_add(pd->layout);
   elm_object_content_set(pd->layout, pd->group_property);
   evas_object_data_set(pd->layout, PROP_DATA, pd);

   /*
   pd->image_property = ui_property_image_add(pd->layout);
    */

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _on_tab_changed, pd->layout);

   return pd->layout;
}
