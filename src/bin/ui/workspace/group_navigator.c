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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "group_navigator.h"
#include "main_window.h"
#include "history.h"
#include "project_manager.h"
#include "change.h"
#include "widget_macro.h"

#define GROUP_NAVIGATOR_DATA "group_navigator_data"

typedef struct
{
   Group *group;

   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *btn_add;
   Evas_Object *btn_del;
   Evas_Object *btn_up;
   Evas_Object *btn_down;

   Elm_Genlist_Item_Class *itc_caption;
   Elm_Genlist_Item_Class *itc_part;
   Elm_Genlist_Item_Class *itc_state;
   Elm_Genlist_Item_Class *itc_state_selected;
   Elm_Genlist_Item_Class *itc_item_caption;
   Elm_Genlist_Item_Class *itc_item;
   Elm_Genlist_Item_Class *itc_program;

   Elm_Object_Item *selected_part_item;
   Elm_Object_Item *parts_caption_item;
   Elm_Object_Item *programs_caption_item;

   Evas_Object *menu;
   Elm_Object_Item *add_state_menu_item;
   Elm_Object_Item *add_part_item_menu_item;
   Elm_Validator_Regexp *name_validator;
   struct {
        Evas_Object *entry_name;
        Evas_Object *spinner_value;
        Evas_Object *combobox;
        Evas_Object *btn_add;
        Evas_Object *btn_cancel;
   } popup;
} Part_List;

static char *part_types[] = {
     N_("NONE"),
     N_("RECTANGLE"),
     N_("TEXT"),
     N_("IMAGE"),
     N_("SWALLOW"),
     N_("TEXTBLOCK"),
     N_("GRADIENT"),
     N_("GROUP"),
     N_("BOX"),
     N_("TABLE"),
     N_("EXTERNAL"),
     N_("PROXY"),
     N_("SPACER")
};
static const char *program_actions[] = {
     N_("NONE"),
     N_("STATE SET"),
     N_("SIGNAL EMIT"),
     N_("DRAG VALUE SET"),
     N_("DRAG VALUE STEP"),
     N_("DRAG VALUE PAGE"),
     N_("PLAY SAMPLE"),
     N_("PLAY TONE"),
     N_("ACTION STOP"),
     NULL
};

static const unsigned int part_types_count = 12;

static char *
_resource_label_get(void *data,
                    Evas_Object *obj __UNUSED__,
                    const char *pr __UNUSED__)
{
   Resource *res= data;

   assert(res != NULL);
   assert(res->name != NULL);

   return strdup(res->name);
}

static char *
_item_caption_label_get(void *data,
                        Evas_Object *obj __UNUSED__,
                        const char *pr)
{
   Part *part = data;
   char buf[BUFF_MAX];

   assert(part != NULL);

   if (!strcmp(pr, "elm.text.end"))
     {
        snprintf(buf, BUFF_MAX, "%d", eina_list_count(part->items));
        return strdup(buf);
     }
   if (!strcmp(pr, "elm.text"))
     return strdup(_("items"));

   return NULL;
}

static char *
_caption_label_get(void *data,
                   Evas_Object *obj,
                   const char *pr)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_List **list = data;
   char buf[BUFF_MAX];

   if (!strcmp(pr, "elm.text"))
     {
        if (list == &pl->group->parts)
          return strdup(_("Parts"));
        else if (list == &pl->group->programs)
          return strdup(_("Programs"));
     }
   if (!strcmp(pr, "elm.text.end"))
     {
        snprintf(buf, BUFF_MAX, "%d", eina_list_count(*list));
        return strdup(buf);
     }
   return NULL;
}

static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *pr __UNUSED__)
{
   if (!strcmp(pr, "elm.text"))
     return strdup(data);

   return NULL;
}

static Eina_Bool
_all_parts_visible(Part_List *pl)
{
   const Eina_List *l;
   const Part *part;
   Eina_Bool visible = true;

   assert(pl != NULL);

   EINA_LIST_FOREACH(pl->group->parts, l, part)
      visible &= part->visible;

   return visible;
}

static void
_on_parts_eye_clicked(void *data __UNUSED__,
                      Evas_Object *obj,
                      void *event_data __UNUSED__)
{
   Evas_Object *eye;
   Part *part;
   const Eina_List *subitems, *l;
   Elm_Object_Item *glit;
   Eina_Bool visible;

   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);

   visible = !_all_parts_visible(pl);

   EINA_LIST_FOREACH(pl->group->parts, l, part)
     {
        part->visible = visible;
        evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_VISIBLE_CHANGED, (void *)part);
     }

   subitems = elm_genlist_item_subitems_get(pl->parts_caption_item);
   EINA_LIST_FOREACH(subitems, l, glit)
     {
        eye = elm_object_item_part_content_get(glit, "elm.swallow.icon");
        if (eye) elm_check_state_set(eye, visible);
      }
}

static void
_on_eye_clicked(void *data,
                Evas_Object *obj,
                void *event_data __UNUSED__)
{
   Evas_Object *eye;
   Part *part = data;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(part != NULL);
   assert(pl != NULL);

   part->visible = !part->visible;
   evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_VISIBLE_CHANGED, (void *)part);

   eye = elm_object_item_part_content_get(pl->parts_caption_item, "elm.swallow.icon");
   if (eye)
     elm_check_state_set(eye, _all_parts_visible(pl));
}

static Evas_Object *
_caption_content_get(void *data,
                     Evas_Object *obj,
                     const char *part)
{
   Eina_List **list = data;
   Evas_Object *content = NULL;
   Eina_Bool visible = true;
   Part *_part;
   Eina_List *l;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   if (list == &pl->group->parts)
     if (!strcmp(part, "elm.swallow.icon"))
       {
          content = elm_check_add(obj);
          edje_object_animation_set(elm_layout_edje_get(content), false);
          EINA_LIST_FOREACH(pl->group->parts, l, _part)
             visible &= _part->visible;
          elm_check_state_set(content, visible);
          elm_object_style_set(content, "eye");
          evas_object_data_set(content, GROUP_NAVIGATOR_DATA, pl);
          edje_object_animation_set(elm_layout_edje_get(content), true);

          evas_object_smart_callback_add(content, "changed", _on_parts_eye_clicked, NULL);
       }

   return content;
}

