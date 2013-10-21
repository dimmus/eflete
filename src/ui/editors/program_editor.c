/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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
* along with this program; If not, see .
*/

#include "program_editor.h"
#include "widget_define.h"

struct _Program_Editor
{
   Evas_Object *mwin;
   Evas_Object *gl_progs;
   Elm_Object_Item *sel;
   struct {
     Group *group;
     const char *program;
     Evas_Object *name;
     Evas_Object *signal;
     Evas_Object *source;
     Evas_Object *in;
     Evas_Object *action;
     Evas_Object *transition;
     Evas_Object *targets;
     Evas_Object *afters;
     Edje_Action_Type act_type;
     Edje_Tween_Mode trans_type;
   } prop_view;
};

struct _Index
{
   int i;
};
typedef struct _Index Index;

static Evas_Object *prog_entry;

static const char *transition_type[] = {
                                    "NONE",
                                    "LINEAR",
                                    "SINUSOIDAL",
                                    "ACCELERATE",
                                    "DECELERATE",
                                    "ACCELERATE FACTOR",
                                    "DECELERATE FACTOR",
                                    "SINUSOIDAL FACTOR",
                                    "DIVISOR INTERP",
                                    "BOUNCE",
                                    "SPRING",
                                    "LAST",
                                    "MASK",
                                    "OPT_FROM_CURRENT"};


static const char *action_type[] = {"NONE",
                                    "STATE_SET",
                                    "ACTION_STOP",
                                    "SIGNAL_EMIT",
                                    "DRAG_VAL_SET",
                                    "DRAG_VAL_STEP",
                                    "DRAG_VAL_PAGE",
                                    "SCRIPT",
                                    "FOCUS_SET",
                                    "RESERVED00",
                                    "FOCUS_OBJECT",
                                    "PARAM_COPY",
                                    "PARAM_SET",
                                    "SOUND_SAMPLE",
                                    "SOUND_TONE"};

typedef struct _Program_Editor Program_Editor;
static Program_Editor window;
#define prop window.prop_view
static Elm_Genlist_Item_Class *_itc_prog;

static Evas_Object *
gl_progs_add(Evas_Object *parent, Group *group);

static Evas_Object *
prop_progs_add(Evas_Object *parent);

static void
prop_progs_update(void);

static void
gl_progs_update_sel_item(const char *str);

static void
_target_item_add(Evas_Object *parent, char *name);

static void
_after_item_add(Evas_Object *parent, char *name);

#define ITEM_STRING_CALLBACK(sub, value) \
static void \
__on_##sub##_##value##_change(void *data __UNUSED__, \
                              Evas_Object *obj, \
                              void *ei __UNUSED__) \
{ \
   const char *value = elm_entry_entry_get(obj); \
   Eina_Bool res = edje_edit_##sub##_##value##_set(prop.group->obj, prop.program, value); \
   if (!res) \
     { \
        NOTIFY_WARNING("The entered data is not valid!") \
        return; \
     } \
}

#define ITEM_1ENTRY_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(parent, entry, EINA_TRUE, DEFAULT_STYLE) \
   elm_object_tooltip_text_set(entry, tooltip); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1ENTRY_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group) \
{ \
   Evas_Object *entry; \
   const char *value; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##sub##_##value##_get(group->obj, prop.program); \
   elm_entry_entry_set(entry, value); \
   evas_object_smart_callback_del(entry, "activated", __on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_##sub##_##value##_change, NULL); \
}

#define ITEM_1ENTRY_PROG_CREATE(text, sub, value) \
   ITEM_STRING_CALLBACK(sub, value) \
   ITEM_1ENTRY_ADD(text, sub, value) \
   ITEM_1ENTRY_UPDATE(sub, value) \


ITEM_1ENTRY_PROG_CREATE("signal", program, signal)
ITEM_1ENTRY_PROG_CREATE("source", program, source)

ITEM_1ENTRY_ADD("name", program, name)

static void
__on_program_name_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_name_set(prop.group->obj, prop.program, value);
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
   gl_progs_update_sel_item(value);
   prop.program = value;
}

