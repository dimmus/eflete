/*
 * Edje Theme Editor
 * Copyright (C) 2013 Samsung Electronics.
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

#include "ui_widget_list.h"
#include "widget_manager.h"

#define NAVIFRAME_DATA_KEY "naviframe"
#define TABS_DATA_KEY "tabs"
#define WIDGETS_NAVIFRAME_DATA_KEY "nf_widgets"
#define LAYOUTS_NAVIFRAME_DATA_KEY "nf_layouts"
#define WIDGETS_TAB_DATA_KEY "widgets_tab"
#define LAYOUTS_TAB_DATA_KEY "layouts_tab"
#define WIDGETLIST_DATA_KEY "widget_list"
#define PARTLIST_DATA_KEY "part_list"

static Elm_Genlist_Item_Class *_itc_widget = NULL;
static Elm_Genlist_Item_Class *_itc_class = NULL;
static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_layout = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;
static Elm_Genlist_Item_Class *_itc_container = NULL;
static Elm_Genlist_Item_Class *_itc_part_item = NULL;

static inline Elm_Object_Item *
_widget_list_get(Evas_Object *naviframe)
{
   assert(naviframe != NULL);

   Elm_Object_Item *item_gl_widgets = elm_naviframe_top_item_get(naviframe);

   return item_gl_widgets;
}

static inline Evas_Object *
_current_naviframe_get(Evas_Object *tabs)
{
   Ewe_Tabs_Item *selected;

   assert(tabs != NULL);

   selected = ewe_tabs_active_item_get(tabs);
   if (evas_object_data_get(tabs, WIDGETS_TAB_DATA_KEY) == selected)
      return evas_object_data_get(tabs, WIDGETS_NAVIFRAME_DATA_KEY);
   return evas_object_data_get(tabs, LAYOUTS_NAVIFRAME_DATA_KEY);
}

static Elm_Object_Item *
_genlist_find_item_by_name(Evas_Object *obj, const char *name)
{
   Part *part;

   assert(obj != NULL);
   assert(name != NULL);

   Elm_Object_Item *item = elm_genlist_first_item_get(obj);

   while (item)
     {
        /* We are searching through parts.
         * If current item is actually BOX's or TABLE's item (contain parent),
         * checking need to be skipped. */
        if (!elm_genlist_item_parent_get(item))
          {
             part = elm_object_item_data_get(item);
             if (strcmp(part->name, name) == 0)
               break;
          }
        item = elm_genlist_item_next_get(item);
     }
   return item;
}

static char *
_item_part_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part)
{
   Part *p = (Part *)data;

   assert(p != NULL);
   assert(p->name != NULL);
   assert(part != NULL);

   if (!strcmp(part, "elm.text"))
     {
        return strdup(p->name);
     }
   return strdup("");
}

static char *
_item_item_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   Eina_Stringshare *item_name = (Eina_Stringshare *)data;
   if (data) return strdup(item_name);
   return strdup("Unknown");
}

static void
_on_check_click(void *data,
               Evas_Object *obj,
               void *event_data __UNUSED__)
{
   Evas_Object *gl_parts, *tabs;
   Part *_part = (Part *)data;

   assert(_part != NULL);

   _part->show = !_part->show;

   gl_parts = evas_object_data_get(obj, PARTLIST_DATA_KEY);

   assert(gl_parts != NULL);

   tabs = evas_object_data_get(gl_parts, TABS_DATA_KEY);

   assert(tabs != NULL);

   if (_part->show)
     evas_object_smart_callback_call(tabs, "wl,part,show", (void *)_part->name);
   else
     evas_object_smart_callback_call(tabs, "wl,part,hide", (void *)_part->name);
}

static void
_on_item_add_clicked(void *data,
                     Evas_Object *obj,
                     void *event_info __UNUSED__)
{
   Part *_part =  (Part* ) data;

   assert(_part != NULL);

   Evas_Object *gl_parts = evas_object_data_get(obj, PARTLIST_DATA_KEY);

   assert(gl_parts != NULL);

   Evas_Object *tabs = evas_object_data_get(gl_parts, TABS_DATA_KEY);

   assert(tabs != NULL);

   /* Checking number of groups */
   App_Data *app = app_data_get();
   Eina_List *groups = edje_file_collection_list(app->project->dev);
   unsigned int count = eina_list_count(groups);
   edje_file_collection_list_free(groups);
   if (count >= 2)
     evas_object_smart_callback_call(tabs, "wl,part,item,add", _part);
   else
     {
        NOTIFY_WARNING(_("There is only one group. <br>"
                         "Need more groups for adding items."))
        return;
     }
}

static void
_on_item_del_clicked(void *data,
                     Evas_Object *obj,
                     void *event_info __UNUSED__)
{
   Eina_Stringshare *item_name = (Eina_Stringshare *)data;

   assert(item_name != NULL);

   Evas_Object *gl_parts = evas_object_data_get(obj, PARTLIST_DATA_KEY);

   assert(gl_parts != NULL);

   Evas_Object *tabs = evas_object_data_get(gl_parts, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,item,del", (char *)item_name);
   return;
}

static Evas_Object *
_item_part_content_get(void *data,
                       Evas_Object *obj,
                       const char *part)
{
   Evas_Object *content = NULL;
   Part *_part = (Part *) data;

   assert(_part != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        content = elm_check_add(obj);
        if (_part->show)
          elm_check_state_set(content, true);
        else
           elm_check_state_set(content, false);
        elm_object_style_set(content, "widgetlist/default");

        evas_object_smart_callback_add(content, "changed", _on_check_click, _part);
        evas_object_data_set(content, PARTLIST_DATA_KEY, obj);
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
          }
     }
   if (!strcmp(part, "elm.swallow.penult"))
     {
        if ((_part->type == EDJE_PART_TYPE_TABLE) || ((_part->type == EDJE_PART_TYPE_BOX)))
          {
             Evas_Object *icon;

             content = elm_button_add(obj);
             ICON_STANDARD_ADD(content, icon, true, "plus");
             elm_object_part_content_set(content, NULL, icon);
             elm_object_style_set(content, "anchor");
             evas_object_data_set(content, PARTLIST_DATA_KEY, obj);
             evas_object_smart_callback_add(content, "clicked", _on_item_add_clicked, _part);
          }
     }
   return content;
}

