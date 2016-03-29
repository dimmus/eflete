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
#include "demo_group.h"
#include "main_window.h"
#include "history.h"
#include "project_manager.h"

#define DEMO_GROUP_DATA "demo_group_data"

typedef struct
{
   Group *group;

   Evas_Object *layout;
   Evas_Object *genlist;

   Elm_Object_Item *selected_part_item;
   Elm_Object_Item *it_swallow, *it_text, *it_signal;

   Eina_List *swallow_list;
   Eina_List *text_list;
   Eina_List *signal_list;
} Part_Demo_List;

static Elm_Genlist_Item_Class *itc_swallow, *itc_text, *itc_signal;
static Elm_Genlist_Item_Class *itc_part;
static Elm_Genlist_Item_Class *itc_part_selected;
static Elm_Genlist_Item_Class *itc_signals;

static void
_program_item_del(Elm_Object_Item *pl, Demo_Signal *part);

#define CAPTION_GENLIST_ITEM_TEXT_GET(TYPE, TITLE) \
static char * \
_##TYPE##_label_cb(void *data, \
                   Evas_Object *obj __UNUSED__, \
                   const char *part) \
{ \
   assert(data != NULL); \
   char buf[5]; /* I'm really think that 5 symbol for parts count enough */ \
   Part_Demo_List *pl = (Part_Demo_List *)data; \
   if (!strcmp(part, "elm.text")) \
     return strdup(TITLE); \
   if (!strcmp(part, "elm.text.end")) \
     { \
        snprintf(buf, 5, "%d", eina_list_count(pl->TYPE##_list)); \
        return strdup(buf); \
     } \
   return NULL; \
}
CAPTION_GENLIST_ITEM_TEXT_GET(swallow, "Swallows")
CAPTION_GENLIST_ITEM_TEXT_GET(text, "Texts")
CAPTION_GENLIST_ITEM_TEXT_GET(signal, "Signals")

static char *
_part_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *pr __UNUSED__)
{
   Part *part = data;

   assert(part != NULL);
   assert(part->name != NULL);

   return strdup(part->name);
}
static char *
_sig_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *pr __UNUSED__)
{
   Demo_Signal *prog = data;

   assert(prog != NULL);
   assert(prog->prog_name != NULL);

   return strdup(prog->prog_name);
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
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}
static void
_expanded_cb(void *data,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Part_Demo_List *pl = data;
   Eina_List *l;
   Demo_Part *part;
   Demo_Signal *sig;

   assert(pl != NULL);

   if (glit == pl->it_text)
     {
        EINA_LIST_FOREACH(pl->text_list, l, part)
          {
             elm_genlist_item_append(pl->genlist,
                                     itc_part,
                                     part,
                                     pl->it_text,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);
          }
     }

   if (glit == pl->it_swallow)
     {
        EINA_LIST_FOREACH(pl->swallow_list, l, part)
          {
             elm_genlist_item_append(pl->genlist,
                                     itc_part,
                                     part,
                                     pl->it_swallow,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);
          }
     }

   if (glit == pl->it_signal)
     {
        EINA_LIST_FOREACH(pl->signal_list, l, sig)
          {
             elm_genlist_item_append(pl->genlist,
                                     itc_signals,
                                     sig,
                                     pl->it_signal,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);
          }
     }
}
static void
_selected_cb(void *data,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info, *plit;
   Part_Demo_List *pl = data;
   Demo_Signal *signal = NULL;
   Demo_Part *part = NULL;

   plit = elm_genlist_item_parent_get(glit);
   if (plit == pl->it_signal)
     signal = (Demo_Signal *)elm_object_item_data_get(glit);
   else
     part = (Demo_Part *)elm_object_item_data_get(glit);

   if ((plit == pl->it_text) &&
       ((part->type == EDJE_PART_TYPE_TEXT) ||
        (part->type == EDJE_PART_TYPE_TEXTBLOCK)))
     evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_TEXT_PART_CLICKED, part);
   else if ((plit == pl->it_swallow) && (part->type == EDJE_PART_TYPE_SWALLOW))
     evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_PART_CLICKED, part);
   else if (plit == pl->it_signal)
     evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_PROGRAM_PART_CLICKED, signal);
   else /* empty everything */
     evas_object_smart_callback_call(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, NULL);
}

