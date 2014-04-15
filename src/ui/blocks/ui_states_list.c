/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
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
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
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
   Evas_Object *state_list = (Evas_Object *)data;
   evas_object_smart_callback_call(state_list, "stl,state,add", NULL);
}

static void
_del_state_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *state_list = (Evas_Object *)data;
   evas_object_smart_callback_call(state_list, "stl,state,del", NULL);
}

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
   evas_object_smart_callback_call(obj, "stl,state,select", glit);
}

Evas_Object *
ui_states_list_add(Evas_Object *parent)
{
   Evas_Object *gl_states;

   if (!parent) return NULL;

   gl_states = elm_genlist_add(parent);
   elm_object_style_set(gl_states, "eflete/default");
   evas_object_size_hint_align_set(gl_states,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_states,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   if (!_itc_state)
     {
        _itc_state = elm_genlist_item_class_new();
        _itc_state->item_style = "eflete/default";
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
   Evas_Object *parent, *box, *button, *icon;

   if ((!object) || (!style) || (!part)) return false;

   parent = elm_object_parent_widget_get(object);
   elm_genlist_clear(object);

   states = edje_edit_part_states_list_get(style->obj, part->name);
   evas_object_data_set(object, STLIST_PART_KEY, part);
   curr_state = eina_stringshare_printf("%s %.2f",
                                        part->curr_state,
                                        part->curr_state_value);
   EINA_LIST_FOREACH(states, l, state_name)
     {
        state = eina_stringshare_add(state_name);
        stit = elm_genlist_item_append(object, _itc_state,
                                       state,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
        if (strcmp(state, curr_state) == 0)
          elm_genlist_item_selected_set(stit, true);

        elm_object_item_data_set(stit, (void *)state);
        elm_object_item_del_cb_set(stit, _item_del);
     }
   edje_edit_string_list_free(states);

   BOX_ADD(parent, box, true, false)
   elm_box_align_set(box, 1.0, 0.5);
   button = elm_button_add(box);
   elm_object_style_set(button, "eflete/simple");
   evas_object_smart_callback_add(button, "clicked", _add_state_cb, object);
   evas_object_show(button);
   ICON_ADD(button, icon, EINA_TRUE, EFLETE_IMG_PATH"icon-add.png")
   elm_object_part_content_set(button, NULL, icon);
   elm_box_pack_end(box, button);
   button = elm_button_add(box);
   elm_object_style_set(button, "eflete/simple");
   evas_object_smart_callback_add(button, "clicked", _del_state_cb, object);
   evas_object_show(button);
   ICON_ADD(button, icon, EINA_TRUE, EFLETE_IMG_PATH"icon-remove.png")
   elm_object_part_content_set(button, NULL, icon);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(parent, "elm.swallow.title", box);
   elm_object_signal_emit(parent, "title,content,show", "eflete");
   eina_stringshare_del(curr_state);

   return true;
}

Eina_Bool
ui_states_list_data_unset(Evas_Object *object)
{
   Evas_Object *parent, *content;

   if (!object) return false;

   elm_genlist_clear(object);
   parent = elm_object_parent_widget_get(object);
   content = elm_object_part_content_get(parent, "elm.swallow.title");
   evas_object_del(content);
   evas_object_del(object);

   return true;
}

Part *
ui_states_list_part_get(Evas_Object *obj)
{
   Part *_part = NULL;
   if (!obj) return NULL;
   _part = evas_object_data_get(obj, STLIST_PART_KEY);
   return _part;
}

Eina_Bool
ui_states_list_state_add(Evas_Object *obj, const char *state)
{
   Elm_Object_Item *eoi;

   if ((!obj) || (!state)) return false;

   eoi = elm_genlist_item_append(obj, _itc_state, state,
                                 NULL, ELM_GENLIST_ITEM_NONE,
                                 NULL, NULL);
   elm_object_item_data_set(eoi, (void *)state);
   elm_genlist_item_selected_set(eoi, true);

   return true;
}

Eina_Bool
ui_states_list_selected_state_del(Evas_Object *obj)
{
   Elm_Object_Item *eoi, *next_eoi;
   if (!obj) return false;

   eoi = elm_genlist_selected_item_get(obj);

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
   Elm_Object_Item *eoi;
   Eina_Stringshare *state;

   if (!obj) return NULL;
   eoi = elm_genlist_selected_item_get(obj);
   if (!eoi) return NULL;
   state = eina_stringshare_add(elm_object_item_data_get(eoi));

   return state;
}