static Evas_Object *
_item_part_item_content_get(void *data,
                       Evas_Object *obj,
                       const char *part)
{
   Eina_Stringshare *item_name = (Eina_Stringshare *) data;

   assert(item_name != NULL);

   if (!strcmp(part, "elm.swallow.penult"))
     {
        Evas_Object *button, *_icon;
        button = elm_button_add(obj);
        ICON_STANDARD_ADD(button, _icon, true, "minus");
        elm_object_part_content_set(button, NULL, _icon);
        elm_object_style_set(button, "anchor");

        evas_object_data_set(button, PARTLIST_DATA_KEY, obj);
        evas_object_smart_callback_add(button, "clicked", _on_item_del_clicked, item_name);
        return button;
     }
   return NULL;
}

static char *
_item_layout_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Style *layout = (Style *)data;

   assert(layout != NULL);
   assert(layout->full_group_name != NULL);

   return strdup(layout->full_group_name);
}


static char *
_item_style_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Style *style = (Style *)data;

   assert(style != NULL);
   assert(style->name != NULL);

   return strdup(style->name);
}

static char *
_item_widget_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char *part __UNUSED__)
{
   Widget *widget = (Widget *)data;

   assert(widget != NULL);
   assert(widget->name != NULL);

   return strdup(widget->name);
}

static char *
_item_class_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Class *class = (Class *)data;

   assert(class != NULL);
   assert(class->name != NULL);

   return strdup(class->name);
}

static void
_navi_gl_styles_pop(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   elm_naviframe_item_pop(nf);
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call (tabs, "wl,style,back", NULL);
}

static void
_navi_gl_parts_pop(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   elm_naviframe_item_pop(nf);
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,back", NULL);
}

static void
_on_part_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Part *_part = elm_object_item_data_get(glit);

   assert(_part != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,select", _part);
}

static void
_on_part_item_select(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Elm_Object_Item *parent = elm_genlist_item_parent_get(glit);
   Part *part = elm_object_item_data_get(parent);

   assert(part != NULL);

   Evas_Object *nf = (Evas_Object *)data;
   Eina_Stringshare *item_name = elm_object_item_data_get(glit);

   assert(item_name != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,select", part);
   evas_object_smart_callback_call(tabs, "wl,part,item,select", (char *)item_name);
   return;
}

static void
_on_part_item_unselect(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   TODO("Is this check needed?");
   if (!elm_genlist_item_parent_get(glit)) return;
   Eina_Stringshare *item_name = elm_object_item_data_get(glit);

   assert(item_name != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,item,unselect", (char *)item_name);
}

static void
_layout_add_cb(void *data,
               Evas_Object *obj,
               void *event_info __UNUSED__)
{
   Evas_Object *block = (Evas_Object *)data;

   assert(block != NULL);

   Evas_Object *widget_list = evas_object_data_get(obj, WIDGETLIST_DATA_KEY);

   assert(widget_list);

   evas_object_smart_callback_call(block, "wl,layout,add", widget_list);
}

static void
_layout_del_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Evas_Object *block = (Evas_Object *)data;

   assert(block != NULL);

   evas_object_smart_callback_call(block, "wl,layout,del", NULL);
}

static void
_unset_cur_style(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Project *project = (Project *)data;

   assert(project != NULL);

   Part *pr;
   EINA_INLIST_FOREACH(project->current_style->parts, pr)
      pr->show = true;
   project->current_style = NULL;
   evas_object_smart_callback_del(obj, "wl,part,back", _unset_cur_style);
}

static void
_add_style_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *block =  elm_object_parent_widget_get(evas_object_data_get((Evas_Object *)data, TABS_DATA_KEY));

   assert(block != NULL);

   evas_object_smart_callback_call(block, "wl,style,add", NULL);
}

static void
_del_style_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *block =  elm_object_parent_widget_get(evas_object_data_get((Evas_Object *)data, TABS_DATA_KEY));

   assert(block != NULL);

   evas_object_smart_callback_call(block, "wl,style,del", NULL);
}

static void
_del_part_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,del", NULL);
}

static void
_add_part_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,add", NULL);
}

static void
_above_part_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,above", NULL);
}

static void
_past_part_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   evas_object_smart_callback_call(tabs, "wl,part,below", NULL);
}

static void
_part_reordered(Evas_Object *data,
                Evas_Object *obj __UNUSED__,
                Elm_Object_Item *item)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   assert(tabs != NULL);

   Part *part = NULL;
   Elm_Object_Item *rel = elm_genlist_item_next_get(item);
   elm_genlist_item_selected_set(item, true);

   if (!elm_genlist_item_parent_get(item))
     {
        if (rel)
          {
             part = elm_object_item_data_get(rel);
             evas_object_smart_callback_call(tabs, "wl,part,moved,up", part);
          }
        else
          {
             rel = elm_genlist_item_prev_get(item);
             part =  elm_object_item_data_get(rel);
             evas_object_smart_callback_call(tabs, "wl,part,moved,down", part);
          }
     }
}