static Evas_Object *
_part_content_get(void *data,
                  Evas_Object *obj,
                  const char *part)
{
   Evas_Object *content = NULL;
   Part *_part = data;

   assert(_part != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
        content = elm_check_add(obj);
        edje_object_animation_set(elm_layout_edje_get(content), false);
        elm_check_state_set(content, _part->visible);
        elm_object_style_set(content, "eye");
        evas_object_data_set(content, GROUP_NAVIGATOR_DATA, pl);
        edje_object_animation_set(elm_layout_edje_get(content), true);

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

static Eina_Bool
_part_state_get(void *data,
                Evas_Object *obj,
                const char *state)
{
   Part *selected_part, *part = data;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(part != NULL);
   assert(pl != NULL);

   selected_part = elm_object_item_data_get(pl->selected_part_item);
   if ((selected_part == part) && ((!strcmp(state, "selected"))))
     return true;
   if ((selected_part != part) && (!strcmp(state, "unselected")))
     return true;
   return false;
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
_on_activated(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info)
{
   Part_List *pl = data;
   const Elm_Genlist_Item_Class* itc;
   State *state;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   assert(pl != NULL);

   itc = elm_genlist_item_item_class_get(glit);
   if (itc == pl->itc_state)
     {
        assert(pl->selected_part_item != NULL);

        state = elm_object_item_data_get(glit);
        CRIT_ON_FAIL(editor_part_selected_state_set(pl->group->edit_object, NULL, false,
                                                    state->part->name,
                                                    state->parsed_name,
                                                    state->parsed_val));
     }
}

void
group_navigator_part_state_select(Evas_Object *obj, State *state)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   const Eina_List *subitems, *l;
   Elm_Object_Item *selected;
   Part *part;

   assert(pl != NULL);
   assert(state != NULL);

   part = elm_object_item_data_get(pl->selected_part_item);

   TODO("replace this with assert after genlist double-click bug will be fixed");
   if (state->part != part)
     group_navigator_part_select(pl->layout, state->part);

   if (elm_genlist_item_expanded_get(pl->selected_part_item))
     {
        /* unselecting previous selected state */
        subitems = elm_genlist_item_subitems_get(pl->selected_part_item);
        EINA_LIST_FOREACH(subitems, l, selected)
           if (elm_genlist_item_item_class_get(selected) == pl->itc_state_selected)
             break;
        assert(selected != NULL);
        elm_genlist_item_item_class_update(selected, pl->itc_state);

        /* selecting new one */
        EINA_LIST_FOREACH(subitems, l, selected)
           if (elm_object_item_data_get(selected) == state)
             break;
        assert(selected != NULL);
        elm_genlist_item_item_class_update(selected, pl->itc_state_selected);
     }

   state->part->current_state = state;

   evas_object_smart_callback_call(ap.win, SIGNAL_PART_STATE_SELECTED, (void *)state->part);
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
   Part *part;
   Resource *res;
   State *state;
   Eina_Stringshare *item_name;

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
        if ((part->type == EDJE_PART_TYPE_BOX) ||
            (part->type == EDJE_PART_TYPE_TABLE))
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_item_caption,
                                     part,
                                     glit,
                                     ELM_GENLIST_ITEM_TREE,
                                     NULL,
                                     NULL);
          }
     }
   else if (itc == pl->itc_item_caption)
     {
        part = elm_object_item_data_get(glit);
        EINA_LIST_FOREACH(part->items, l, item_name)
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_item,
                                     item_name,
                                     glit,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);
          }
     }
   else if (glit == pl->parts_caption_item)
     {
        EINA_LIST_FOREACH(pl->group->parts, l, part)
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_part,
                                     part,
                                     pl->parts_caption_item,
                                     ELM_GENLIST_ITEM_TREE,
                                     NULL,
                                     NULL);
          }
     }
   else if (glit == pl->programs_caption_item)
     {
        EINA_LIST_FOREACH(pl->group->programs, l, res)
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_program,
                                     res,
                                     pl->programs_caption_item,
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

static void
_unselect_part(Part_List *pl)
{
   Elm_Object_Item *glit_part;

   assert(pl != NULL);
   assert(pl->selected_part_item != NULL);

   pl->group->current_part->current_item_name = NULL;
   pl->group->current_part = NULL;
   glit_part = pl->selected_part_item;
   pl->selected_part_item = NULL;
   elm_genlist_item_fields_update(glit_part, "unselected", ELM_GENLIST_ITEM_FIELD_STATE);
   elm_object_item_disabled_set(pl->add_state_menu_item, true);
   elm_object_item_disabled_set(pl->add_part_item_menu_item, true);
   elm_object_disabled_set(pl->btn_del, true);
   elm_object_disabled_set(pl->btn_down, true);
   elm_object_disabled_set(pl->btn_up, true);
}

static void
_selected_cb(void *data,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Elm_Object_Item *glit_part, *next_item;
   const Elm_Genlist_Item_Class* itc;
   Eina_Stringshare *item_name;
   Part_List *pl = data;
   Part *part;
   Resource *res;
   const Eina_List *items_list;

   assert(pl != NULL);

   elm_object_disabled_set(pl->btn_del, true);
   if ((glit == pl->parts_caption_item) ||
       (glit == pl->programs_caption_item))
     {
        if (pl->selected_part_item)
          _unselect_part(pl);
        if (pl->group->current_program)
          {
             eina_stringshare_del(pl->group->current_program);
             pl->group->current_program = NULL;
          }
        evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_NAVIGATOR_UNSELECTED, NULL);
        return;
     }
   itc = elm_genlist_item_item_class_get(glit);

   TODO("REFACTOR")
   if (pl->group->current_program)
     {
        eina_stringshare_del(pl->group->current_program);
        pl->group->current_program = NULL;
     }
   if (itc == pl->itc_program)
     {
        if (pl->selected_part_item)
          _unselect_part(pl);

        elm_object_disabled_set(pl->btn_del, false);
        res = elm_object_item_data_get(glit);
        pl->group->current_program = eina_stringshare_add(res->name);
        evas_object_smart_callback_call(ap.win, SIGNAL_PROGRAM_SELECTED, (void *)res->name);
     }
   else
     {
        if (itc == pl->itc_item)
          item_name = elm_object_item_data_get(glit);
        else
          item_name = NULL;

        glit_part = glit;
        TODO("check for programs");
        while (elm_genlist_item_parent_get(glit_part) != pl->parts_caption_item)
          glit_part = elm_genlist_item_parent_get(glit_part);

        part = elm_object_item_data_get(glit_part);
        if ((pl->selected_part_item != glit_part) || (part->current_item_name != item_name))
          {
             if ((pl->group->current_part != part) && (pl->selected_part_item))
               _unselect_part(pl);
             pl->selected_part_item = glit_part;
             part->current_item_name = item_name;
             pl->group->current_part = part;
             elm_genlist_item_fields_update(glit_part, "selected", ELM_GENLIST_ITEM_FIELD_STATE);
             evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, part);
          }
        else
          part->current_item_name = NULL;
        elm_object_item_disabled_set(pl->add_state_menu_item, false);
        if ((part->type == EDJE_PART_TYPE_BOX) ||
            (part->type == EDJE_PART_TYPE_TABLE))
          elm_object_item_disabled_set(pl->add_part_item_menu_item, false);
        /* enabling or disabling del button */
        if ((itc == pl->itc_item_caption) ||
            (((itc == pl->itc_state) || (itc == pl->itc_state_selected)) &&
             (!strcmp(((State *)elm_object_item_data_get(glit))->name, "default 0.00"))))
          elm_object_disabled_set(pl->btn_del, true);
        else
          elm_object_disabled_set(pl->btn_del, false);
        /* enabling or disabling up and down buttons */
        if (itc == pl->itc_part)
          {
             items_list = elm_genlist_item_subitems_get(pl->parts_caption_item);
             if (glit == eina_list_data_get(items_list))
               elm_object_disabled_set(pl->btn_up, true);
             else
               elm_object_disabled_set(pl->btn_up, false);

             if (glit == eina_list_data_get(eina_list_last(items_list)))
               elm_object_disabled_set(pl->btn_down, true);
             else
               elm_object_disabled_set(pl->btn_down, false);
          }
        else if (itc == pl->itc_item)
          {
             if (elm_genlist_item_item_class_get(elm_genlist_item_prev_get(glit)) == pl->itc_item)
               elm_object_disabled_set(pl->btn_up, false);
             else
               elm_object_disabled_set(pl->btn_up, true);

             next_item = elm_genlist_item_next_get(glit);
             if (next_item && (elm_genlist_item_item_class_get(next_item) == pl->itc_item))
               elm_object_disabled_set(pl->btn_down, false);
             else
               elm_object_disabled_set(pl->btn_down, true);
          }
        else
          {
             elm_object_disabled_set(pl->btn_down, true);
             elm_object_disabled_set(pl->btn_up, true);
          }
     }
}

