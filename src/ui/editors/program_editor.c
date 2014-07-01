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

#include "program_editor.h"
#include "ui_main_window.h"

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
     struct {
        Evas_Object *item;
        Evas_Object *combobox;
        Evas_Object *layout1, *entry1;
        Evas_Object *layout2, *entry2;
     } action;
     Evas_Object *script;
     struct {
        Evas_Object *item;
        Evas_Object *combobox;
        Evas_Object *layout1, *entry1;
        Evas_Object *layout2, *entry2;
        Evas_Object *layout3, *entry3;
     } transition;
     Evas_Object *targets;
     Evas_Object *afters;
     Edje_Action_Type act_type;
     Edje_Tween_Mode trans_type;
     Evas_Object *prop_box;
   } prop_view;
};

static const char *transition_type[] = {
                                    N_("NONE"),
                                    N_("LINEAR"),
                                    N_("SINUSOIDAL"),
                                    N_("ACCELERATE"),
                                    N_("DECELERATE"),
                                    N_("ACCELERATE FACTOR"),
                                    N_("DECELERATE FACTOR"),
                                    N_("SINUSOIDAL FACTOR"),
                                    N_("DIVISOR INTERP"),
                                    N_("BOUNCE"),
                                    N_("SPRING")};


static const char *action_type[] = {N_("NONE"),
                                    N_("STATE_SET"),
                                    N_("ACTION_STOP"),
                                    N_("SIGNAL_EMIT"),
                                    N_("DRAG_VAL_SET"),
                                    N_("DRAG_VAL_STEP"),
                                    N_("DRAG_VAL_PAGE"),
                                    N_("SCRIPT"),
                                    N_("FOCUS_SET"),
                                    N_("RESERVED00"),
                                    N_("FOCUS_OBJECT"),
                                    N_("PARAM_COPY"),
                                    N_("PARAM_SET"),
                                    N_("SOUND_SAMPLE"),
                                    N_("SOUND_TONE")};

typedef struct _Program_Editor Program_Editor;
static Elm_Genlist_Item_Class *_itc_prog;
static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = EDITORS_BANNED_SYMBOLS
};

#define prop prog_edit->prop_view
#define action prog_edit->prop_view.action
#define transition prog_edit->prop_view.transition

#define ITEM_ADD_(parent, item, text, style) \
   item = elm_layout_add(parent); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(item, EFLETE_EDJ, "property/item/"style); \
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
        NOTIFY_WARNING(_("The entered data is not valid!")) \
        return; \
     } \
}

#define ITEM_1ENTRY_ADD(text, sub, value, regex) \
static Evas_Object * \
_prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD_(parent, item, text, "editor") \
   EWE_ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   REGEX_SET(entry, regex); \
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
   ewe_entry_entry_set(entry, value); \
   evas_object_smart_callback_del(entry, "activated", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", \
                                  _on_##sub##_##value##_change, prog_edit); \
}

#define ITEM_1ENTRY_PROG_CREATE(text, sub, value, regex) \
   ITEM_STRING_CALLBACK(sub, value) \
   ITEM_1ENTRY_ADD(text, sub, value, regex) \
   ITEM_1ENTRY_UPDATE(sub, value)

#define REGEX_SET(entry, regex) \
   ewe_entry_regex_set(entry, regex, EWE_REG_EXTENDED); \
   ewe_entry_regex_autocheck_set(entry, true); \
   ewe_entry_regex_glow_set(entry, true);

static void _on_v1_active(void *data, Evas_Object *obj, void *ei);
static void _on_v2_active(void *data, Evas_Object *obj, void *ei);
static void _on_state_active(void *data, Evas_Object *obj, void *ei);
static void _on_value_active(void *data, Evas_Object *obj, void *ei);
static void _on_value2_active(void *data, Evas_Object *obj, void *ei);

ITEM_1ENTRY_PROG_CREATE(_("signal"), program, signal, EDJE_NAME_REGEX)
ITEM_1ENTRY_PROG_CREATE(_("source"), program, source, EDJE_NAME_REGEX)
ITEM_1ENTRY_ADD(_("name"), program, name, EDJE_NAME_REGEX)

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
        NOTIFY_WARNING(_("The entered data is not valid!"))
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
        NOTIFY_WARNING(_("The entered data is not valid!"))
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
        NOTIFY_WARNING(_("The entered data is not valid!"))
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
        NOTIFY_WARNING(_("The entered data is not valid!"))
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

   ITEM_ADD_(parent, item, _("script"), "script");

   BOX_ADD(item, box, true, false);
   EWE_ENTRY_ADD(item, entry, false, DEFAULT_STYLE)
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
   ewe_entry_entry_set(entry, script);
   eina_list_free(childs);
}

