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
#ifndef EO_BETA_API
# define EO_BETA_API
#endif

#ifndef EFL_BETA_API_SUPPORT
# define EFL_BETA_API_SUPPORT
#endif

#ifndef EFL_EO_API_SUPPORT
# define EFL_EO_API_SUPPORT
#endif

#include "group_navigator.h"
#include "main_window.h"
#include "history.h"
#include "project_manager2.h"
#include "change.h"
#include "widget_macro.h"
#include "validator.h"

#define GROUP_NAVIGATOR_DATA "group_navigator_data"

static Evas_Object *dummy_navi = NULL;

typedef struct
{
   int index;
   int part_type;
   Eina_Stringshare *data;
} Combobox_Item;

typedef struct
{
   Group2 *group;
   Part2 *part;
   Program2 *program;

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
   Elm_Genlist_Item_Class *itc_group_data;

   Elm_Object_Item *parts_caption_item;
   Elm_Object_Item *programs_caption_item;
   Elm_Object_Item *data_caption_item;

   Evas_Object *menu;
   Elm_Object_Item *add_state_menu_item;
   Elm_Object_Item *add_part_item_menu_item;
   Elm_Validator_Regexp *name_validator;
   Resource_Name_Validator *part_name_validator;
   Resource_Name_Validator *program_name_validator;
   Resource_Name_Validator *group_data_name_validator;

   Evas_Object *popup_win;
   struct {
        int copy_part, part_type, program_selected;
        Combobox_Item *state_selected, *item_selected;
        Evas_Object *box;
        Evas_Object *entry_name;
        Evas_Object *spinner_value;
        Evas_Object *combobox;
        Evas_Object *combobox_copy;
        Evas_Object *btn_add;
        Evas_Object *btn_cancel;
        Elm_Genlist_Item_Class *itc;
   } popup;
} Part_List;

static Edje_Part_Type part_types[] = {
     EDJE_PART_TYPE_RECTANGLE,
     EDJE_PART_TYPE_TEXT,
     EDJE_PART_TYPE_IMAGE,
     EDJE_PART_TYPE_SWALLOW,
     EDJE_PART_TYPE_TEXTBLOCK,
     EDJE_PART_TYPE_GROUP,
     EDJE_PART_TYPE_BOX,
     EDJE_PART_TYPE_TABLE,
     EDJE_PART_TYPE_PROXY,
     EDJE_PART_TYPE_SPACER,
     EDJE_PART_TYPE_VECTOR,
     EDJE_PART_TYPE_NONE
};
static const char *program_actions[] = {
     N_("None"),
     N_("State set"),
     N_("Signal emit"),
     N_("Drag value set"),
     N_("Drag value step"),
     N_("Drag value page"),
     N_("Play sample"),
     N_("Play tone"),
     N_("Action stop"),
     NULL
};

#define BTN_ADD(LAYOUT, BTN, SWALLOW, STYLE) \
   BTN = elm_button_add(LAYOUT); \
   elm_object_style_set(BTN, STYLE); \
   elm_object_part_content_set(LAYOUT, SWALLOW, BTN);

static char *
_item_label_get(void *data,
                Evas_Object *obj EINA_UNUSED,
                const char *pr EINA_UNUSED)
{
   Eina_Strbuf *buf;
   Resource2 *res = data;
   char *result;

   assert(res != NULL);

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "item [%d]", res->common.id);
   if (res->common.name)
     eina_strbuf_append_printf(buf, " - %s", res->common.name);
   result = strdup(eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   return result;
}

static char *
_resource_label_get(void *data,
                    Evas_Object *obj EINA_UNUSED,
                    const char *pr EINA_UNUSED)
{
   Resource2 *res = data;

   assert(res != NULL);
   assert(res->common.name != NULL);

   return strdup(res->common.name);
}

static char *
_state_resource_label_get(void *data,
                          Evas_Object *obj EINA_UNUSED,
                          const char *pr EINA_UNUSED)
{
   char *ret;
   State2 *res = data;
   Eina_Stringshare *label;

   assert(res != NULL);
   assert(res->common.name != NULL);

   label = eina_stringshare_printf("%s %.2f", res->common.name, res->val);
   ret = strdup(label);
   eina_stringshare_del(label);
   return ret;
}

static char *
_item_caption_label_get(void *data,
                        Evas_Object *obj EINA_UNUSED,
                        const char *pr)
{
   Part2 *part = data;
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
        else if (list == &pl->group->data_items)
          return strdup(_("Data items"));
     }
   if (!strcmp(pr, "elm.text.end"))
     {
        snprintf(buf, BUFF_MAX, "%d", eina_list_count(*list));
        return strdup(buf);
     }
   return NULL;
}

static Eina_Bool
_all_parts_visible(Part_List *pl)
{
   const Eina_List *l;
   const Part2 *part;
   Eina_Bool visible = true;

   assert(pl != NULL);

   EINA_LIST_FOREACH(pl->group->parts, l, part)
      visible &= part->visible;

   return visible;
}

static void
_on_parts_eye_clicked(void *data EINA_UNUSED,
                      Evas_Object *obj,
                      void *event_data EINA_UNUSED)
{
   Evas_Object *eye;
   Part2 *part;
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
                void *event_data EINA_UNUSED)
{
   Evas_Object *eye;
   Part2 *part = data;
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
   Part2 *_part;
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

          evas_object_smart_callback_add(content, signals.elm.check.changed, _on_parts_eye_clicked, NULL);
       }

   return content;
}

static Evas_Object *
_part_content_get(void *data,
                  Evas_Object *obj,
                  const char *part)
{
   Evas_Object *content = NULL;
   Part2 *_part = data;

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
        evas_object_smart_callback_add(content, signals.elm.check.changed, _on_eye_clicked, _part);
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
           case EDJE_PART_TYPE_VECTOR:
              IMAGE_ADD_NEW(obj, content, "icon", "vector");
              break;
          default:
              ERR("Unhandled part type");
              abort();
          }
     }
#ifdef HAVE_TIZEN
   if (!strcmp(part, "elm.swallow.end1"))
     {
        switch (_part->type)
          {
           case EDJE_PART_TYPE_RECTANGLE:
              IMAGE_ADD_NEW(obj, content, "icon", "rectangle1");
              break;
           case EDJE_PART_TYPE_IMAGE:
              IMAGE_ADD_NEW(obj, content, "icon", "image1");
              break;
           case EDJE_PART_TYPE_SPACER:
              IMAGE_ADD_NEW(obj, content, "icon", "spacer1");
              break;
           case EDJE_PART_TYPE_SWALLOW:
              IMAGE_ADD_NEW(obj, content, "icon", "swallow1");
              break;
           case EDJE_PART_TYPE_TEXT:
              IMAGE_ADD_NEW(obj, content, "icon", "text1");
              break;
           case EDJE_PART_TYPE_TEXTBLOCK:
              IMAGE_ADD_NEW(obj, content, "icon", "textblock1");
              break;
           case EDJE_PART_TYPE_GROUP:
              IMAGE_ADD_NEW(obj, content, "icon", "group1");
              break;
           case EDJE_PART_TYPE_PROXY:
              IMAGE_ADD_NEW(obj, content, "icon", "proxy1");
              break;
           case EDJE_PART_TYPE_TABLE:
              IMAGE_ADD_NEW(obj, content, "icon", "table1");
              break;
           case EDJE_PART_TYPE_BOX:
              IMAGE_ADD_NEW(obj, content, "icon", "box1");
              break;
           case EDJE_PART_TYPE_EXTERNAL:
              IMAGE_ADD_NEW(obj, content, "icon", "external");
              break;
           case EDJE_PART_TYPE_VECTOR:
              IMAGE_ADD_NEW(obj, content, "icon", "vector1");
              break;
          default:
              ERR("Unhandled part type");
              abort();
          }
     }
   if (!strcmp(part, "elm.swallow.end2"))
     {
        switch (_part->type)
          {
           case EDJE_PART_TYPE_RECTANGLE:
              IMAGE_ADD_NEW(obj, content, "icon", "rectangle2");
              break;
           case EDJE_PART_TYPE_IMAGE:
              IMAGE_ADD_NEW(obj, content, "icon", "image2");
              break;
           case EDJE_PART_TYPE_SPACER:
              IMAGE_ADD_NEW(obj, content, "icon", "spacer2");
              break;
           case EDJE_PART_TYPE_SWALLOW:
              IMAGE_ADD_NEW(obj, content, "icon", "swallow2");
              break;
           case EDJE_PART_TYPE_TEXT:
              IMAGE_ADD_NEW(obj, content, "icon", "text2");
              break;
           case EDJE_PART_TYPE_TEXTBLOCK:
              IMAGE_ADD_NEW(obj, content, "icon", "textblock2");
              break;
           case EDJE_PART_TYPE_GROUP:
              IMAGE_ADD_NEW(obj, content, "icon", "group2");
              break;
           case EDJE_PART_TYPE_PROXY:
              IMAGE_ADD_NEW(obj, content, "icon", "proxy2");
              break;
           case EDJE_PART_TYPE_TABLE:
              IMAGE_ADD_NEW(obj, content, "icon", "table2");
              break;
           case EDJE_PART_TYPE_BOX:
              IMAGE_ADD_NEW(obj, content, "icon", "box2");
              break;
           case EDJE_PART_TYPE_EXTERNAL:
              IMAGE_ADD_NEW(obj, content, "icon", "external");
              break;
           case EDJE_PART_TYPE_VECTOR:
              IMAGE_ADD_NEW(obj, content, "icon", "vector2");
              break;
          default:
              ERR("Unhandled part type");
              abort();
          }
     }
