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

#include "ui_states_list.h"

#define STLIST_PART_KEY "stlist_part_key"
#define STATES_LIST "states_list"

static Elm_Genlist_Item_Class *_itc_state = NULL;

static void
_add_state_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *box = (Evas_Object *)data;

   assert(box != NULL);

   evas_object_smart_callback_call(box, "stl,state,add", NULL);
}

static void
_del_state_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *box = (Evas_Object *)data;

   assert(box != NULL);

   evas_object_smart_callback_call(box, "stl,state,del", NULL);
}

static char *
_item_state_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   return strdup(data);
}

static void
_on_state_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Object *box = (Evas_Object *)data;

   assert(box != NULL);

   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   evas_object_smart_callback_call(box, "stl,state,select", glit);
}

Evas_Object *
ui_states_list_add(Evas_Object *parent)
{
   Evas_Object *gl_states, *box, *box_btn, *btn, *ic;

   assert(parent != NULL);

   BOX_ADD(parent, box, false, false)
   elm_box_padding_set(box, 0, 6);
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
                                  _on_state_selected, box);
   TODO("add delete data on EVAS_OBJECT_DEL event")

   BOX_ADD(parent, box_btn, true, false)
   elm_box_align_set(box_btn, 1.0, 0.5);
   btn = elm_button_add(box_btn);
   elm_object_style_set(btn, "anchor");
   evas_object_smart_callback_add(btn, "clicked", _add_state_cb, box);
   elm_object_disabled_set(btn, true);
   evas_object_data_set(box, "st_btn_plus", btn);
   evas_object_show(btn);
   ICON_STANDARD_ADD(btn, ic, true, "plus");
   elm_object_part_content_set(btn, NULL, ic);
   elm_box_pack_end(box_btn, btn);
   btn = elm_button_add(box_btn);
   elm_object_style_set(btn, "anchor");
   evas_object_smart_callback_add(btn, "clicked", _del_state_cb, box);
   elm_object_disabled_set(btn, true);
   evas_object_data_set(box, "st_btn_minus", btn);
   evas_object_show(btn);
   ICON_STANDARD_ADD(btn, ic, true, "minus");
   elm_object_part_content_set(btn, NULL, ic);
   elm_box_pack_end(box_btn, btn);

   elm_box_pack_end(box, box_btn);
   elm_box_pack_end(box, gl_states);
   evas_object_data_set(box, "st_gl", gl_states);
   return box;
}

static void
_item_del(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   Elm_Object_Item *item = (Elm_Object_Item *)event_info;
   Eina_Stringshare *state = elm_object_item_data_get(item);
   eina_stringshare_del(state);
}

Eina_Bool
ui_states_list_data_set(Evas_Object *object,
                        Style *style,
                        Part *part)
{
   Eina_List *states, *l;
   const char *state_name;
   Eina_Stringshare *state, *curr_state;
   Elm_Object_Item *stit;
   Evas_Object *gl;

   assert(object != NULL);
   assert(style != NULL);
   assert(part != NULL);

   gl = evas_object_data_get(object, "st_gl");
   elm_genlist_clear(gl);
   evas_object_show(gl);

   states = edje_edit_part_states_list_get(style->obj, part->name);
   evas_object_data_set(object, STLIST_PART_KEY, part);
   curr_state = eina_stringshare_printf("%s %1.2f",
                                        part->curr_state,
                                        part->curr_state_value);
   EINA_LIST_FOREACH(states, l, state_name)
     {
        state = eina_stringshare_add(state_name);
        stit = elm_genlist_item_append(gl, _itc_state,
                                       state,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
        if (strcmp(state, curr_state) == 0)
          elm_genlist_item_selected_set(stit, true);
        elm_object_item_data_set(stit, (void *)state);
        elm_object_item_del_cb_set(stit, _item_del);
     }
   elm_object_disabled_set(evas_object_data_get(object, "st_btn_plus"), false);
   elm_object_disabled_set(evas_object_data_get(object, "st_btn_minus"), false);
   edje_edit_string_list_free(states);

   eina_stringshare_del(curr_state);

   return true;
}

Eina_Bool
ui_states_list_data_unset(Evas_Object *object)
{
   Evas_Object *gl;

   assert(object != NULL);

   elm_object_disabled_set(evas_object_data_get(object, "st_btn_plus"), true);
   elm_object_disabled_set(evas_object_data_get(object, "st_btn_minus"), true);
   gl = evas_object_data_get(object, "st_gl");
   evas_object_data_del(object, STLIST_PART_KEY);
   elm_genlist_clear(gl);
   evas_object_hide(gl);

   return true;
}

Part *
ui_states_list_part_get(Evas_Object *obj)
{
   Part *_part = NULL;

   assert(obj != NULL);

   _part = evas_object_data_get(obj, STLIST_PART_KEY);
   return _part;
}

Eina_Bool
ui_states_list_state_add(Evas_Object *obj, const char *state)
{
   Evas_Object *gl;
   Elm_Object_Item *eoi;

   assert(obj != NULL);
   assert(state != NULL);

   gl = evas_object_data_get(obj, "st_gl");
   eoi = elm_genlist_item_append(gl, _itc_state, state,
                                 NULL, ELM_GENLIST_ITEM_NONE,
                                 NULL, NULL);
   elm_object_item_data_set(eoi, (void *)state);
   elm_genlist_item_selected_set(eoi, true);

   return true;
}

Eina_Bool
ui_states_list_selected_state_del(Evas_Object *obj)
{
   Evas_Object *gl;
   Elm_Object_Item *eoi, *next_eoi;

   assert(obj != NULL);

   gl = evas_object_data_get(obj, "st_gl");
   eoi = elm_genlist_selected_item_get(gl);

   if (!eoi)
     {
        WARN("None one part does'nt selected!");
        return false;
     }

   next_eoi = elm_genlist_item_next_get(eoi);
   if (!next_eoi) next_eoi = elm_genlist_item_prev_get(eoi);
   elm_genlist_item_selected_set(next_eoi, true);
   elm_object_item_del(eoi);

   return true;

}

Eina_Stringshare *
ui_states_list_selected_state_get(Evas_Object *obj)
{
   Evas_Object *gl;
   Elm_Object_Item *eoi;
   Eina_Stringshare *state;

   assert(obj != NULL);

   gl = evas_object_data_get(obj, "st_gl");
   eoi = elm_genlist_selected_item_get(gl);

   if (!eoi) return NULL;
   state = eina_stringshare_add(elm_object_item_data_get(eoi));

   return state;
}