#define ENTRY_UPDATE(ENTRY, IS_DISABLED, LAYOUT, TEXT) \
   elm_object_disabled_set(ENTRY, IS_DISABLED); \
   if (TEXT) elm_object_part_text_set(LAYOUT, "elm.text", TEXT);

#define TRANS_ENTRIES_DEFAULT_SET(IS_DISABLED) \
   elm_object_disabled_set(transition.entry1, IS_DISABLED); \
   elm_object_disabled_set(transition.entry2, IS_DISABLED); \
   elm_object_disabled_set(transition.entry3, IS_DISABLED); \
   if (IS_DISABLED) \
     { \
       ewe_entry_entry_set(transition.entry1, ""); \
       ewe_entry_entry_set(transition.entry2, ""); \
       ewe_entry_entry_set(transition.entry3, ""); \
     }

#define TRANS_VAL_GET(_val_num, _entry) \
   value = edje_edit_program_transition_value##_val_num##_get(prop.style->obj, \
              prop.program); \
   snprintf(buff, sizeof(buff), "%1.2f", value); \
   ewe_entry_entry_set(_entry, buff);

#define CALLBACK_UPDATE(_activated_cb, _entry) \
        evas_object_smart_callback_del(_entry, "activated", _activated_cb); \
        evas_object_smart_callback_add(_entry, "activated", _activated_cb, \
                                       prog_edit);

static void
_trans_entries_set(Program_Editor *prog_edit,
                   Eina_Bool is_update)
{
   char buff[BUFF_MAX];
   double value;

   switch (prop.trans_type)
     {
      case EDJE_TWEEN_MODE_NONE:
        {
           TRANS_ENTRIES_DEFAULT_SET(true);
           break;
        }
      case EDJE_TWEEN_MODE_LINEAR:
      case EDJE_TWEEN_MODE_SINUSOIDAL:
      case EDJE_TWEEN_MODE_ACCELERATE:
      case EDJE_TWEEN_MODE_DECELERATE:
        {
           ENTRY_UPDATE(transition.entry2, true, transition.layout2, NULL);
           ENTRY_UPDATE(transition.entry3, true, transition.layout3, NULL);
           break;
        }
      case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
      case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
      case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, "factor");
           ENTRY_UPDATE(transition.entry3, true, transition.layout3, NULL);
           CALLBACK_UPDATE(_on_v1_active, transition.entry2)
           if (is_update)
             {
                TRANS_VAL_GET(1, transition.entry2);
             }
           break;
        }
      case EDJE_TWEEN_MODE_DIVISOR_INTERP:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, "gradient");
           ENTRY_UPDATE(transition.entry3, false, transition.layout3, "factor");
           CALLBACK_UPDATE(_on_v1_active, transition.entry2)
           CALLBACK_UPDATE(_on_v2_active, transition.entry3)
           if (is_update)
             {
                TRANS_VAL_GET(1, transition.entry2);
                TRANS_VAL_GET(2, transition.entry3);
             }
           break;
        }
      case EDJE_TWEEN_MODE_BOUNCE:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, "decay");
           ENTRY_UPDATE(transition.entry3, false, transition.layout3, "bounces");
           CALLBACK_UPDATE(_on_v1_active, transition.entry2)
           CALLBACK_UPDATE(_on_v2_active, transition.entry3)
           if (is_update)
             {
                TRANS_VAL_GET(1, transition.entry2);
                TRANS_VAL_GET(2, transition.entry3);
             }
           break;
        }
      case EDJE_TWEEN_MODE_SPRING:
        {
           ENTRY_UPDATE(transition.entry2, false, transition.layout2, "decay");
           ENTRY_UPDATE(transition.entry3, false, transition.layout3, "swings");
           CALLBACK_UPDATE(_on_v1_active, transition.entry2)
           CALLBACK_UPDATE(_on_v2_active, transition.entry3)
           if (is_update)
             {
                TRANS_VAL_GET(1, transition.entry2);
                TRANS_VAL_GET(2, transition.entry3);
             }
           break;
        }
      case EDJE_TWEEN_MODE_CUBIC_BEZIER: // TODO: implement
      default:
        break;
     }
}