static void
__on_in_from_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_in_from_set(prop.group->obj, prop.program,
                                                 atof(value));
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
__on_in_range_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_in_range_set(prop.group->obj, prop.program,
                                                 atof(value));
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
__on_target_name_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_target_add(prop.group->obj, prop.program,
                                                value);
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
_on_hoversel_trans_sel(void *data,
                 Evas_Object *obj,
                 void *ei)
{
   Index *index = elm_object_item_data_get(ei);
   Evas_Object *box = (Evas_Object *)data;
   Eina_List *childs = elm_box_children_get(box);
   Evas_Object *entry1 = eina_list_nth(childs, 1);
   Evas_Object *entry2 = eina_list_nth(childs, 2);
   Evas_Object *entry3 = eina_list_nth(childs, 3);
/*  Evas_Object *check = eina_list_nth(childs, 4);*/
   elm_entry_entry_set(entry1, "");
   elm_entry_entry_set(entry2, "");
   elm_entry_entry_set(entry3, "");

   elm_object_text_set(obj, transition_type[index->i]);
   edje_edit_program_transition_set(prop.group->obj, prop.program, (Edje_Tween_Mode)index->i);
   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        prop.trans_type = EDJE_TWEEN_MODE_NONE;
        NOTIFY_INFO(5, "Transition block can used only with STATE_SET action");
        elm_object_text_set(obj, transition_type[0]);
        evas_object_hide(entry1);
        evas_object_hide(entry2);
        evas_object_hide(entry3);
        return;
     }
   evas_object_show(entry1);
   evas_object_show(entry2);
   evas_object_show(entry3);
   prop.trans_type = (Edje_Tween_Mode)index->i;
   switch (prop.trans_type)
     {
      case EDJE_TWEEN_MODE_NONE:
        {
           evas_object_hide(entry1);
           evas_object_hide(entry2);
           evas_object_hide(entry3);
/*           evas_object_hide(check);*/
        }
      break;
      case EDJE_TWEEN_MODE_LINEAR:
      case EDJE_TWEEN_MODE_SINUSOIDAL:
      case EDJE_TWEEN_MODE_ACCELERATE:
      case EDJE_TWEEN_MODE_DECELERATE:
        {
           evas_object_hide(entry2);
           evas_object_hide(entry3);
        }
      break;
      case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
      case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
      case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
        {
           evas_object_hide(entry3);
        }
      break;
      case EDJE_TWEEN_MODE_DIVISOR_INTERP:
      case EDJE_TWEEN_MODE_BOUNCE:
      case EDJE_TWEEN_MODE_SPRING:
      case EDJE_TWEEN_MODE_LAST:
      case EDJE_TWEEN_MODE_MASK:
      case EDJE_TWEEN_MODE_OPT_FROM_CURRENT:
      break;
      default:
      break;
     }
   eina_list_free(childs);
}

static void
_on_hoversel_sel(void *data,
                 Evas_Object *obj,
                 void *ei)
{
   Index *index = elm_object_item_data_get(ei);
   Evas_Object *box = (Evas_Object *)data;
   Eina_List *childs = elm_box_children_get(box);
   Evas_Object *entry1 = eina_list_nth(childs, 1);
   Evas_Object *entry2 = eina_list_nth(childs, 2);
   Evas_Object *hoversel_trans = NULL;
   Evas_Object *entry_trans = NULL;
   elm_entry_entry_set(entry1, "");
   elm_entry_entry_set(entry2, "");

   elm_object_text_set(obj, action_type[index->i]);
   edje_edit_program_action_set(prop.group->obj, prop.program, (Edje_Action_Type)index->i);
   evas_object_hide(entry1);
   evas_object_hide(entry2);
   prop.act_type = (Edje_Action_Type)index->i;

   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        box = elm_object_part_content_get(prop.transition, "elm.swallow.content");
        childs = elm_box_children_get(box);
        hoversel_trans = eina_list_nth(childs, 0);
        prop.trans_type = EDJE_TWEEN_MODE_NONE;
        entry_trans = eina_list_nth(childs, 1);
        evas_object_hide(entry_trans);
        entry_trans = eina_list_nth(childs, 2);
        evas_object_hide(entry_trans);
        entry_trans = eina_list_nth(childs, 3);
        evas_object_hide(entry_trans);
        elm_object_text_set(hoversel_trans, transition_type[0]);
     }

   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
      break;
      case EDJE_ACTION_TYPE_STATE_SET:
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           evas_object_show(entry1);
           evas_object_show(entry2);
        }
      break;
      default:
      break;
     }
   eina_list_free(childs);
}

static void
_on_transition_time_active(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_time_set(prop.group->obj, prop.program,
                                              atof(value)))
     NOTIFY_WARNING("The entered data is not valid!");
}


static void
_on_state_active(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   const char *state;
   state = elm_entry_entry_get(obj);
   if (!edje_edit_program_state_set(prop.group->obj, prop.program, state))
     NOTIFY_WARNING("The entered data is not valid!");
}