static void
_on_entry_changed(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Part_List *pl = data;
   const char *name;

   assert(pl != NULL);

   name = elm_entry_entry_get(pl->popup.entry_name);
   if ((elm_validator_regexp_status_get(pl->name_validator) != ELM_REG_NOERROR) ||
       (edje_edit_part_exist(pl->group->edit_object, name)))
     elm_object_disabled_set(pl->popup.btn_add, true);
   else
     elm_object_disabled_set(pl->popup.btn_add, false);
}

static void
_state_validate(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Part_List *pl = data;
   const char *name;
   Part *part;
   double val;

   assert(pl != NULL);

   part = elm_object_item_data_get(pl->selected_part_item);

   name = elm_entry_entry_get(pl->popup.entry_name);
   val = elm_spinner_value_get(pl->popup.spinner_value);
   val = ((int) (val * 100)) / 100.0; /* only first two digets after point are used */
   if ((elm_validator_regexp_status_get(pl->name_validator) != ELM_REG_NOERROR) ||
       (edje_edit_state_exist(pl->group->edit_object, part->name, name, val)))
     elm_object_disabled_set(pl->popup.btn_add, true);
   else
     elm_object_disabled_set(pl->popup.btn_add, false);
}

static void
_item_validate(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Part_List *pl = data;
   const char *name, *item;
   Part *part;
   Eina_Bool valid;
   Eina_List *l;

   assert(pl != NULL);

   part = elm_object_item_data_get(pl->selected_part_item);

   name = elm_entry_entry_get(pl->popup.entry_name);
   valid = (elm_validator_regexp_status_get(pl->name_validator) == ELM_REG_NOERROR);
   valid = valid && (ewe_combobox_select_item_get(pl->popup.combobox) != NULL);
   EINA_LIST_FOREACH(part->items, l, item)
      valid = valid && strcmp(item, name);

   elm_object_disabled_set(pl->popup.btn_add, !valid);
}

static void
_program_validate(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Part_List *pl = data;
   const char *program;

   assert(pl != NULL);

   program = elm_entry_entry_get(pl->popup.entry_name);

   if ((elm_validator_regexp_status_get(pl->name_validator) != ELM_REG_NOERROR) ||
       (edje_edit_program_exist(pl->group->edit_object, program)))
     elm_object_disabled_set(pl->popup.btn_add, true);
   else
     elm_object_disabled_set(pl->popup.btn_add, false);
}

static void
_job_popup_del(void *data)
{
   Part_List *pl = data;

   assert(pl != NULL);

   pl->popup.entry_name = NULL;
   evas_object_del(ap.popup);
   ap.popup = NULL;
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_popup_cancel_clicked(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   ecore_job_add(_job_popup_del, data);
}

static void
_popup_add_part_ok_clicked(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   Ewe_Combobox_Item *item;
   Edje_Part_Type type = EDJE_PART_TYPE_NONE;
   Part_List *pl = data;
   const char *name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   if (elm_object_disabled_get(pl->popup.btn_add)) return;
   item = ewe_combobox_select_item_get(pl->popup.combobox);
   switch (item->index)
     {
      case 0:
         type = EDJE_PART_TYPE_RECTANGLE;
         break;
      case 1:
         type = EDJE_PART_TYPE_TEXT;
         break;
      case 2:
         type = EDJE_PART_TYPE_IMAGE;
         break;
      case 3:
         type = EDJE_PART_TYPE_SWALLOW;
         break;
      case 4:
         type = EDJE_PART_TYPE_TEXTBLOCK;
         break;
      case 5:
         type = EDJE_PART_TYPE_GROUP;
         break;
      case 6:
         type = EDJE_PART_TYPE_BOX;
         break;
      case 7:
         type = EDJE_PART_TYPE_TABLE;
         break;
      case 8:
         type = EDJE_PART_TYPE_PROXY;
         break;
      case 9:
         type = EDJE_PART_TYPE_SPACER;
         break;
     }
   assert(type != EDJE_PART_TYPE_NONE);

   name = elm_entry_entry_get(pl->popup.entry_name);
   msg = eina_stringshare_printf(_("added new part \"%s\""), name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_add(pl->group->edit_object, change, false, name, type));

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
   ecore_job_add(_job_popup_del, pl);
}

void
group_navigator_part_add(Evas_Object *obj, Part *part)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit;

   assert(pl != NULL);
   assert(part != NULL);

   if (elm_genlist_item_expanded_get(pl->parts_caption_item))
     {
        glit = elm_genlist_item_append(pl->genlist,
                                       pl->itc_part,
                                       part,
                                       pl->parts_caption_item,
                                       ELM_GENLIST_ITEM_TREE,
                                       NULL,
                                       NULL);
     }
   else
     {
        elm_genlist_item_expanded_set(pl->parts_caption_item, EINA_TRUE);
        glit = eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(pl->parts_caption_item)));

     }
   elm_genlist_item_selected_set(glit, true);
   elm_genlist_item_update(pl->parts_caption_item);
}

static void
_combobox_collapsed(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Part_List *pl = data;

   assert(pl != NULL);

   elm_object_focus_set(pl->popup.entry_name, true);
}

static void
_on_menu_add_part_clicked(void *data __UNUSED__,
                          Evas_Object *obj,
                          void *ei __UNUSED__)
{
   Edje_Part_Type type;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   Eina_Stringshare *title;
   Evas_Object *box, *item;

   assert(pl != NULL);

   ap.popup = elm_popup_add(ap.win);
   elm_popup_orient_set(ap.popup, ELM_POPUP_ORIENT_CENTER);
   title = eina_stringshare_printf(_("Add new part to group \"%s\""), pl->group->name);
   elm_object_part_text_set(ap.popup, "title,text", title);
   eina_stringshare_del(title);

   BOX_ADD(ap.popup, box, false, false);

   LAYOUT_PROP_ADD(box, _("Part name:"), "popup", "1swallow")
   ENTRY_ADD(box, pl->popup.entry_name, true);
   eo_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, pl->name_validator);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter name for new part here."));
   evas_object_smart_callback_add(pl->popup.entry_name, "changed", _on_entry_changed, pl);
   evas_object_smart_callback_add(pl->popup.entry_name, "activated", _popup_add_part_ok_clicked, pl);
   evas_object_show(pl->popup.entry_name);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Part type:"), "popup", "1swallow")
   EWE_COMBOBOX_ADD(item, pl->popup.combobox)
   ewe_combobox_select_item_set(pl->popup.combobox, 0);
   for (type = EDJE_PART_TYPE_RECTANGLE; type <= EDJE_PART_TYPE_SPACER; type++)
     {
        if ((type == EDJE_PART_TYPE_GRADIENT) ||
            (type == EDJE_PART_TYPE_EXTERNAL))
          continue;
        ewe_combobox_item_add(pl->popup.combobox, part_types[type]);
     }
   ewe_combobox_select_item_set(pl->popup.combobox, 0);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);
   evas_object_smart_callback_add(pl->popup.combobox, "collapsed", _combobox_collapsed, pl);
   elm_box_pack_end(box, item);

   elm_object_content_set(ap.popup, box);
   BUTTON_ADD(box, pl->popup.btn_add, _("Add"));
   evas_object_smart_callback_add(pl->popup.btn_add, "clicked", _popup_add_part_ok_clicked, pl);
   elm_object_part_content_set(ap.popup, "button1", pl->popup.btn_add);
   elm_object_disabled_set(pl->popup.btn_add, true);

   BUTTON_ADD(box, pl->popup.btn_cancel, _("Cancel"));
   evas_object_smart_callback_add(pl->popup.btn_cancel, "clicked", _popup_cancel_clicked, pl);
   elm_object_part_content_set(ap.popup, "button2", pl->popup.btn_cancel);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);

   evas_object_show(ap.popup);
   elm_object_focus_set(pl->popup.entry_name, true);
}

