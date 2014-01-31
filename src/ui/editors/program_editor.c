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


struct _Program_Editor
{
   Evas_Object *mwin;
   Evas_Object *gl_progs;
   Elm_Object_Item *sel;
   struct {
      Evas_Object *popup;
      Evas_Object *entry;
   } popup;
   struct {
     Style *style;
     const char *program;
     Evas_Object *name;
     Evas_Object *signal;
     Evas_Object *source;
     Evas_Object *in;
     Evas_Object *action;
     Evas_Object *script;
     Evas_Object *transition;
     Evas_Object *targets;
     Evas_Object *afters;
     Edje_Action_Type act_type;
     Edje_Tween_Mode trans_type;
     Evas_Object *prop_box;
   } prop_view;
};

/*
   FIXME: this struct needed to delete, when hoversel can return
  index of selected item.
*/
struct _Index
{
   int i;
};

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

typedef struct _Index Index;
typedef struct _Program_Editor Program_Editor;
static Elm_Genlist_Item_Class *_itc_prog;
static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = EDITORS_BANNED_SYMBOLS
};

#define prop prog_edit->prop_view

#define ITEM_ADD_(parent, item, text, style) \
   item = elm_layout_add(parent); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(item, TET_EDJ, "property/item/"style); \
   elm_object_part_text_set(item, "elm.text", text); \
   evas_object_show(item);


#define ITEM_STRING_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                              Evas_Object *obj, \
                              void *ei __UNUSED__) \
{ \
   Program_Editor *prog_edit = (Program_Editor *)data; \
   const char *value = elm_entry_entry_get(obj); \
   Eina_Bool res = edje_edit_##sub##_##value##_set(prop.style->obj, \
                                                   prop.program, \
                                                   value); \
   if (!res) \
     { \
        NOTIFY_WARNING("The entered data is not valid!") \
        return; \
     } \
}

#define ITEM_1ENTRY_ADD(text, sub, value) \
static Evas_Object * \
_prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD_(parent, item, text, "editor") \
   ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   elm_object_tooltip_text_set(entry, tooltip); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1ENTRY_UPDATE(sub, value) \