#endif /* HAVE_TIZEN */
   return content;
}

static void
_expand_request_cb(void *data EINA_UNUSED,
                   Evas_Object *o EINA_UNUSED,
                   void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
_contract_request_cb(void *data EINA_UNUSED,
                     Evas_Object *o EINA_UNUSED,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

static Elm_Object_Item * _part_item_find(Part_List *pl, Part2 *part);
static Elm_Object_Item * _program_glit_find(Part_List *pl, Program2 *part);
static Elm_Object_Item * _group_data_item_find(Part_List *pl, Resource2 *group_data);

void
group_navigator_part_state_select(Evas_Object *obj, State2 *state)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   const Eina_List *subitems, *l;
   Elm_Object_Item *selected, *part_item;

   assert(pl != NULL);
   assert(state != NULL);

   part_item = _part_item_find(pl, state->part);
   elm_genlist_item_expanded_set(part_item, true);
   if (elm_genlist_item_expanded_get(part_item))
     {
        /* unselecting previous selected state */
        subitems = elm_genlist_item_subitems_get(part_item);
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
        elm_genlist_item_selected_set(selected, true);
     }

   state->part->current_state = state;
}

static void
_expanded_cb(void *data,
             Evas_Object *o EINA_UNUSED,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Part_List *pl = data;
   const Elm_Genlist_Item_Class* itc;
   Eina_List *l;
   Part2 *part;
   Resource2 *res;
   State2 *state;
   Part_Item2 *item;

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
             if ((first_item) || ((strcmp(state->common.name, "default") != 0) || (state->val != 0)))
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
        EINA_LIST_FOREACH(part->items, l, item)
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_item,
                                     item,
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
   else if (glit == pl->data_caption_item)
     {
        EINA_LIST_FOREACH(pl->group->data_items, l, res)
          {
             elm_genlist_item_append(pl->genlist,
                                     pl->itc_group_data,
                                     res,
                                     pl->data_caption_item,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);
          }
     }
}

static void
_contracted_cb(void *data EINA_UNUSED,
               Evas_Object *o EINA_UNUSED,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
_unselect_internal(Part_List *pl)
{
   assert(pl != NULL);

#if HAVE_TIZEN
   if (pl->group->current_selected != NULL)
   if (pl->group->current_selected->common.type == RESOURCE2_TYPE_PART)
     {
        Elm_Object_Item *part_item = _part_item_find(pl, pl->part);
        Evas_Object *check = elm_object_item_part_content_get(part_item, "elm.swallow.icon");
        if (check)
          elm_object_signal_emit(check, "unselected", "eflete");
     }
#endif

   pl->part = NULL;
   pl->group->current_selected = NULL;

   elm_object_item_disabled_set(pl->add_state_menu_item, true);
   ui_menu_disable_set(ap.menu, MENU_EDIT_STATE_ADD, true);
   elm_object_item_disabled_set(pl->add_part_item_menu_item, true);
   ui_menu_disable_set(ap.menu, MENU_EDIT_ITEM_ADD, true);

   elm_object_disabled_set(pl->btn_del, true);
   elm_object_disabled_set(pl->btn_down, true);
   elm_object_disabled_set(pl->btn_up, true);
 }

static void
_enable_buttons(Part_List *pl, Elm_Object_Item *glit)
{
   const Eina_List *items_list;

   assert(pl != NULL);

   if (pl->part)
     {
        elm_object_item_disabled_set(pl->add_state_menu_item, false);
        ui_menu_disable_set(ap.menu, MENU_EDIT_STATE_ADD, false);
        ui_menu_disable_set(ap.menu, MENU_WINDOW_MANAGER_SCRIPT, true);

        if ((pl->part->type == EDJE_PART_TYPE_BOX) ||
            (pl->part->type == EDJE_PART_TYPE_TABLE))
          {
             elm_object_item_disabled_set(pl->add_part_item_menu_item, false);
             ui_menu_disable_set(ap.menu, MENU_EDIT_ITEM_ADD, false);
          }
     }
   if (pl->group->current_selected)
     {
        if ((pl->group->current_selected->common.type != RESOURCE2_TYPE_STATE) ||
            (((State2 *)pl->group->current_selected)->val != 0) ||
            (strcmp(pl->group->current_selected->common.name, "default") != 0))
        elm_object_disabled_set(pl->btn_del, false);

        if ((pl->group->current_selected->common.type == RESOURCE2_TYPE_PART) ||
            (pl->group->current_selected->common.type == RESOURCE2_TYPE_ITEM))
          {
             items_list = elm_genlist_item_subitems_get(elm_genlist_item_parent_get(glit));

             if (glit != eina_list_data_get(items_list))
               elm_object_disabled_set(pl->btn_up, false);

             if (glit != eina_list_data_get(eina_list_last(items_list)))
               elm_object_disabled_set(pl->btn_down, false);
          }
        if ((pl->group->current_selected->common.type == RESOURCE2_TYPE_PROGRAM) &&
            (((Program2 *)pl->group->current_selected)->type == EDJE_ACTION_TYPE_SCRIPT))
           ui_menu_disable_set(ap.menu, MENU_WINDOW_MANAGER_SCRIPT, false);
        else
           ui_menu_disable_set(ap.menu, MENU_WINDOW_MANAGER_SCRIPT, true);

        TODO("handle part items here");
     }
   else
      ui_menu_disable_set(ap.menu, MENU_WINDOW_MANAGER_SCRIPT, false);
}

static void
_unselect_part(Part_List *pl)
{
   assert(pl != NULL);

   _unselect_internal(pl);

   evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_CHANGED, pl->group);
   elm_genlist_item_selected_set(elm_genlist_selected_item_get(pl->genlist), false);
}

static void
_unselected_cb(void *data,
               Evas_Object *o,
               void *event_info)
{
   Part_List *pl = data;
   Elm_Object_Item *glit = event_info;

   assert(pl != NULL);

   /* focusing genlist to trigger unfocus callbacks in property */
   elm_object_focus_set(o, true);

   _unselect_internal(pl);

   _enable_buttons(pl, glit);
   evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, NULL);
}

static void
_selected_cb(void *data,
             Evas_Object *o EINA_UNUSED,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Part_List *pl = data;
   State2 *state;

   assert(pl != NULL);

   _unselect_internal(pl);

   if ((glit == pl->parts_caption_item) ||
       (glit == pl->programs_caption_item) ||
       (glit == pl->data_caption_item))
     {
        evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, NULL);
     }
   else
     {
        elm_object_item_data_get(glit);
        pl->group->current_selected = (Resource2 *)elm_object_item_data_get(glit);
        if (pl->group->current_selected->common.type == RESOURCE2_TYPE_PART)
          {
             pl->part = (Part2 *)pl->group->current_selected;
             evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, pl->part);
#if HAVE_TIZEN
             Evas_Object *check = elm_object_item_part_content_get(glit, "elm.swallow.icon");
             if (check)
               elm_object_signal_emit(check, "selected", "eflete");
#endif
          }
        else if (pl->group->current_selected->common.type == RESOURCE2_TYPE_STATE)
          {
             pl->part = ((State2 *)pl->group->current_selected)->part;
             evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, pl->part);
             state = elm_object_item_data_get(glit);
             CRIT_ON_FAIL(editor_part_selected_state_set(pl->group->edit_object, NULL, false, true,
                                                         state->part->common.name,
                                                         state->common.name,
                                                         state->val));
          }
        else if (pl->group->current_selected->common.type == RESOURCE2_TYPE_ITEM)
          {
             pl->part = ((Part_Item2 *)pl->group->current_selected)->part;
             evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, pl->part);
          }
        else
          evas_object_smart_callback_call(pl->layout, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, NULL);
     }
   evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_CHANGED, pl->group);

   _enable_buttons(pl, glit);
}

static void
_on_part_name_changed(void *data,
                      Evas_Object *obj,
                      void *event_info EINA_UNUSED)
{
   Part_List *pl = data;

   assert(pl != NULL);

   if (resource_name_validator_status_get(pl->part_name_validator) != ELM_REG_NOERROR)
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, true);
       elm_object_signal_emit(obj, "validation,default,fail", "elm");
     }
   else
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, false);
       elm_object_signal_emit(obj, "validation,default,pass", "elm");
     }
}

static void
_state_validate(void *data,
                Evas_Object *obj,
                void *event_info EINA_UNUSED)
{
   Part_List *pl = data;
   const char *name;
   double val;

   assert(pl != NULL);
   assert(pl->part != NULL);

   name = elm_entry_entry_get(pl->popup.entry_name);
   val = elm_spinner_value_get(pl->popup.spinner_value);
   val = ((int) (val * 100)) / 100.0; /* only first two digets after point are used */

   if ((elm_validator_regexp_status_get(pl->name_validator) != ELM_REG_NOERROR) ||
       (edje_edit_state_exist(pl->group->edit_object, pl->part->common.name, name, val)))
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, true);
       elm_object_signal_emit(obj, "validation,default,fail", "elm");
     }
   else
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, false);
       elm_object_signal_emit(obj, "validation,default,pass", "elm");
     }
}