#define ACTION_STATE_GET(_state_get, _entry) \
         str = _state_get(prop.style->obj, prop.program); \
         ewe_entry_entry_set(_entry, str); \
         edje_edit_string_free(str);

#define ACTION_VAL_GET(_val_get, _entry) \
         value = _val_get(prop.style->obj, prop.program); \
         sprintf(buff, "%1.2f", value); \
         ewe_entry_entry_set(_entry, buff);

static void
_action_entries_set(Program_Editor *prog_edit, Eina_Bool is_update)
{
   Eina_Stringshare *str = NULL;
   double value;
   char buff[BUFF_MAX];

   switch (prop.act_type)
     {
      case EDJE_ACTION_TYPE_NONE:
      case EDJE_ACTION_TYPE_ACTION_STOP:
      case EDJE_ACTION_TYPE_FOCUS_SET:
      case EDJE_ACTION_TYPE_FOCUS_OBJECT:
        {
           ENTRY_UPDATE(action.entry1, true, action.layout1, NULL);
           ENTRY_UPDATE(action.entry2, true, action.layout2, NULL);
           break;
        }
      case EDJE_ACTION_TYPE_SCRIPT:
        {
           prop.script = _prop_item_program_script_add(prop.prop_box, _("script"));
           _prop_item_program_script_update(prog_edit);
           elm_box_pack_after(prop.prop_box, prop.script, action.item);
           TRANS_ENTRIES_DEFAULT_SET(true);
           break;
        }
      case EDJE_ACTION_TYPE_STATE_SET:
        {
           ENTRY_UPDATE(action.entry1, false, action.layout1, "state name");
           REGEX_SET(action.entry1, EDJE_NAME_REGEX);
           ENTRY_UPDATE(action.entry2, false, action.layout2, "state value");
           REGEX_SET(action.entry2, FLOAT_NUMBER_0_1_REGEX);
           CALLBACK_UPDATE(_on_state_active, action.entry1);
           CALLBACK_UPDATE(_on_value_active, action.entry2);
           if (is_update)
             {
                ACTION_STATE_GET(edje_edit_program_state_get, action.entry1)
                ACTION_VAL_GET(edje_edit_program_value_get, action.entry2);
             }
           break;
        }
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           ENTRY_UPDATE(action.entry1, false, action.layout1, "signal name");
           REGEX_SET(action.entry1, EDJE_NAME_REGEX);
           ENTRY_UPDATE(action.entry2, false, action.layout2, "emitter");
           REGEX_SET(action.entry2, EDJE_NAME_REGEX);
           CALLBACK_UPDATE(_on_state_active, action.entry1);
           CALLBACK_UPDATE(_on_value_active, action.entry2);
           if (is_update)
             {
                ACTION_STATE_GET(edje_edit_program_state_get, action.entry1)
                ACTION_STATE_GET(edje_edit_program_state2_get, action.entry2);
             }
           break;
        }
      case EDJE_ACTION_TYPE_DRAG_VAL_SET:
      case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
      case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
        {
           ENTRY_UPDATE(action.entry1, false, action.layout1, "x");
           REGEX_SET(action.entry1, FLOAT_NUMBER_REGEX);
           ENTRY_UPDATE(action.entry2, false, action.layout2, "y");
           REGEX_SET(action.entry2, FLOAT_NUMBER_REGEX);
           CALLBACK_UPDATE(_on_value_active, action.entry1);
           CALLBACK_UPDATE(_on_value2_active, action.entry2);
           if (is_update)
             {
                ACTION_VAL_GET(edje_edit_program_value_get, action.entry1)
                ACTION_VAL_GET(edje_edit_program_value2_get, action.entry2);
             }
           break;
        }
      default:
        break;
     }
}