static void \
_prop_item_##sub##_##value##_update(Evas_Object *item, Program_Editor *prog_edit) \
{ \
   Evas_Object *entry; \
   const char *value; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##sub##_##value##_get(prop.style->obj, prop.program); \
   elm_entry_entry_set(entry, value); \
   evas_object_smart_callback_del(entry, "activated", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", \
                                  _on_##sub##_##value##_change, prog_edit); \
}

#define ITEM_1ENTRY_PROG_CREATE(text, sub, value) \
   ITEM_STRING_CALLBACK(sub, value) \
   ITEM_1ENTRY_ADD(text, sub, value) \
   ITEM_1ENTRY_UPDATE(sub, value) \


ITEM_1ENTRY_PROG_CREATE("signal", program, signal)
ITEM_1ENTRY_PROG_CREATE("source", program, source)
ITEM_1ENTRY_ADD("name", program, name)

static void
_gl_progs_update_sel_item(const char *str, Program_Editor *prog_edit)
{
   Eina_Stringshare *item_data = elm_object_item_data_get(prog_edit->sel);
   eina_stringshare_replace(&item_data, str);
   elm_object_item_data_set(prog_edit->sel, (void *)item_data);
   elm_genlist_item_update(prog_edit->sel);
}

static void
_item_prog_del(void *data,
               Evas_Object *obj __UNUSED__)
{
   eina_stringshare_del(data);
}

static void
_on_program_name_change(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_name_set(prop.style->obj, prop.program,
                                              value);
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
   _gl_progs_update_sel_item(value, prog_edit);
   prop.program = value;
}

static void
_on_in_from_change(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_in_from_set(prop.style->obj, prop.program,
                                                 atof(value));
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
_on_in_range_change(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_in_range_set(prop.style->obj, prop.program,
                                                 atof(value));
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
_on_target_name_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = edje_edit_program_target_add(prop.style->obj, prop.program,
                                                value);
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static Evas_Object *
_prop_item_program_script_add(Evas_Object *parent,
                     const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entry = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, "script", "script");

   BOX_ADD(item, box, true, false);
   ENTRY_ADD(item, entry, false, DEFAULT_STYLE)
   elm_scroller_policy_set(entry, ELM_SCROLLER_POLICY_OFF,
                                  ELM_SCROLLER_POLICY_AUTO);
   elm_entry_editable_set(entry, false);
   elm_box_pack_end(box, entry);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_script_update(Program_Editor *prog_edit)
{
   Evas_Object *box = NULL;
   char *script = NULL;
   Evas_Object *entry = NULL;

   box = elm_object_part_content_get(prop.script, "elm.swallow.content");
   Eina_List *childs = elm_box_children_get(box);

   script = edje_edit_script_program_get(prop.style->obj, prop.program);
   entry = eina_list_nth(childs, 0);
   script = elm_entry_utf8_to_markup(script);
   elm_entry_entry_set(entry, script);
   eina_list_free(childs);
}

static void
_on_hoversel_trans_sel(void *data,
                 Evas_Object *obj,
                 void *ei)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Index *index = elm_object_item_data_get(ei);
   Evas_Object *item = prop.transition;
   Evas_Object *box = elm_object_part_content_get(item, "elm.swallow.content");
   Eina_List *childs = elm_box_children_get(box);
   Evas_Object *entry1 = eina_list_nth(childs, 1);
   Evas_Object *entry2 = eina_list_nth(childs, 2);
   Evas_Object *entry3 = eina_list_nth(childs, 3);
   elm_entry_entry_set(entry1, "");
   elm_entry_entry_set(entry2, "");
   elm_entry_entry_set(entry3, "");

   elm_object_text_set(obj, transition_type[index->i]);
   edje_edit_program_transition_set(prop.style->obj, prop.program,
                                    (Edje_Tween_Mode)index->i);
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
   Program_Editor *prog_edit = (Program_Editor*)data;
   Index *index = elm_object_item_data_get(ei);
   Evas_Object *item = prop.action;
   Evas_Object *box = elm_object_part_content_get(item, "elm.swallow.content");
   Eina_List *childs = elm_box_children_get(box);
   Evas_Object *entry1 = eina_list_nth(childs, 1);
   Evas_Object *entry2 = eina_list_nth(childs, 2);
   Evas_Object *hoversel_trans = NULL;
   Evas_Object *entry_trans = NULL;
   elm_entry_entry_set(entry1, "");
   elm_entry_entry_set(entry2, "");

   elm_object_text_set(obj, action_type[index->i]);
   edje_edit_program_action_set(prop.style->obj, prop.program,
                                (Edje_Action_Type)index->i);
   evas_object_hide(entry1);
   evas_object_hide(entry2);
   prop.act_type = (Edje_Action_Type)index->i;

   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        box = elm_object_part_content_get(prop.transition,
                                          "elm.swallow.content");
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

   if (prop.script)
     {
        elm_box_unpack(prop.prop_box, prop.script);
        evas_object_del(prop.script);
        prop.script = NULL;
     }

   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
      break;
      case EDJE_ACTION_TYPE_SCRIPT:
        {

           prop.script = _prop_item_program_script_add(prop.prop_box, "script");
           _prop_item_program_script_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.script, prop.action);
           evas_object_hide(entry1);
           evas_object_hide(entry2);
        }
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
_on_transition_time_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_time_set(prop.style->obj, prop.program,
                                              atof(value)))
     NOTIFY_WARNING("The entered data is not valid!");
}


static void
_on_state_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *state;
   state = elm_entry_entry_get(obj);
   if (!edje_edit_program_state_set(prop.style->obj, prop.program, state))
     NOTIFY_WARNING("The entered data is not valid!");
}

static void
_on_value2_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
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
           if (!edje_edit_program_value2_set(prop.style->obj, prop.program,
                                             atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");
        }
      break;
      default:
      break;
     }
}

static void
_on_v1_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_value1_set(prop.style->obj, prop.program,
                                                atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");

}

static void
_on_v2_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value;
   value = elm_entry_entry_get(obj);
   if (!edje_edit_program_transition_value2_set(prop.style->obj, prop.program,
                                                atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");

}

static void
_on_value_active(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
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
           if (!edje_edit_program_value_set(prop.style->obj, prop.program,
                                            atof(value)))
             NOTIFY_WARNING("The entered data is not valid!");
        }
      break;
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           if (!edje_edit_program_state2_set(prop.style->obj, prop.program,
                                             value))
             NOTIFY_WARNING("the entered data is not valid!");
        }
      break;
      default:
      break;
     }
}

static void
_on_after_name_change(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   const char *value = elm_entry_entry_get(obj);
   Eina_Bool res = false;

   res = edje_edit_program_after_add(prop.style->obj, prop.program, value);
   if (!res)
     {
        NOTIFY_WARNING("The entered data is not valid!")
        return;
     }
}

