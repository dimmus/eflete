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

#include "add_part_dialog.h"

#define APD_GS_KEY "entry_gs"

static Elm_Genlist_Item_Class *_itc_part = NULL;

static char *
_item_part_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   char *style_label = (char *)data;
   if (!style_label)
     {
        ERR("It impossible, but it is occurred, part name is missing!");
        return NULL;
     }
   return strdup(style_label);
}

static void
_item_part_del(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__)
{
}
static void
_swallow_add_on_click(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   const char *name = elm_object_text_get(entry);
   evas_object_smart_callback_call(gs, "gs,swallow,add", strdup(name));
   evas_object_del(mwin);
}

static void
_txtblock_add_on_click(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_object_text_get(entry);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   evas_object_smart_callback_call(gs, "gs,txtblock,add", strdup(name));
   evas_object_del(mwin);
}

static void
_group_add_on_click(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_object_text_get(entry);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   evas_object_smart_callback_call(gs, "gs,group,add", strdup(name));
   evas_object_del(mwin);
}

static void
_spacer_add_on_click(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_object_text_get(entry);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   evas_object_smart_callback_call(gs, "gs,spacer,add", strdup(name));
   evas_object_del(mwin);
}


static void
_txt_add_on_click(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_object_text_get(entry);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   evas_object_smart_callback_call(gs, "gs,txt,add", strdup(name));
   evas_object_del(mwin);
}



static void
_rect_add_on_click(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_object_text_get(entry);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   evas_object_smart_callback_call(gs, "gs,rect,add", strdup(name));
   evas_object_del(mwin);
}

static void
_img_add_on_click(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_object_text_get(entry);
   Evas_Object *mwin =  elm_object_parent_widget_get(elm_object_parent_widget_get(entry));
   evas_object_smart_callback_call(gs, "gs,img,add", strdup(name));
   evas_object_del(mwin);
}

Evas_Object *
new_part_dialog_add(Evas_Object *parent, Evas_Object  *groupspace)
{
   Evas_Object *mwin;
   Evas_Object *box, *entry;
   Evas_Object *glist;

   mwin = mw_add(parent);
   mw_title_set(mwin, "Add new part.");

   box = elm_box_add(mwin);
   elm_box_align_set(box, 0.5, 1.0);
   elm_box_homogeneous_set(box, EINA_FALSE);
   elm_win_inwin_content_set(mwin, box);
   evas_object_show(box);

   entry = elm_entry_add(mwin);
   elm_entry_single_line_set(entry, EINA_TRUE);
   elm_entry_scrollable_set(entry, EINA_TRUE);
   elm_entry_scrollbar_policy_set(entry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_text_set(entry, "New_part_name");
   evas_object_show(entry);
   elm_box_pack_end(box, entry);
   evas_object_data_set(entry, APD_GS_KEY, groupspace);

   if (!_itc_part)
     {
        _itc_part = elm_genlist_item_class_new();
        _itc_part->item_style = "default";
        _itc_part->func.text_get = _item_part_label_get;
        _itc_part->func.content_get = NULL;
        _itc_part->func.state_get = NULL;
        _itc_part->func.del = _item_part_del;
     }

   glist = elm_genlist_add(mwin);
   evas_object_size_hint_align_set(glist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(glist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(glist);
   elm_box_pack_end(box, glist);

   elm_genlist_item_append(glist, _itc_part, "Rectangle", NULL,
                           ELM_GENLIST_ITEM_NONE, _rect_add_on_click, entry);
   elm_genlist_item_append(glist, _itc_part, "Text", NULL,
                           ELM_GENLIST_ITEM_NONE, _txt_add_on_click, entry);
   elm_genlist_item_append(glist, _itc_part, "Image", NULL,
                           ELM_GENLIST_ITEM_NONE, _img_add_on_click, entry);
   elm_genlist_item_append(glist, _itc_part, "Swallow", NULL,
                           ELM_GENLIST_ITEM_NONE, _swallow_add_on_click, entry);
   elm_genlist_item_append(glist, _itc_part, "Textblock", NULL,
                           ELM_GENLIST_ITEM_NONE, _txtblock_add_on_click, entry);
   elm_genlist_item_append(glist, _itc_part, "Group", NULL,
                           ELM_GENLIST_ITEM_NONE, _group_add_on_click, entry);
   elm_genlist_item_append(glist, _itc_part, "Spacer", NULL,
                           ELM_GENLIST_ITEM_NONE, _spacer_add_on_click, entry);
   evas_object_show(mwin);
   return mwin;
}