static void
_on_combobox_trans_sel(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Ewe_Combobox_Item *combitem = ei;
   ewe_entry_entry_set(transition.entry1, "");
   ewe_entry_entry_set(transition.entry2, "");
   ewe_entry_entry_set(transition.entry3, "");

   edje_edit_program_transition_set(prop.style->obj, prop.program,
                                   (Edje_Tween_Mode)(combitem->index));
   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        prop.trans_type = EDJE_TWEEN_MODE_NONE;
        NOTIFY_INFO(5, _("Transition block can used only with STATE_SET action"));
        TRANS_ENTRIES_DEFAULT_SET(true);
        ewe_combobox_select_item_set(combitem->owner, 0);
        return;
     }

   TRANS_ENTRIES_DEFAULT_SET(false);
   prop.trans_type = (Edje_Tween_Mode)combitem->index;
   _trans_entries_set(prog_edit, false);
}

static void
_on_combobox_action_sel(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei)
{
   Program_Editor *prog_edit = (Program_Editor*)data;
   Ewe_Combobox_Item *combitem = ei;
   ewe_entry_entry_set(action.entry1, "");
   ewe_entry_entry_set(action.entry2, "");

   edje_edit_program_action_set(prop.style->obj, prop.program,
                                (Edje_Action_Type)combitem->index);
   prop.act_type = (Edje_Action_Type)combitem->index;

   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     {
        ewe_combobox_select_item_set(transition.combobox, 0);
        ENTRY_UPDATE(transition.entry1, false, transition.layout1, NULL);
        ENTRY_UPDATE(transition.entry2, false, transition.layout2, "param1");
        ENTRY_UPDATE(transition.entry3, false, transition.layout3, "param2");
     }

   if (prop.script)
     {
        elm_box_unpack(prop.prop_box, prop.script);
        evas_object_del(prop.script);
        prop.script = NULL;
     }
   _action_entries_set(prog_edit, false);
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
     NOTIFY_WARNING(_("The entered data is not valid!"));
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
     NOTIFY_WARNING(_("The entered data is not valid!"));
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
             NOTIFY_WARNING(_("The entered data is not valid!"));
           break;
        }
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
             NOTIFY_WARNING(_("The entered data is not valid!"));
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
             NOTIFY_WARNING(_("The entered data is not valid!"));
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
             NOTIFY_WARNING(_("The entered data is not valid!"));
           break;
        }
      case EDJE_ACTION_TYPE_SIGNAL_EMIT:
        {
           if (!edje_edit_program_state2_set(prop.style->obj, prop.program,
                                             value))
             NOTIFY_WARNING(_("the entered data is not valid!"));
           break;
        }
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
        NOTIFY_WARNING(_("The entered data is not valid!"))
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
   BUTTON_ADD(element_box, button, _("Del"));
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   EWE_ENTRY_ADD(element_box, entry, true, DEFAULT_STYLE);
   ewe_entry_entry_set(entry, name);
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
   BUTTON_ADD(element_box, button, _("Del"));
   evas_object_size_hint_weight_set(button, 0.0, 0.0);
   EWE_ENTRY_ADD(element_box, entry, true, DEFAULT_STYLE);
   ewe_entry_entry_set(entry, name);
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