static void
_after_remove_button_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Evas_Object *entry = NULL;
   Eina_List *childs = NULL;

   Evas_Object *element_box = elm_object_part_content_get(prop.afters,
                                 "elm.swallow.content");

   Evas_Object *entrys_box = elm_object_parent_widget_get(element_box);

   childs = elm_box_children_get(element_box);
   entry = eina_list_nth(childs, 0);
   elm_box_unpack(entrys_box, element_box);
   edje_edit_program_after_del(prop.style->obj, prop.program,
                               elm_entry_entry_get(entry));
   evas_object_smart_callback_del(entry, "activated", _on_after_name_change);
   eina_list_free(childs);
   evas_object_del(element_box);
}

static void
_after_item_add(Program_Editor *prog_edit, char *name)
{
   Evas_Object *element_box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *entry = NULL;
   Eina_List *childs = NULL;
   Evas_Object *item_box = NULL;

   childs = elm_box_children_get(elm_object_part_content_get(prop.afters,
                                          "elm.swallow.content"));
   item_box = eina_list_nth(childs, 0);
   BOX_ADD(item_box, element_box, true, false);
   BUTTON_ADD(element_box, button, "Del");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   ENTRY_ADD(element_box, entry, true, DEFAULT_STYLE);
   elm_entry_entry_set(entry, name);
   evas_object_smart_callback_add(entry, "activated",_on_after_name_change,
                                  prog_edit);
   evas_object_smart_callback_add(button, "clicked", _after_remove_button_cb,
                                  prog_edit);
   elm_box_pack_end(element_box, entry);
   elm_box_pack_end(element_box, button);
   elm_box_pack_end(item_box, element_box);
   eina_list_free(childs);
}

static void
_after_add_button_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   _after_item_add(prog_edit, "");
}

static void
_target_remove_button_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Evas_Object *entry = NULL;
   Eina_List *childs = NULL;

   Evas_Object *element_box = elm_object_part_content_get(prop.targets,
                                 "elm.swallow.content");
   Evas_Object *entrys_box = elm_object_parent_widget_get(element_box);

   childs = elm_box_children_get(element_box);
   entry = eina_list_nth(childs, 0);
   elm_box_unpack(entrys_box, element_box);
   edje_edit_program_target_del(prop.style->obj, prop.program,
                                elm_entry_entry_get(entry));
   evas_object_smart_callback_del(entry, "activated", _on_target_name_change);
   eina_list_free(childs);
   evas_object_del(element_box);
}

static void
_target_item_add(Program_Editor *prog_edit, char *name)
{
   Evas_Object *element_box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *entry = NULL;
   Evas_Object *item_box = NULL;
   Eina_List *childs = NULL;

   childs = elm_box_children_get(elm_object_part_content_get(prop.targets,
                                          "elm.swallow.content"));
   item_box = eina_list_nth(childs, 0);

   BOX_ADD(item_box, element_box, true, false);
   BUTTON_ADD(element_box, button, "Del");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   ENTRY_ADD(element_box, entry, true, DEFAULT_STYLE);
   elm_entry_entry_set(entry, name);
   evas_object_smart_callback_add(entry, "activated", _on_target_name_change,
                                  prog_edit);
   evas_object_smart_callback_add(button, "clicked", _target_remove_button_cb,
                                  prog_edit);
   elm_box_pack_end(element_box, entry);
   elm_box_pack_end(element_box, button);
   elm_box_pack_end(item_box, element_box);
   eina_list_free(childs);
}

static void
_target_add_button_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   _target_item_add(prog_edit, "");
}