static void
_part_items_expand(void *data,
                   Evas_Object *obj,
                   void *event_info)
{
   Evas_Object *nf = (Evas_Object *)data;

   assert(nf != NULL);

   Elm_Object_Item *glit = event_info;
   Part *_part = elm_object_item_data_get(glit);

   assert(_part != NULL);

   Eina_List *l_items = NULL, *l_n_items = NULL;
   Eina_Stringshare *item_name = NULL;

   EINA_LIST_FOREACH_SAFE(_part->items, l_items, l_n_items, item_name)
     {
         elm_genlist_item_append(obj, _itc_part_item, item_name,
                                 glit, ELM_GENLIST_ITEM_NONE,
                                 _on_part_item_select, nf);
     }
}

static void
_part_items_contract(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
_part_items_expand_req(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, true);
}

static void
_part_items_contract_req(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, false);
}

static void
_wl_item_selected(void *data __UNUSED__,
                  Evas_Object *obj,
                  void *event_info __UNUSED__)
{
   Elm_Object_Item *dec_eoi = NULL, *eoi = NULL;
   eoi = elm_genlist_selected_item_get(obj);
   dec_eoi = elm_genlist_decorated_item_get(obj);
   if (dec_eoi)
     {
        elm_genlist_item_decorate_mode_set(dec_eoi, "mode", false);
        elm_genlist_item_update(dec_eoi);
     }
   elm_genlist_item_selected_set(eoi, true);
   return;
}

static void
_on_style_clicked_double(void *data,
                         Evas_Object *obj,
                         void *event_info)
{
   Project *pr = (Project *)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Elm_Object_Item *eoi;
   Evas_Object *nf, *nf_widgets, *nf_layouts, *tabs, *gl_parts, *bt, *ic;
   Eina_Inlist *parts;
   Style *_style, *clicked_style;
   Part *_part;
   Evas_Object *button = NULL;
   Evas_Object *_icon = NULL;
   Eina_List *naviframe_items = NULL;

   assert(pr != NULL);

   nf = evas_object_data_get(obj, NAVIFRAME_DATA_KEY);
   tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   nf_widgets = evas_object_data_get(tabs, WIDGETS_NAVIFRAME_DATA_KEY);
   nf_layouts = evas_object_data_get(tabs, LAYOUTS_NAVIFRAME_DATA_KEY);
   _style = elm_object_item_data_get(glit);

   assert(nf != NULL);
   assert(tabs != NULL);
   assert(nf_widgets != NULL);
   assert(nf_layouts != NULL);
   assert(_style != NULL);

   if ((_style->__type != STYLE) && (_style->__type != LAYOUT))
     return;
   if (nf == nf_widgets)
     {
        naviframe_items = elm_naviframe_items_get(nf_layouts);
        if (eina_list_count(naviframe_items) == 2)
          _navi_gl_parts_pop(nf_layouts, NULL, NULL);
     }
   else if (nf == nf_layouts)
     {
        naviframe_items = elm_naviframe_items_get(nf_widgets);
        if (eina_list_count(naviframe_items) == 3)
          _navi_gl_parts_pop(nf_widgets, NULL, NULL);
     }
   eina_list_free(naviframe_items);

   clicked_style = _style;
   if (_style->isAlias)
     {
       assert(_style->main_group != NULL);
        _style = _style->main_group;
     }

   parts = _style->parts;

   evas_object_smart_callback_call(tabs, "wl,style,select", clicked_style);

   if (!_itc_part)
     {
        _itc_part = elm_genlist_item_class_new();
        _itc_part->item_style = "default";
        _itc_part->func.text_get = _item_part_label_get;
        _itc_part->func.content_get = _item_part_content_get;
        _itc_part->func.state_get = NULL;
        _itc_part->func.del = NULL;
     }

   if (!_itc_container)
     {
        _itc_container = elm_genlist_item_class_new();
        _itc_container->item_style = "default";
        _itc_container->func.text_get = _item_part_label_get;
        _itc_container->func.content_get = _item_part_content_get;
        _itc_container->func.state_get = NULL;
        _itc_container->func.del = NULL;
     }

   if (!_itc_part_item)
     {
        _itc_part_item = elm_genlist_item_class_new();
        _itc_part_item->item_style = "default";
        _itc_part_item->func.text_get = _item_item_label_get;
        _itc_part_item->func.content_get = _item_part_item_content_get;
        _itc_part_item->func.state_get = NULL;
        _itc_part_item->func.del = NULL;
     }

   gl_parts = elm_genlist_add(nf);
   elm_genlist_longpress_timeout_set(gl_parts, 0.2);
   evas_object_data_set(gl_parts, NAVIFRAME_DATA_KEY, nf);
   evas_object_data_set(gl_parts, TABS_DATA_KEY, tabs);
   elm_genlist_reorder_mode_set(gl_parts, true);
   elm_genlist_select_mode_set(gl_parts, ELM_OBJECT_SELECT_MODE_DEFAULT);
   evas_object_smart_callback_add(tabs, "wl,part,back", _unset_cur_style, pr);
   evas_object_size_hint_align_set(gl_parts, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_parts, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   EINA_INLIST_FOREACH(parts, _part)
     {
        if ((_part->type == EDJE_PART_TYPE_TABLE) ||
            (_part->type == EDJE_PART_TYPE_BOX))
          eoi = elm_genlist_item_append(gl_parts, _itc_container, _part,
                                        NULL, ELM_GENLIST_ITEM_TREE,
                                        _on_part_select, nf);
        else
          eoi = elm_genlist_item_append(gl_parts, _itc_part, _part,
                                        NULL, ELM_GENLIST_ITEM_NONE,
                                        _on_part_select, nf);
        elm_object_item_data_set(eoi, _part);
     }
   evas_object_smart_callback_add(gl_parts, "moved",
                                  (Evas_Smart_Cb)_part_reordered, nf);
   evas_object_smart_callback_add(gl_parts, "expanded", _part_items_expand, nf);
   evas_object_smart_callback_add(gl_parts, "contracted",
                                  _part_items_contract, NULL);
   evas_object_smart_callback_add(gl_parts, "expand,request",
                                  _part_items_expand_req, NULL);
   evas_object_smart_callback_add(gl_parts, "contract,request",
                                  _part_items_contract_req, NULL);
   evas_object_smart_callback_add(gl_parts, "unselected", _on_part_item_unselect, nf);

   ICON_STANDARD_ADD(nf, ic, false, "arrow_left");

   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   evas_object_smart_callback_add(bt, "clicked", _navi_gl_parts_pop, nf);
   elm_object_style_set(bt, "anchor");
   evas_object_show(gl_parts);

   elm_naviframe_item_push(nf, clicked_style->full_group_name, bt, NULL, gl_parts, NULL);

   button = elm_button_add(nf);
   ICON_STANDARD_ADD(button, _icon, true, "plus");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add(button, "clicked", _add_part_cb, nf);
   elm_object_style_set(button, "anchor");
   elm_object_part_content_set(nf, "elm.swallow.bt3", button);

   button = elm_button_add(nf);
   ICON_STANDARD_ADD(button, _icon, true, "minus");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _del_part_cb, nf);
   elm_object_style_set(button, "anchor");
   elm_object_part_content_set(nf, "elm.swallow.bt2", button);

   button = elm_button_add(nf);
   ICON_STANDARD_ADD(button, _icon, true, "arrow_up");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _above_part_cb, nf);
   elm_object_style_set(button, "anchor");
   elm_object_part_content_set(nf, "elm.swallow.bt1", button);

   button = elm_button_add(nf);
   ICON_STANDARD_ADD(button, _icon, true, "arrow_down");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _past_part_cb, nf);
   elm_object_style_set(button, "anchor");
   elm_object_part_content_set(nf, "elm.swallow.bt0", button);
}