static void
_on_value2_active(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   const char *value;
   value = elm_entry_entry_get(obj);
   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
      case EDJE_ACTION_TYPE_STATE_SET:
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
      case EDJE_ACTION_TYPE_SCRIPT:
      break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           if (!edje_edit_program_value2_set(prop.group->obj, prop.program, atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");
        }
      break;
      default:
      break;
     }
}
/*
  Wait patch to EDJE for getters/setter in transition property of program
static void
_on_v1_active(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_v1_set(prop.group->obj, prop.program, atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");

}

static void
_on_v2_active(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_v2_set(prop.group->obj, prop.program, atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");

}
*/

static void
_on_value_active(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   const char *value;
   value = elm_entry_entry_get(obj);
   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
      case EDJE_ACTION_TYPE_SCRIPT:
      break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
      case EDJE_ACTION_TYPE_STATE_SET:
        {
           if (!edje_edit_program_value_set(prop.group->obj, prop.program, atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");
        }
      break;
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           if (!edje_edit_program_state2_set(prop.group->obj, prop.program, value))
             NOTIFY_WARNING("the entered data is not valid!");
        }
      break;
      default:
      break;
     }
}

static void
__on_after_name_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_after_add(prop.group->obj, prop.program,
                                                value);
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
_after_remove_button_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *entry = NULL;
   Eina_List *childs = NULL;

   Evas_Object *element_box = (Evas_Object *)data;
   Evas_Object *entrys_box = elm_object_parent_widget_get(element_box);

   childs = elm_box_children_get(element_box);
   entry = eina_list_nth(childs, 0);
   elm_box_unpack(entrys_box, element_box);
   edje_edit_program_after_del(prop.group->obj, prop.program,
                                elm_entry_entry_get(entry));
   evas_object_smart_callback_del(entry, "activated", __on_after_name_change);
   eina_list_free(childs);
   evas_object_del(element_box);
}

static void
_after_add_button_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *entrys_box = (Evas_Object *)data;
   _after_item_add(entrys_box, "");
}


static void
_target_remove_button_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *entry = NULL;
   Eina_List *childs = NULL;

   Evas_Object *element_box = (Evas_Object *)data;
   Evas_Object *entrys_box = elm_object_parent_widget_get(element_box);

   childs = elm_box_children_get(element_box);
   entry = eina_list_nth(childs, 0);
   elm_box_unpack(entrys_box, element_box);
   edje_edit_program_target_del(prop.group->obj, prop.program,
                                elm_entry_entry_get(entry));
   evas_object_smart_callback_del(entry, "activated", __on_target_name_change);
   eina_list_free(childs);
   evas_object_del(element_box);
}

static void
_target_add_button_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *entrys_box = (Evas_Object *)data;
   _target_item_add(entrys_box, "");
}

void
prop_item_program_transition_update(Evas_Object *item)
{
   Evas_Object *box, *hoversel, *entry1, *entry2, *entry3;
/*   Evas_Object *check;*/
   Eina_List *nodes;
   char buff[BUFF_MAX];
   double value;
   if (!item) return;
   prop.trans_type = edje_edit_program_transition_get(prop.group->obj, prop.program);
   box = elm_object_part_content_get(item, "elm.swallow.content");
   nodes = elm_box_children_get(box);
   hoversel = eina_list_nth(nodes, 0);
   entry1 = eina_list_nth(nodes, 1);
   entry2 = eina_list_nth(nodes, 2);
   entry3 = eina_list_nth(nodes, 3);
/*   check = eina_list_nth(nodes, 4);*/
   evas_object_show(entry1);
   evas_object_show(entry2);
   evas_object_show(entry3);
/*   evas_object_show(check);*/
   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     prop.trans_type = EDJE_TWEEN_MODE_NONE;
   elm_object_text_set(hoversel, transition_type[(int)prop.trans_type]);
   value = edje_edit_program_transition_time_get(prop.group->obj, prop.program);
   evas_object_smart_callback_del(entry1, "activated",
                                  _on_transition_time_active);
   evas_object_smart_callback_add(entry1, "activated",
                                  _on_transition_time_active, NULL);

   snprintf(buff, sizeof(buff), "%1.2f", value);
   elm_entry_entry_set(entry1, buff);
   switch (prop.trans_type)
     {
      case EDJE_TWEEN_MODE_NONE:
        {
           evas_object_hide(entry1);
           evas_object_hide(entry2);
           evas_object_hide(entry3);
/*           evas_object_hide(check);*/
        }
      break;
      case EDJE_TWEEN_MODE_LINEAR:
      case EDJE_TWEEN_MODE_SINUSOIDAL:
      case EDJE_TWEEN_MODE_ACCELERATE:
      case EDJE_TWEEN_MODE_DECELERATE:
        {
           evas_object_hide(entry2);
           evas_object_hide(entry3);
        }
      break;
      case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
      case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
      case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
        {
           evas_object_hide(entry3);
        }
      break;
      case EDJE_TWEEN_MODE_DIVISOR_INTERP:
      case EDJE_TWEEN_MODE_BOUNCE:
      case EDJE_TWEEN_MODE_SPRING:
      case EDJE_TWEEN_MODE_LAST:
      case EDJE_TWEEN_MODE_MASK:
      case EDJE_TWEEN_MODE_OPT_FROM_CURRENT:
        {
/*           value = edje_edit_program_transition_v1_get(prop.group->obj, prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry2, buff);
           evas_object_smart_callback_del(entry2, "activated", _on_v1_active);
           evas_object_smart_callback_add(entry2, "activated", _on_v1_active, NULL);
           value = edje_edit_program_transition_v2_get(prop.group->obj, prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry3, buff);
           evas_object_smart_callback_del(entry3, "activated", _on_v2_active);
           evas_object_smart_callback_add(entry3, "activated", _on_v2_active, NULL);
*/
        }
      break;
      default:
      break;
     }
   eina_list_free(nodes);
}