static void
_prop_item_program_transition_update(Program_Editor *prog_edit)
{
   Evas_Object *box, *hoversel, *entry1, *entry2, *entry3;
   Eina_List *nodes;
   char buff[BUFF_MAX];
   double value;
   if (!prop.transition) return;
   prop.trans_type =
      edje_edit_program_transition_get(prop.style->obj, prop.program);
   box = elm_object_part_content_get(prop.transition, "elm.swallow.content");
   nodes = elm_box_children_get(box);
   hoversel = eina_list_nth(nodes, 0);
   entry1 = eina_list_nth(nodes, 1);
   entry2 = eina_list_nth(nodes, 2);
   entry3 = eina_list_nth(nodes, 3);
   evas_object_show(entry1);
   evas_object_show(entry2);
   evas_object_show(entry3);
   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     prop.trans_type = EDJE_TWEEN_MODE_NONE;
   elm_object_text_set(hoversel, transition_type[(int)prop.trans_type]);
   value = edje_edit_program_transition_time_get(prop.style->obj,
                                                 prop.program);
   evas_object_smart_callback_del(entry1, "activated",
                                  _on_transition_time_active);
   evas_object_smart_callback_add(entry1, "activated",
                                  _on_transition_time_active, prog_edit);

   snprintf(buff, sizeof(buff), "%1.2f", value);
   elm_entry_entry_set(entry1, buff);
   switch (prop.trans_type)
     {
      case EDJE_TWEEN_MODE_NONE:
        {
           evas_object_hide(entry1);
           evas_object_hide(entry2);
           evas_object_hide(entry3);
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
           value =
              edje_edit_program_transition_value1_get(prop.style->obj,
                                                      prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry2, buff);
           evas_object_smart_callback_del(entry2, "activated", _on_v1_active);
           evas_object_smart_callback_add(entry2, "activated", _on_v1_active,
                                          prog_edit);
           value = edje_edit_program_transition_value2_get(prop.style->obj,
                                                           prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry3, buff);
           evas_object_smart_callback_del(entry3, "activated", _on_v2_active);
           evas_object_smart_callback_add(entry3, "activated", _on_v2_active,
                                          prog_edit);

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
static Evas_Object *
_prop_item_program_transition_add(Evas_Object *parent,
                                  Program_Editor *prog_edit,
                                  const char *tooltip)
{
   Evas_Object *item, *box, *hoversel, *entry1, *entry2, *entry3;
   Elm_Object_Item *hovit;
   Index *index;
   int i = 0;

   ITEM_ADD_(parent, item, "transition", "editor")
   BOX_ADD(item, box, true, true)
   HOVERSEL_ADD(item, hoversel, false)
   ENTRY_ADD(item, entry1, true, DEFAULT_STYLE)
   ENTRY_ADD(item, entry2, true, DEFAULT_STYLE)
   ENTRY_ADD(item, entry3, true, DEFAULT_STYLE)
   elm_hoversel_hover_parent_set(hoversel, prog_edit->mwin);
   elm_object_text_set(hoversel, transition_type[0]);

   for (i = 0; i < TRANSITIONS_COUNT; i++)
     {
        hovit = elm_hoversel_item_add(hoversel, transition_type[i], NULL,
                                 ELM_ICON_NONE, NULL, NULL);
        INDEX_APPEND(i)
     }
   evas_object_smart_callback_add(hoversel, "selected", _on_hoversel_trans_sel,
                                  prog_edit);


   elm_box_pack_end(box, hoversel);
   elm_box_pack_end(box, entry1);
   elm_box_pack_end(box, entry2);
   elm_box_pack_end(box, entry3);

   elm_object_tooltip_text_set(item, tooltip);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static Evas_Object *
_prop_item_program_action_add(Evas_Object *parent,
                             Program_Editor *prog_edit,
                             const char *tooltip)
{
   Evas_Object *item, *box, *hoversel, *entry1, *entry2;
   Elm_Object_Item *hovit;
   Index *index;

   ITEM_ADD_(parent, item, "action", "editor")
   BOX_ADD(item, box, true, true)
   HOVERSEL_ADD(item, hoversel, false)
   ENTRY_ADD(item, entry1, true, DEFAULT_STYLE)
   ENTRY_ADD(item, entry2, true, DEFAULT_STYLE)
   evas_object_hide(entry1);
   evas_object_hide(entry2);

   elm_hoversel_hover_parent_set(hoversel, prog_edit->mwin);
   elm_object_text_set(hoversel, action_type[0]);

   hovit = elm_hoversel_item_add(hoversel, action_type[0], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(0)
   hovit = elm_hoversel_item_add(hoversel, action_type[1], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(1)
   hovit = elm_hoversel_item_add(hoversel, action_type[2], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(2)
   hovit = elm_hoversel_item_add(hoversel, action_type[3], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(3)
   hovit = elm_hoversel_item_add(hoversel, action_type[4], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(4)
   hovit = elm_hoversel_item_add(hoversel, action_type[5], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(5)
   hovit = elm_hoversel_item_add(hoversel, action_type[6], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(6)
   hovit = elm_hoversel_item_add(hoversel, action_type[7], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(7)
   hovit = elm_hoversel_item_add(hoversel, action_type[8], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(8)
   hovit = elm_hoversel_item_add(hoversel, action_type[10], NULL, ELM_ICON_NONE,
                                 NULL, NULL);
   INDEX_APPEND(10)
   evas_object_smart_callback_add(hoversel, "selected", _on_hoversel_sel,
                                  prog_edit);

   elm_object_tooltip_text_set(item, tooltip);
   elm_box_pack_end(box, hoversel);
   elm_box_pack_end(box, entry1);
   elm_box_pack_end(box, entry2);

   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}
#undef INDEX_APPEND

static void
_prop_item_program_action_update(Program_Editor *prog_edit)
{
   Evas_Object *box, *hoversel, *entry1, *entry2;
   Eina_List *nodes = NULL;
   Eina_Stringshare *str = NULL;
   char buff[BUFF_MAX];
   double value;

   if (!prop.action) return;
   prop.act_type = edje_edit_program_action_get(prop.style->obj, prop.program);
   box = elm_object_part_content_get(prop.action, "elm.swallow.content");
   nodes = elm_box_children_get(box);
   hoversel = eina_list_nth(nodes, 0);
   entry1 = eina_list_nth(nodes, 1);
   entry2 = eina_list_nth(nodes, 2);
   evas_object_show(entry1);
   evas_object_show(entry2);

   elm_object_text_set(hoversel, action_type[(int)prop.act_type]);

   if (prop.script)
     {
        elm_box_unpack(prop.prop_box, prop.script);
        evas_object_del(prop.script);
        prop.script = NULL;
     }

   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_SCRIPT:
        {
           prop.script = _prop_item_program_script_add(prop.prop_box, "script");
           _prop_item_program_script_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.script, prop.action);
           evas_object_hide(entry1);
           evas_object_hide(entry2);
        }
      break;
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
        {
           evas_object_hide(entry1);
           evas_object_hide(entry2);
        }
      break;
      case EDJE_ACTION_TYPE_STATE_SET:
        {
           str = edje_edit_program_state_get(prop.style->obj, prop.program);
           elm_entry_entry_set(entry1, str);
           edje_edit_string_free(str);
           evas_object_smart_callback_del(entry1, "activated", _on_state_active);
           evas_object_smart_callback_add(entry1, "activated", _on_state_active,
                                          prog_edit);
           value = edje_edit_program_value_get(prop.style->obj, prop.program);
           sprintf(buff, "%1.2f", value);
           elm_entry_entry_set(entry2, buff);
           evas_object_smart_callback_del(entry2, "activated", _on_value_active);
           evas_object_smart_callback_add(entry2, "activated", _on_value_active,
                                          prog_edit);
        }
      break;
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           str = edje_edit_program_state_get(prop.style->obj, prop.program);
           elm_entry_entry_set(entry1, str);
           edje_edit_string_free(str);
           evas_object_smart_callback_del(entry1, "activated", _on_state_active);
           evas_object_smart_callback_add(entry1, "activated", _on_state_active,
                                          prog_edit);
           str = edje_edit_program_state2_get(prop.style->obj, prop.program);
           elm_entry_entry_set(entry2, str);
           edje_edit_string_free(str);
           evas_object_smart_callback_del(entry2, "activated", _on_value_active);
           evas_object_smart_callback_add(entry2, "activated", _on_value_active,
                                          prog_edit);
        }
      break;
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           value = edje_edit_program_value_get(prop.style->obj, prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry1, buff);
           evas_object_smart_callback_del(entry1, "activated", _on_value_active);
           evas_object_smart_callback_add(entry1, "activated", _on_value_active,
                                          prog_edit);
           value = edje_edit_program_value2_get(prop.style->obj, prop.program);
           snprintf(buff, sizeof(buff), "%1.2f", value);
           elm_entry_entry_set(entry2, buff);
           evas_object_smart_callback_del(entry2, "activated", _on_value2_active);
           evas_object_smart_callback_add(entry2, "activated", _on_value2_active,
                                          prog_edit);
        }
      break;
      default:
      break;
     }
   eina_list_free(nodes);
}

static Evas_Object *
_prop_item_program_after_add(Evas_Object *parent,
                             Program_Editor *prog_edit,
                             const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;
   Evas_Object *button = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, "after", "editor");
   BOX_ADD(item, box, true, false);
   BOX_ADD(box, entrys_box, false, false);

   BUTTON_ADD(box, button, "Add");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   evas_object_smart_callback_add(button, "clicked", _after_add_button_cb,
                                  prog_edit);

   elm_box_pack_end(box, entrys_box);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_after_update(Program_Editor *prog_edit)
{
   Eina_List *afters_list = NULL;
   Eina_List *childs = NULL;
   int count_afters = 0;
   int i = 0;

   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;

   if (!prop.afters) return;

   afters_list = edje_edit_program_afters_get(prop.style->obj, prop.program);
   count_afters = eina_list_count(afters_list);

   box = elm_object_part_content_get(prop.afters, "elm.swallow.content");
   childs = elm_box_children_get(box);
   entrys_box = eina_list_nth(childs, 0);
   elm_box_clear(entrys_box);

   if (!count_afters) return;

   for (i = 0; i < count_afters; i++)
     _after_item_add(prog_edit, eina_list_nth(afters_list, i));
   eina_list_free(childs);
   eina_list_free(afters_list);
}



static Evas_Object *
_prop_item_program_target_add(Evas_Object *parent,
                              Program_Editor *prog_edit,
                              const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entries_box = NULL;
   Evas_Object *button = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, "targets", "editor");
   BOX_ADD(item, box, true, false);
   BOX_ADD(box, entries_box, false, false);

   BUTTON_ADD(box, button, "Add");
   evas_object_size_hint_weight_set(button, 0.0, 0.0);

   elm_box_pack_end(box, entries_box);
   elm_box_pack_end(box, button);
   elm_object_part_content_set(item, "elm.swallow.content", box);

   evas_object_smart_callback_add(button, "clicked", _target_add_button_cb,
                                  prog_edit);
   return item;
}

static void
_prop_item_program_targets_update(Program_Editor *prog_edit)
{
   Eina_List *targets_list = NULL;
   Eina_List *childs = NULL;
   int count_targets = 0;
   int i = 0;

   Evas_Object *box = NULL;
   Evas_Object *entrys_box = NULL;

   if (!prop.targets) return;

   targets_list = edje_edit_program_targets_get(prop.style->obj, prop.program);
   count_targets = eina_list_count(targets_list);

   box = elm_object_part_content_get(prop.targets, "elm.swallow.content");
   childs = elm_box_children_get(box);
   entrys_box = eina_list_nth(childs, 0);
   elm_box_clear(entrys_box);

   if (!count_targets) return;

   for (i = 0; i < count_targets; i++)
     _target_item_add(prog_edit, eina_list_nth(targets_list, i));
   eina_list_free(childs);
   eina_list_free(targets_list);
}

static void
_prop_item_program_in_update(Program_Editor *prog_edit)
{
   Evas_Object *box = elm_object_part_content_get(prop.in,
                                                  "elm.swallow.content");
   Evas_Object *entry = NULL;
   Eina_List *childs = elm_box_children_get(box);
   double range = 0;
   char instr[BUFF_MAX];

   range = edje_edit_program_in_from_get(prop.style->obj, prop.program);
   entry = eina_list_nth(childs, 0);
   snprintf(instr, sizeof(instr), "%2.3f", range);
   elm_entry_entry_set(entry, instr);
   evas_object_smart_callback_del(entry, "activated", _on_in_from_change);
   evas_object_smart_callback_add(entry, "activated",
                                  _on_in_from_change, prog_edit);
   range = edje_edit_program_in_range_get(prop.style->obj,
                                          prop.program);
   entry = eina_list_nth(childs, 1);
   snprintf(instr, sizeof(instr), "%2.3f", range);
   elm_entry_entry_set(entry, instr);
   evas_object_smart_callback_del(entry, "activated", _on_in_range_change);
   evas_object_smart_callback_add(entry, "activated",
                                  _on_in_range_change, prog_edit);
   eina_list_free(childs);
}

static Evas_Object *
_prop_item_program_in_add(Evas_Object *parent,
                     const char *tooltip __UNUSED__)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *entry = NULL;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, "in", "editor");

   BOX_ADD(item, box, true, false);
   ENTRY_ADD(item, entry, true, DEFAULT_STYLE)
   elm_box_pack_end(box, entry);
   ENTRY_ADD(item, entry, true, DEFAULT_STYLE)
   elm_box_pack_end(box, entry);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_name_update(Program_Editor *prog_edit)
{
   Evas_Object *entry = elm_object_part_content_get(prop.name,
                                                    "elm.swallow.content");
   elm_entry_entry_set(entry, prop.program);
   evas_object_smart_callback_del(entry, "activated", _on_program_name_change);
   evas_object_smart_callback_add(entry, "activated", _on_program_name_change,
                                  prog_edit);
}

static Evas_Object *
_prop_progs_add(Evas_Object *parent, Program_Editor *prog_edit)
{
   Evas_Object *box;

   BOX_ADD(parent, box, false, false)
   evas_object_size_hint_align_set(box, 0.5, 0);

   prop.name = _prop_item_program_name_add(box, "Unique name of program ");
   prop.signal = _prop_item_program_signal_add(box, "signal");
   prop.source = _prop_item_program_source_add(box, "source");
   prop.in = _prop_item_program_in_add(box, "in");
   prop.action = _prop_item_program_action_add(box, prog_edit, "action");
   prop.transition = _prop_item_program_transition_add(box, prog_edit,
                                                       "transition");
   prop.targets = _prop_item_program_target_add(box, prog_edit, "targets");
   prop.afters = _prop_item_program_after_add(box, prog_edit, "afters");

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
_prop_progs_update(Program_Editor *prog_edit)
{
   _prop_item_program_name_update(prog_edit);
   _prop_item_program_signal_update(prop.signal, prog_edit);
   _prop_item_program_source_update(prop.source, prog_edit);
   _prop_item_program_targets_update(prog_edit);
   _prop_item_program_in_update(prog_edit);
   _prop_item_program_action_update(prog_edit);
   _prop_item_program_transition_update(prog_edit);
   _prop_item_program_after_update(prog_edit);
}

static void
_on_gen_prog_sel(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)ei;

   prog_edit->sel = glit;
   Eina_Stringshare *program = elm_object_item_data_get(glit);
   prop.program = program;
   _prop_progs_update(prog_edit);
}

static char *
_item_prog_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   Eina_Stringshare *prog_label = (Eina_Stringshare *)data;
   return strdup(prog_label);
}

static Evas_Object *
_gl_progs_add(Program_Editor *prog_edit)
{
   Evas_Object *gl_progs = NULL;;
   Eina_List *progs_list = NULL;
   Eina_List *l = NULL;

   const char *prog_name = NULL;
   Eina_Stringshare *item_data = NULL;
   Elm_Object_Item *glit = NULL;

   if (!_itc_prog)
     {
        _itc_prog = elm_genlist_item_class_new();
        _itc_prog->item_style = DEFAULT_STYLE;
        _itc_prog->func.text_get = _item_prog_label_get;
        _itc_prog->func.content_get = NULL;
        _itc_prog->func.state_get = NULL;
        _itc_prog->func.del = _item_prog_del;
     }

   gl_progs = elm_genlist_add(prog_edit->mwin);
   elm_object_style_set(gl_progs, DEFAULT_STYLE);
   evas_object_size_hint_align_set(gl_progs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_progs, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   elm_scroller_policy_set(gl_progs, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);
   evas_object_show(gl_progs);

   progs_list = edje_edit_programs_list_get(prop.style->obj);
   EINA_LIST_FOREACH(progs_list, l, prog_name)
     {
        if (!prog_name) continue; /* if the list have NULL item */
        item_data = eina_stringshare_add(prog_name);
        glit = elm_genlist_item_append(gl_progs, _itc_prog, item_data,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
        elm_object_item_data_set(glit, (void *)item_data);
     }

   evas_object_smart_callback_add(gl_progs, "selected", _on_gen_prog_sel,
                                  prog_edit);
   edje_edit_string_list_free(progs_list);
   elm_scroller_policy_set(gl_progs, ELM_SCROLLER_POLICY_AUTO,
                                   ELM_SCROLLER_POLICY_AUTO);
   return gl_progs;
}

static void
_on_program_editor_close(void *data,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   free(prog_edit);
}

static void
_on_editor_save(void *data,
                Evas_Object* obj __UNUSED__,
                void *ei __UNUSED__)
{
   Style *style = (Style *)data;
   edje_edit_save(style->obj);
}

static void
_on_editor_cancel(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *ei __UNUSED__)
{
   Evas_Object *mwin = (Evas_Object *)data;
   evas_object_del(mwin);
}

static void
_on_add_popup_bt_add(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   Elm_Object_Item *glit_prog = NULL;
   Program_Editor *prog_edit = (Program_Editor*)data;
   Eina_Stringshare *name = elm_entry_entry_get(prog_edit->popup.entry);

   if ((!name) || (strcmp(name, "") == 0))
     {
        NOTIFY_WARNING("Program's 'name can not be empty!");
        return;
     }
   if(!edje_edit_program_add(prop.style->obj, name))
     {
        NOTIFY_WARNING("Program's name must be unique!");
        return;
     }

   glit_prog = elm_genlist_item_append(prog_edit->gl_progs, _itc_prog, name,
                                       NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_prog, true);
   evas_object_del(prog_edit->popup.popup);
   prog_edit->popup.popup = NULL;
   prog_edit->popup.entry = NULL;
}

static void
_on_add_popup_bt_cancel(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;

   evas_object_del(prog_edit->popup.popup);
   prog_edit->popup.popup = NULL;
   prog_edit->popup.entry = NULL;
}


static void
_on_bt_prog_del(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Program_Editor *prog_edit = (Program_Editor*)data;

   Elm_Object_Item *glit = elm_genlist_selected_item_get(prog_edit->gl_progs);
   const char *program_name = NULL;

   program_name = elm_object_item_part_text_get(glit, "elm.text");
   if (!edje_edit_program_del(prop.style->obj, program_name))
     {
        NOTIFY_WARNING("Failed to delete %s from style %s", program_name,
                    prop.style->name)
     }
   else
     elm_object_item_del(glit);
}

static void
_on_bt_prog_add(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *box = NULL;
   Evas_Object *button = NULL;
   Evas_Object *prog_box, *prog_label;

   Program_Editor *prog_edit = (Program_Editor*)data;

   prog_edit->popup.popup = elm_popup_add(prog_edit->mwin);
   elm_object_style_set(prog_edit->popup.popup, "eflete");
   elm_object_part_text_set(prog_edit->popup.popup, "title,text",
                            "Add new program:");

   BOX_ADD(prog_edit->popup.popup, box, false, false);
   BOX_ADD(box, prog_box, true, false);

   LABEL_ADD(prog_box, prog_label, "Program name: ")
   elm_box_pack_end(prog_box, prog_label);

   ENTRY_ADD(prog_box, prog_edit->popup.entry, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(prog_edit->popup.entry,
                                  elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(prog_edit->popup.entry, "guide",
                            "Type new program name here.");
   elm_box_pack_end(prog_box, prog_edit->popup.entry);

   elm_box_pack_end(box, prog_box);
   elm_object_content_set(prog_edit->popup.popup, box);

   BUTTON_ADD(prog_edit->popup.popup, button, "Ok");
   evas_object_smart_callback_add(button, "clicked", _on_add_popup_bt_add,
                                  prog_edit);
   elm_object_part_content_set(prog_edit->popup.popup, "button1", button);

   BUTTON_ADD(prog_edit->popup.popup, button, "Cancel");
   evas_object_smart_callback_add(button, "clicked", _on_add_popup_bt_cancel,
                                  prog_edit);
   elm_object_part_content_set(prog_edit->popup.popup, "button2", button);

   evas_object_show(prog_edit->popup.popup);
}

Evas_Object *
program_editor_window_add(Style *style)
{
   Evas_Object *mw_box, *pans;
   Evas_Object *scroller;
   Evas_Object *bt, *box;
   Program_Editor *prog_edit = NULL;


   if (!style)
     {
        ERR("Failed create image editor for current style");
        return NULL;
     }

   prog_edit = (Program_Editor *)mem_calloc(1, sizeof(Program_Editor));

   prop.style = style;
   prog_edit->mwin = mw_add(NULL, NULL);
   mw_title_set(prog_edit->mwin, "Program editor");
   evas_object_event_callback_add(prog_edit->mwin, EVAS_CALLBACK_FREE,
                                  _on_program_editor_close, prog_edit);

   BOX_ADD(prog_edit->mwin, mw_box, false, false);

   pans = elm_panes_add(mw_box);
   elm_object_style_set(pans, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(pans, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pans, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(pans, 0.2);
   evas_object_show(pans);

   BOX_ADD(mw_box, box, false, false);

   prog_edit->gl_progs = _gl_progs_add(prog_edit);
   elm_box_pack_end(box, prog_edit->gl_progs);
   evas_object_show(prog_edit->gl_progs);

   evas_object_size_hint_align_set(prog_edit->gl_progs, EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(prog_edit->gl_progs, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   BUTTON_ADD(box, bt, "New program");
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_add, prog_edit);
   elm_box_pack_end(box, bt);

   BUTTON_ADD(box, bt, "Delete");
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_del, prog_edit);
   elm_box_pack_end(box, bt);
   elm_object_part_content_set(pans, "left", box);

   SCROLLER_ADD(pans, scroller);
   prop.prop_box = _prop_progs_add(scroller, prog_edit);
   elm_object_content_set(scroller,   prop.prop_box);
   elm_object_part_content_set(pans, "right", scroller);

   BOX_ADD(mw_box, box, true, false);
   evas_object_size_hint_align_set(box, 1, 0.5);

   BUTTON_ADD(box, bt, "Apply");
   evas_object_smart_callback_add(bt, "clicked", _on_editor_save, prop.style);
   elm_box_pack_end(box, bt);

   BUTTON_ADD(box, bt, "Close");
   evas_object_smart_callback_add(bt, "clicked", _on_editor_cancel,
                                  prog_edit->mwin);
   elm_box_pack_end(box, bt);

   elm_box_pack_end(mw_box, pans);
   elm_box_pack_end(mw_box, box);

   elm_win_inwin_content_set(prog_edit->mwin, mw_box);
   evas_object_show(prog_edit->mwin);
   return prog_edit->mwin;
}

#undef prop
#undef ITEM_ADD_
