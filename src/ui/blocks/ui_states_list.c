/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui_states_list.h"

#define STLIST_PART_KEY "stlist_part_key"
#define STATES_LIST "states_list"

static Elm_Genlist_Item_Class *_itc_state = NULL;

static char *
_item_state_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   return strdup(data);
}

static void
_on_state_selected(void *data __UNUSED__,
                   Evas_Object *obj,
                   void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   evas_object_smart_callback_call(obj, "st,state,select", glit);
}

Evas_Object *
ui_states_list_add(Evas_Object *parent)
{
   Evas_Object *gl_states;

   if (!parent) return NULL;

   gl_states = elm_genlist_add(parent);
   evas_object_size_hint_align_set(gl_states,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_states,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   if (!_itc_state)
     {
        _itc_state = elm_genlist_item_class_new();
        _itc_state->item_style = "default";
        _itc_state->func.text_get = _item_state_label_get;
        _itc_state->func.content_get = NULL;
        _itc_state->func.state_get = NULL;
        _itc_state->func.del = NULL;
     }

   evas_object_smart_callback_add(gl_states, "selected",
                                  _on_state_selected, NULL);
   /*TODO: add delete data on EVAS_OBJECT_DEL event */
   return gl_states;
}

Eina_Bool
ui_states_list_data_set(Evas_Object *object,
                        Group *group,
                        Part *part)
{
   Eina_List *states, *l;
   const char *state_name;
   Elm_Object_Item *stit;
   Evas_Object *gl_states = (Evas_Object *)object;

   if ((!gl_states) || (!group) || (!part)) return EINA_FALSE;

   states = evas_object_data_get(gl_states, STATES_LIST);
   if (states)
     {
        edje_edit_string_list_free(states);
        evas_object_data_del(gl_states, STATES_LIST);
     }
   elm_genlist_clear(gl_states);

   states = edje_edit_part_states_list_get(group->obj, part->name);
   evas_object_data_set(gl_states, STLIST_PART_KEY, part);
   EINA_LIST_FOREACH(states, l, state_name)
     {
        stit = elm_genlist_item_append(gl_states, _itc_state,
                                       state_name,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
        elm_object_item_data_set(stit, (void *)state_name);
     }
   evas_object_data_set(gl_states, STATES_LIST, states);

   return EINA_TRUE;
}

Part *
ui_state_list_part_get(Evas_Object *obj)
{
   Part *_part = NULL;
   if (!obj) return NULL;
   _part = evas_object_data_get(obj, STLIST_PART_KEY);
   return _part;
}