#define INDEX_APPEND(value) \
   index = mem_malloc(sizeof(Index)); \
   index->i = value; \
   elm_object_item_data_set(hovit, index);
Evas_Object *
prop_item_program_transition_add(Evas_Object *parent,
                                 const char *tooltip)
{
   Evas_Object *item, *box, *hoversel, *entry1, *entry2, *entry3;
/*   Evas_Object *check;*/
   Elm_Object_Item *hovit;
   Index *index;
   int i = 0;

   ITEM_ADD(parent, item, "transition")
   BOX_ADD(item, box, EINA_TRUE, EINA_TRUE)
   HOVERSEL_ADD(item, hoversel, EINA_FALSE)
   ENTRY_ADD(item, entry1, EINA_TRUE, DEFAULT_STYLE)
   ENTRY_ADD(item, entry2, EINA_TRUE, DEFAULT_STYLE)
   ENTRY_ADD(item, entry3, EINA_TRUE, DEFAULT_STYLE)
/*   CHECK_ADD(item, check, "default");
   elm_check_state_set(check, EINA_FALSE);
   elm_object_part_text_set(check, NULL, "CURRENT");*/
   elm_hoversel_hover_parent_set(hoversel, window.mwin);
   elm_object_text_set(hoversel, transition_type[0]);

   for (i = 0; i < TRANSITIONS_COUNT; i++)
     {
        hovit = elm_hoversel_item_add(hoversel, transition_type[i], NULL,
                                 ELM_ICON_NONE, NULL, NULL);
        INDEX_APPEND(i)
     }
   evas_object_smart_callback_add(hoversel, "selected", _on_hoversel_trans_sel, box);


   elm_box_pack_end(box, hoversel);
   elm_box_pack_end(box, entry1);
   elm_box_pack_end(box, entry2);
   elm_box_pack_end(box, entry3);
 /*  elm_box_pack_end(box, check);*/

   elm_object_tooltip_text_set(item, tooltip);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

Evas_Object *
prop_item_program_action_add(Evas_Object *parent,
                             const char *tooltip)
{
   Evas_Object *item, *box, *hoversel, *entry1, *entry2;
   Elm_Object_Item *hovit;
   Index *index;

   ITEM_ADD(parent, item, "action")
   BOX_ADD(item, box, EINA_TRUE, EINA_TRUE)
   HOVERSEL_ADD(item, hoversel, EINA_FALSE)
   ENTRY_ADD(item, entry1, EINA_TRUE, DEFAULT_STYLE)
   ENTRY_ADD(item, entry2, EINA_TRUE, DEFAULT_STYLE)
   evas_object_hide(entry1);
   evas_object_hide(entry2);

   elm_hoversel_hover_parent_set(hoversel, window.mwin);
   elm_object_text_set(hoversel, action_type[0]);

   hovit = elm_hoversel_item_add(hoversel, action_type[0], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(0)
   hovit = elm_hoversel_item_add(hoversel, action_type[1], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(1)
   hovit = elm_hoversel_item_add(hoversel, action_type[2], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(2)
   hovit = elm_hoversel_item_add(hoversel, action_type[3], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(3)
   hovit = elm_hoversel_item_add(hoversel, action_type[4], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(4)
   hovit = elm_hoversel_item_add(hoversel, action_type[5], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(5)
   hovit = elm_hoversel_item_add(hoversel, action_type[6], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(6)
   hovit = elm_hoversel_item_add(hoversel, action_type[8], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(8)
   hovit = elm_hoversel_item_add(hoversel, action_type[10], NULL, ELM_ICON_NONE, NULL, NULL);
   INDEX_APPEND(10)
   evas_object_smart_callback_add(hoversel, "selected", _on_hoversel_sel, box);

   elm_object_tooltip_text_set(item, tooltip);
   elm_box_pack_end(box, hoversel);
   elm_box_pack_end(box, entry1);
   elm_box_pack_end(box, entry2);

   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}
#undef INDEX_APPEND

void
prop_item_program_action_update(Evas_Object *item)
{
   Evas_Object *box, *hoversel, *entry1, *entry2;
   Eina_List *nodes;
   Eina_Stringshare *str;
   char buff[BUFF_MAX];
   double value;
   if (!item) return;
   prop.act_type = edje_edit_program_action_get(prop.group->obj, prop.program);
   box = elm_object_part_content_get(item, "elm.swallow.content");
   nodes = elm_box_children_get(box);
   hoversel = eina_list_nth(nodes, 0);
   entry1 = eina_list_nth(nodes, 1);
   entry2 = eina_list_nth(nodes, 2);
   evas_object_show(entry1);
   evas_object_show(entry2);

   elm_object_text_set(hoversel, action_type[(int)prop.act_type]);

   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_SCRIPT:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
        {
           evas_object_hide(entry1);
           evas_object_hide(entry2);
        }
      break;
      case EDJE_ACTION_TYPE_STATE_SET:
        {
           str = edje_edit_program_state_get(prop.group->obj, prop.program);
           elm_entry_entry_set(entry1, str);
           edje_edit_string_free(str);
           evas_object_smart_callback_del(entry1, "activated", _on_state_active);
           evas_object_smart_callback_add(entry1, "activated", _on_state_active, NULL);
           value = edje_edit_program_value_get(prop.group->obj, prop.program);
           sprintf(buff, "%1.2f", value);
           elm_entry_entry_set(entry2, buff);
           evas_object_smart_callback_del(entry2, "activated", _on_value_active);
           evas_object_smart_callback_add(entry2, "activated", _on_value_active, NULL);
        }
      break;
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           str = edje_edit_program_state_get(prop.group->obj, prop.program);
           elm_entry_entry_set(entry1, str);
           edje_edit_string_free(str);
           evas_object_smart_callback_del(entry1, "activated", _on_state_active);
           evas_object_smart_callback_add(entry1, "activated", _on_state_active, NULL);
           str = edje_edit_program_state2_get(prop.group->obj, prop.program);
           elm_entry_entry_set(entry2, str);
           edje_edit_string_free(str);
           evas_object_smart_callback_del(entry2, "activated", _on_value_active);
           evas_object_smart_callback_add(entry2, "activated", _on_value_active, NULL);
        }
      break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           value = edje_edit_program_value_get(prop.group->obj, prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry1, buff);
           evas_object_smart_callback_del(entry1, "activated", _on_value_active);
           evas_object_smart_callback_add(entry1, "activated", _on_value_active, NULL);
           value = edje_edit_program_value2_get(prop.group->obj, prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry2, buff);
           evas_object_smart_callback_del(entry2, "activated", _on_value2_active);
           evas_object_smart_callback_add(entry2, "activated", _on_value2_active, NULL);
        }
      break;
      default:
      break;
     }
   eina_list_free(nodes);
}

static void
_after_item_add(Evas_Object *parent, char *name)
{
   Evas_Object *element_box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *entry = NULL;

   BOX_ADD(parent, element_box, EINA_TRUE, EINA_FALSE);
   BUTTON_ADD(element_box, button, "Del");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   ENTRY_ADD(element_box, entry, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_entry_set(entry, name);
   evas_object_smart_callback_add(entry, "activated",
                                  __on_after_name_change, NULL);
   evas_object_smart_callback_add(button, "clicked", _after_remove_button_cb,
                                  element_box);
   elm_box_pack_end(element_box, entry);
   elm_box_pack_end(element_box, button);
   elm_box_pack_end(parent, element_box);
}

Evas_Object *
prop_item_program_after_add(Evas_Object *parent,
                     const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;
   Evas_Object *button = NULL;

   if (!parent) return NULL;

   ITEM_ADD(parent, item, "after");
   BOX_ADD(item, box, EINA_TRUE, EINA_FALSE);
   BOX_ADD(box, entrys_box, EINA_FALSE, EINA_FALSE);

   BUTTON_ADD(box, button, "Add");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   evas_object_smart_callback_add(button, "clicked", _after_add_button_cb,
                                  entrys_box);

   elm_box_pack_end(box, entrys_box);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

void
prop_item_program_after_update(Evas_Object *item)
{
   Eina_List *afters_list = NULL;
   Eina_List *childs = NULL;
   int count_afters = 0;
   int i = 0;

   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;

   if (!item) return;

   afters_list = edje_edit_program_afters_get(prop.group->obj, prop.program);
   count_afters = eina_list_count(afters_list);

   box = elm_object_part_content_get(item, "elm.swallow.content");
   childs = elm_box_children_get(box);
   entrys_box = eina_list_nth(childs, 0);
   elm_box_clear(entrys_box);

   if (!count_afters) return;

   for (i = 0; i < count_afters; i++)
     _after_item_add(entrys_box, eina_list_nth(afters_list, i));
}


static void
_target_item_add(Evas_Object *parent, char *name)
{
   Evas_Object *element_box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *entry = NULL;

   BOX_ADD(parent, element_box, EINA_TRUE, EINA_FALSE);
   BUTTON_ADD(element_box, button, "Del");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   ENTRY_ADD(element_box, entry, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_entry_set(entry, name);
   evas_object_smart_callback_add(entry, "activated",
                                  __on_target_name_change, NULL);
   evas_object_smart_callback_add(button, "clicked", _target_remove_button_cb,
                                  element_box);
   elm_box_pack_end(element_box, entry);
   elm_box_pack_end(element_box, button);
   elm_box_pack_end(parent, element_box);
}

Evas_Object *
prop_item_program_target_add(Evas_Object *parent,
                     const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;
   Evas_Object *button = NULL;

   if (!parent) return NULL;

   ITEM_ADD(parent, item, "targets");
   BOX_ADD(item, box, EINA_TRUE, EINA_FALSE);
   BOX_ADD(box, entrys_box, EINA_FALSE, EINA_FALSE);

   BUTTON_ADD(box, button, "Add");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   evas_object_smart_callback_add(button, "clicked", _target_add_button_cb,
                                  entrys_box);

   elm_box_pack_end(box, entrys_box);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

void
prop_item_program_targets_update(Evas_Object *item)
{
   Eina_List *targets_list = NULL;
   Eina_List *childs = NULL;
   int count_targets = 0;
   int i = 0;

   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;

   if (!item) return;

   targets_list = edje_edit_program_targets_get(prop.group->obj, prop.program);
   count_targets = eina_list_count(targets_list);

   box = elm_object_part_content_get(item, "elm.swallow.content");
   childs = elm_box_children_get(box);
   entrys_box = eina_list_nth(childs, 0);
   elm_box_clear(entrys_box);

   if (!count_targets) return;

   for (i = 0; i < count_targets; i++)
     _target_item_add(entrys_box, eina_list_nth(targets_list, i));
}

void
prop_item_program_in_update(Evas_Object *item)
{
   Evas_Object *box = elm_object_part_content_get(item, "elm.swallow.content");
   Evas_Object *entry = NULL;
   Eina_List *childs = elm_box_children_get(box);
   double range = 0;
   char instr[BUFF_MAX];
   range = edje_edit_program_in_from_get(prop.group->obj, prop.program);
   entry = eina_list_nth(childs, 0);
   snprintf(instr, sizeof(instr), "%2.3f", range);
   elm_entry_entry_set(entry, instr);
   evas_object_smart_callback_del(entry, "activated", __on_in_from_change);
   evas_object_smart_callback_add(entry, "activated",
                                  __on_in_from_change, NULL);
   range = edje_edit_program_in_range_get(prop.group->obj, prop.program);
   entry = eina_list_nth(childs, 1);
   snprintf(instr, sizeof(instr), "%2.3f", range);
   elm_entry_entry_set(entry, instr);
   evas_object_smart_callback_del(entry, "activated", __on_in_range_change);
   evas_object_smart_callback_add(entry, "activated",
                                  __on_in_range_change, NULL);
   eina_list_free(childs);
}

Evas_Object *
prop_item_program_in_add(Evas_Object *parent,
                     const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entry = NULL;

   if (!parent) return NULL;

   ITEM_ADD(parent, item, "in");
   BOX_ADD(item, box, EINA_TRUE, EINA_FALSE);
   ENTRY_ADD(item, entry, EINA_TRUE, DEFAULT_STYLE)
   elm_box_pack_end(box, entry);
   ENTRY_ADD(item, entry, EINA_TRUE, DEFAULT_STYLE)
   elm_box_pack_end(box, entry);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
prop_item_program_name_update(Evas_Object *item)
{
   Evas_Object *entry = elm_object_part_content_get(item, "elm.swallow.content");
   elm_entry_entry_set(entry, prop.program);
   evas_object_smart_callback_del(entry, "activated", __on_program_name_change);
   evas_object_smart_callback_add(entry, "activated",
                                  __on_program_name_change, NULL);
}

static Evas_Object *
prop_progs_add(Evas_Object *parent)
{
   Evas_Object *box;

   BOX_ADD(parent, box, EINA_FALSE, EINA_FALSE)
   elm_box_align_set(box, 0.5, 0.0);

   prop.name = prop_item_program_name_add(box, "Unique name of program ");
   prop.signal = prop_item_program_signal_add(box, "signal");
   prop.source = prop_item_program_source_add(box, "source");
   prop.in = prop_item_program_in_add(box, "in");
   prop.action = prop_item_program_action_add(box, "action");
   prop.transition = prop_item_program_transition_add(box, "transition");
   prop.targets= prop_item_program_target_add(box, "targets");
   prop.afters = prop_item_program_after_add(box, "afters");

   elm_box_pack_end(box, prop.name);
   elm_box_pack_end(box, prop.signal);
   elm_box_pack_end(box, prop.source);
   elm_box_pack_end(box, prop.in);
   elm_box_pack_end(box, prop.action);
   elm_box_pack_end(box, prop.transition);
   elm_box_pack_end(box, prop.targets);
   elm_box_pack_end(box, prop.afters);

   return box;
}

static void
prop_progs_update(void)
{
   prop_item_program_name_update(prop.name);
   prop_item_program_signal_update(prop.signal, prop.group);
   prop_item_program_source_update(prop.source, prop.group);
   prop_item_program_targets_update(prop.targets);
   prop_item_program_in_update(prop.in);
   prop_item_program_action_update(prop.action);
   prop_item_program_transition_update(prop.transition);
   prop_item_program_after_update(prop.afters);
}

static void
_on_gen_prog_sel(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)ei;
   window.sel = glit;
   Eina_Stringshare *program = elm_object_item_data_get(glit);
   prop.program = program;
   prop_progs_update();
}

static char *
_item_prog_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   Eina_Stringshare *prog_label = (Eina_Stringshare *)data;
   return strdup(prog_label);
}

static void
gl_progs_update_sel_item(const char *str)
{
   Eina_Stringshare *item_data = elm_object_item_data_get(window.sel);
   eina_stringshare_replace(&item_data, str);
   elm_object_item_data_set(window.sel, (void *)item_data);
   elm_genlist_item_update(window.sel);
}

static Evas_Object *
gl_progs_add(Evas_Object *parent, Group *group)
{
   Evas_Object *gl_progs;
   Eina_List *progs_list, *l;
   const char *prog_name;
   Eina_Stringshare *item_data;
   Elm_Object_Item *glit;

   if (!_itc_prog)
     {
        _itc_prog = elm_genlist_item_class_new();
        _itc_prog->item_style = "default";
        _itc_prog->func.text_get = _item_prog_label_get;
        _itc_prog->func.content_get = NULL;
        _itc_prog->func.state_get = NULL;
        _itc_prog->func.del = NULL;
     }

   gl_progs = elm_genlist_add(parent);
   evas_object_size_hint_align_set(gl_progs,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_progs,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   evas_object_show(gl_progs);
   progs_list = edje_edit_programs_list_get(group->obj);
   EINA_LIST_FOREACH(progs_list, l, prog_name)
     {
        if (!prog_name) continue; /* if the list have NULL item */
        item_data = eina_stringshare_add(prog_name);
        glit = elm_genlist_item_append(gl_progs, _itc_prog, item_data,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
        elm_object_item_data_set(glit, (void *)item_data);
     }

   evas_object_smart_callback_add(gl_progs, "selected",
                                  _on_gen_prog_sel, NULL);
   edje_edit_string_list_free(progs_list);
   return gl_progs;
}

static void
__on_program_editor_close(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   /* clear stringshare */
}




static void
_on_editor_save(void *data,
                Evas_Object* obj __UNUSED__,
                void *ei __UNUSED__)
{
   Group *group = (Group *)data;
   edje_edit_save(group->obj);
}

static void
_on_editor_cancel(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *ei __UNUSED__)
{
   evas_object_del(window.mwin);
}

static void
_on_add_popup_bt_add(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   Eina_Stringshare *name = elm_entry_entry_get(prog_entry);
   Elm_Object_Item *glit_prog;

   if (!name)
     {
        NOTIFY_WARNING("Program's 'name can not be empty!");
        return;
     }
   if(!edje_edit_program_add(prop.group->obj, name))
     {
        NOTIFY_WARNING("Program's name must be unique!");
        return;
     }

   glit_prog = elm_genlist_item_append(window.gl_progs, _itc_prog, name,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
   elm_genlist_item_selected_set(glit_prog, EINA_TRUE);
   evas_object_del(popup);
}

static void
_on_add_popup_bt_cancel(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}


static void
_on_bt_prog_del(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Elm_Object_Item *glit = elm_genlist_selected_item_get(window.gl_progs);
   const char *program_name;

   program_name = elm_object_item_part_text_get(glit, "elm.text");
   edje_edit_program_del(prop.group->obj, program_name);

   elm_object_item_del(glit);
}

static void
_on_bt_prog_add(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *popup, *box, *bt_yes, *bt_no;
   Evas_Object *prog_box, *prog_label;
   popup = elm_popup_add(window.mwin);
   elm_object_style_set(popup, "eflete/popup");
   elm_object_part_text_set(popup, "title,text", "Add new program:");

   box = elm_box_add(popup);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

   prog_box = elm_box_add(box);
   elm_box_horizontal_set(prog_box, EINA_TRUE);
   evas_object_size_hint_weight_set(prog_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(prog_box, EVAS_HINT_FILL, 0.0);
   evas_object_show(prog_box);

   LABEL_ADD(prog_box, prog_label, "Program name: ")
   elm_box_pack_end(prog_box, prog_label);

   ENTRY_ADD(prog_box, prog_entry, EINA_TRUE, DEFAULT_STYLE);
   elm_box_pack_end(prog_box, prog_entry);

   elm_box_pack_end(box, prog_box);
   elm_object_content_set(popup, box);
   evas_object_show(box);

   BUTTON_ADD(popup, bt_yes, "Add");
   evas_object_smart_callback_add(bt_yes, "clicked", _on_add_popup_bt_add,
                                  popup);
   elm_object_part_content_set(popup, "button1", bt_yes);

   BUTTON_ADD(popup, bt_no, "Cancel");
   evas_object_smart_callback_add(bt_no, "clicked", _on_add_popup_bt_cancel,
                                  popup);
   elm_object_part_content_set(popup, "button2", bt_no);

   evas_object_show(popup);
}

Evas_Object *
program_editor_window_add(Evas_Object *parent, Group *group)
{
   Evas_Object *mw_box, *pans;
   Evas_Object *glist, *prog_prop, *scroller;
   Evas_Object *bt_box, *bt, *box;

   prop.group = group;
   window.mwin = mw_add(parent);
   mw_title_set(window.mwin, "Program editor");
   evas_object_event_callback_add(window.mwin, EVAS_CALLBACK_FREE,
                                  __on_program_editor_close, NULL);

   mw_box = elm_box_add(window.mwin);
   evas_object_size_hint_weight_set(mw_box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mw_box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(mw_box);

   pans = elm_panes_add(mw_box);
   evas_object_size_hint_weight_set(pans, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pans, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(pans, 0.2);
   evas_object_show(pans);

   box = elm_box_add(parent);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(box);

   glist = gl_progs_add(window.mwin, group);
   window.gl_progs = glist;
   elm_box_pack_end(box, glist);
   evas_object_show(glist);

   evas_object_size_hint_align_set(glist,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(glist,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   BUTTON_ADD(parent, bt, "New program");
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_add, NULL);
   elm_box_pack_end(box, bt);

   BUTTON_ADD(parent, bt, "Delete");
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_del, NULL);
   elm_box_pack_end(box, bt);
   elm_object_part_content_set(pans, "left", box);

   SCROLLER_ADD(window.mwin, scroller);
   prog_prop = prop_progs_add(scroller);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   elm_object_content_set(scroller, prog_prop);
   elm_object_part_content_set(pans, "right", scroller);

   bt_box = elm_box_add(mw_box);
   evas_object_size_hint_weight_set(bt_box, EVAS_HINT_EXPAND, 0.0);
   elm_box_horizontal_set(bt_box, EINA_TRUE);
   elm_box_align_set(bt_box, 1.0, 1.0);
   evas_object_show(bt_box);

   BUTTON_ADD(parent, bt, "Apply");
   evas_object_smart_callback_add(bt, "clicked", _on_editor_save, group);
   elm_box_pack_end(bt_box, bt);

   BUTTON_ADD(parent, bt, "Close");
   evas_object_smart_callback_add(bt, "clicked", _on_editor_cancel, NULL);
   elm_box_pack_end(bt_box, bt);

   elm_box_pack_end(mw_box, pans);
   elm_box_pack_end(mw_box, bt_box);

   elm_win_inwin_content_set(window.mwin, mw_box);
   evas_object_show(window.mwin);
   return window.mwin;
}

#undef prop