static void
_item_selected(void *data,
               Evas_Object *obj EINA_UNUSED,
               void *event_info)
{
   Part_List *pl = data;

   Combobox_Item *combo_item;
   combo_item = elm_object_item_data_get(event_info);
   if (combo_item)
     pl->popup.item_selected = combo_item;
   else
     pl->popup.item_selected = NULL;
}

static void
_item_validate(void *data,
               Evas_Object *obj EINA_UNUSED,
               void *event_info)
{
   Part_List *pl = data;
   const char *name;
   Part_Item2 *item;
   Eina_List *l;
   Combobox_Item *combo_item;

   assert(pl != NULL);
   assert(pl->part != NULL);

   combo_item = elm_object_item_data_get(event_info);
   if (combo_item)
     pl->popup.item_selected = combo_item;

   name = elm_entry_entry_get(pl->popup.entry_name);
   if (elm_validator_regexp_status_get(pl->name_validator) != ELM_REG_NOERROR)
     {
       elm_object_signal_emit(obj, "validation,default,fail", "elm");
       goto item_data_invalidated;
     }
   elm_object_signal_emit(obj, "validation,default,pass", "elm");

   if ((!pl->popup.item_selected) || (pl->popup.item_selected && (pl->popup.item_selected->data == NULL)) ||
       !strcmp(_("Select the name of the source group."), elm_object_text_get(pl->popup.combobox)))
     goto item_data_invalidated;

   EINA_LIST_FOREACH(pl->part->items, l, item)
     {
       if (item->common.name && !strcmp(item->common.name, name))
         goto item_data_invalidated;
     }

   popup_button_disabled_set(pl->popup_win, BTN_OK, false);
   return;

item_data_invalidated:
   popup_button_disabled_set(pl->popup_win, BTN_OK, true);
   return;
}

static void
_program_validate(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   Part_List *pl = data;

   assert(pl != NULL);

   if (resource_name_validator_status_get(pl->program_name_validator) != ELM_REG_NOERROR)
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, true);
       elm_object_signal_emit(obj, "validation,default,fail", "elm");
     }
   else
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, false);
       elm_object_signal_emit(obj, "validation,default,pass", "elm");
     }
}

static void
_popup_add_part_close_cb(void *data,
                         Evas_Object *obj EINA_UNUSED,
                         void *ei)
{
   Popup_Button pb = (Popup_Button)ei;
   if (pb != BTN_OK) return;

   Part_List *pl = data;
   const char *name, *copy_name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   if (resource_name_validator_status_get(pl->part_name_validator) != ELM_REG_NOERROR)
      return;

   if (pl->popup.copy_part)
     {
        name = elm_object_text_get(pl->popup.combobox_copy);
        copy_name = elm_entry_entry_get(pl->popup.entry_name);
        msg = eina_stringshare_printf(_("copied new part \"%s\" from \"%s\""), name, copy_name);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_part_copy(pl->group->edit_object, change, false, true, name, copy_name));
        pl->popup.copy_part = 0;
     }
   else
     {
        /* if part add was clicked and only name was typed,
           so there are no changes yet and part_type contain EDJE_PART_TYPE_NONE */
        if (pl->popup.part_type == 0)
          pl->popup.part_type = EDJE_PART_TYPE_RECTANGLE;
        name = elm_entry_entry_get(pl->popup.entry_name);
        msg = eina_stringshare_printf(_("added new part \"%s\""), name);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_part_add(pl->group->edit_object, change, false, true, name, pl->popup.part_type));
        pl->popup.part_type = EDJE_PART_TYPE_RECTANGLE; /* get that selected stuff down, next type RECT again */
     }

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_add(Evas_Object *obj, Part2 *part)
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
_part_selected_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info)
{
   Part_List *pl = data;
   Combobox_Item *item;
   Edje_Part_Type type;

   assert(pl != NULL);

   item = elm_object_item_data_get(event_info);
   pl->popup.copy_part = item->index;
   if (item->index != 0)
     {
        type = edje_edit_part_type_get(pl->group->edit_object, elm_object_text_get(pl->popup.combobox_copy));
        elm_object_text_set(pl->popup.combobox, part_type_text_get(type));
        elm_object_disabled_set(pl->popup.combobox, true);
     }
   else
     {
        elm_object_text_set(pl->popup.combobox, part_type_text_get(pl->popup.part_type));
        elm_object_disabled_set(pl->popup.combobox, false);
     }
}

static void
_state_selected_cb(void *data,
                   Evas_Object *obj EINA_UNUSED,
                   void *event_info)
{
   Part_List *pl = data;
   Combobox_Item *item;

   assert(pl != NULL);

   item = elm_object_item_data_get(event_info);
   pl->popup.state_selected = item;
}

static void
_program_selected_cb(void *data,
                     Evas_Object *obj EINA_UNUSED,
                     void *event_info)
{
   Part_List *pl = data;
   Combobox_Item *item;

   assert(pl != NULL);

   item = elm_object_item_data_get(event_info);
   pl->popup.program_selected = item->index;
}

static void
_type_selected_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info)
{
   Part_List *pl = data;
   Combobox_Item *item;

   assert(pl != NULL);

   item = elm_object_item_data_get(event_info);
   pl->popup.part_type = item->index;
}

static char *
_combobox_text_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part EINA_UNUSED)
{
   Combobox_Item *item = (Combobox_Item *)data;
   return strdup(item->data);
}

static void
_combobox_item_pressed_cb(void *data EINA_UNUSED, Evas_Object *obj,
                          void *event_info)
{
   Combobox_Item *item = elm_object_item_data_get(event_info);
   elm_object_text_set(obj, item->data);
   elm_combobox_hover_end(obj);
   elm_entry_cursor_end_set(obj);
}

static Evas_Object *
_add_part_content_get(void *data, Evas_Object *popup EINA_UNUSED, Evas_Object **to_focus)
{
   Combobox_Item *combobox_item;
   unsigned int i = 0;
   Eina_List *l;
   Part2 *part;

   Part_List *pl = (Part_List *)data;
   Evas_Object *box, *item;

   BOX_ADD(ap.win, box, false, false);
   elm_box_padding_set(box, 0, 10);

   LAYOUT_PROP_ADD(box, _("Part name"), "popup", "1swallow")
   ENTRY_ADD(box, pl->popup.entry_name, true);
   efl_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, pl->part_name_validator);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter the name of the new part"));
   resource_name_validator_list_set(pl->part_name_validator, &pl->group->parts, false);
   evas_object_smart_callback_add(pl->popup.entry_name, signals.elm.entry.changed, _on_part_name_changed, pl);
   evas_object_show(pl->popup.entry_name);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Part type"), "popup", "1swallow")
   COMBOBOX_ADD(item, pl->popup.combobox)
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_pressed,
                                  _combobox_item_pressed_cb, NULL);
   for (i = 0; part_types[i] != EDJE_PART_TYPE_NONE; i++)
     {
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->data = eina_stringshare_add(part_type_text_get(part_types[i]));
        combobox_item->index = part_types[i];
        elm_genlist_item_append(pl->popup.combobox, pl->popup.itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_object_text_set(pl->popup.combobox, part_type_text_get(part_types[0]));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_selected, _type_selected_cb, pl);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Part copy"), "popup", "1swallow")
   COMBOBOX_ADD(item, pl->popup.combobox_copy)
   evas_object_smart_callback_add(pl->popup.combobox_copy, signals.elm.combobox.item_pressed,
                                  _combobox_item_pressed_cb, NULL);
   combobox_item = mem_malloc(sizeof(Combobox_Item));
   combobox_item->index = 0;
   combobox_item->data = eina_stringshare_add(_("None"));
   elm_genlist_item_append(pl->popup.combobox_copy, pl->popup.itc,
                           combobox_item, NULL,
                           ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_object_text_set(pl->popup.combobox_copy, "None");
   i = 1;
   EINA_LIST_FOREACH(pl->group->parts, l, part)
     {
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i++;
        combobox_item->data = eina_stringshare_add(part->common.name);
        elm_genlist_item_append(pl->popup.combobox_copy, pl->popup.itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   evas_object_show(pl->popup.combobox_copy);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox_copy);
   evas_object_smart_callback_add(pl->popup.combobox_copy, signals.elm.combobox.item_selected, _part_selected_cb, pl);
   elm_box_pack_end(box, item);

   if (to_focus) *to_focus = pl->popup.entry_name;
   pl->popup.box = box;
   return box;
}

static void
_on_menu_add_part_clicked(void *data EINA_UNUSED,
                          Evas_Object *obj,
                          void *ei EINA_UNUSED)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_Stringshare *title;

   assert(pl != NULL);

   title = eina_stringshare_printf(_("Add New Part to Group \"%s\""), pl->group->common.name);
   pl->popup_win = popup_add(title, NULL,
                             BTN_OK | BTN_CANCEL,
                             _add_part_content_get,
                             pl);
   evas_object_smart_callback_add(pl->popup_win, POPUP_CLOSE_CB, _popup_add_part_close_cb, pl);
   popup_button_disabled_set(pl->popup_win, BTN_OK, true);

   eina_stringshare_del(title);
}

static void
_on_group_data_name_changed(void *data,
                            Evas_Object *obj EINA_UNUSED,
                            void *event_info EINA_UNUSED)
{
   Part_List *pl = data;

   assert(pl != NULL);

   if (resource_name_validator_status_get(pl->group_data_name_validator) != ELM_REG_NOERROR)
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, true);
       elm_object_signal_emit(obj, "validation,default,fail", "elm");
     }
   else
     {
       popup_button_disabled_set(pl->popup_win, BTN_OK, false);
       elm_object_signal_emit(obj, "validation,default,pass", "elm");
     }
}

