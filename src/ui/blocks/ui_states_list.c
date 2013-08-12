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
/*
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   App_Data *ap = (App_Data *)data;
   Part_State *state;
   Evas_Object *prop_view, *part_view, *state_view;

   state = elm_object_item_data_get(glit);

   prop_view = ui_block_property_get(ap);
   part_view = ui_property_part_view_get(prop_view);
   state_view = ui_prop_part_info_state_view_add(part_view, state);
   ui_prop_part_info_state_set(part_view, state_view);
   evas_object_show(state_view);
*/
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
