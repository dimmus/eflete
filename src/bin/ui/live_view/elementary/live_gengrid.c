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

static void
_on_gengrid_swallow_check(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *ei __UNUSED__)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *)data;
   Eina_List *part_list = evas_object_data_get(object, SWALLOW_LIST);

   if (!eina_list_data_find(part_list, part->name))
     part_list =  eina_list_append(part_list, part);
   evas_object_data_set(object, SWALLOW_LIST, part_list);

   Elm_Object_Item *item = NULL;
   item = elm_gengrid_first_item_get(object);
   while (item)
     {
        elm_gengrid_item_update(item);
        item = elm_gengrid_item_next_get(item);
     }
}

static void
_on_gengrid_text_check(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *ei __UNUSED__)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *)data;
   Eina_List *part_list = evas_object_data_get(object, TEXT_LIST);

   if (!eina_list_data_find(part_list, part->name))
     part_list =  eina_list_append(part_list, part);
   evas_object_data_set(object, TEXT_LIST, part_list);

   Elm_Object_Item *item = NULL;
   item = elm_gengrid_first_item_get(object);
   while (item)
     {
        elm_gengrid_item_update(item);
        item = elm_gengrid_item_next_get(item);
     }
}

static void
_gengrid_send_signal(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *ei __UNUSED__)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Elm_Object_Item *item = NULL;

   assert(data != NULL);

   item = elm_gengrid_first_item_get(data);
   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   while (item)
     {
        elm_object_item_signal_emit(item, sig->sig_name, sig->source_name);
        item = elm_gengrid_item_next_get(item);
     }
}


/*********** GEN GRID CREATING FUNCTIONS ****************/
static char *
_grid_text_get(void *data __UNUSED__,
               Evas_Object *obj,
               const char  *part)
{
   Eina_List *part_list = evas_object_data_get(obj, TEXT_LIST);
   Demo_Part *demo_part;
   Eina_List *l = NULL;

   EINA_LIST_FOREACH(part_list, l, demo_part)
     {
        if (!strcmp(demo_part->name, part))
          {
             if (!demo_part->text_content)
               return NULL;
             else
               return strdup(demo_part->text_content);
          }
     }

   return NULL;
}

static Evas_Object *
_grid_content_get(void *data __UNUSED__,
                  Evas_Object *obj,
                  const char  *part)
{
   Eina_List *part_list = evas_object_data_get(obj, SWALLOW_LIST);
   Demo_Part *demo_part;
   Eina_List *l = NULL;

   EINA_LIST_FOREACH(part_list, l, demo_part)
     {
        if (!strcmp(demo_part->name, part))
          {
             if ((demo_part->swallow_content == CONTENT_NONE) && (demo_part->object))
               demo_part->object = NULL;
             else if (demo_part->swallow_content != CONTENT_NONE)
               demo_part->object = object_generate(demo_part, obj);

             if (demo_part->object)
               {
                  evas_object_color_set(demo_part->object,
                                        demo_part->r,
                                        demo_part->g,
                                        demo_part->b,
                                        demo_part->a);

                  evas_object_size_hint_min_set(demo_part->object,
                                                demo_part->min_w,
                                                demo_part->min_h);
                  evas_object_size_hint_max_set(demo_part->object,
                                                demo_part->max_w,
                                                demo_part->max_h);
               }

             return demo_part->object;
          }
     }

   return NULL;
}

static Evas_Object *
_create_gengrid(Evas_Object *obj, Eina_Bool item_style, const char *style)
{
   Elm_Gengrid_Item_Class *ic = NULL;
   int i;
   Evas_Object *grid = NULL;
   double scale = elm_config_scale_get();

   assert(obj != NULL);
   assert(style != NULL);

   grid = elm_gengrid_add(obj);
   elm_gengrid_item_size_set(grid, scale * 100, scale * 100);
   evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   ic = elm_gengrid_item_class_new();
   ic->func.text_get = _grid_text_get;
   ic->func.content_get = _grid_content_get;
   ic->func.state_get = NULL;
   ic->func.del = NULL;

   if (item_style)
     {
        char **c;

        c = eina_str_split(style, "/", 2);
        ic->item_style = strdup(c[0]);
        elm_object_style_set(grid, c[1]);

        free(c[0]);
        free(c);
     }
   else
     {
        ic->item_style = strdup(style);
        elm_object_style_set(grid, style);
     }

   for (i = 0; i < 40; i++)
     elm_gengrid_item_append(grid, ic, eina_stringshare_printf("Item #%d", i),
                             NULL, NULL);

   elm_gengrid_item_class_free(ic);
   return grid;
}
/*********** GEN GRID CREATING FUNCTIONS ****END*********/

void
_demo_gengrid_del(void *data __UNUSED__,
                  Evas *evas __UNUSED__,
                  Evas_Object *object,
                  void *event_info __UNUSED__)
{
   evas_object_smart_callback_del_full(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_gengrid_swallow_check, object);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_DEMO_TEXT_SET, _on_gengrid_text_check, object);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _gengrid_send_signal, object);
}

Evas_Object *
widget_gengrid_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *object = NULL;
   Eina_Bool item_style;
   Eina_List *swallow_list = NULL, *text_list = NULL;

   item_style = strcmp(group->class, "item") == 0;
   object = _create_gengrid(parent, item_style, group->style);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_gengrid_swallow_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, _on_gengrid_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _gengrid_send_signal, object);
   evas_object_event_callback_add(object, EVAS_CALLBACK_DEL, _demo_gengrid_del, NULL);

   evas_object_data_set(object, SWALLOW_LIST, swallow_list);
   evas_object_data_set(object, TEXT_LIST, text_list);

   return object;
}