static Evas_Object *
_item_style_content_get(void *data,
                       Evas_Object *obj,
                       const char *part)
{
   Style *_style = (Style *)data;
   Evas_Object *icon = NULL;

   assert(_style != NULL);

   if ((!strcmp(part, "elm.swallow.end")) && (_style->isAlias))
     IMAGE_ADD_NEW(obj, icon, "icon", "alias_link");

   return icon;
}

static void
_on_widget_clicked_double(void *data,
                          Evas_Object *obj,
                          void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Elm_Object_Item *glit_class, *glit_style;
   Evas_Object *nf, *gl_class, *bt, *ic;
   Eina_Inlist *styles = NULL, *classes = NULL;
   Widget *_widget;
   Class *_class;
   Style *_style = NULL;
   Evas_Object *button = NULL;
   Evas_Object *_icon = NULL;

   _style = elm_object_item_data_get(glit);

   assert(_style != NULL);

   if (_style->__type == LAYOUT)
     {
        _on_style_clicked_double(data, obj, event_info);
        return;
     }
   _style = NULL;

   nf = evas_object_data_get(obj, NAVIFRAME_DATA_KEY);
   _widget = elm_object_item_data_get(glit);

   assert(nf != NULL);
   assert(_widget != NULL);

   classes = _widget->classes;

   assert(classes != NULL);

   if (!_itc_class)
     {
        _itc_class = elm_genlist_item_class_new();
        _itc_class->item_style = "group_index";
        _itc_class->func.text_get = _item_class_label_get;
        _itc_class->func.content_get = NULL;
        _itc_class->func.state_get = NULL;
        _itc_class->func.del = NULL;
     }

   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "default";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = _item_style_content_get;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }

   gl_class = elm_genlist_add(nf);
   evas_object_data_set(gl_class, NAVIFRAME_DATA_KEY, nf);
   elm_genlist_select_mode_set(gl_class, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_align_set(gl_class, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_class, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   EINA_INLIST_FOREACH(classes, _class)
     {
        glit_class = elm_genlist_item_append(gl_class, _itc_class, _class,
                                             NULL, ELM_GENLIST_ITEM_GROUP,
                                             NULL, NULL);
        styles = _class->styles;

        EINA_INLIST_FOREACH(styles, _style)
          {
             glit_style = elm_genlist_item_append(gl_class, _itc_style,
                             _style, glit_class, ELM_GENLIST_ITEM_NONE,
                             NULL, NULL);
             elm_object_item_data_set(glit_style, _style);
          }
     }
   evas_object_smart_callback_add(gl_class, "clicked,double",
                                  _on_style_clicked_double, data);

   ICON_STANDARD_ADD(nf, ic, false, "arrow_left");
   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "anchor");
   evas_object_smart_callback_add(bt, "clicked", _navi_gl_styles_pop, nf);
   elm_naviframe_item_push(nf, _widget->name, bt, NULL, gl_class, NULL);

   button = elm_button_add(nf);
   ICON_STANDARD_ADD(button, _icon, true, "minus");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _del_style_cb, nf);
   elm_object_style_set(button, "anchor");
   elm_object_part_content_set(nf, "elm.swallow.bt0", button);

   button = elm_button_add(nf);
   ICON_STANDARD_ADD(button, _icon, true, "plus");
   evas_object_size_hint_align_set(button, -1, EVAS_HINT_FILL);
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _add_style_cb, obj);
   elm_object_style_set(button, "anchor");
   elm_object_part_content_set(nf, "elm.swallow.bt1", button);
   evas_object_show(gl_class);
}

