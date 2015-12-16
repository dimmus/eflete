/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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
#include "demo_group.h"
#include "signals.h"
#include "string_common.h"
#include "main_window.h"
#include "editor.h"
#include "new_history.h"

#define DEMO_GROUP_DATA "demo_group_data"

typedef struct
{
   Group *group;

   Evas_Object *layout;
   Evas_Object *genlist;

   Elm_Object_Item *selected_part_item;
} Part_Demo_List;

static Elm_Genlist_Item_Class *itc_group;
static Elm_Genlist_Item_Class *itc_part;
static Elm_Genlist_Item_Class *itc_part_selected;
static Elm_Genlist_Item_Class *itc_signals;

static char *
_group_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *pr __UNUSED__)
{
   assert(data);

   return strdup(data);
}
static char *
_part_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *pr __UNUSED__)
{
   Part_ *part = data;

   assert(part != NULL);
   assert(part->name != NULL);

   return strdup(part->name);
}

static void
_on_clicked_double(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   if (elm_genlist_item_type_get(glit) == ELM_GENLIST_ITEM_TREE)
     elm_genlist_item_expanded_set(glit, !elm_genlist_item_expanded_get(glit));
}
static void
_expand_request_cb(void *data __UNUSED__,
                   Evas_Object *o __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}
static void
_contract_request_cb(void *data __UNUSED__,
                     Evas_Object *o __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}
static void
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}
static void
_selected_cb(void *data __UNUSED__,
             Evas_Object *o __UNUSED__,
             void *event_info __UNUSED__)
{
}

Evas_Object *
demo_group_add(Group *group)
{
   Elm_Object_Item *it_swallow, *it_text;
   Eina_List *l;
   Part_ *part;

   assert(group != NULL);
   assert(ap.win != NULL);

   Part_Demo_List *pl = mem_calloc(1, sizeof(Part_Demo_List));
   pl->layout = elm_layout_add(ap.win);
   elm_layout_theme_set(pl->layout, "layout", "navigator", "default");
   evas_object_show(pl->layout);

   evas_object_data_set(pl->layout, DEMO_GROUP_DATA, pl);

   /* add some genlists */
   if (!itc_group)
     {
        itc_group = elm_genlist_item_class_new();
        itc_group->item_style = "part";
        itc_group->func.text_get = _group_label_get;
     }

   if (!itc_part)
     {
        itc_part = elm_genlist_item_class_new();
        itc_part->item_style = "state";
        itc_part->func.text_get = _part_label_get;
        itc_part_selected = elm_genlist_item_class_new();
        itc_part_selected->item_style = "state_selected";
        itc_part_selected->func.text_get = _part_label_get;
     }
   if (!itc_signals)
     {
        itc_signals = elm_genlist_item_class_new();
        itc_signals->item_style = "item";
        itc_signals->func.text_get = _group_label_get;
     }
   /* filling genlist */
   pl->genlist = elm_genlist_add(pl->layout);
   elm_genlist_homogeneous_set(pl->layout, true);
   elm_scroller_policy_set(pl->genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(pl->genlist);
   elm_object_content_set(pl->layout, pl->genlist);
   evas_object_smart_callback_add(pl->genlist, "clicked,double", _on_clicked_double, pl);
   evas_object_smart_callback_add(pl->genlist, "expand,request", _expand_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contract,request", _contract_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contracted", _contracted_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "selected", _selected_cb, pl);
   evas_object_data_set(pl->genlist, DEMO_GROUP_DATA, pl);

   it_swallow = elm_genlist_item_append(pl->genlist,
                                        itc_group,
                                        eina_stringshare_add("Swallows"),
                                        NULL,
                                        ELM_GENLIST_ITEM_TREE,
                                        NULL,
                                        NULL);
   it_text = elm_genlist_item_append(pl->genlist,
                                     itc_group,
                                     eina_stringshare_add("Text"),
                                     NULL,
                                     ELM_GENLIST_ITEM_TREE,
                                     NULL,
                                     NULL);
   elm_genlist_item_append(pl->genlist,
                           itc_group,
                           eina_stringshare_add("Signals"),
                           NULL,
                           ELM_GENLIST_ITEM_TREE,
                           NULL,
                           NULL);

   EINA_LIST_FOREACH(group->parts, l, part)
     {
        if ((part->type == EDJE_PART_TYPE_TEXT) || (part->type == EDJE_PART_TYPE_TEXTBLOCK))
          elm_genlist_item_append(pl->genlist,
                                  itc_part,
                                  part,
                                  it_text,
                                  ELM_GENLIST_ITEM_NONE,
                                  NULL,
                                  NULL);
        else if (part->type == EDJE_PART_TYPE_SWALLOW)
          elm_genlist_item_append(pl->genlist,
                                  itc_part,
                                  part,
                                  it_swallow,
                                  ELM_GENLIST_ITEM_NONE,
                                  NULL,
                                  NULL);
     }

   elm_object_text_set(pl->layout, pl->group->name);

   pl->group = group;

   return pl->layout;
}

void
demo_group_part_select(Evas_Object *obj, Part_ *part __UNUSED__)
{
   Part_Demo_List *pl = evas_object_data_get(obj, DEMO_GROUP_DATA);

   assert(pl != NULL);
}