static void
_popup_add_group_data_close_cb(void *data,
                               Evas_Object *obj EINA_UNUSED,
                               void *event_info)
{
   Popup_Button pb = (Popup_Button)event_info;
   if (pb != BTN_OK) return;

   Part_List *pl = data;
   const char *name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   name = elm_entry_entry_get(pl->popup.entry_name);
   msg = eina_stringshare_printf(_("added new data item \"%s\""), name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_group_data_add(pl->group->edit_object, change, false, true, name));

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

static Evas_Object *
_add_group_data_content_get(void *data, Evas_Object *popup EINA_UNUSED, Evas_Object **to_focus)
{
   Part_List *pl = (Part_List *) data;
   Evas_Object *box, *item;
   BOX_ADD(ap.win, box, false, false);

   LAYOUT_PROP_ADD(box, _("Data item name"), "popup", "1swallow")
   ENTRY_ADD(box, pl->popup.entry_name, true);
   efl_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, pl->group_data_name_validator);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter the name of the new group_data"));
   resource_name_validator_list_set(pl->group_data_name_validator, &pl->group->data_items, false);
   evas_object_smart_callback_add(pl->popup.entry_name, signals.elm.entry.changed, _on_group_data_name_changed, pl);
   evas_object_show(pl->popup.entry_name);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);
   elm_object_focus_set(pl->popup.entry_name, true);

   pl->popup.box = box;
   if (to_focus) *to_focus = pl->popup.entry_name;

   return pl->popup.box;
}

static void
_on_menu_add_group_data_clicked(void *data EINA_UNUSED,
                                Evas_Object *obj,
                                void *ei EINA_UNUSED)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   Eina_Stringshare *title;

   assert(pl != NULL);

   title = eina_stringshare_printf(_("Add New Data Item to Group \"%s\""), pl->group->common.name);
   pl->popup_win = popup_add(title, NULL,
                             BTN_OK | BTN_CANCEL,
                             _add_group_data_content_get, pl);
   evas_object_smart_callback_add(pl->popup_win, POPUP_CLOSE_CB, _popup_add_group_data_close_cb, pl);
   popup_button_disabled_set(pl->popup_win, BTN_OK, true);
   eina_stringshare_del(title);
}

static void
_popup_add_state_close_cb(void *data,
                          Evas_Object *obj EINA_UNUSED,
                          void *event_info)
{
   Popup_Button pb = (Popup_Button)event_info;
   if (pb != BTN_OK) return;

   Part_List *pl = data;
   const char *name, *name_from;
   double val, val_from;
   State2 *state_from = NULL;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(pl->part != NULL);

   if (elm_validator_regexp_status_get(pl->name_validator) != ELM_REG_NOERROR)
      return;

   name = elm_entry_entry_get(pl->popup.entry_name);
   val = elm_spinner_value_get(pl->popup.spinner_value);
   val = ((int) (val * 100)) / 100.0; /* only first two digets after point are used */

   if (pl->popup.state_selected->index == 0)
     {
        msg = eina_stringshare_printf(_("added new state \"%s\" %.2f"), name, val);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_state_add(pl->group->edit_object, change, false, true,
                                      pl->part->common.name, name, val));
     }
   else
     {
        state_name_split(pl->popup.state_selected->data, &name_from, &val_from);
        state_from = (State2 *)resource_manager_v_find(pl->part->states,
                                                       name_from,
                                                       val_from);
        msg = eina_stringshare_printf(_("added new state \"%s\" %.2f as copy of \"%s\" %.2f"),
                                      name, val, state_from->common.name, state_from->val);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_state_copy(pl->group->edit_object, change, false, true,
                                       pl->part->common.name,
                                       state_from->common.name, state_from->val,
                                       name, val));
     }

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_state_add(Evas_Object *obj, Part2 *part, State2 *state)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_Bool items_expanded = false;
   Elm_Object_Item *glit, *part_item;
   const Eina_List *l;

   assert(pl != NULL);
   assert(part != NULL);
   assert(state != NULL);

   part_item = _part_item_find(pl, state->part);
   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     {
        items_expanded = elm_genlist_item_expanded_get(eina_list_data_get(
              eina_list_last(elm_genlist_item_subitems_get(part_item))));
     }
   elm_genlist_item_expanded_set(part_item, false);
   elm_genlist_item_expanded_set(part_item, true);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     elm_genlist_item_expanded_set(eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(
              part_item))), items_expanded);

   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(part_item), l, glit)
     {
        if (elm_object_item_data_get(glit) == state)
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}

static Evas_Object *
_add_state_content_get(void *data, Evas_Object *popup EINA_UNUSED, Evas_Object **to_focus)
{
   Part_List *pl = (Part_List *)data;
   Evas_Object *box, *item;
   Eina_List *l;
   Eina_Stringshare *label;
   State2 *state = NULL;
   Combobox_Item *combobox_item;
   unsigned int i = 0;

   BOX_ADD(ap.win, box, false, false);
   elm_box_padding_set(box, 0, 10);

   LAYOUT_PROP_ADD(box, _("State name"), "popup", "1swallow");
   ENTRY_ADD(item, pl->popup.entry_name, true);
   efl_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE,
                         elm_validator_regexp_helper, pl->name_validator);
   evas_object_smart_callback_add(pl->popup.entry_name, signals.elm.entry.changed, _state_validate, pl);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter the name of the new state"));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Value"), "popup", "1swallow_subtext")
   SPINNER_ADD(item, pl->popup.spinner_value, 0.0, 1.0, 0.1, true);
   elm_object_style_set(pl->popup.spinner_value, "vertical");
   elm_spinner_label_format_set(pl->popup.spinner_value, "%1.2f");
   elm_object_part_text_set(item, "elm.subtext", _("Available values: 0.0 - 1.0"));
   evas_object_smart_callback_add(pl->popup.spinner_value, signals.elm.spinner.changed_user, _state_validate, pl);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.spinner_value);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Duplicate state"), "popup", "1swallow")

   COMBOBOX_ADD(item, pl->popup.combobox)
   elm_object_text_set(pl->popup.combobox, _("None"));
   combobox_item = mem_malloc(sizeof(Combobox_Item));
   combobox_item->index = 0;
   combobox_item->data = eina_stringshare_add(_("None"));
   pl->popup.state_selected = combobox_item;
   elm_genlist_item_append(pl->popup.combobox, pl->popup.itc,
                           combobox_item, NULL,
                           ELM_GENLIST_ITEM_NONE, NULL, NULL);
   i = 1;
   EINA_LIST_FOREACH(pl->part->states, l, state)
     {
        label = eina_stringshare_printf("%s %.2f", state->common.name, state->val);
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i++;
        combobox_item->data = eina_stringshare_add(label);
        elm_genlist_item_append(pl->popup.combobox, pl->popup.itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
        eina_stringshare_del(label);
     }
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_pressed,
                                  _combobox_item_pressed_cb, NULL);
   evas_object_show(pl->popup.combobox);
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_selected, _state_selected_cb, pl);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);
   elm_box_pack_end(box, item);

   if (to_focus) *to_focus = pl->popup.entry_name;
   pl->popup.box = box;
   return box;
}

static void
_on_menu_add_state_clicked(void *data EINA_UNUSED,
                           Evas_Object *obj,
                           void *ei EINA_UNUSED)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_Stringshare *title;
   assert(pl != NULL);
   assert(pl->part != NULL);

   title = eina_stringshare_printf(_("Add New State to Part \"%s\""), pl->part->common.name);
   pl->popup_win = popup_add(title, NULL,
                             BTN_OK | BTN_CANCEL,
                             _add_state_content_get, pl);
   evas_object_smart_callback_add(pl->popup_win, POPUP_CLOSE_CB, _popup_add_state_close_cb, pl);
   popup_button_disabled_set(pl->popup_win, BTN_OK, true);

   eina_stringshare_del(title);
}