Eina_Bool
ui_widget_list_class_data_reload(Evas_Object *gl_classes, Eina_Inlist *classes)
{
   Style *_style = NULL;
   Class *_class = NULL;
   Eina_Inlist *styles = NULL;
   Elm_Object_Item *glit_style = NULL;
   Elm_Object_Item *glit_class = NULL;

   assert(gl_classes != NULL);

   elm_genlist_clear(gl_classes);

   assert(classes != NULL);

   if (!_itc_class)
     {
        _itc_class = elm_genlist_item_class_new();
        _itc_class->item_style = "group_index";
        _itc_class->func.text_get = _item_class_label_get;
        _itc_class->func.content_get = NULL;
        _itc_class->func.state_get = NULL;
        _itc_class->func.del = NULL;
     }

   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "default";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = NULL;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }

   EINA_INLIST_FOREACH(classes, _class)
     {
        glit_class = elm_genlist_item_append(gl_classes, _itc_class, _class,
                                             NULL, ELM_GENLIST_ITEM_NONE,
                                             NULL, NULL);
        styles = _class->styles;

        EINA_INLIST_FOREACH(styles, _style)
          {
             glit_style = elm_genlist_item_append(gl_classes, _itc_style,
                             _style, glit_class, ELM_GENLIST_ITEM_NONE,
                             NULL, NULL);
             elm_object_item_data_set(glit_style, _style);
          }
     }
   return true;
}

