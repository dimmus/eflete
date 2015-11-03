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

#include "property_private.h"
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

/* enum for identifying current property
 (group's, image's, sound's or other kind of peroperty) */
enum _Property_Type {
   PROPERTY,
   IMAGE_PROPERTY
};
typedef enum _Property_Type Property_Type;

struct _Prop_Data
{
   Evas_Object *group_property;
   Evas_Object *image_property;
   Evas_Object *sound_property;
   Evas_Object *colorclass_property;
   Evas_Object *layout;

   Property_Type type;
};
typedef struct _Prop_Data Prop_Data;

static void
_on_different_clicked(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *property = data;
   PROP_DATA_GET()

   assert(pd != NULL);

   ui_property_group_unset(pd->group_property);
   elm_object_content_unset(pd->layout);

   /* hide everything */
   evas_object_hide(pd->group_property);
   evas_object_hide(pd->image_property);

   pd->type = PROPERTY;
}

static void
_on_image_editor_clicked(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Evas_Object *property = data;
   PROP_DATA_GET()

   assert(pd != NULL);

   ui_property_group_unset(pd->group_property);
   elm_object_content_unset(pd->layout);
   elm_object_content_set(pd->layout, pd->image_property);
   evas_object_hide(pd->group_property);
   evas_object_show(pd->image_property);

   pd->type = IMAGE_PROPERTY;
}

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
        if (pd->type == PROPERTY)
          ui_property_group_unset(pd->group_property);
        return;
     }
   /* it has group here, and group's tab was clicked,
      but if previous tab was image_property tab,
      we need to remove it from there and set group property in there */
   if (pd->type == IMAGE_PROPERTY)
     {
        elm_object_content_unset(pd->layout);
        elm_object_content_set(pd->layout, pd->group_property);
        evas_object_hide(pd->image_property);
     }
   ui_property_group_set(pd->group_property, group);

   pd->type = PROPERTY;

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

   pd->image_property = ui_property_image_add(pd->layout);

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _on_tab_changed, pd->layout);
   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_EDITOR_TAB_CLICKED, _on_image_editor_clicked, pd->layout);
   evas_object_smart_callback_add(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, _on_different_clicked, pd->layout);

   return pd->layout;
}