static void
_popup_add_item_close_cb(void *data,
                         Evas_Object *obj EINA_UNUSED,
                         void *event_info)
{
   Popup_Button pb = (Popup_Button) event_info;
   if (pb != BTN_OK) return;

   Part_List *pl = data;
   const char *name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(pl->part != NULL);

   name = elm_entry_entry_get(pl->popup.entry_name);

   msg = eina_stringshare_printf(_("added new item \"%s\" to part \"%s\""), name, pl->part->common.name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_item_index_append(pl->group->edit_object,
                                              change,
                                              false,
                                              true,
                                              pl->part->common.name,
                                              name,
                                              pl->popup.item_selected->data,
                                              eina_list_count(pl->part->items)));
   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_item_add(Evas_Object *obj, Part2 *part, unsigned int index)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *part_glit;
   Elm_Object_Item *items_glit, *glit;
   const Eina_List *l;
   Part_Item2 *part_item;

   assert(pl != NULL);

   part_glit = _part_item_find(pl, part);

   assert(part_glit != NULL);

   part_item = (Part_Item2 *)resource_manager_id_find(part->items, index);
   elm_genlist_item_expanded_set(part_glit, true);
   items_glit = eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(part_glit)));
   elm_genlist_item_update(items_glit);
   elm_genlist_item_expanded_set(items_glit, false);
   elm_genlist_item_expanded_set(items_glit, true);

   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(items_glit), l, glit)
     {
        if (elm_object_item_data_get(glit) == part_item)
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}

static Evas_Object *
_add_item_content_get(void *data, Evas_Object *popup EINA_UNUSED, Evas_Object **to_focus)
{
   Part_List *pl = (Part_List *)data;
   Combobox_Item *combobox_item;
   Group2 *group;
   Evas_Object *box, *item;
   Eina_List *l;
   unsigned int i = 0;

   BOX_ADD(ap.win, box, false, false);
   elm_box_padding_set(box, 0, 10);

   LAYOUT_PROP_ADD(box, _("Name"), "popup", "1swallow")
   ENTRY_ADD(item, pl->popup.entry_name, true);
   efl_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE,
                         elm_validator_regexp_helper, pl->name_validator);
   evas_object_smart_callback_add(pl->popup.entry_name, signals.elm.entry.changed, _item_validate, pl);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter the name of the new item"));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Source group"), "popup", "1swallow")
   COMBOBOX_ADD(item, pl->popup.combobox)
   elm_object_text_set(pl->popup.combobox, _("Select the name of the source group."));

   EINA_LIST_FOREACH(ap.project->RM.groups, l, group)
     {
        TODO("Add checks for recursion");
        if (pl->part->group != group)
          {
             combobox_item = mem_malloc(sizeof(Combobox_Item));
             combobox_item->index = i++;
             combobox_item->data = eina_stringshare_add(group->common.name);
             elm_genlist_item_append(pl->popup.combobox, pl->popup.itc,
                                     combobox_item, NULL,
                                     ELM_GENLIST_ITEM_NONE, NULL, NULL);
          }
     }
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_pressed,
                                  _combobox_item_pressed_cb, NULL);
   evas_object_show(pl->popup.combobox);
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.dismissed, _item_validate, pl);
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_selected, _item_selected, pl);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);

   elm_box_pack_end(box, item);
   pl->popup.box = box;
   if (to_focus) *to_focus = pl->popup.entry_name;

   return pl->popup.box;
}

static void
_on_menu_add_item_clicked(void *data EINA_UNUSED,
                          Evas_Object *obj,
                          void *ei EINA_UNUSED)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_Stringshare *title;
   assert(pl != NULL);
   assert(pl->part != NULL);

   title = eina_stringshare_printf(_("Add New Item to Part \"%s\""), pl->part->common.name);
   pl->popup_win = popup_add(title, NULL,
                             BTN_OK | BTN_CANCEL,
                             _add_item_content_get, pl);
   evas_object_smart_callback_add(pl->popup_win, POPUP_CLOSE_CB, _popup_add_item_close_cb, pl);
   popup_button_disabled_set(pl->popup_win, BTN_OK, true);

   eina_stringshare_del(title);
}

static void
_popup_add_program_close_cb(void *data,
                            Evas_Object *obj EINA_UNUSED,
                            void *event_info)
{
   Popup_Button pb = (Popup_Button) event_info;
   if (pb != BTN_OK) return;

   Edje_Action_Type type = EDJE_ACTION_TYPE_NONE;
   Part_List *pl = data;
   const char *name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);

   if (resource_name_validator_status_get(pl->program_name_validator) != ELM_REG_NOERROR)
      return;

   switch (pl->popup.program_selected)
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
   CRIT_ON_FAIL(editor_program_add(pl->group->edit_object, change, false, true, name, type));

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

static Evas_Object *
_add_program_content_get(void *data, Evas_Object *popup EINA_UNUSED, Evas_Object **to_focus)
{
   Part_List *pl = (Part_List *) data;
   Evas_Object *box, *item;
   unsigned int i = 0;
   Combobox_Item *combobox_item;

   BOX_ADD(ap.win, box, false, false);
   elm_box_padding_set(box, 0, 10);

   LAYOUT_PROP_ADD(box, _("Program name"), "popup", "1swallow")
   ENTRY_ADD(item, pl->popup.entry_name, true);
   efl_event_callback_add(pl->popup.entry_name, ELM_ENTRY_EVENT_VALIDATE,
                         resource_name_validator_helper, pl->program_name_validator);
   resource_name_validator_list_set(pl->program_name_validator, &pl->group->programs, false);
   evas_object_smart_callback_add(pl->popup.entry_name, signals.elm.entry.changed, _program_validate, pl);
   elm_object_part_text_set(pl->popup.entry_name, "guide", _("Enter the name of the new program"));
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Action type"), "popup", "1swallow")
   COMBOBOX_ADD(item, pl->popup.combobox)
   for (i = 0; program_actions[i]; i++)
     {
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i;
        combobox_item->data = eina_stringshare_add(program_actions[i]);
        elm_genlist_item_append(pl->popup.combobox, pl->popup.itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_object_text_set(pl->popup.combobox, program_actions[0]);
   elm_object_part_content_set(item, "elm.swallow.content", pl->popup.combobox);
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_pressed,
                                  _combobox_item_pressed_cb, NULL);
   evas_object_smart_callback_add(pl->popup.combobox, signals.elm.combobox.item_selected, _program_selected_cb, pl);

   elm_box_pack_end(box, item);
   pl->popup.box = box;
   if (to_focus) *to_focus = pl->popup.entry_name;

   return pl->popup.box;
}

static void
_on_menu_add_program_clicked(void *data EINA_UNUSED,
                             Evas_Object *obj,
                             void *ei EINA_UNUSED)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Eina_Stringshare *title;

   assert(pl != NULL);

   title = eina_stringshare_add(_("Add New Program"));
   pl->popup_win = popup_add(title, NULL,
                             BTN_OK | BTN_CANCEL,
                             _add_program_content_get, pl);
   evas_object_smart_callback_add(pl->popup_win, POPUP_CLOSE_CB, _popup_add_program_close_cb, pl);
   popup_button_disabled_set(pl->popup_win, BTN_OK, true);

   eina_stringshare_del(title);
}

void
group_navigator_program_add(Evas_Object *obj, Eina_Stringshare *program)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit;
   const Eina_List *l;
   Resource2 *res;

   assert(pl != NULL);
   assert(program != NULL);

   elm_genlist_item_expanded_set(pl->programs_caption_item, false);
   elm_genlist_item_expanded_set(pl->programs_caption_item, true);
   elm_genlist_item_update(pl->programs_caption_item);
   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(pl->programs_caption_item), l, glit)
     {
        res = elm_object_item_data_get(glit);
        if (res->common.name == program) /* comparing stringshares */
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
   Resource2 *program;

   assert(pl != NULL);
   assert(glit != NULL);

   program = elm_object_item_data_get(glit);

   assert(program != NULL);

   msg = eina_stringshare_printf(_("deleted program \"%s\""), program->common.name);
   change = change_add(msg);
   eina_stringshare_del(msg);

   eina_stringshare_ref(program->common.name);
   CRIT_ON_FAIL(editor_program_del(pl->group->edit_object, change, false, true, program->common.name));
   eina_stringshare_del(program->common.name);

   history_change_add(pl->group->history, change);
}

void
group_navigator_program_del(Evas_Object *obj, Program2 *program)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *program_glit, *to_select;

   assert(pl != NULL);
   assert(program != NULL);

   program_glit = _program_glit_find(pl, program);

   to_select = elm_genlist_item_next_get(program_glit);

   if ((to_select == NULL) || (elm_genlist_item_item_class_get(to_select) != pl->itc_program))
     to_select = elm_genlist_item_prev_get(program_glit);
   if ((to_select == NULL) || (elm_genlist_item_item_class_get(to_select) != pl->itc_program))
     to_select = pl->programs_caption_item;

   elm_object_item_del(program_glit);
   elm_genlist_item_update(pl->programs_caption_item);
   elm_genlist_item_selected_set(to_select, true);
}

void
group_navigator_group_data_add(Evas_Object *obj, Eina_Stringshare *group_data)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit;
   const Eina_List *l;
   Resource2 *res;

   assert(pl != NULL);
   assert(group_data != NULL);

   elm_genlist_item_expanded_set(pl->data_caption_item, false);
   elm_genlist_item_expanded_set(pl->data_caption_item, true);
   elm_genlist_item_update(pl->data_caption_item);
   EINA_LIST_FOREACH(elm_genlist_item_subitems_get(pl->data_caption_item), l, glit)
     {
        res = elm_object_item_data_get(glit);
        if (res->common.name == group_data) /* comparing stringshares */
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}