static void
_popup_add_state_ok_clicked(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   Ewe_Combobox_Item *item;
   Part_List *pl = data;
   const char *name;
   double val;
   Part *part;
   State *state_from;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   if (elm_object_disabled_get(pl->popup.btn_add)) return;

   part = elm_object_item_data_get(pl->selected_part_item);
   name = elm_entry_entry_get(pl->popup.entry_name);
   val = elm_spinner_value_get(pl->popup.spinner_value);
   val = ((int) (val * 100)) / 100.0; /* only first two digets after point are used */

   item = ewe_combobox_select_item_get(pl->popup.combobox);
   if (item->index == 0)
     {
        msg = eina_stringshare_printf(_("added new state \"%s\" %.2f"), name, val);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_state_add(pl->group->edit_object, change, false,
                                      part->name, name, val));
     }
   else
     {
        state_from = pm_resource_get(part->states, item->title);
        msg = eina_stringshare_printf(_("added new state \"%s\" %.2f as copy of \"%s\" %.2f"),
                                      name, val, state_from->parsed_name, state_from->parsed_val);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_state_copy(pl->group->edit_object, change, false,
                                       part->name,
                                       state_from->parsed_name, state_from->parsed_val,
                                       name, val));
     }

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
   ecore_job_add(_job_popup_del, pl);
}

void
group_navigator_part_state_add(Evas_Object *obj, Part *part, State *state)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_Bool items_expanded = false;
   Elm_Object_Item *glit;
   const Eina_List *l;

   assert(pl != NULL);
   assert(part != NULL);
   assert(state != NULL);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     {
        items_expanded = elm_genlist_item_expanded_get(eina_list_data_get(
              eina_list_last(elm_genlist_item_subitems_get(pl->selected_part_item))));
     }
   elm_genlist_item_expanded_set(pl->selected_part_item, false);
   elm_genlist_item_expanded_set(pl->selected_part_item, true);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     elm_genlist_item_expanded_set(eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(
              pl->selected_part_item))), items_expanded);

   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(pl->selected_part_item), l, glit)
     {
        if (elm_object_item_data_get(glit) == state)
          {
             elm_genlist_item_selected_set(glit, true);
             _on_activated(pl, pl->genlist, glit);
             break;
          }
     }
}

static void
_on_menu_add_state_clicked(void *data __UNUSED__,
                           Evas_Object *obj,
                           void *ei __UNUSED__)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Part *part;
   State *state;
   Eina_Stringshare *title;
   Evas_Object *box, *item;
   Eina_List *l;

   assert(pl != NULL);

   part = elm_object_item_data_get(pl->selected_part_item);

   ap.popup = elm_popup_add(ap.win);
   elm_popup_orient_set(ap.popup, ELM_POPUP_ORIENT_CENTER);
   title = eina_stringshare_printf(_("Add new state to part \"%s\""), part->name);
   elm_object_part_text_set(ap.popup, "title,text", title);
   eina_stringshare_del(title);

   BOX_ADD(ap.popup, box, false, false);
   LAYOUT_PROP_ADD(box, _("Name:"), "popup", "1swallow")
   ENTRY_ADD(item, pl->popup.entry_name, true);
   eo_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE,
                         elm_validator_regexp_helper, pl->name_validator);
   evas_object_smart_callback_add(pl->popup.entry_name, "changed", _state_validate, pl);
   evas_object_smart_callback_add(pl->popup.entry_name, "activated", _popup_add_state_ok_clicked, pl);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter name for new state here."));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Value:"), "popup", "1swallow_subtext")
   SPINNER_ADD(item, pl->popup.spinner_value, 0.0, 1.0, 0.1, true);
   elm_spinner_label_format_set(pl->popup.spinner_value, "%1.2f");
   elm_object_part_text_set(item, "elm.subtext", _("Available values: 0.0 - 1.0"));
   evas_object_smart_callback_add(pl->popup.spinner_value, "changed", _state_validate, pl);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.spinner_value);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Duplicate state:"), "popup", "1swallow")
   EWE_COMBOBOX_ADD(item, pl->popup.combobox)

   ewe_combobox_item_add(pl->popup.combobox, _("None"));
   ewe_combobox_select_item_set(pl->popup.combobox, 0);
   EINA_LIST_FOREACH(part->states, l, state)
     {
       ewe_combobox_item_add(pl->popup.combobox, state->name);
     }
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);

   elm_box_pack_end(box, item);
   elm_object_content_set(ap.popup, box);

   BUTTON_ADD(ap.popup, pl->popup.btn_add, _("Add"));
   evas_object_smart_callback_add(pl->popup.btn_add, "clicked", _popup_add_state_ok_clicked, pl);
   elm_object_part_content_set(ap.popup, "button1", pl->popup.btn_add);
   elm_object_disabled_set(pl->popup.btn_add, true);

   BUTTON_ADD(ap.popup, pl->popup.btn_cancel, _("Cancel"));
   evas_object_smart_callback_add(pl->popup.btn_cancel, "clicked", _popup_cancel_clicked, pl);
   elm_object_part_content_set(ap.popup, "button2", pl->popup.btn_cancel);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(ap.popup);
   elm_object_focus_set(pl->popup.entry_name, true);

}

static void
_popup_add_item_ok_clicked(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   Ewe_Combobox_Item *item;
   Part_List *pl = data;
   Part *part;
   const char *name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   if (elm_object_disabled_get(pl->popup.btn_add)) return;

   part = elm_object_item_data_get(pl->selected_part_item);
   name = elm_entry_entry_get(pl->popup.entry_name);
   item = ewe_combobox_select_item_get(pl->popup.combobox);

   msg = eina_stringshare_printf(_("added new item \"%s\" to part \"%s\""), name, part->name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_item_append(pl->group->edit_object, change, false, part->name, name, item->title));

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
   ecore_job_add(_job_popup_del, pl);
}

void
group_navigator_part_item_add(Evas_Object *obj, Part *part __UNUSED__, Eina_Stringshare * item_name)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *items_glit, *glit;
   const Eina_List *l;

   assert(pl != NULL);
   assert(item_name != NULL);

   elm_genlist_item_expanded_set(pl->selected_part_item, true);
   items_glit = eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(pl->selected_part_item)));
   elm_genlist_item_update(items_glit);
   elm_genlist_item_expanded_set(items_glit, false);
   elm_genlist_item_expanded_set(items_glit, true);

   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(items_glit), l, glit)
     {
        if (elm_object_item_data_get(glit) == item_name) /* comparing stringshares */
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}