static Evas_Object *
_prop_item_program_transition_add(Evas_Object *parent,
                                  Program_Editor *prog_edit,
                                  const char *tooltip)
{
   Evas_Object *item, *box;
   int i = 0;

   ITEM_ADD_(parent, item, _("transition"), "editor")
   BOX_ADD(item, box, false, true)
   EWE_COMBOBOX_ADD(item, transition.combobox)

   ITEM_ADD_(box, transition.layout1, _("length"), "editor");
   EWE_ENTRY_ADD(transition.layout1, transition.entry1, true, DEFAULT_STYLE)
   REGEX_SET(transition.entry1, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(transition.layout1,
                               "elm.swallow.content",
                               transition.entry1);

   ITEM_ADD_(box, transition.layout2, _("param1"), "editor");
   EWE_ENTRY_ADD(transition.layout2, transition.entry2, true, DEFAULT_STYLE)
   REGEX_SET(transition.entry2, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(transition.layout2,
                               "elm.swallow.content",
                               transition.entry2);

   ITEM_ADD_(box, transition.layout3, _("param2"), "editor");
   EWE_ENTRY_ADD(transition.layout3, transition.entry3, true, DEFAULT_STYLE)
   REGEX_SET(transition.entry3, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(transition.layout3,
                               "elm.swallow.content",
                               transition.entry3);

   for (i = 0; i < TRANSITIONS_COUNT; i++)
     ewe_combobox_item_add(transition.combobox, transition_type[i]);
   evas_object_smart_callback_add(transition.combobox, "selected",
                                  _on_combobox_trans_sel, prog_edit);

   elm_box_pack_end(box, transition.combobox);
   elm_box_pack_end(box, transition.layout1);
   elm_box_pack_end(box, transition.layout2);
   elm_box_pack_end(box, transition.layout3);

   elm_object_tooltip_text_set(item, tooltip);
   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_transition_update(Program_Editor *prog_edit)
{
   char buff[BUFF_MAX];
   double value;
   if (!transition.item) return;
   prop.trans_type = edje_edit_program_transition_get(prop.style->obj, prop.program);

   if (prop.act_type != EDJE_ACTION_TYPE_STATE_SET)
     prop.trans_type = EDJE_TWEEN_MODE_NONE;
   ewe_combobox_select_item_set(transition.combobox, (int)prop.trans_type);
   if (prop.trans_type == EDJE_TWEEN_MODE_NONE)
     {
        TRANS_ENTRIES_DEFAULT_SET(true);
        return;
     }

   value = edje_edit_program_transition_time_get(prop.style->obj, prop.program);
   evas_object_smart_callback_del(transition.entry1, "activated",
                                  _on_transition_time_active);
   evas_object_smart_callback_add(transition.entry1, "activated",
                                  _on_transition_time_active, prog_edit);

   snprintf(buff, sizeof(buff), "%1.2f", value);
   ewe_entry_entry_set(transition.entry1, buff);
   TRANS_ENTRIES_DEFAULT_SET(false);

   _trans_entries_set(prog_edit, true);
}

static Evas_Object *
_prop_item_program_action_add(Evas_Object *parent,
                              Program_Editor *prog_edit,
                              const char *tooltip)
{
   Evas_Object *item, *box;
   int i = 0;

   ITEM_ADD_(parent, item, _("action"), "editor")
   BOX_ADD(item, box, false, true)
   EWE_COMBOBOX_ADD(box, action.combobox)
   ITEM_ADD_(box, action.layout1, _("param1"), "editor");
   EWE_ENTRY_ADD(action.layout1, action.entry1, true, DEFAULT_STYLE)
   elm_object_part_content_set(action.layout1,
                               "elm.swallow.content",
                               action.entry1);

   ITEM_ADD_(box, action.layout2, _("param2"), "editor");
   EWE_ENTRY_ADD(action.layout2, action.entry2, true, DEFAULT_STYLE)
   elm_object_part_content_set(action.layout2,
                               "elm.swallow.content",
                               action.entry2);

   for (i = 0; i < ACTIONS_COUNT; i++)
     {
        /*9th action state is reserved, can't be an item*/
        if (i == 9) continue;
        ewe_combobox_item_add(action.combobox, _(action_type[i]));
     }
   evas_object_smart_callback_add(action.combobox, "selected",
                                  _on_combobox_action_sel, prog_edit);

   elm_object_tooltip_text_set(item, tooltip);
   elm_box_pack_end(box, action.combobox);
   elm_box_pack_end(box, action.layout1);
   elm_box_pack_end(box, action.layout2);

   elm_object_part_content_set(item, "elm.swallow.content", box);

   return item;
}

static void
_prop_item_program_action_update(Program_Editor *prog_edit)
{
   prop.act_type = edje_edit_program_action_get(prop.style->obj, prop.program);
   ewe_entry_entry_set(action.entry1, "");
   ewe_entry_entry_set(action.entry2, "");
   elm_object_disabled_set(action.entry1, true);
   elm_object_disabled_set(action.entry2, true);

   ewe_combobox_select_item_set(action.combobox, (int)prop.act_type);

   if (prop.script)
     {
        elm_box_unpack(prop.prop_box, prop.script);
        evas_object_del(prop.script);
        prop.script = NULL;
     }

   _action_entries_set(prog_edit, true);
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

   ITEM_ADD_(parent, item, _("after"), "editor");
   BOX_ADD(item, box, true, false);
   BOX_ADD(box, entrys_box, false, false);

   BUTTON_ADD(box, button, _("Add"));
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

   ITEM_ADD_(parent, item, _("targets"), "editor");
   BOX_ADD(item, box, true, false);
   BOX_ADD(box, entries_box, false, false);

   BUTTON_ADD(box, button, _("Add"));
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
   ewe_entry_entry_set(entry, instr);
   evas_object_smart_callback_del(entry, "activated", _on_in_from_change);
   evas_object_smart_callback_add(entry, "activated",
                                  _on_in_from_change, prog_edit);
   range = edje_edit_program_in_range_get(prop.style->obj,
                                          prop.program);
   entry = eina_list_nth(childs, 1);
   snprintf(instr, sizeof(instr), "%2.3f", range);
   ewe_entry_entry_set(entry, instr);
   evas_object_smart_callback_del(entry, "activated", _on_in_range_change);
   evas_object_smart_callback_add(entry, "activated",
                                  _on_in_range_change, prog_edit);
   eina_list_free(childs);
}

static Evas_Object *
_prop_item_program_in_add(Evas_Object *parent,
                     const char *tooltip __UNUSED__)
{
   Evas_Object *item, *sub_item, *box;
   Evas_Object *entry1, *entry2;

   if (!parent) return NULL;

   ITEM_ADD_(parent, item, _("in"), "editor");

   BOX_ADD(item, box, false, false);
   ITEM_ADD_(box, sub_item, _("range"), "editor");
   EWE_ENTRY_ADD(sub_item, entry1, true, DEFAULT_STYLE)
   REGEX_SET(entry1, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(sub_item, "elm.swallow.content", entry1);
   elm_box_pack_end(box, sub_item);

   ITEM_ADD_(box, sub_item, _("from"), "editor");
   EWE_ENTRY_ADD(sub_item, entry2, true, DEFAULT_STYLE)
   REGEX_SET(entry2, FLOAT_NUMBER_REGEX);
   elm_object_part_content_set(sub_item, "elm.swallow.content", entry2);
   elm_box_pack_end(box, sub_item);

   elm_object_part_content_set(item, "elm.swallow.content", box);
   return item;
}

static void
_prop_item_program_name_update(Program_Editor *prog_edit)
{
   Evas_Object *entry = elm_object_part_content_get(prop.name,
                                                    "elm.swallow.content");

   ewe_entry_entry_set(entry, prop.program);
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

   prop.name = _prop_item_program_name_add(box, _("Unique name of program "));
   prop.signal = _prop_item_program_signal_add(box, _("signal"));
   prop.source = _prop_item_program_source_add(box, _("source"));
   prop.in = _prop_item_program_in_add(box, _("in"));
   action.item = _prop_item_program_action_add(box, prog_edit, _("action"));
   transition.item = _prop_item_program_transition_add(box, prog_edit, _("transition"));
   prop.targets = _prop_item_program_target_add(box, prog_edit, _("targets"));
   prop.afters = _prop_item_program_after_add(box, prog_edit, _("afters"));

   elm_box_pack_end(box, prop.name);
   elm_box_pack_end(box, prop.signal);
   elm_box_pack_end(box, prop.source);
   elm_box_pack_end(box, prop.in);
   elm_box_pack_end(box, action.item);
   elm_box_pack_end(box, transition.item);
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
   edje_edit_without_source_save(style->obj, true);
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
        NOTIFY_WARNING(_("Program's 'name can not be empty!"));
        return;
     }
   if (!edje_edit_program_add(prop.style->obj, name))
     {
        NOTIFY_WARNING(_("Program's name must be unique!"));
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
        NOTIFY_WARNING(_("Failed to delete program [%s] from style [%s]"), program_name,
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
                            _("Add new program:"));

   BOX_ADD(prog_edit->popup.popup, box, false, false);
   BOX_ADD(box, prog_box, true, false);

   LABEL_ADD(prog_box, prog_label, _("Program name: "))
   elm_box_pack_end(prog_box, prog_label);

   EWE_ENTRY_ADD(prog_box, prog_edit->popup.entry, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(prog_edit->popup.entry,
                                  elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(prog_edit->popup.entry, "guide",
                            _("Type new program name here."));
   elm_box_pack_end(prog_box, prog_edit->popup.entry);

   elm_box_pack_end(box, prog_box);
   elm_object_content_set(prog_edit->popup.popup, box);

   BUTTON_ADD(prog_edit->popup.popup, button, _("Ok"));
   evas_object_smart_callback_add(button, "clicked", _on_add_popup_bt_add,
                                  prog_edit);
   elm_object_part_content_set(prog_edit->popup.popup, "button1", button);

   BUTTON_ADD(prog_edit->popup.popup, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_add_popup_bt_cancel,
                                  prog_edit);
   elm_object_part_content_set(prog_edit->popup.popup, "button2", button);

   evas_object_show(prog_edit->popup.popup);
}

static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_locked_set(ap->menu_hash, false);
}

Evas_Object *
program_editor_window_add(Style *style)
{
   Evas_Object *mw_box, *pans;
   Evas_Object *scroller;
   Evas_Object *bt, *box;
   Program_Editor *prog_edit = NULL;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_create();

   if ((!style) || (!style->obj))
     {
        ERR("Failed create program editor for current style");
        return NULL;
     }

   prog_edit = (Program_Editor *)mem_calloc(1, sizeof(Program_Editor));

   prop.style = style;
   prog_edit->mwin = mw_add(NULL, NULL);
   mw_title_set(prog_edit->mwin, _("Program editor"));
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

   BUTTON_ADD(box, bt, _("New program"));
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_add, prog_edit);
   elm_box_pack_end(box, bt);

   BUTTON_ADD(box, bt, _("Delete"));
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

   BUTTON_ADD(box, bt, _("Apply"));
   evas_object_smart_callback_add(bt, "clicked", _on_editor_save, prop.style);
   elm_box_pack_end(box, bt);

   BUTTON_ADD(box, bt, _("Close"));
   evas_object_smart_callback_add(bt, "clicked", _on_editor_cancel,
                                  prog_edit->mwin);
   elm_box_pack_end(box, bt);

   elm_box_pack_end(mw_box, pans);
   elm_box_pack_end(mw_box, box);

   elm_win_inwin_content_set(prog_edit->mwin, mw_box);

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_event_callback_add(prog_edit->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(prog_edit->mwin);
   return prog_edit->mwin;
}

#undef action
#undef transition
#undef prop
#undef ACTION_STATE_GET
#undef ACTION_VAL_GET
#undef CALLBACK_UPDATE
#undef TRANS_ENTRIES_DEFAULT_SET
#undef TRANS_VAL_GET
#undef REGEX_SET