static void
_group_data_del(Part_List *pl,
                Elm_Object_Item *glit)
{
   Eina_Stringshare *msg;
   Change *change;
   Resource2 *group_data;

   assert(pl != NULL);
   assert(glit != NULL);

   group_data = elm_object_item_data_get(glit);

   assert(group_data != NULL);

   msg = eina_stringshare_printf(_("deleted group_data \"%s\""), group_data->common.name);
   change = change_add(msg);
   eina_stringshare_del(msg);

   eina_stringshare_ref(group_data->common.name);
   CRIT_ON_FAIL(editor_group_data_del(pl->group->edit_object, change, false, true, group_data->common.name));
   eina_stringshare_del(group_data->common.name);

   history_change_add(pl->group->history, change);
}

void
group_navigator_group_data_del(Evas_Object *obj, Resource2 *data)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *data_glit, *to_select;

   assert(pl != NULL);
   assert(data != NULL);

   data_glit = _group_data_item_find(pl, data);

   to_select = elm_genlist_item_next_get(data_glit);

   if ((to_select == NULL) || (elm_genlist_item_item_class_get(to_select) != pl->itc_group_data))
     to_select = elm_genlist_item_prev_get(data_glit);
   if ((to_select == NULL) || (elm_genlist_item_item_class_get(to_select) != pl->itc_group_data))
     to_select = pl->data_caption_item;

   elm_object_item_del(data_glit);
   elm_genlist_item_update(pl->data_caption_item);
   elm_genlist_item_selected_set(to_select, true);
}

static void
_on_btn_plus_clicked(void *data,
                     Evas_Object *obj EINA_UNUSED,
                     void *ei EINA_UNUSED)
{
   Part_List *pl = data;
   Evas_Coord x, y, h;

   assert(pl != NULL);

#if !HAVE_TIZEN
   evas_object_geometry_get(obj, &x, &y, NULL, &h);
#else
   evas_object_geometry_get(pl->btn_down, &x, &y, NULL, &h);
#endif

   elm_menu_move(pl->menu, x, y + h);
   evas_object_show(pl->menu);
}

static void
_part_del(Part_List *pl,
          Elm_Object_Item *glit)
{
   Part2 *part;
   Eina_Stringshare *part_name;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);

   part = elm_object_item_data_get(glit);
   _unselect_internal(pl);

   part_name = eina_stringshare_add(part->common.name);
   msg = eina_stringshare_printf(_("deleted part \"%s\""), part_name);
   change = change_add(msg);
   CRIT_ON_FAIL(editor_part_del(pl->group->edit_object, change, false, true, part_name));
   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
   eina_stringshare_del(part_name);
}

static Elm_Object_Item *
_part_item_find(Part_List *pl, Part2 *part)
{
   Elm_Object_Item *part_item;
   const Eina_List *part_items;
   Part2 *pr;

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

static Elm_Object_Item *
_program_glit_find(Part_List *pl, Program2 *program)
{
   Elm_Object_Item *program_glit;
   const Eina_List *program_glits;
   Program2 *pr;

   assert(pl != NULL);
   assert(program != NULL);

   elm_genlist_item_expanded_set(pl->programs_caption_item, true);
   program_glits = elm_genlist_item_subitems_get(pl->programs_caption_item);

   program_glit = eina_list_data_get(program_glits);
   pr = elm_object_item_data_get(program_glit);
   while (pr != program)
     {
        program_glits = eina_list_next(program_glits);
        program_glit = eina_list_data_get(program_glits);
        pr = elm_object_item_data_get(program_glit);

        assert(pr != NULL);
     }
   assert(program_glit != NULL);
   return program_glit;
}

static Elm_Object_Item *
_group_data_item_find(Part_List *pl, Resource2 *group_data)
{
   Elm_Object_Item *group_data_item;
   const Eina_List *group_data_items;
   Resource2 *pr;

   assert(pl != NULL);
   assert(group_data != NULL);

   elm_genlist_item_expanded_set(pl->data_caption_item, true);
   group_data_items = elm_genlist_item_subitems_get(pl->data_caption_item);

   group_data_item = eina_list_data_get(group_data_items);
   pr = elm_object_item_data_get(group_data_item);
   while (pr != group_data)
     {
        group_data_items = eina_list_next(group_data_items);
        group_data_item = eina_list_data_get(group_data_items);
        pr = elm_object_item_data_get(group_data_item);

        assert(pr != NULL);
     }
   assert(group_data_item != NULL);
   return group_data_item;
}

static void
_select_next_part(Part_List *pl, Eina_Bool reverse)
{
   Elm_Object_Item *part_item, *selected_part_item;
   const Eina_List *part_items, *l = NULL;

   assert(pl != NULL);

   part_items = elm_genlist_item_subitems_get(pl->parts_caption_item);
   selected_part_item = (pl->part) ? _part_item_find(pl, pl->part) : NULL;
   if (selected_part_item)
     l = eina_list_data_find_list(part_items, selected_part_item);
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
group_navigator_part_del(Evas_Object *obj, Part2 *part)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *part_item, *to_select;

   assert(pl != NULL);
   assert(part != NULL);

   part_item = _part_item_find(pl, part);

   to_select = elm_genlist_item_next_get(part_item);

   while ((to_select != NULL) && (elm_genlist_item_item_class_get(to_select) != pl->itc_part))
     to_select = elm_genlist_item_next_get(to_select);
   if (to_select == NULL)
     {
        to_select = elm_genlist_item_prev_get(part_item);
        while ((to_select != NULL) && (elm_genlist_item_item_class_get(to_select) != pl->itc_part))
          to_select = elm_genlist_item_prev_get(to_select);
     }
   if (to_select == NULL)
     to_select = pl->parts_caption_item;

   if (part == pl->part)
     _unselect_part(pl);

   elm_object_item_del(part_item);
   elm_genlist_item_update(pl->parts_caption_item);
   elm_genlist_item_selected_set(to_select, true);
}

static void
_state_del(Part_List *pl,
           Elm_Object_Item *glit)
{
   State2 *state;
   Eina_Stringshare *part_name, *state_name;
   double state_val;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(pl->part != NULL);
   assert(glit != NULL);

   state = elm_object_item_data_get(glit);

   assert(state != NULL);
   assert((strcmp(state->common.name, "default") || (state->val != 0))); /* default state can't be deleted */

   msg = eina_stringshare_printf(_("deleted state \"%s\" %.2f"), state->common.name, state->val);
   change = change_add(msg);
   eina_stringshare_del(msg);
   part_name = eina_stringshare_ref(state->part->common.name);
   state_name = eina_stringshare_ref(state->common.name);
   state_val = state->val;
   TODO("recheck string args logic");
   CRIT_ON_FAIL(editor_state_del(pl->group->edit_object, change, false, true, part_name, state_name, state_val));
   eina_stringshare_del(part_name);
   eina_stringshare_del(state_name);
   history_change_add(pl->group->history, change);
}

void
group_navigator_part_state_del(Evas_Object *obj, Part2 *part EINA_UNUSED, State2 *state)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit;
   Elm_Object_Item *part_item;
   Elm_Object_Item *to_select;
   const Eina_List *subitems;
   const Eina_List *l;

   assert(pl != NULL);
   assert(state != NULL);

   part_item = _part_item_find(pl, part);
   elm_genlist_item_expanded_set(part_item, true);

   subitems = elm_genlist_item_subitems_get(part_item);
   /* find state's genlist item */
   EINA_LIST_FOREACH(subitems, l, glit)
     {
        if (elm_object_item_data_get(glit) == state)
          break;
     }
   assert(glit != NULL);

   to_select = elm_genlist_item_next_get(glit);
   if ((to_select == NULL) ||
       ((elm_genlist_item_item_class_get(to_select) != pl->itc_state) &&
        (elm_genlist_item_item_class_get(to_select) != pl->itc_state_selected)))
     to_select = elm_genlist_item_prev_get(glit);

   /* there should be at least default state */
   assert(to_select != NULL);
   elm_genlist_item_selected_set(to_select, true);
   elm_object_item_del(glit);
}

static void
_item_del(Part_List *pl,
          Elm_Object_Item *glit)
{
   Part_Item2 *item;
   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);
   assert(pl->part != NULL);

   item = elm_object_item_data_get(glit);

   assert(item != NULL);

   msg = eina_stringshare_printf(_("deleted item \"%s\""), item->common.name);
   change = change_add(msg);
   eina_stringshare_del(msg);

   CRIT_ON_FAIL(editor_part_item_index_del(pl->group->edit_object, change, false, true,  pl->part->common.name, item->common.id));

   history_change_add(pl->group->history, change);
}

