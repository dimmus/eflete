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

#include "part_list.h"

#define PART_LIST_DATA "part_list_data"

typedef struct
{
   Group *group;

   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *btn_add;
   Evas_Object *btn_del;
   Evas_Object *btn_up;
   Evas_Object *btn_down;

   Elm_Genlist_Item_Class *itc_part;
   Elm_Genlist_Item_Class *itc_state;
   Elm_Genlist_Item_Class *itc_state_selected;
   Elm_Genlist_Item_Class *itc_item;
} Part_List;

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

static char *
_state_label_get(void *data,
                 Evas_Object *obj __UNUSED__,
                 const char *pr __UNUSED__)
{
   State *state = data;

   assert(state != NULL);
   assert(state->name != NULL);

   return strdup(state->name);
}

static void
_on_eye_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_data __UNUSED__)
{
   Part_ *part = data;

   assert(part != NULL);

   part->visible = !part->visible;

   TODO("Add signal here");
}

static Evas_Object *
_part_content_get(void *data,
                  Evas_Object *obj,
                  const char *part)
{
   Evas_Object *content = NULL;
   Part_ *_part = data;

   assert(_part != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        content = elm_check_add(obj);
        if (_part->visible)
          elm_check_state_set(content, true);
        else
           elm_check_state_set(content, false);
        elm_object_style_set(content, "eye");

        evas_object_smart_callback_add(content, "changed", _on_eye_clicked, _part);
     }
   if (!strcmp(part, "elm.swallow.end"))
     {
        switch (_part->type)
          {
           case EDJE_PART_TYPE_RECTANGLE:
              IMAGE_ADD_NEW(obj, content, "icon", "rectangle");
              break;
           case EDJE_PART_TYPE_IMAGE:
              IMAGE_ADD_NEW(obj, content, "icon", "image");
              break;
           case EDJE_PART_TYPE_SPACER:
              IMAGE_ADD_NEW(obj, content, "icon", "spacer");
              break;
           case EDJE_PART_TYPE_SWALLOW:
              IMAGE_ADD_NEW(obj, content, "icon", "swallow");
              break;
           case EDJE_PART_TYPE_TEXT:
              IMAGE_ADD_NEW(obj, content, "icon", "text");
              break;
           case EDJE_PART_TYPE_TEXTBLOCK:
              IMAGE_ADD_NEW(obj, content, "icon", "textblock");
              break;
           case EDJE_PART_TYPE_GROUP:
              IMAGE_ADD_NEW(obj, content, "icon", "group");
              break;
           case EDJE_PART_TYPE_PROXY:
              IMAGE_ADD_NEW(obj, content, "icon", "proxy");
              break;
           case EDJE_PART_TYPE_TABLE:
              IMAGE_ADD_NEW(obj, content, "icon", "table");
              break;
           case EDJE_PART_TYPE_BOX:
              IMAGE_ADD_NEW(obj, content, "icon", "box");
              break;
           case EDJE_PART_TYPE_EXTERNAL:
              IMAGE_ADD_NEW(obj, content, "icon", "external");
              break;
          default:
              ERR("Unhandled part type");
              abort();
          }
     }
   return content;
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
_on_clicked_double(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   if (elm_genlist_item_type_get(glit) == ELM_GENLIST_ITEM_TREE)
     elm_genlist_item_expanded_set(glit, !elm_genlist_item_expanded_get(glit));
}

static void
_expanded_cb(void *data,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Part_List *pl = data;
   const Elm_Genlist_Item_Class* itc;
   Eina_List *l;
   Part_ *part;
   State *state;

   TODO("remove this hack after https://phab.enlightenment.org/D2965 will be accepted");
   Eina_Bool first_item = true;

   assert(pl != NULL);

   itc = elm_genlist_item_item_class_get(glit);

   if (itc == pl->itc_part)
     {
        part = elm_object_item_data_get(glit);
        EINA_LIST_FOREACH(part->states, l, state)
          {
             /* default state should be listed first */
             if ((first_item) || (strcmp(state->name, "default 0.00") != 0))
               {
                  elm_genlist_item_append(pl->genlist,
                                          (state->part->current_state == state) ? pl->itc_state_selected : pl->itc_state,
                                          state,
                                          glit,
                                          ELM_GENLIST_ITEM_NONE,
                                          NULL,
                                          NULL);
                  first_item = false;
               }
             else
               elm_genlist_item_prepend(pl->genlist,
                                        (state->part->current_state == state) ? pl->itc_state_selected : pl->itc_state,
                                        state,
                                        glit,
                                        ELM_GENLIST_ITEM_NONE,
                                        NULL,
                                        NULL);
          }
     }
}

static void
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

Evas_Object *
part_list_add(Group *group)
{
   Evas_Object *icon;
   Part_List *pl;
   Eina_List *l;
   Part_ *part;

   assert(group != NULL);
   assert(ap.win != NULL);

   pl = mem_calloc(1, sizeof(Part_List));
   pl->layout = elm_layout_add(ap.win);
   elm_layout_theme_set(pl->layout, "layout", "part_list", "default");
   evas_object_show(pl->layout);

   evas_object_data_set(pl->layout, PART_LIST_DATA, pl);

   pl->group = group;

#define BTN_ADD(BTN, ICON_NAME, CALLBACK) \
   pl->BTN = elm_button_add(pl->layout); \
   ICON_STANDARD_ADD(pl->BTN, icon, true, ICON_NAME); \
   elm_object_part_content_set(pl->BTN, NULL, icon); \
   evas_object_smart_callback_add(pl->BTN, "clicked", CALLBACK, pl); \
   elm_object_style_set(pl->BTN, "anchor"); \
   elm_object_part_content_set(pl->layout, "elm.swallow." #BTN, pl->BTN);

   BTN_ADD(btn_add, "plus", NULL);
   BTN_ADD(btn_del, "minus", NULL);
   BTN_ADD(btn_up, "arrow_up", NULL);
   BTN_ADD(btn_down, "arrow_down", NULL);

#undef BTN_ADD
   pl->itc_part = elm_genlist_item_class_new();
   pl->itc_part->item_style = "part";
   pl->itc_part->func.text_get = _part_label_get;
   pl->itc_part->func.content_get = _part_content_get;

   pl->itc_state = elm_genlist_item_class_new();
   pl->itc_state->item_style = "state";
   pl->itc_state->func.text_get = _state_label_get;

   pl->itc_state_selected = elm_genlist_item_class_new();
   pl->itc_state_selected->item_style = "state_selected";
   pl->itc_state_selected->func.text_get = _state_label_get;

   pl->itc_item = elm_genlist_item_class_new();
   pl->itc_item->item_style = "item";
   pl->itc_item->func.text_get = NULL;

   pl->genlist = elm_genlist_add(pl->layout);
   evas_object_show(pl->genlist);
   elm_object_content_set(pl->layout, pl->genlist);
   evas_object_smart_callback_add(pl->genlist, "clicked,double", _on_clicked_double, NULL);
   evas_object_smart_callback_add(pl->genlist, "expand,request", _expand_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contract,request", _contract_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "expanded", _expanded_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contracted", _contracted_cb, pl);

   EINA_LIST_FOREACH(group->parts, l, part)
     {
        elm_genlist_item_append(pl->genlist,
                                pl->itc_part,
                                part,
                                NULL,
                                ELM_GENLIST_ITEM_TREE,
                                NULL,
                                NULL);
     }

   elm_object_text_set(pl->layout, pl->group->name);

   TODO("Add deletion callback and free resources");
   return pl->layout;
}