static void
_on_menu_add_item_clicked(void *data __UNUSED__,
                          Evas_Object *obj,
                          void *ei __UNUSED__)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Part *part;
   Group *group;
   Eina_Stringshare *title;
   Evas_Object *box, *item;
   Eina_List *l;

   assert(pl != NULL);

   part = elm_object_item_data_get(pl->selected_part_item);

   ap.popup = elm_popup_add(ap.win);
   elm_popup_orient_set(ap.popup, ELM_POPUP_ORIENT_CENTER);
   title = eina_stringshare_printf(_("Add new item to part \"%s\""), part->name);
   elm_object_part_text_set(ap.popup, "title,text", title);
   eina_stringshare_del(title);

   BOX_ADD(ap.popup, box, false, false);
   LAYOUT_PROP_ADD(box, _("Name:"), "popup", "1swallow")
   ENTRY_ADD(item, pl->popup.entry_name, true);
   eo_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE,
                         elm_validator_regexp_helper, pl->name_validator);
   evas_object_smart_callback_add(pl->popup.entry_name, "changed", _item_validate, pl);
   evas_object_smart_callback_add(pl->popup.entry_name, "activated", _popup_add_item_ok_clicked, pl);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter name for new item here."));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Source group:"), "popup", "1swallow")
   EWE_COMBOBOX_ADD(item, pl->popup.combobox)
   ewe_combobox_text_set(pl->popup.combobox, _("Select source group here."));
   evas_object_smart_callback_add(pl->popup.combobox, "selected", _item_validate, pl);

   EINA_LIST_FOREACH(ap.project->groups, l, group)
     {
        TODO("Add checks for recursion");
        if (part->group != group)
          ewe_combobox_item_add(pl->popup.combobox, group->name);
     }
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);

   elm_box_pack_end(box, item);
   elm_object_content_set(ap.popup, box);

   BUTTON_ADD(ap.popup, pl->popup.btn_add, _("Add"));
   evas_object_smart_callback_add(pl->popup.btn_add, "clicked", _popup_add_item_ok_clicked, pl);
   elm_object_part_content_set(ap.popup, "button1", pl->popup.btn_add);
   elm_object_disabled_set(pl->popup.btn_add, true);

   BUTTON_ADD(ap.popup, pl->popup.btn_cancel, _("Cancel"));
   evas_object_smart_callback_add(pl->popup.btn_cancel, "clicked", _popup_cancel_clicked, pl);
   elm_object_part_content_set(ap.popup, "button2", pl->popup.btn_cancel);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(ap.popup);
   elm_object_focus_set(pl->popup.entry_name, true);
}

static void
_popup_add_program_ok_clicked(void *data,
                              Evas_Object *obj __UNUSED__,
                              void *event_info __UNUSED__)
{
   Ewe_Combobox_Item *item;
   Edje_Action_Type type = EDJE_ACTION_TYPE_NONE;
   Part_List *pl = data;
   const char *name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   if (elm_object_disabled_get(pl->popup.btn_add)) return;

   item = ewe_combobox_select_item_get(pl->popup.combobox);
   switch (item->index)
     {
      case 0:
         type = EDJE_ACTION_TYPE_NONE;
         break;
      case 1:
         type = EDJE_ACTION_TYPE_STATE_SET;
         break;
      case 2:
         type = EDJE_ACTION_TYPE_SIGNAL_EMIT;
         break;
      case 3:
         type = EDJE_ACTION_TYPE_DRAG_VAL_SET;
         break;
      case 4:
         type = EDJE_ACTION_TYPE_DRAG_VAL_STEP;
         break;
      case 5:
         type = EDJE_ACTION_TYPE_DRAG_VAL_PAGE;
         break;
      case 6:
         type = EDJE_ACTION_TYPE_SOUND_SAMPLE;
         break;
      case 7:
         type = EDJE_ACTION_TYPE_SOUND_TONE;
         break;
      case 8:
         type = EDJE_ACTION_TYPE_ACTION_STOP;
         break;
     }

   name = elm_entry_entry_get(pl->popup.entry_name);

   msg = eina_stringshare_printf(_("added new program \"%s\""), name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_program_add(pl->group->edit_object, change, false, name));
   CRIT_ON_FAIL(editor_program_action_set(pl->group->edit_object, change, false, name, type));

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
   ecore_job_add(_job_popup_del, pl);
}

static void
_on_menu_add_program_clicked(void *data __UNUSED__,
                             Evas_Object *obj,
                             void *ei __UNUSED__)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Evas_Object *box, *item;
   unsigned int i;

   assert(pl != NULL);

   ap.popup = elm_popup_add(ap.win);
   elm_popup_orient_set(ap.popup, ELM_POPUP_ORIENT_CENTER);
   elm_object_part_text_set(ap.popup, "title,text", _("Add new program"));

   BOX_ADD(ap.popup, box, false, false);
   LAYOUT_PROP_ADD(box, _("Name:"), "popup", "1swallow")
   ENTRY_ADD(item, pl->popup.entry_name, true);
   eo_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE,
                         elm_validator_regexp_helper, pl->name_validator);
   evas_object_smart_callback_add(pl->popup.entry_name, "changed", _program_validate, pl);
   evas_object_smart_callback_add(pl->popup.entry_name, "activated", _popup_add_program_ok_clicked, pl);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter name for new program here."));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Action type:"), "popup", "1swallow")
   EWE_COMBOBOX_ADD(item, pl->popup.combobox)
   ewe_combobox_select_item_set(pl->popup.combobox, 0);

   for (i = 0; program_actions[i]; i++)
     {
        ewe_combobox_item_add(pl->popup.combobox, program_actions[i]);
     }
   ewe_combobox_select_item_set(pl->popup.combobox, 0);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);
   evas_object_smart_callback_add(pl->popup.combobox, "collapsed", _combobox_collapsed, pl);

   elm_box_pack_end(box, item);

   elm_object_content_set(ap.popup, box);

   BUTTON_ADD(ap.popup, pl->popup.btn_add, _("Add"));
   evas_object_smart_callback_add(pl->popup.btn_add, "clicked", _popup_add_program_ok_clicked, pl);
   elm_object_part_content_set(ap.popup, "button1", pl->popup.btn_add);
   elm_object_disabled_set(pl->popup.btn_add, true);

   BUTTON_ADD(ap.popup, pl->popup.btn_cancel, _("Cancel"));
   evas_object_smart_callback_add(pl->popup.btn_cancel, "clicked", _popup_cancel_clicked, pl);
   elm_object_part_content_set(ap.popup, "button2", pl->popup.btn_cancel);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(ap.popup);
   elm_object_focus_set(pl->popup.entry_name, true);
}

void
group_navigator_program_add(Evas_Object *obj, Eina_Stringshare *program)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit;
   const Eina_List *l;
   Resource *res;

   assert(pl != NULL);
   assert(program != NULL);

   elm_genlist_item_expanded_set(pl->programs_caption_item, false);
   elm_genlist_item_expanded_set(pl->programs_caption_item, true);
   elm_genlist_item_update(pl->programs_caption_item);
   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(pl->programs_caption_item), l, glit)
     {
        res = elm_object_item_data_get(glit);
        if (res->name == program) /* comparing stringshares */
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}

static void
_program_del(Part_List *pl,
             Elm_Object_Item *glit)
{
   Eina_Stringshare *msg;
   Change *change;
   Resource *program;

   assert(pl != NULL);
   assert(glit != NULL);

   program = elm_object_item_data_get(glit);

   assert(program != NULL);

   msg = eina_stringshare_printf(_("deleted program \"%s\""), program->name);
   change = change_add(msg);
   eina_stringshare_del(msg);

   eina_stringshare_ref(program->name);
   CRIT_ON_FAIL(editor_program_del(pl->group->edit_object, change, false, program->name));
   eina_stringshare_del(program->name);
   if (pl->group->current_program)
     eina_stringshare_del(pl->group->current_program);

   history_change_add(pl->group->history, change);
}

void
group_navigator_program_del(Evas_Object *obj, Eina_Stringshare *program __UNUSED__)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);

   elm_genlist_item_update(pl->programs_caption_item);
   if (elm_genlist_item_expanded_get(pl->programs_caption_item))
     {
        elm_genlist_item_expanded_set(pl->programs_caption_item, false);
        elm_genlist_item_expanded_set(pl->programs_caption_item, true);
     }
   elm_genlist_item_selected_set(pl->programs_caption_item, true);
}