Evas_Object *
ui_widget_list_add(Evas_Object *parent)
{
   Evas_Object *tabs;
   Evas_Object *nf_widgets, *nf_layouts;
   Evas_Object *ic, *bt;
   Evas_Object *gl_widgets = NULL;
   Evas_Object *gl_layouts = NULL;
   Elm_Object_Item *it_widgets, *it_layouts;
   Ewe_Tabs_Item *widgets_tab, *layouts_tab;

   assert(parent != NULL);

   if (!_itc_widget)
     {
        _itc_widget = elm_genlist_item_class_new();
        _itc_widget->item_style = "title";
        _itc_widget->func.text_get = _item_widget_label_get;
        _itc_widget->func.content_get = NULL;
        _itc_widget->func.state_get = NULL;
        _itc_widget->func.del = NULL;
     }
   if (!_itc_layout)
     {
        _itc_layout = elm_genlist_item_class_new();
        _itc_layout->item_style = "title";
        _itc_layout->decorate_item_style = "mode";
        _itc_layout->func.text_get = _item_layout_label_get;
        _itc_layout->func.content_get = _item_style_content_get;
        _itc_layout->func.state_get = NULL;
        _itc_layout->func.del = NULL;
     }
   tabs = ewe_tabs_add(parent);
   widgets_tab = ewe_tabs_item_append(tabs, NULL, _("Themes"), NULL);
   layouts_tab = ewe_tabs_item_append(tabs, NULL, _("Layouts"), NULL);

#define NAVI(TYPE, TEXT) \
   nf_##TYPE = elm_naviframe_add(tabs); \
   ewe_tabs_item_content_set(tabs, TYPE##_tab, nf_##TYPE); \
   evas_object_size_hint_align_set(nf_##TYPE, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(nf_##TYPE, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
    \
   gl_##TYPE = elm_genlist_add(nf_##TYPE); \
   evas_object_size_hint_align_set(gl_##TYPE, \
                                   EVAS_HINT_FILL, \
                                   EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(gl_##TYPE, \
                                    EVAS_HINT_EXPAND, \
                                    EVAS_HINT_EXPAND); \
   evas_object_data_set(gl_##TYPE, NAVIFRAME_DATA_KEY, nf_##TYPE); \
   evas_object_data_set(gl_##TYPE, TABS_DATA_KEY, tabs); \
   evas_object_data_set(nf_##TYPE, TABS_DATA_KEY, tabs); \
    \
   it_##TYPE = elm_naviframe_item_push(nf_##TYPE, NULL, NULL, NULL, gl_##TYPE, NULL); \
   elm_object_item_part_text_set(it_##TYPE, "subtitle", TEXT);

   NAVI(widgets, _("Widget list"));
   NAVI(layouts, _("Layouts list"));
#undef NAVI
   ICON_STANDARD_ADD(nf_layouts, ic, false, "plus");
   bt = elm_button_add(nf_layouts);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "anchor");
   evas_object_show(bt);
   evas_object_data_set(bt, WIDGETLIST_DATA_KEY, tabs);
   evas_object_smart_callback_add (bt, "clicked", _layout_add_cb, parent);
   elm_object_item_part_content_set(it_layouts, "elm.swallow.bt1", bt);
   ICON_STANDARD_ADD(nf_layouts, ic, false, "minus");
   bt = elm_button_add(nf_layouts);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "anchor");
   evas_object_show(bt);
   evas_object_data_set(bt, WIDGETLIST_DATA_KEY, tabs);
   evas_object_smart_callback_add (bt, "clicked", _layout_del_cb, parent);
   elm_object_item_part_content_set(it_layouts, "elm.swallow.bt0", bt);

   evas_object_data_set(tabs, WIDGETS_NAVIFRAME_DATA_KEY, nf_widgets);
   evas_object_data_set(tabs, LAYOUTS_NAVIFRAME_DATA_KEY, nf_layouts);
   evas_object_data_set(tabs, WIDGETS_TAB_DATA_KEY, widgets_tab);
   evas_object_data_set(tabs, LAYOUTS_TAB_DATA_KEY, layouts_tab);

   return tabs;
}

Eina_Bool
ui_widget_list_title_set(Evas_Object *object, const char *title)
{
   Elm_Object_Item *item_gl_widgets;

   assert(object != NULL);
   assert(title != NULL);

   item_gl_widgets = elm_naviframe_bottom_item_get(evas_object_data_get(object,
                                                   WIDGETS_NAVIFRAME_DATA_KEY));

   assert(item_gl_widgets != NULL);

   elm_object_item_part_text_set(item_gl_widgets, "elm.text.title", title);

   item_gl_widgets = elm_naviframe_bottom_item_get(evas_object_data_get(object,
                                                   LAYOUTS_NAVIFRAME_DATA_KEY));

   assert(item_gl_widgets != NULL);

   elm_object_item_part_text_set(item_gl_widgets, "elm.text.title", title);

   return true;
}

TODO("fix renaming")
/*
static void
_wl_key_down_cb(void *data __UNUSED__,
                Evas *e __UNUSED__,
                Evas_Object *obj,
                void *event)
{
   Evas_Event_Key_Down *ev = event;
   Elm_Object_Item *eoi = NULL, *dec_eoi = NULL;
   Style *_style = NULL;

   if ((evas_key_modifier_is_set(ev->modifiers, "Alt")) ||
       (evas_key_modifier_is_set(ev->modifiers, "Shift")) ||
       (evas_key_modifier_is_set(ev->modifiers, "Control")))
      return;

   dec_eoi = elm_genlist_decorated_item_get(obj);
   if (!strcmp(ev->key, "F2"))
     {
        eoi = elm_genlist_selected_item_get(obj);
        if (!eoi)
          {
             NOTIFY_INFO(3, _("Please select element for rename"));
             return;
          }
        _style = elm_object_item_data_get(eoi);
        if (_style->__type == LAYOUT)
          {
             if (dec_eoi)
               {
                  elm_genlist_item_decorate_mode_set(dec_eoi, "mode",
                                                     false);
                  elm_genlist_item_update(dec_eoi);
               }
             elm_genlist_item_decorate_mode_set(eoi, "mode", true);
          }
        return;
     }
   if ((!strcmp(ev->key, "Escape")) && (dec_eoi))
     {
        elm_genlist_item_decorate_mode_set(dec_eoi, "mode", false);
        elm_genlist_item_update(dec_eoi);
        elm_genlist_item_selected_set(dec_eoi, true);
        return;
     }
   return;
} */

Eina_Bool
ui_widget_list_layouts_reload(Evas_Object *gl_layouts, Project *project)
{
   Elm_Object_Item *eoi = NULL;
   Style *_layout = NULL;

   assert(gl_layouts != NULL);
   assert(project != NULL);

   elm_genlist_clear(gl_layouts);
   EINA_INLIST_FOREACH(project->layouts, _layout)
     {
        eoi = elm_genlist_item_append(gl_layouts, _itc_layout, _layout,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      NULL, NULL);
        elm_object_item_data_set(eoi, _layout);
     }
   return true;
}

Eina_Bool
ui_widget_list_data_set(Evas_Object *object, Project *project)
{
   Widget *_widget;
   Style *_layout;
   Elm_Object_Item *eoi;
   Evas_Object *nf_widgets;
   Evas_Object *gl_widgets;
   Evas_Object *nf_layouts;
   Evas_Object *gl_layouts;
   Eina_Inlist *widget_list = NULL;

   assert(object != NULL);
   assert(project != NULL);

   widget_list = project->widgets;
   nf_widgets = evas_object_data_get(object, WIDGETS_NAVIFRAME_DATA_KEY);

   assert(nf_widgets != NULL);

   gl_widgets = elm_object_item_part_content_get(_widget_list_get(nf_widgets),
                                                 "elm.swallow.content");

   assert(gl_widgets != NULL);

   elm_genlist_select_mode_set(gl_widgets, ELM_OBJECT_SELECT_MODE_ALWAYS);

   nf_layouts = evas_object_data_get(object, LAYOUTS_NAVIFRAME_DATA_KEY);

   assert(nf_layouts != NULL);

   gl_layouts = elm_object_item_part_content_get(_widget_list_get(nf_layouts),
                                                 "elm.swallow.content");

   assert(gl_layouts != NULL);

   elm_genlist_select_mode_set(gl_layouts, ELM_OBJECT_SELECT_MODE_ALWAYS);

   EINA_INLIST_FOREACH(widget_list, _widget)
     {
        eoi = elm_genlist_item_append(gl_widgets, _itc_widget, _widget,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      _wl_item_selected, NULL);
        elm_object_item_data_set(eoi, _widget);
     }

   EINA_INLIST_FOREACH(project->layouts, _layout)
     {
        eoi = elm_genlist_item_append(gl_layouts, _itc_layout, _layout,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      _wl_item_selected, NULL);
        elm_object_item_data_set(eoi, _layout);
     }

#define CALLBACKS(TYPE) \
   evas_object_smart_callback_add(gl_##TYPE, "clicked,double", \
                                  _on_widget_clicked_double, project);

TODO("fix renaming")
/*   evas_object_event_callback_add(gl_##TYPE, EVAS_CALLBACK_KEY_DOWN, \
                                  _wl_key_down_cb, NULL); \ */
   CALLBACKS(widgets);
   CALLBACKS(layouts);
#undef CALLBACKS
   return true;
}

Eina_Bool
ui_widget_list_part_add(Evas_Object *object, Style *style, const char *name)
{
   Evas_Object *gl_parts, *nf;
   Elm_Object_Item *eoi;
   Part *part;

   assert(object != NULL);
   assert(style != NULL);
   assert(name != NULL);

   part = wm_part_add(style, name);

   assert(part != NULL);

   if (style->__type == STYLE)
     {
        nf = evas_object_data_get(object, WIDGETS_NAVIFRAME_DATA_KEY);
     }
   else if (style->__type == LAYOUT)
     {
        nf = evas_object_data_get(object, LAYOUTS_NAVIFRAME_DATA_KEY);
     }
   else
     {
        ERR("Wrong style type");
        abort();
     }

   assert(nf != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(nf),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     eoi = elm_genlist_item_append(gl_parts, _itc_container, part,
                                   NULL, ELM_GENLIST_ITEM_TREE,
                                   _on_part_select, nf);
   else
     eoi = elm_genlist_item_append(gl_parts, _itc_part, part, NULL,
                                   ELM_GENLIST_ITEM_NONE, _on_part_select, nf);
   elm_object_item_data_set(eoi, part);
   elm_genlist_item_selected_set(eoi, true);

   return true;
}

Eina_Bool
ui_widget_list_selected_part_del(Evas_Object *object, Style *style)
{
   Evas_Object *gl_parts;
   Elm_Object_Item *eoi, *next_eoi;
   Part *part;

   assert(object != NULL);
   assert(style != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   eoi = elm_genlist_selected_item_get(gl_parts);

   if (!eoi)
     {
        NOTIFY_INFO(3, _("No part selected"));
        return false;
     }
   if (elm_genlist_item_parent_get(eoi))
     {
        eoi = elm_genlist_item_parent_get(eoi);
        elm_genlist_item_subitems_clear(eoi);
     }
   else
     elm_genlist_item_subitems_clear(eoi);
   part = (Part *)elm_object_item_data_get(eoi);
   wm_part_del(style, part);

   next_eoi = elm_genlist_item_next_get(eoi);
   if (!next_eoi) next_eoi = elm_genlist_item_prev_get(eoi);
   if (next_eoi) elm_genlist_item_selected_set(next_eoi, true);/*  */
   elm_object_item_del(eoi);

   return true;
}

static Eina_Bool
_selected_part_move(Evas_Object *object, Style *style, Eina_Bool move_up)
{
   Evas_Object *gl_parts = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *new_eoi = NULL;
   Elm_Object_Item *prev_eoi = NULL;
   Elm_Object_Item *next_eoi = NULL;
   Elm_Object_Item *prev_next_eoi = NULL;
   Part *part = NULL;
   Eina_Inlist *prev_elm =  NULL;
   Eina_Inlist *next_elm =  NULL;
   Eina_Inlist *itr = NULL;
   Part *_part = NULL;
   Evas_Object *nf;
   Eina_Bool expanded;

   assert(object != NULL);
   assert(style != NULL);

   nf = _current_naviframe_get(object);

   assert(nf != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(nf),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   eoi = elm_genlist_selected_item_get(gl_parts);
   if (!eoi)
     {
        NOTIFY_INFO(3, _("No part selected"));
        return false;
     }
   expanded = elm_genlist_item_expanded_get(eoi);
   if (move_up)
     {
        prev_eoi = elm_genlist_item_prev_get(eoi);
        if (!prev_eoi)
          {
             NOTIFY_INFO(3, _("Selected part is currently on top of the list"));
             return false;
          }
        if (elm_genlist_item_parent_get(prev_eoi))
          prev_eoi = elm_genlist_item_parent_get(prev_eoi);
     }
   else
     {
        next_eoi = elm_genlist_item_next_get(eoi);
        /* if actual moving item is Container */
        while (elm_genlist_item_parent_get(next_eoi))
          next_eoi = elm_genlist_item_next_get(next_eoi);
        if (!next_eoi)
          {
             NOTIFY_INFO(3, _("Selected part is currently on bottom of the list"));
             return false;
          }
        /* if next item is Container with expanded items */
        if (elm_genlist_item_subitems_count(next_eoi) > 0)
          {
             next_eoi = elm_genlist_item_next_get(next_eoi);
             prev_next_eoi = next_eoi;
             while (elm_genlist_item_parent_get(next_eoi))
               {
                  prev_next_eoi = next_eoi;
                  next_eoi = elm_genlist_item_next_get(next_eoi);
               }
             if (!next_eoi) next_eoi = prev_next_eoi;
          }
     }
   part = elm_object_item_data_get(eoi);

   if (part->__type != PART) return false;

   if (part->type == EDJE_PART_TYPE_TABLE ||
       part->type == EDJE_PART_TYPE_BOX)
     {
        new_eoi = (move_up) ?
           elm_genlist_item_insert_before(gl_parts, _itc_container, part, NULL,
                                          prev_eoi, elm_genlist_item_type_get(eoi),
                                          _on_part_select, nf) :
           elm_genlist_item_insert_after(gl_parts, _itc_container, part, NULL,
                                         next_eoi, elm_genlist_item_type_get(eoi),
                                         _on_part_select, nf);
     }
   else
     {
        new_eoi = (move_up) ?
           elm_genlist_item_insert_before(gl_parts, _itc_part, part, NULL,
                                          prev_eoi, elm_genlist_item_type_get(eoi),
                                          _on_part_select, nf) :
           elm_genlist_item_insert_after(gl_parts, _itc_part, part, NULL,
                                         next_eoi, elm_genlist_item_type_get(eoi),
                                         _on_part_select, nf);
     }
   eoi = elm_genlist_selected_item_get(gl_parts);
   elm_object_item_del(eoi);
   elm_genlist_item_selected_set(new_eoi, EINA_TRUE);
   elm_genlist_item_expanded_set(new_eoi, expanded);

   for(itr = style->parts; itr != NULL; itr = itr->next)
     {
        _part = EINA_INLIST_CONTAINER_GET(itr, Part);
        if (_part == part)
          {
             prev_elm = itr->prev;
             style->parts = eina_inlist_remove(style->parts, itr);
             style->parts = (move_up) ? eina_inlist_prepend_relative(style->parts,
                                                                     itr, prev_elm):
                                        eina_inlist_append_relative(style->parts,
                                                                    itr, next_elm);
             break;
          }
     }

   return true;
}

Eina_Bool
ui_widget_list_selected_part_above(Evas_Object *object, Style *style)
{
   return _selected_part_move(object, style, true);
}

Eina_Bool
ui_widget_list_selected_part_below(Evas_Object *object, Style *style)
{
   return _selected_part_move(object, style, false);
}

Part *
ui_widget_list_selected_part_get(Evas_Object *object)
{
   Evas_Object *gl_parts;
   Part *part;
   Elm_Object_Item *eoi;
   Elm_Object_Item *parent_item = NULL;

   assert(object != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   eoi = elm_genlist_selected_item_get(gl_parts);
   if (!eoi) return NULL;
   parent_item = elm_genlist_item_parent_get(eoi);
   if (parent_item) eoi = parent_item;

   part = (Part *) elm_object_item_data_get(eoi);
   if ((!part) || (part->__type != PART)) return NULL;

   return part;
}

Eina_Bool
ui_widget_list_part_selected_set(Evas_Object *object,
                                 const char *part,
                                 Eina_Bool selected)
{
   Evas_Object *gl_parts;

   assert(object != NULL);
   assert(part != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   Elm_Object_Item *item = _genlist_find_item_by_name(gl_parts, part);
   assert(item != NULL);

   elm_genlist_item_selected_set(item, selected);
   elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);

   return true;
}

Eina_Bool
ui_widget_list_part_update(Evas_Object *object, const char *part)
{
   Evas_Object *gl_parts;
   Elm_Object_Item *item;

   assert(object != NULL);
   assert(part != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   item = _genlist_find_item_by_name(gl_parts, part);

   assert(item != NULL);

   elm_genlist_item_update(item);

   return true;
}

Eina_List *
ui_widget_list_selected_parts_get(Evas_Object *object)
{
   Evas_Object *gl_parts;
   Part *part;
   Elm_Object_Item *eoi;
   Eina_List *selected, *l;
   Eina_List *parts = NULL;

   assert(object != NULL);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");

   assert(gl_parts != NULL);

   selected = (Eina_List *)elm_genlist_selected_items_get(gl_parts);
   if (!selected) return NULL;

   EINA_LIST_FOREACH(selected, l, eoi)
     {
        part = (Part *)elm_object_item_data_get(eoi);
        parts = eina_list_append(parts, part);
     }

   return parts;
}

void
ui_widget_list_style_parts_reload(Evas_Object *object, Style *style)
{
   Part *_part = NULL;
   Elm_Object_Item *eoi = NULL;
   Evas_Object *nf = NULL, *gl_parts = NULL;

   assert(object != NULL);
   assert(style != NULL);

   gl_parts = elm_object_item_part_content_get(
                              _widget_list_get(_current_naviframe_get(object)),
                              "elm.swallow.content");

   assert(gl_parts != NULL);

   nf = evas_object_data_get(gl_parts, NAVIFRAME_DATA_KEY);

   assert(nf != NULL);

   elm_genlist_clear(gl_parts);

   EINA_INLIST_FOREACH(style->parts, _part)
     {
        if ((_part->type == EDJE_PART_TYPE_TABLE) ||
            (_part->type == EDJE_PART_TYPE_BOX))
          eoi = elm_genlist_item_append(gl_parts, _itc_container, _part,
                                        NULL, ELM_GENLIST_ITEM_TREE,
                                        _on_part_select, nf);
        else
          eoi = elm_genlist_item_append(gl_parts, _itc_part, _part,
                                        NULL, ELM_GENLIST_ITEM_NONE,
                                        _on_part_select, nf);
        elm_object_item_data_set(eoi, _part);
     }
}

Eina_Bool
ui_widget_list_tab_activate(Evas_Object *object, unsigned int tab_index)
{
   Ewe_Tabs_Item * tab_item;
   const Eina_List *tabs_list;

   assert(object != NULL);

   tabs_list = ewe_tabs_items_list_get(object);
   tab_item = eina_list_nth(tabs_list, tab_index);
   if (!tab_item) return false;

   ewe_tabs_active_item_set(object, tab_item);
   return true;
}

Eina_Bool
ui_widget_list_part_items_refresh(Evas_Object *obj, Part *part, Eina_Bool addition)
{
   Elm_Object_Item *iterator = NULL;
   Part *item_data = NULL;
   Eina_List *l_items = NULL, *l_n_items = NULL;
   Eina_Stringshare *item_name = NULL;
   Evas_Object *nf = NULL;
   Evas_Object *part_list = NULL;
   Elm_Object_Item *eoi = NULL;

   assert(obj != NULL);
   assert(part != NULL);

   part_list = elm_object_item_part_content_get(
                           _widget_list_get(_current_naviframe_get(obj)),
                            "elm.swallow.content");

   assert(part_list != NULL);

   nf = evas_object_data_get(part_list, NAVIFRAME_DATA_KEY);

   assert(nf != NULL);

   iterator = elm_genlist_first_item_get(part_list);
   if (!iterator) return false;

   while (iterator)
     {
        item_data = elm_object_item_data_get(iterator);

        /* We are searching through parts.
         * If current item is actually BOX's or TABLE's item (contain parent),
         * checking need to be skipped. */
        if (!elm_genlist_item_parent_get(iterator))
          {
             assert(item_data != NULL);
             assert(item_data->__type == PART);

             if (item_data == part)
               {
                  if (!elm_genlist_item_expanded_get(iterator)) return true;
                  elm_genlist_item_subitems_clear(iterator);
                  EINA_LIST_FOREACH_SAFE(item_data->items, l_items, l_n_items, item_name)
                    {

                       eoi = elm_genlist_item_append(part_list, _itc_part_item, item_name,
                                                     iterator, ELM_GENLIST_ITEM_NONE,
                                                     _on_part_item_select, nf);
                    }
                  if (addition)
                    elm_genlist_item_selected_set(eoi, true);
                  else
                    elm_genlist_item_selected_set(iterator, true);
                  return true;
               }
          }
        iterator = elm_genlist_item_next_get(iterator);
     }
   return false;
}