void
group_navigator_part_item_del(Evas_Object *obj, Part_Item2 *item)
{
   Elm_Object_Item *part_item, *to_select;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *items_glit, *item_glit;
   Part_Item2 *it;
   const Eina_List *part_items;

   assert(pl != NULL);
   assert(item != NULL);

   part_item = _part_item_find(pl, item->part);

   assert(part_item != NULL);

   elm_genlist_item_expanded_set(part_item, true);
   items_glit = eina_list_data_get(eina_list_last(elm_genlist_item_subitems_get(part_item)));
   elm_genlist_item_expanded_set(items_glit, true);

   part_items = elm_genlist_item_subitems_get(items_glit);
   item_glit = eina_list_data_get(part_items);
   it = elm_object_item_data_get(item_glit);
   while (it != item)
     {
        part_items = eina_list_next(part_items);
        item_glit = eina_list_data_get(part_items);
        it = elm_object_item_data_get(item_glit);
     }
   assert(item_glit != NULL);
   while (eina_list_next(part_items))
     {
        part_items = eina_list_next(part_items);
        elm_genlist_item_update(eina_list_data_get(part_items));
     }

   to_select = elm_genlist_item_next_get(item_glit);

   if ((to_select == NULL) || (elm_genlist_item_item_class_get(to_select) != pl->itc_item))
     to_select = elm_genlist_item_prev_get(item_glit);
   if ((to_select == NULL) || (elm_genlist_item_item_class_get(to_select) != pl->itc_item))
     to_select = items_glit;

   elm_object_item_del(item_glit);
   elm_genlist_item_update(items_glit);
   elm_genlist_item_selected_set(to_select, true);
}

static void
_on_btn_minus_clicked(void *data,
                      Evas_Object *obj EINA_UNUSED,
                      void *ei EINA_UNUSED)
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
   else if (itc == pl->itc_group_data)
     _group_data_del(pl, glit);

   TODO("Check if we still need this")
   /* Need to save pl->group->edit_object, since we changed it */
   CRIT_ON_FAIL(editor_save_all(ap.project->global_object));
   TODO("Remove this line once edje_edit_image_add would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;
}

static void
_part_restack(Part_List *pl, Elm_Object_Item *glit, Eina_Bool move_up)
{
   Part2 *part, *rel_part = NULL;
   Eina_List *part_list_node = NULL;
   Eina_List *rel_part_list_node = NULL;

   Eina_Stringshare *msg;
   Change *change;

   assert(pl != NULL);
   assert(glit != NULL);

   part = elm_object_item_data_get(glit);
   part_list_node = eina_list_data_find_list(pl->group->parts, part);

   assert(part_list_node != NULL);

   if (move_up)
     {
       rel_part_list_node = eina_list_prev(part_list_node);
       rel_part = eina_list_data_get(rel_part_list_node);
     }
   else
     {
        rel_part_list_node = eina_list_next(part_list_node);
        rel_part = eina_list_data_get(rel_part_list_node);
        Part2 *part_tmp = rel_part;
        rel_part = part;
        part = part_tmp;
     }

   if (rel_part)
      msg = eina_stringshare_printf(_("part \"%s\" placed below part \"%s\" in the stack"), part->common.name, rel_part->common.name);
   else
      msg = eina_stringshare_printf(_("part \"%s\" restacked to the top of the stack"), part->common.name);
   change = change_add(msg);

   CRIT_ON_FAIL(editor_part_restack(pl->group->edit_object, change, false, true,
                                    part->common.name,
                                    (rel_part) ? rel_part->common.name : NULL));
   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_restack(Evas_Object *obj, Part2 *part, Part2 *rel_part)
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
   group_navigator_select(pl->layout, (Resource2 *)part);
}

static void
_part_item_restack(Part_List *pl, Elm_Object_Item *glit, Eina_Bool move_up)
{
   Eina_Stringshare *msg;
   Change *change;
   Part_Item2 *part_item;

   assert(pl != NULL);
   assert(pl->part != NULL);
   assert(glit != NULL);

   part_item = elm_object_item_data_get(glit);

   if (move_up)
     {
        msg = eina_stringshare_printf(_("part item \"%s\" restacked BELOW"),
                                      part_item->common.name);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_part_item_index_restack_below(pl->group->edit_object, change, false, true,
                                                          pl->part->common.name,
                                                          part_item->common.id));
     }
   else
     {
        msg = eina_stringshare_printf(_("part item \"%s\" restacked ABOVE"),
                                      part_item->common.name);
        change = change_add(msg);
        CRIT_ON_FAIL(editor_part_item_index_restack_above(pl->group->edit_object, change, false, true,
                                                          pl->part->common.name,
                                                          part_item->common.id));
     }

   history_change_add(pl->group->history, change);
   eina_stringshare_del(msg);
}

void
group_navigator_part_item_restack(Evas_Object *obj,
                                  Part2 *part,
                                  unsigned int item_index,
                                  Eina_Bool move_up EINA_UNUSED)
{
   Elm_Object_Item *part_glit;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *glit, *items_glit;
   const Eina_List *l, *subitems;
   Part_Item2 *part_item;

   assert(pl != NULL);
   assert(part != NULL);

   part_glit = _part_item_find(pl, part);
   assert(part_glit != NULL);

   if (move_up)
     part_item = (Part_Item2 *)resource_manager_id_find(part->items, item_index - 1);
   else
     part_item = (Part_Item2 *)resource_manager_id_find(part->items, item_index + 1);

   elm_genlist_item_expanded_set(part_glit, true);

   subitems = elm_genlist_item_subitems_get(part_glit);
   items_glit = eina_list_data_get(eina_list_last(subitems));
   elm_genlist_item_expanded_set(items_glit, false);
   elm_genlist_item_expanded_set(items_glit, true);

   subitems = elm_genlist_item_subitems_get(items_glit);
   EINA_LIST_FOREACH(subitems, l, glit)
     {
        if (elm_object_item_data_get(glit) == part_item)
          {
             elm_genlist_item_selected_set(glit, true);
             break;
          }
     }
}


static void
_on_btn_down_clicked(void *data,
                     Evas_Object *obj EINA_UNUSED,
                     void *ei EINA_UNUSED)
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
   elm_genlist_item_selected_set(glit, true);
}

static void
_on_btn_up_clicked(void *data,
                   Evas_Object *obj EINA_UNUSED,
                   void *ei EINA_UNUSED)
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
   elm_genlist_item_selected_set(glit, true);
}

static void
_combobox_item_del(void *data,
                   Evas_Object *obj EINA_UNUSED)
{
   Combobox_Item *item = (Combobox_Item *)data;
   eina_stringshare_del(item->data);
   free(item);
}

static void
_group_navigator_del(void *data,
                     Evas *e EINA_UNUSED,
                     Evas_Object *obj EINA_UNUSED,
                     void *event_info EINA_UNUSED)
{
   Part_List *pl = (Part_List *)data;

   elm_genlist_item_class_free(pl->itc_part);
   elm_genlist_item_class_free(pl->itc_caption);
   elm_genlist_item_class_free(pl->itc_group_data);
   elm_genlist_item_class_free(pl->itc_program);
   elm_genlist_item_class_free(pl->itc_item);
   elm_genlist_item_class_free(pl->itc_item_caption);
   elm_genlist_item_class_free(pl->itc_state);
   elm_genlist_item_class_free(pl->itc_state_selected);
   elm_genlist_item_class_free(pl->popup.itc);

   evas_object_data_del(pl->layout, GROUP_NAVIGATOR_DATA);
   evas_object_data_del(pl->menu, GROUP_NAVIGATOR_DATA);
   evas_object_data_del(pl->genlist, GROUP_NAVIGATOR_DATA);

   resource_name_validator_free(pl->part_name_validator);
   resource_name_validator_free(pl->program_name_validator);
   resource_name_validator_free(pl->group_data_name_validator);
   elm_validator_regexp_free(pl->name_validator);

   pl->group = NULL;

   free(pl);

}