static void
_on_btn_plus_clicked(void *data,
                     Evas_Object *obj,
                     void *ei __UNUSED__)
{
   Part_List *pl = data;
   Evas_Coord x, y, h;

   assert(pl != NULL);

   evas_object_geometry_get(obj, &x, &y, NULL, &h);

   elm_menu_move(pl->menu, x, y + h);
   evas_object_show(pl->menu);
}

static void
_part_del(Part_List *pl,
          Elm_Object_Item *glit)
{
   Part *part;
   Eina_Stringshare *part_name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);

   part = elm_object_item_data_get(glit);

   part_name = eina_stringshare_add(part->name);
   msg = eina_stringshare_printf(_("deleted part \"%s\""), part_name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_del(pl->group->edit_object, change, false, part_name));
   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
   eina_stringshare_del(part_name);
}

static Elm_Object_Item *
_part_item_find(Part_List *pl, Part *part)
{
   Elm_Object_Item *part_item;
   const Eina_List *part_items;
   Part *pr;

   assert(pl != NULL);
   assert(part != NULL);

   elm_genlist_item_expanded_set(pl->parts_caption_item, true);
   part_items = elm_genlist_item_subitems_get(pl->parts_caption_item);

   part_item = eina_list_data_get(part_items);
   pr = elm_object_item_data_get(part_item);
   while (pr != part)
     {
        part_items = eina_list_next(part_items);
        part_item = eina_list_data_get(part_items);
        pr = elm_object_item_data_get(part_item);

        assert(pr != NULL);
     }
   assert(part_item != NULL);
   return part_item;
}

static void
_select_next_part(Part_List *pl, Eina_Bool reverse)
{
   Elm_Object_Item *part_item;
   const Eina_List *part_items, *l = NULL;

   assert(pl != NULL);

   part_items = elm_genlist_item_subitems_get(pl->parts_caption_item);
   if (pl->selected_part_item)
     l = eina_list_data_find_list(part_items, pl->selected_part_item);
   if (reverse)
     {
        if (eina_list_prev(l))
          l = eina_list_prev(l);
        else
          l = eina_list_last(l);
     }
   else
     {
        if (eina_list_next(l))
          l = eina_list_next(l);
        else
          l = part_items;
     }
   part_item = eina_list_data_get(l);
   elm_genlist_item_selected_set(part_item, true);
}

void
group_navigator_part_del(Evas_Object *obj, Part *part)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *part_item;

   assert(pl != NULL);
   assert(part != NULL);

   part_item = _part_item_find(pl, part);
   if (pl->selected_part_item == part_item)
     {
        _unselect_part(pl);
        evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_NAVIGATOR_UNSELECTED, NULL);
     }

   elm_object_item_del(part_item);
   elm_genlist_item_update(pl->parts_caption_item);
}

static void
_state_del(Part_List *pl,
           Elm_Object_Item *glit)
{
   State *state;
   Eina_Stringshare *part_name, *state_name;
   double state_val;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(pl->selected_part_item != NULL);
   assert(glit != NULL);

   state = elm_object_item_data_get(glit);

   assert(state != NULL);
   assert(strcmp(state->name, "default 0.00")); /* default state can't be deleted */

   msg = eina_stringshare_printf(_("deleted state \"%s\" %.2f"), state->parsed_name, state->parsed_val);
   change = change_add(msg);
   eina_stringshare_del(msg);
   part_name = eina_stringshare_ref(state->part->name);
   state_name = eina_stringshare_ref(state->parsed_name);
   state_val = state->parsed_val;
   TODO("recheck string args logic");
   CRIT_ON_FAIL(editor_state_del(pl->group->edit_object, change, false, part_name, state_name, state_val));
   eina_stringshare_del(part_name);
   eina_stringshare_del(state_name);
   history_change_add(pl->group->history, change);
}

void
group_navigator_part_state_del(Evas_Object *obj, Part *part __UNUSED__, State *state)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   const Elm_Genlist_Item_Class* itc;
   Elm_Object_Item *glit;
   Elm_Object_Item *default_glit;
   const Eina_List *subitems;
   const Eina_List *l;

   assert(pl != NULL);
   assert(state != NULL);

   elm_genlist_item_expanded_set(pl->selected_part_item, true);

   subitems = elm_genlist_item_subitems_get(pl->selected_part_item);
   /* "default 0.0" is always first in states list */
   default_glit = eina_list_data_get(subitems);
   /* find state's genlist item */
   EINA_LIST_FOREACH(subitems, l, glit)
     {
        if (elm_object_item_data_get(glit) == state)
          break;
     }
   assert(glit != NULL);

   /* resetting state */
   itc = elm_genlist_item_item_class_get(glit);
   if (itc == pl->itc_state_selected)
     _on_activated(pl, NULL, default_glit);

   elm_object_item_del(glit);
   elm_genlist_item_selected_set(default_glit, true);
}

static void
_item_del(Part_List *pl,
          Elm_Object_Item *glit)
{
   Part *part;
   Eina_Stringshare *item_name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);
   assert(pl->selected_part_item != NULL);

   part = elm_object_item_data_get(pl->selected_part_item);
   item_name = elm_object_item_data_get(glit);

   assert(part != NULL);
   assert(item_name != NULL);

   msg = eina_stringshare_printf(_("deleted item \"%s\""), item_name);
   change = change_add(msg);
   eina_stringshare_del(msg);

   eina_stringshare_ref(item_name);
   CRIT_ON_FAIL(editor_part_item_del(pl->group->edit_object, change, false, part->name, item_name));
   eina_stringshare_del(item_name);

   history_change_add(pl->group->history, change);
}

void
group_navigator_part_item_del(Evas_Object *obj, Part *part __UNUSED__, Eina_Stringshare *item_name)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *items_glit;

   assert(pl != NULL);
   assert(item_name != NULL);

   elm_genlist_item_expanded_set(pl->selected_part_item, true);
   items_glit = eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(pl->selected_part_item)));

   elm_genlist_item_expanded_set(items_glit, false);
   elm_genlist_item_update(items_glit);
   elm_genlist_item_expanded_set(items_glit, true);
}