static void
_clicked_cb(void *data,
            Evas_Object *o __UNUSED__,
            void *event_info __UNUSED__)
{
   Demo_Signal *signal = (Demo_Signal *)data;
   evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SIGNAL_SEND, signal);
}

static Evas_Object *
_content_get(void *data __UNUSED__,
             Evas_Object *obj,
             const char *part)
{
   Evas_Object *button = NULL, *ic = NULL;
   if (!strcmp(part, "elm.swallow.end"))
     {
        BUTTON_ADD(obj, button, NULL);
        ICON_STANDARD_ADD(button, ic, false, "media_player/play");
        elm_object_part_content_set(button, NULL, ic);
        evas_object_smart_callback_add(button, "clicked", _clicked_cb, data);
     }
   return button;
}

void
_program_add(void *data,
             Evas_Object *obj __UNUSED__,
             void *ei)
{
   Part_Demo_List *pl = data;
   Demo_Signal *demo_sig, *pr;
   Eina_Stringshare *sig_name, *source_name, *state1, *state2;
   Eina_Stringshare *program_name = ei;
   Eina_Bool correct = false;
   Elm_Object_Item *part_item;
   const Eina_List *part_items;

   assert(pl);

   part_items = elm_genlist_item_subitems_get(pl->it_signal);
   part_item = eina_list_data_get(part_items);
   pr = elm_object_item_data_get(part_item);

   /* find program */
   while ((pr) && (pr->prog_name != program_name))
     {
        part_items = eina_list_next(part_items);
        part_item = eina_list_data_get(part_items);
        pr = elm_object_item_data_get(part_item);
     }

   sig_name = edje_edit_program_signal_get(pl->group->edit_object, program_name);
   source_name = edje_edit_program_source_get(pl->group->edit_object, program_name);
   state1 = edje_edit_program_state_get(pl->group->edit_object, program_name);
   state2 = edje_edit_program_state2_get(pl->group->edit_object, program_name);
   if (!source_name) source_name = eina_stringshare_add("");
   if ((sig_name) && (strcmp(sig_name, "drag") != 0) &&
       (strncmp(sig_name, "mouse", strlen("mouse")) != 0))
     correct = true;

   /* if program is not exist */
   if ((!part_item) && (correct))
     {
        demo_sig = mem_calloc(1, sizeof(Demo_Part));
        demo_sig->prog_name = eina_stringshare_add(program_name);
        demo_sig->sig_name = eina_stringshare_add(sig_name);
        demo_sig->source_name = eina_stringshare_add(source_name);
        demo_sig->action = edje_edit_program_action_get(pl->group->edit_object, program_name);
        demo_sig->emit_signal = eina_stringshare_add(state1);
        demo_sig->emitter = eina_stringshare_add(state2);
        pl->signal_list = eina_list_append(pl->signal_list, demo_sig);
        elm_genlist_item_append(pl->genlist,
                                itc_signals,
                                demo_sig,
                                pl->it_signal,
                                ELM_GENLIST_ITEM_NONE,
                                NULL,
                                NULL);
     }
   else if ((part_item) && (correct)) /* if program exist and data is correct */
     {
        eina_stringshare_del(pr->prog_name);
        eina_stringshare_del(pr->sig_name);
        eina_stringshare_del(pr->source_name);
        eina_stringshare_del(pr->emit_signal);
        eina_stringshare_del(pr->emitter);
        pr->prog_name = eina_stringshare_add(program_name);
        pr->sig_name = eina_stringshare_add(sig_name);
        pr->source_name = eina_stringshare_add(source_name);
        pr->emit_signal = eina_stringshare_add(state1);
        pr->emitter = eina_stringshare_add(state2);
        elm_genlist_item_update(part_item);
     }
   else if ((part_item) && (!correct)) /* if exists and data is NOT correct */
     {
        pl->signal_list = eina_list_remove(pl->signal_list, pr);
        _program_item_del(pl->it_signal, pr);
        eina_stringshare_del(pr->prog_name);
        eina_stringshare_del(pr->sig_name);
        eina_stringshare_del(pr->source_name);
        eina_stringshare_del(pr->emit_signal);
        eina_stringshare_del(pr->emitter);
        free(pr);
        elm_object_item_del(part_item);
     }
   eina_stringshare_del(sig_name);
   eina_stringshare_del(source_name);
   eina_stringshare_del(state1);
   eina_stringshare_del(state2);
}