Evas_Object *
group_navigator_add(Evas_Object *parent, Group2 *group)
{
   Part_List *pl;
   Elm_Object_Item *menu_item;

   assert(group != NULL);
   assert(parent != NULL);

   pl = mem_calloc(1, sizeof(Part_List));
   pl->layout = elm_layout_add(parent);
   elm_layout_theme_set(pl->layout, "layout", "navigator", "default");
   evas_object_show(pl->layout);

#ifdef HAVE_TIZEN
   Evas_Object *icon;
   IMAGE_ADD_NEW(pl->layout, icon, "icon", "group_navigator");
   elm_object_part_content_set(pl->layout, "elm.swallow.icon", icon);
#endif

   evas_object_data_set(pl->layout, GROUP_NAVIGATOR_DATA, pl);

   pl->group = group;

   BTN_ADD(pl->layout, pl->btn_add, "elm.swallow.btn1", "plus");
   evas_object_smart_callback_add(pl->btn_add, signals.elm.button.clicked, _on_btn_plus_clicked, pl);
   BTN_ADD(pl->layout, pl->btn_del, "elm.swallow.btn0", "minus");
   evas_object_smart_callback_add(pl->btn_del, signals.elm.button.clicked, _on_btn_minus_clicked, pl);
   BTN_ADD(pl->layout, pl->btn_down, "elm.swallow.btn3", "down");
   evas_object_smart_callback_add(pl->btn_down, signals.elm.button.clicked, _on_btn_down_clicked, pl);
   BTN_ADD(pl->layout, pl->btn_up, "elm.swallow.btn2", "up");
   evas_object_smart_callback_add(pl->btn_up, signals.elm.button.clicked, _on_btn_up_clicked, pl);

   elm_object_disabled_set(pl->btn_del, true);
   elm_object_disabled_set(pl->btn_down, true);
   elm_object_disabled_set(pl->btn_up, true);

   pl->itc_part = elm_genlist_item_class_new();
   pl->itc_part->item_style = "part";
   pl->itc_part->func.text_get = _resource_label_get;
   pl->itc_part->func.content_get = _part_content_get;

   pl->itc_state = elm_genlist_item_class_new();
   pl->itc_state->item_style = "state";
   pl->itc_state->func.text_get = _state_resource_label_get;

   pl->itc_state_selected = elm_genlist_item_class_new();
   pl->itc_state_selected->item_style = "state_selected";
   pl->itc_state_selected->func.text_get = _state_resource_label_get;

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

   pl->itc_group_data = elm_genlist_item_class_new();
   pl->itc_group_data->item_style = "program";
   pl->itc_group_data->func.text_get = _resource_label_get;

   pl->popup.itc = elm_genlist_item_class_new();
   pl->popup.itc->item_style = "default";
   pl->popup.itc->func.text_get = _combobox_text_get;
   pl->popup.itc->func.del = _combobox_item_del;

   pl->genlist = elm_genlist_add(pl->layout);
   elm_genlist_homogeneous_set(pl->genlist, true);
   elm_scroller_policy_set(pl->genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(pl->genlist);
   elm_object_content_set(pl->layout, pl->genlist);
   evas_object_smart_callback_add(pl->genlist, signals.elm.genlist.expand_request, _expand_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, signals.elm.genlist.contract_request, _contract_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, signals.elm.genlist.expanded, _expanded_cb, pl);
   evas_object_smart_callback_add(pl->genlist, signals.elm.genlist.contracted, _contracted_cb, pl);
   evas_object_smart_callback_add(pl->genlist, signals.elm.genlist.selected, _selected_cb, pl);
   evas_object_smart_callback_add(pl->genlist, signals.elm.genlist.unselected, _unselected_cb, pl);
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
   pl->data_caption_item = elm_genlist_item_append(pl->genlist,
                                                    pl->itc_caption,
                                                    &group->data_items,
                                                    NULL,
                                                    ELM_GENLIST_ITEM_TREE,
                                                    NULL,
                                                    NULL);

   elm_object_text_set(pl->layout, pl->group->common.name);

   pl->menu = elm_menu_add(ap.win);
   evas_object_data_set(pl->menu, GROUP_NAVIGATOR_DATA, pl);

   menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Part"), _on_menu_add_part_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(menu_item), "elm.shortcut", "q");
#if !HAVE_TIZEN
   elm_menu_item_separator_add(pl->menu, NULL);
#endif

   pl->add_state_menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("State"), _on_menu_add_state_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(pl->add_state_menu_item), "elm.shortcut", "w");
   elm_object_item_disabled_set(pl->add_state_menu_item, true);
   ui_menu_disable_set(ap.menu, MENU_EDIT_STATE_ADD, true);
   pl->add_part_item_menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Item"), _on_menu_add_item_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(pl->add_part_item_menu_item), "elm.shortcut", "e");
   elm_object_item_disabled_set(pl->add_part_item_menu_item, true);
   ui_menu_disable_set(ap.menu, MENU_EDIT_ITEM_ADD, true);
#if !HAVE_TIZEN
   elm_menu_item_separator_add(pl->menu, NULL);
#endif

   menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Program"), _on_menu_add_program_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(menu_item), "elm.shortcut", "r");
#if !HAVE_TIZEN
   elm_menu_item_separator_add(pl->menu, NULL);
#endif

   menu_item = elm_menu_item_add(pl->menu, NULL, NULL, _("Data item"), _on_menu_add_group_data_clicked, NULL);
   elm_object_part_text_set(elm_menu_item_object_get(menu_item), "elm.shortcut", "t");

   pl->name_validator = elm_validator_regexp_new(PART_NAME_REGEX, NULL);
   pl->part_name_validator = resource_name_validator_new(PART_NAME_REGEX, NULL);
   pl->group_data_name_validator = resource_name_validator_new(PART_NAME_REGEX, NULL);
   pl->program_name_validator = resource_name_validator_new(NAME_REGEX, NULL);

   if (group->main_group)
     elm_object_disabled_set(pl->layout, true);
   else
     {
        elm_genlist_item_expanded_set(pl->data_caption_item, true);
        elm_genlist_item_expanded_set(pl->programs_caption_item, true);
        elm_genlist_item_expanded_set(pl->parts_caption_item, true);

        elm_genlist_item_bring_in(pl->parts_caption_item, ELM_GENLIST_ITEM_SCROLLTO_TOP);
     }

   evas_object_event_callback_add(pl->layout, EVAS_CALLBACK_DEL, _group_navigator_del, pl);

   TODO("Add deletion callback and free resources");
   return pl->layout;
}

Evas_Object *
group_navigator_dummy_get(void)
{
   Evas_Object *btn, *genlist;

   if (dummy_navi) goto exit;

   dummy_navi = elm_layout_add(ap.win);
   elm_layout_theme_set(dummy_navi, "layout", "navigator", "default");
   BTN_ADD(dummy_navi, btn, "elm.swallow.btn1", "plus");
   BTN_ADD(dummy_navi, btn, "elm.swallow.btn0", "minus");
   BTN_ADD(dummy_navi, btn, "elm.swallow.btn3", "down");
   BTN_ADD(dummy_navi, btn, "elm.swallow.btn2", "up");

   genlist = elm_genlist_add(dummy_navi);
   elm_layout_content_set(dummy_navi, NULL, genlist);

   elm_object_disabled_set(dummy_navi, true);
exit:
   evas_object_show(dummy_navi);
   return dummy_navi;
}

void
group_navigator_select(Evas_Object *obj, Resource2 *res)
{
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Elm_Object_Item *part_item;

   assert(pl != NULL);

   if (res)
     {
        assert((res->common.type == RESOURCE2_TYPE_STATE) ||
               (res->common.type == RESOURCE2_TYPE_PART));

        TODO("add state selection to groupview");
        /* remove after adding state selection to groupview */
        if (res == elm_object_item_data_get(elm_genlist_selected_item_get(pl->genlist)))
          group_navigator_part_state_select(obj, ((Part2 *)res)->current_state);
        else
          {
             part_item = _part_item_find(pl, (Part2 *)res);
             elm_genlist_item_selected_set(part_item, true);
          }

      /* uncomment after adding state selection to groupview */
#if 0
        if (res->common.type == RESOURCE2_TYPE_PART)
          {
             part_item = _part_item_find(pl, (Part2 *)res);
             elm_genlist_item_selected_set(part_item, true);
          }
        else
          group_navigator_part_state_select(obj, (State2 *)res);
#endif
     }
   else
     _unselect_part(pl);
}

void
group_navigator_part_update(Evas_Object *obj, Part2 *part)
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
group_navigator_group_data_update(Evas_Object *obj, Resource2 *group_data)
{
   Elm_Object_Item *group_data_item;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);

   assert(pl != NULL);
   assert(group_data != NULL);

   if (elm_genlist_item_expanded_get(pl->data_caption_item))
     {
        group_data_item = _group_data_item_find(pl, group_data);
        assert(group_data_item != NULL);

        elm_genlist_item_update(group_data_item);
     }
}

void
group_navigator_add_part_request(Evas_Object *obj)
{
   _on_menu_add_part_clicked(NULL, obj, NULL);
}

void
group_navigator_add_group_data_request(Evas_Object *obj)
{
   _on_menu_add_group_data_clicked(NULL, obj, NULL);
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
   State2 *state;
   Eina_List *l;

   if (pl->part != NULL)
     {
        if (pl->part == elm_object_item_data_get(elm_genlist_selected_item_get(pl->genlist)))
          group_navigator_part_state_select(obj, pl->part->current_state);
        else
          {
             if (eina_list_count(pl->part->states) > 1)
               {
                  l = eina_list_data_find_list(pl->part->states, pl->part->current_state);

                  assert(l != NULL);

                  if (eina_list_next(l) != NULL)
                    state = eina_list_data_get(eina_list_next(l));
                  else
                    state = eina_list_data_get(pl->part->states);

                  CRIT_ON_FAIL(editor_part_selected_state_set(pl->group->edit_object, NULL, false, true,
                                                              state->part->common.name,
                                                              state->common.name,
                                                              state->val));
               }
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
   Elm_Object_Item *part_item;
   Part_List *pl = evas_object_data_get(obj, GROUP_NAVIGATOR_DATA);
   Evas_Object *eye;

   assert(pl != NULL);

   if (pl->part != NULL)
     {
        part_item = _part_item_find(pl, pl->part);

        assert(part_item != NULL);

        eye = elm_object_item_part_content_get(part_item, "elm.swallow.icon");
        if (eye) elm_check_state_set(eye, !elm_check_state_get(eye));
        /* elementary not call callback if we change the check state, while it
         * not fixed, we need this small hack */
        _on_eye_clicked(pl->part, obj, NULL);
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