static void
_on_btn_minus_clicked(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   Part_List *pl = data;
   Elm_Object_Item *glit;
   const Elm_Genlist_Item_Class* itc;

   assert(pl != NULL);

   glit = elm_genlist_selected_item_get(pl->genlist);

   assert(glit != NULL);

   itc = elm_genlist_item_item_class_get(glit);
   if (itc == pl->itc_part)
     _part_del(pl, glit);
   else if ((itc == pl->itc_state_selected) || (itc == pl->itc_state))
     _state_del(pl, glit);
   else if (itc == pl->itc_item)
     _item_del(pl, glit);
   else if (itc == pl->itc_program)
     _program_del(pl, glit);

   TODO("Check if we still need this")
   /* Need to save pl->group->edit_object, since we changed it */
   CRIT_ON_FAIL(editor_save_all(ap.project->global_object));
   TODO("Remove this line once edje_edit_image_add would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;
}

static void
_part_restack(Part_List *pl, Elm_Object_Item *glit, Eina_Bool move_up)
{
   Part *part, *rel_part;
   Elm_Object_Item *rel_glit;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);

   if (move_up)
     {
        rel_glit = elm_genlist_item_prev_get(glit);
        /* Finding where to move item. genlist don't allows to ignore subitems */
        while ((rel_glit != NULL) && (elm_genlist_item_item_class_get(rel_glit) != pl->itc_part))
          rel_glit = elm_genlist_item_prev_get(rel_glit);
        assert(rel_glit != NULL); /* we shouldn't move up first item */
        assert(elm_genlist_item_item_class_get(rel_glit) == pl->itc_part);
     }
   else
     {
        rel_glit = elm_genlist_item_next_get(glit);
        while ((rel_glit != NULL) && (elm_genlist_item_item_class_get(rel_glit) != pl->itc_part))
          rel_glit = elm_genlist_item_next_get(rel_glit);
        assert(rel_glit != NULL); /* we shouldn't move down last item */
        rel_glit = elm_genlist_item_next_get(rel_glit);
        while ((rel_glit != NULL) && (elm_genlist_item_item_class_get(rel_glit) != pl->itc_part))
          rel_glit = elm_genlist_item_next_get(rel_glit);

        /* NULL after second loop means that part will be moved to first position */
        assert((rel_glit == NULL) || (elm_genlist_item_item_class_get(rel_glit) == pl->itc_part));
     }

   part = elm_object_item_data_get(glit);
   rel_part = elm_object_item_data_get(rel_glit);

   if (rel_part)
      msg = eina_stringshare_printf(_("part \"%s\" placed below part \"%s\" in the stack"), part->name, rel_part->name);
   else
      msg = eina_stringshare_printf(_("part \"%s\" restacked to the top of the stack"), part->name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_restack(pl->group->edit_object, change, false,
                                    part->name,
                                    (rel_part) ? rel_part->name : NULL));
   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_restack(Evas_Object *obj, Part *part, Part *rel_part)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit, *rel_glit;

   assert(pl != NULL);
   assert(part != NULL);

   if (elm_genlist_item_expanded_get(pl->parts_caption_item))
     {
        glit = _part_item_find(pl, part);
        assert(glit != NULL);
        assert(elm_genlist_item_parent_get(glit) == pl->parts_caption_item);

        if (rel_part)
          {
             rel_glit = _part_item_find(pl, rel_part);
             assert(rel_glit != NULL);

             elm_genlist_item_insert_before(pl->genlist,
                                            pl->itc_part,
                                            part,
                                            pl->parts_caption_item,
                                            rel_glit,
                                            ELM_GENLIST_ITEM_TREE,
                                            NULL,
                                            NULL);
          }
        else
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_part,
                                     part,
                                     pl->parts_caption_item,
                                     ELM_GENLIST_ITEM_TREE,
                                     NULL,
                                     NULL);
          }
        elm_object_item_del(glit);
     }
   group_navigator_part_select(pl->layout, part);
}

static void
_part_item_restack(Part_List *pl, Elm_Object_Item *glit, Eina_Bool move_up)
{
   Part *part;
   Eina_Stringshare *part_item, *rel_part_item;
   Elm_Object_Item *rel_glit;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);

   if (move_up)
     {
        rel_glit = elm_genlist_item_prev_get(glit);
        assert(rel_glit != NULL); /* we shouldn't move up first item */
        assert(elm_genlist_item_item_class_get(rel_glit) == pl->itc_item);
     }
   else
     {
        rel_glit = elm_genlist_item_next_get(glit);
        assert(rel_glit != NULL); /* we shouldn't move down last item */
        rel_glit = elm_genlist_item_next_get(rel_glit);
        if (elm_genlist_item_item_class_get(rel_glit) != pl->itc_item)
          rel_glit = NULL;
     }

   part = elm_object_item_data_get(pl->selected_part_item);

   part_item = elm_object_item_data_get(glit);
   rel_part_item = elm_object_item_data_get(rel_glit);

   if (rel_part_item)
      msg = eina_stringshare_printf(_("part item \"%s\" placed below part item \"%s\" in the stack"), part_item, rel_part_item);
   else
      msg = eina_stringshare_printf(_("part item \"%s\" restacked to the top of the stack"), part_item);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_item_restack(pl->group->edit_object, change, false,
                                         part->name,
                                         part_item,
                                         rel_part_item));
   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_item_restack(Evas_Object *obj,
                                  Part *part,
                                  Eina_Stringshare *part_item,
                                  Eina_Stringshare *relative_part_item __UNUSED__)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit, *items_glit;
   const Eina_List *l;

   assert(pl != NULL);
   assert(part != NULL);
   assert(part_item != NULL);

   glit = _part_item_find(pl, part);
   assert(glit != NULL);

   elm_genlist_item_expanded_set(pl->selected_part_item, true);
   items_glit = eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(pl->selected_part_item)));
   elm_genlist_item_expanded_set(items_glit, false);
   elm_genlist_item_expanded_set(items_glit, true);

   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(items_glit), l, glit)
     {
        if (elm_object_item_data_get(glit) == part_item) /* comparing stringshares */
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}


static void
_on_btn_down_clicked(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *ei __UNUSED__)
{
   Part_List *pl = data;
   Elm_Object_Item *glit;
   const Elm_Genlist_Item_Class* itc;

   assert(pl != NULL);

   glit = elm_genlist_selected_item_get(pl->genlist);

   assert(glit != NULL);

   itc = elm_genlist_item_item_class_get(glit);
   if (itc == pl->itc_part)
     _part_restack(pl, glit, false);
   else if (itc == pl->itc_item)
     _part_item_restack(pl, glit, false);
}

static void
_on_btn_up_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *ei __UNUSED__)
{
   Part_List *pl = data;
   Elm_Object_Item *glit;
   const Elm_Genlist_Item_Class* itc;

   assert(pl != NULL);

   glit = elm_genlist_selected_item_get(pl->genlist);

   assert(glit != NULL);

   itc = elm_genlist_item_item_class_get(glit);
   if (itc == pl->itc_part)
     _part_restack(pl, glit, true);
   else if (itc == pl->itc_item)
     _part_item_restack(pl, glit, true);
}