static void
_part_renamed(void *data,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Rename *ren = ei;
   Eina_List *l;
   Demo_Part *demo_part;
   Part_Demo_List *pl = data;

   Elm_Object_Item *part_item, *list_item = NULL;
   const Eina_List *part_items;
   Demo_Part *pr;

   assert(ren != NULL);

   EINA_LIST_FOREACH(pl->text_list, l, demo_part)
     {
        if (demo_part->name == ren->old_name)
          {
             eina_stringshare_del(demo_part->name);
             demo_part->name = eina_stringshare_add(ren->new_name);

             list_item = pl->it_text;
          }
     }

   /* didn't found genlist item in text, search inside of swallow list */
   if (!list_item)
     {
        EINA_LIST_FOREACH(pl->swallow_list, l, demo_part)
          {
             if (demo_part->name == ren->old_name)
               {
                  eina_stringshare_del(demo_part->name);
                  demo_part->name = eina_stringshare_add(ren->new_name);
                  list_item = pl->it_swallow;
               }
          }
     }

   elm_genlist_item_expanded_set(list_item, true);
   part_items = elm_genlist_item_subitems_get(list_item);
   part_item = eina_list_data_get(part_items);
   pr = elm_object_item_data_get(part_item);
   while (pr != demo_part)
     {
        part_items = eina_list_next(part_items);
        part_item = eina_list_data_get(part_items);
        pr = elm_object_item_data_get(part_item);
     }
   elm_genlist_item_update(part_item);
}

Evas_Object *
demo_group_add(Group *group)
{
   Part *part;
   Eina_List *l;
   Demo_Part *demo_part;
   Demo_Signal *demo_sig;

   assert(group != NULL);
   assert(ap.win != NULL);

   Part_Demo_List *pl = mem_calloc(1, sizeof(Part_Demo_List));
   pl->layout = elm_layout_add(ap.win);
   elm_layout_theme_set(pl->layout, "layout", "navigator", "default");
   evas_object_show(pl->layout);

   evas_object_data_set(pl->layout, DEMO_GROUP_DATA, pl);

   pl->group = group;
   /* add some genlists */
   if (!itc_swallow)
     {
        itc_swallow = elm_genlist_item_class_new();
        itc_swallow->item_style = "caption";
        itc_swallow->func.text_get = _swallow_label_cb;
     }
   if (!itc_text)
     {
        itc_text = elm_genlist_item_class_new();
        itc_text->item_style = "caption";
        itc_text->func.text_get = _text_label_cb;
     }
   if (!itc_signal)
     {
        itc_signal = elm_genlist_item_class_new();
        itc_signal->item_style = "caption";
        itc_signal->func.text_get = _signal_label_cb;
     }

   if (!itc_part)
     {
        itc_part = elm_genlist_item_class_new();
        itc_part->item_style = "state";
        itc_part->func.text_get = _part_label_get;
     }
   if (!itc_part_selected)
     {
        itc_part_selected = elm_genlist_item_class_new();
        itc_part_selected->item_style = "state_selected";
        itc_part_selected->func.text_get = _part_label_get;
     }
   if (!itc_signals)
     {
        itc_signals = elm_genlist_item_class_new();
        itc_signals->item_style = "signal";
        itc_signals->func.text_get = _sig_label_get;
        itc_signals->func.content_get = _content_get;
     }

   /* filling genlist */
   pl->genlist = elm_genlist_add(pl->layout);
   elm_genlist_homogeneous_set(pl->genlist, true);
   elm_scroller_policy_set(pl->genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   evas_object_show(pl->genlist);
   elm_object_content_set(pl->layout, pl->genlist);

   evas_object_smart_callback_add(ap.win, SIGNAL_PART_RENAMED, _part_renamed, pl);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_UPDATE, _program_add, pl);

   evas_object_smart_callback_add(pl->genlist, "expand,request", _expand_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contract,request", _contract_request_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "expanded", _expanded_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "contracted", _contracted_cb, pl);
   evas_object_smart_callback_add(pl->genlist, "selected", _selected_cb, pl);
   evas_object_data_set(pl->genlist, DEMO_GROUP_DATA, pl);

   pl->it_swallow = elm_genlist_item_append(pl->genlist,
                                            itc_swallow,
                                            pl,
                                            NULL,
                                            ELM_GENLIST_ITEM_TREE,
                                            NULL,
                                            NULL);
   elm_genlist_item_expanded_set(pl->it_swallow, true);
   pl->it_text =    elm_genlist_item_append(pl->genlist,
                                            itc_text,
                                            pl,
                                            NULL,
                                            ELM_GENLIST_ITEM_TREE,
                                            NULL,
                                            NULL);
   elm_genlist_item_expanded_set(pl->it_text, true);
   pl->it_signal =  elm_genlist_item_append(pl->genlist,
                                            itc_signal,
                                            pl,
                                            NULL,
                                            ELM_GENLIST_ITEM_TREE,
                                            NULL,
                                            NULL);
   elm_genlist_item_expanded_set(pl->it_signal, true);

   EINA_LIST_FOREACH(pl->group->parts, l, part)
     {
        if ((part->type == EDJE_PART_TYPE_TEXT) ||
            (part->type == EDJE_PART_TYPE_TEXTBLOCK))
          {
             demo_part = mem_calloc(1, sizeof(Demo_Part));
             demo_part->name = eina_stringshare_add(part->name);
             demo_part->type = part->type;
             pl->text_list = eina_list_append(pl->text_list, demo_part);
             elm_genlist_item_append(pl->genlist,
                                     itc_part,
                                     demo_part,
                                     pl->it_text,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);
          }
        else if (part->type == EDJE_PART_TYPE_SWALLOW)
          {
             demo_part = mem_calloc(1, sizeof(Demo_Part));
             demo_part->name = eina_stringshare_add(part->name);
             demo_part->type = part->type;
             demo_part->a = 255;
             demo_part->r = 255;
             demo_part->g = 255;
             demo_part->b = 255;
             pl->swallow_list = eina_list_append(pl->swallow_list, demo_part);
             elm_genlist_item_append(pl->genlist,
                                     itc_part,
                                     demo_part,
                                     pl->it_swallow,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);

          }
     }

   Resource *prog_name;
   Eina_Stringshare *sig_name, *source_name, *state1, *state2;
   EINA_LIST_FOREACH(pl->group->programs, l, prog_name)
     {
        sig_name = edje_edit_program_signal_get(pl->group->edit_object, prog_name->name);
        source_name = edje_edit_program_source_get(pl->group->edit_object, prog_name->name);
        state1 = edje_edit_program_state_get(pl->group->edit_object, prog_name->name);
        state2 = edje_edit_program_state2_get(pl->group->edit_object, prog_name->name);
        if (!source_name) source_name = eina_stringshare_add("");
        if ((sig_name) && (strcmp(sig_name, "drag") != 0) &&
            (strncmp(sig_name, "mouse", strlen("mouse")) != 0))
          {
             demo_sig = mem_calloc(1, sizeof(Demo_Signal));
             demo_sig->prog_name = eina_stringshare_add(prog_name->name);
             demo_sig->sig_name = eina_stringshare_add(sig_name);
             demo_sig->source_name = eina_stringshare_add(source_name);
             demo_sig->action = edje_edit_program_action_get(pl->group->edit_object, prog_name->name);
             demo_sig->emit_signal = eina_stringshare_add(state1);
             demo_sig->emitter = eina_stringshare_add(state2);
             pl->signal_list = eina_list_append(pl->signal_list, demo_sig);
             elm_genlist_item_append(pl->genlist,
                                     itc_signals,
                                     demo_sig,
                                     pl->it_signal,
                                     ELM_GENLIST_ITEM_NONE,
                                     NULL,
                                     NULL);

          }
        eina_stringshare_del(sig_name);
        eina_stringshare_del(source_name);
        eina_stringshare_del(state1);
        eina_stringshare_del(state2);
     }

   elm_object_text_set(pl->layout, pl->group->name);

   return pl->layout;
}

void
demo_group_del(Evas_Object *demo)
{
   Part_Demo_List *pl = evas_object_data_get(demo, DEMO_GROUP_DATA);
   Demo_Part *demo_part;
   Demo_Signal *demo_sig;

   TODO("probably call for some content del? to remove content from demo?");
   /* WILL DO IN ANOTHER COMMIT */

   EINA_LIST_FREE(pl->swallow_list, demo_part)
     {
        eina_stringshare_del(demo_part->name);
        eina_stringshare_del(demo_part->image_path);
        free(demo_part);
     }

   EINA_LIST_FREE(pl->text_list, demo_part)
     {
        eina_stringshare_del(demo_part->name);
        free(demo_part);
     }

   EINA_LIST_FREE(pl->signal_list, demo_sig)
     {
        eina_stringshare_del(demo_sig->prog_name);
        eina_stringshare_del(demo_sig->sig_name);
        eina_stringshare_del(demo_sig->source_name);
        eina_stringshare_del(demo_sig->emit_signal);
        eina_stringshare_del(demo_sig->emitter);
        free(demo_sig);
     }

   free(pl);
}

void
demo_group_part_add(Evas_Object *demo, Part *part)
{
   Part_Demo_List *pl = evas_object_data_get(demo, DEMO_GROUP_DATA);
   Demo_Part *demo_part;

   assert(part->name != NULL);

   if ((part->type == EDJE_PART_TYPE_TEXT) ||
       (part->type == EDJE_PART_TYPE_TEXTBLOCK))
     {
        demo_part = mem_calloc(1, sizeof(Demo_Part));
        demo_part->name = eina_stringshare_add(part->name);
        demo_part->type = part->type;
        pl->text_list = eina_list_append(pl->text_list, demo_part);
        elm_genlist_item_append(pl->genlist,
                                itc_part,
                                demo_part,
                                pl->it_text,
                                ELM_GENLIST_ITEM_NONE,
                                NULL,
                                NULL);
     }
   else if (part->type == EDJE_PART_TYPE_SWALLOW)
     {
        demo_part = mem_calloc(1, sizeof(Demo_Part));
        demo_part->name = eina_stringshare_add(part->name);
        demo_part->type = part->type;
        demo_part->a = 255;
        demo_part->r = 255;
        demo_part->g = 255;
        demo_part->b = 255;
        pl->swallow_list = eina_list_append(pl->swallow_list, demo_part);
        elm_genlist_item_append(pl->genlist,
                                itc_part,
                                demo_part,
                                pl->it_swallow,
                                ELM_GENLIST_ITEM_NONE,
                                NULL,
                                NULL);
     }
}

static void
_program_item_del(Elm_Object_Item *pl, Demo_Signal *part)
{
   Elm_Object_Item *part_item;
   const Eina_List *part_items;
   Demo_Signal *pr;

   assert(pl != NULL);
   assert(part != NULL);

   if (!elm_genlist_item_expanded_get(pl)) return;

   part_items = elm_genlist_item_subitems_get(pl);

   part_item = eina_list_data_get(part_items);
   pr = elm_object_item_data_get(part_item);
   while (pr->prog_name != part->prog_name)
     {
        part_items = eina_list_next(part_items);
        part_item = eina_list_data_get(part_items);
        pr = elm_object_item_data_get(part_item);

        assert(pr != NULL);
     }
   assert(part_item != NULL);

   elm_object_item_del(part_item);
   elm_genlist_item_update(pl);
   return;
}
static void
_part_item_del(Elm_Object_Item *pl, Demo_Part *part)
{
   Elm_Object_Item *part_item;
   const Eina_List *part_items;
   Demo_Part *pr;

   assert(pl != NULL);
   assert(part != NULL);

   if (!elm_genlist_item_expanded_get(pl)) return;

   part_items = elm_genlist_item_subitems_get(pl);

   part_item = eina_list_data_get(part_items);
   pr = elm_object_item_data_get(part_item);
   while (pr->name != part->name)
     {
        part_items = eina_list_next(part_items);
        part_item = eina_list_data_get(part_items);
        pr = elm_object_item_data_get(part_item);

        assert(pr != NULL);
     }
   assert(part_item != NULL);

   elm_object_item_del(part_item);
   elm_genlist_item_update(pl);
   return;
}

void
demo_group_part_del(Evas_Object *demo, Part *part)
{
   Part_Demo_List *pl = evas_object_data_get(demo, DEMO_GROUP_DATA);
   Demo_Part *demo_part;
   Eina_List *l;

   assert(pl);
   assert(part->name != NULL);

   if ((part->type == EDJE_PART_TYPE_TEXT) ||
       (part->type == EDJE_PART_TYPE_TEXTBLOCK))
     {
        EINA_LIST_FOREACH(pl->text_list, l, demo_part)
          {
             if (demo_part->name == part->name)
               {
                  pl->text_list = eina_list_remove(pl->text_list, demo_part);
                  _part_item_del(pl->it_text, demo_part);
                  eina_stringshare_del(demo_part->name);
                  free(demo_part);
                  return;
               }
          }
     }
   else if (part->type == EDJE_PART_TYPE_SWALLOW)
     {
        EINA_LIST_FOREACH(pl->swallow_list, l, demo_part)
          {
             if (demo_part->name == part->name)
               {
                  pl->swallow_list = eina_list_remove(pl->swallow_list, demo_part);
                  _part_item_del(pl->it_swallow, demo_part);
                  eina_stringshare_del(demo_part->name);
                  eina_stringshare_del(demo_part->image_path);
                  free(demo_part);
                  return;
               }
          }
     }
}

void
demo_group_program_del(Evas_Object *demo, Eina_Stringshare *program_name)
{
   Part_Demo_List *pl = evas_object_data_get(demo, DEMO_GROUP_DATA);
   Demo_Signal *demo_sig;
   Eina_List *l;

   assert(pl);
   assert(program_name != NULL);

   EINA_LIST_FOREACH(pl->signal_list, l, demo_sig)
     {
        if (demo_sig->prog_name == program_name)
          {
             pl->signal_list = eina_list_remove(pl->signal_list, demo_sig);
             _program_item_del(pl->it_signal, demo_sig);
             eina_stringshare_del(demo_sig->prog_name);
             eina_stringshare_del(demo_sig->sig_name);
             eina_stringshare_del(demo_sig->source_name);
             eina_stringshare_del(demo_sig->emit_signal);
             eina_stringshare_del(demo_sig->emitter);
             free(demo_sig);
             return;
          }
     }
}

void
demo_group_demo_update(Evas_Object *demo)
{
   Part_Demo_List *pl = evas_object_data_get(demo, DEMO_GROUP_DATA);
   Demo_Part *part;
   Eina_List *l;

   assert(pl);

   EINA_LIST_FOREACH(pl->text_list, l, part)
     {
        evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_TEXT_SET, part);
     }
   EINA_LIST_FOREACH(pl->swallow_list, l, part)
     {
        if (part->object)
          part->change = true;
        evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, part);
     }
}