Evas_Object *
group_navigator_add(Evas_Object *parent, Group *group)
{
   Evas_Object *icon;
   Part_List *pl;
   Elm_Object_Item *menu_item;

   assert(group != NULL);
   assert(parent != NULL);

   pl = mem_calloc(1, sizeof(Part_List));
   pl->layout = elm_layout_add(parent);
   elm_layout_theme_set(pl->layout, "layout", "navigator", "default");
   evas_object_show(pl->layout);

   evas_object_data_set(pl->layout, GROUP_NAVIGATOR_DATA, pl);

   pl->group = group;

#define BTN_ADD(BTN, SWALLOW, ICON_NAME, CALLBACK) \
   pl->BTN = elm_button_add(pl->layout); \
   ICON_STANDARD_ADD(pl->BTN, icon, true, ICON_NAME); \
   elm_object_part_content_set(pl->BTN, NULL, icon); \
   evas_object_smart_callback_add(pl->BTN, "clicked", CALLBACK, pl); \
   elm_object_style_set(pl->BTN, "anchor"); \
   elm_object_part_content_set(pl->layout, SWALLOW, pl->BTN);

   BTN_ADD(btn_add, "elm.swallow.btn3", "plus", _on_btn_plus_clicked);
   BTN_ADD(btn_del, "elm.swallow.btn2", "minus", _on_btn_minus_clicked);
   BTN_ADD(btn_down, "elm.swallow.btn1", "arrow_down", _on_btn_down_clicked);
   BTN_ADD(btn_up, "elm.swallow.btn0", "arrow_up", _on_btn_up_clicked);

   elm_object_disabled_set(pl->btn_del, true);
   elm_object_disabled_set(pl->btn_down, true);
   elm_object_disabled_set(pl->btn_up, true);

#undef BTN_ADD
   pl->itc_part = elm_genlist_item_class_new();
   pl->itc_part->item_style = "part";
   pl->itc_part->func.text_get = _resource_label_get;
   pl->itc_part->func.content_get = _part_content_get;
   pl->itc_part->func.state_get = _part_state_get;

   pl->itc_state = elm_genlist_item_class_new();
   pl->itc_state->item_style = "state";
   pl->itc_state->func.text_get = _resource_label_get;

   pl->itc_state_selected = elm_genlist_item_class_new();
   pl->itc_state_selected->item_style = "state_selected";
   pl->itc_state_selected->func.text_get = _resource_label_get;

   pl->itc_item = elm_genlist_item_class_new();
   pl->itc_item->item_style = "item";
   pl->itc_item->func.text_get = _item_label_get;

   pl->itc_item_caption = elm_genlist_item_class_new();
   pl->itc_item_caption->item_style = "items_caption";
   pl->itc_item_caption->func.text_get = _item_caption_label_get;

   TODO("create new style or fix default (we need to set number at the end)");
   pl->itc_caption = elm_genlist_item_class_new();
   pl->itc_caption->item_style = "caption";
   pl->itc_caption->func.text_get = _caption_label_get;
   pl->itc_caption->func.content_get = _caption_content_get;

   pl->itc_program = elm_genlist_item_class_new();
   pl->itc_program->item_style = "program";
   pl->itc_program->func.text_get = _resource_label_get;

   pl->genlist = elm_genlist_add(pl->layout);
   elm_genlist_homogeneous_set(pl->genlist, true);
   elm_scroller_policy_set(pl->genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(pl->genlist);
   elm_object_content_set(pl->layout, pl->genlist);
   evas_object_smart_callback_add(pl->genlist, "activated", _on_activated, pl);
   evas_object_smart_callback_add(pl->genlist, "expand,request", _expand_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contract,request", _contract_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "expanded", _expanded_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contracted", _contracted_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "selected", _selected_cb, pl);
   evas_object_data_set(pl->genlist, GROUP_NAVIGATOR_DATA, pl);
   /*elm_genlist_tree_effect_enabled_set(pl->genlist, EINA_TRUE);*/

   pl->parts_caption_item = elm_genlist_item_append(pl->genlist,
                                                    pl->itc_caption,
                                                    &group->parts,
                                                    NULL,
                                                    ELM_GENLIST_ITEM_TREE,
                                                    NULL,
                                                    NULL);
   pl->programs_caption_item = elm_genlist_item_append(pl->genlist,
                                                    pl->itc_caption,
                                                    &group->programs,
                                                    NULL,
                                                    ELM_GENLIST_ITEM_TREE,
                                                    NULL,
                                                    NULL);


   elm_object_text_set(pl->layout, pl->group->name);

   pl->menu = elm_menu_add(ap.win);
   evas_object_data_set(pl->menu, GROUP_NAVIGATOR_DATA, pl);

   menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Part"), _on_menu_add_part_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(menu_item), "elm.shortcut", "q");
   elm_menu_item_separator_add(pl->menu, NULL);

   pl->add_state_menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("State"), _on_menu_add_state_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(pl->add_state_menu_item), "elm.shortcut", "w");
   elm_object_item_disabled_set(pl->add_state_menu_item, true);
   pl->add_part_item_menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Item"), _on_menu_add_item_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(pl->add_part_item_menu_item), "elm.shortcut", "e");
   elm_object_item_disabled_set(pl->add_part_item_menu_item, true);
   elm_menu_item_separator_add(pl->menu, NULL);

   menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Program"), _on_menu_add_program_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(menu_item), "elm.shortcut", "r");

   pl->name_validator = elm_validator_regexp_new(PART_NAME_REGEX, NULL);
   if (group->main_group)
     elm_object_disabled_set(pl->layout, true);
   else
     {
        elm_genlist_item_expanded_set(pl->parts_caption_item, true);
        elm_genlist_item_expanded_set(pl->programs_caption_item, true);
     }

   TODO("Add deletion callback and free resources");
   return pl->layout;
}

void
group_navigator_part_select(Evas_Object *obj, Part *part)
{
   Elm_Object_Item *part_item;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);

   if (part)
     {
        part_item = _part_item_find(pl, part);
        elm_genlist_item_selected_set(part_item, true);
     }
   else
     {
        _unselect_part(pl);
        evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_NAVIGATOR_UNSELECTED, NULL);
        elm_genlist_item_selected_set(elm_genlist_selected_item_get(pl->genlist), false);
     }
}

void
group_navigator_part_update(Evas_Object *obj, Part *part)
{
   Elm_Object_Item *part_item;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);
   assert(part != NULL);

   if (elm_genlist_item_expanded_get(pl->parts_caption_item))
     {
        part_item = _part_item_find(pl, part);
        assert(part_item != NULL);

        elm_genlist_item_update(part_item);
     }
}

void
group_navigator_add_part_request(Evas_Object *obj)
{
   _on_menu_add_part_clicked(NULL, obj, NULL);
}

void
group_navigator_add_part_item_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   if (!elm_object_item_disabled_get(pl->add_part_item_menu_item))
     _on_menu_add_item_clicked(NULL, obj, NULL);
}

void
group_navigator_add_state_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   if (!elm_object_item_disabled_get(pl->add_state_menu_item))
     _on_menu_add_state_clicked(NULL, obj, NULL);
}

void
group_navigator_add_program_request(Evas_Object *obj)
{
   _on_menu_add_program_clicked(NULL, obj, NULL);
}

void
group_navigator_delete_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   if (!elm_object_disabled_get(pl->btn_del))
     _on_btn_minus_clicked(pl, NULL, NULL);
}

void
group_navigator_state_next_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Part *part;
   State *state;
   Eina_List *l;

   if (pl->selected_part_item != NULL)
     {
        part = elm_object_item_data_get(pl->selected_part_item);
        if (eina_list_count(part->states) > 1)
          {
             l = eina_list_data_find_list(part->states, part->current_state);

             assert(l != NULL);

             if (eina_list_next(l) != NULL)
               state = eina_list_data_get(eina_list_next(l));
             else
               state = eina_list_data_get(part->states);

             CRIT_ON_FAIL(editor_part_selected_state_set(pl->group->edit_object, NULL, false,
                                                         state->part->name,
                                                         state->parsed_name,
                                                         state->parsed_val));
          }
     }
}

void
group_navigator_part_next_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);

   _select_next_part(pl, false);
}

void
group_navigator_part_prev_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);

   _select_next_part(pl, true);
}

void
group_navigator_part_showhide_request(Evas_Object *obj)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Part *part;
   Evas_Object *eye;

   assert(pl != NULL);

   if (pl->selected_part_item != NULL)
     {
        part = elm_object_item_data_get(pl->selected_part_item);
        if (part)
          {
             eye = elm_object_item_part_content_get(pl->selected_part_item, "elm.swallow.icon");
             if (eye) elm_check_state_set(eye, !elm_check_state_get(eye));
             /* elementary not call callback if we change the check state, while it
              * not fixed, we need this small hack */
             _on_eye_clicked(part, obj, NULL);
          }
     }
}

void
group_navigator_all_parts_showhide_request(Evas_Object *obj)
{
   Evas_Object *eye;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);

   _on_parts_eye_clicked(NULL, obj, NULL);
   eye = elm_object_item_part_content_get(pl->parts_caption_item, "elm.swallow.icon");
   if (eye)
     elm_check_state_set(eye, _all_parts_visible(pl));
}
