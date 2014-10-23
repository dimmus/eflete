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

static inline Elm_Object_Item *
_widget_list_get(Evas_Object *naviframe)
{
   Elm_Object_Item *item_gl_widgets = elm_naviframe_top_item_get(naviframe);

   return item_gl_widgets;
}

static inline Evas_Object *
_current_naviframe_get(Evas_Object *tabs)
{
   Ewe_Tabs_Item *selected;
   selected = ewe_tabs_active_item_get(tabs);
   if (evas_object_data_get(tabs, WIDGETS_TAB_DATA_KEY) == selected)
      return evas_object_data_get(tabs, WIDGETS_NAVIFRAME_DATA_KEY);
   return evas_object_data_get(tabs, LAYOUTS_NAVIFRAME_DATA_KEY);
}

static Elm_Object_Item *
_genlist_find_item_by_name(Evas_Object *obj, const char *name)
{
   Part *part;
   Elm_Object_Item *item = elm_genlist_first_item_get(obj);

   while (item)
     {
        part = elm_object_item_data_get(item);
        if (strncmp(part->name, name, strlen(name)) == 0)
          break;
        item = elm_genlist_item_next_get(item);
     }
   return item;
}

static char *
_item_part_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   Part *p = (Part *)data;
   if (!p->name)
     {
        ERR("It impossible, but it is occurred, part name is missing!");
        return NULL;
     }

   if (!strcmp(part, "elm.text"))
     {
        return strdup(p->name);
     }
   return "";
}

static void
_on_check_click(void *data,
               Evas_Object *obj,
               void *event_data __UNUSED__)
{
   Evas_Object *gl_parts, *tabs;
   Part *_part = (Part *)data;
   _part->show = !_part->show;

   gl_parts = evas_object_data_get(obj, PARTLIST_DATA_KEY);
   tabs = evas_object_data_get(gl_parts, TABS_DATA_KEY);

   if (_part->show)
     evas_object_smart_callback_call(tabs, "wl,part,show", (void *)_part->name);
   else
     evas_object_smart_callback_call(tabs, "wl,part,hide", (void *)_part->name);
}

static Evas_Object *
_item_part_content_get(void *data,
                       Evas_Object *obj,
                       const char *part)
{
   Part *_part = (Part *) data;
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *check = elm_check_add(obj);
        if (_part->show)
          elm_check_state_set(check, true);
        else
           elm_check_state_set(check, false);
        elm_object_style_set(check, "eflete/widgetlist/default");

        evas_object_size_hint_aspect_set(check, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_smart_callback_add(check, "changed", _on_check_click, _part);
        evas_object_data_set(check, PARTLIST_DATA_KEY, obj);
        return check;
     }
   if (!strcmp(part, "elm.swallow.end"))
     {
        Evas_Object *icon = NULL;
        Evas *ev = evas_object_evas_get(obj);
        if (_part->type == EDJE_PART_TYPE_RECTANGLE)
          GET_IMAGE(icon, ev, "icon-rectangle");
        if (_part->type == EDJE_PART_TYPE_IMAGE)
          GET_IMAGE(icon, ev, "icon-image");
        if (_part->type == EDJE_PART_TYPE_SPACER)
          GET_IMAGE(icon, ev, "icon-spacer");
        if (_part->type == EDJE_PART_TYPE_SWALLOW)
          GET_IMAGE(icon, ev, "icon-swallow");
        if (_part->type == EDJE_PART_TYPE_TEXT)
          GET_IMAGE(icon, ev, "icon-text");
        if (_part->type == EDJE_PART_TYPE_TEXTBLOCK)
          GET_IMAGE(icon, ev, "icon-textblock");
        if (_part->type == EDJE_PART_TYPE_GROUP)
          GET_IMAGE(icon, ev, "icon-group");
        if (_part->type == EDJE_PART_TYPE_PROXY)
          GET_IMAGE(icon, ev, "icon-proxy");
        if (_part->type == EDJE_PART_TYPE_TABLE)
          GET_IMAGE(icon, ev, "icon-table");
        if (_part->type == EDJE_PART_TYPE_BOX)
          GET_IMAGE(icon, ev, "icon-box");
        if (_part->type == EDJE_PART_TYPE_EXTERNAL)
          GET_IMAGE(icon, ev, "icon-external");

        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        return icon;
     }
   return NULL;
}

static char *
_item_layout_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Style *layout = (Style *)data;
   if (!layout->full_group_name)
     {
        ERR("It impossible, but it is occurred, layout name is missing!");
        return NULL;
     }
   return strdup(layout->full_group_name);
}


static char *
_item_style_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Style *style = (Style *)data;
   if (!style->name)
     {
        ERR("It impossible, but it is occurred, style name is missing!");
        return NULL;
     }
   return strdup(style->name);
}

static char *
_item_widget_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char *part __UNUSED__)
{
   Widget *widget = (Widget *)data;
   if (!widget->name)
     {
        ERR("It impossible, but it is occurred, widget name is missing!");
        return NULL;
     }
   return strdup(widget->name);
}

static char *
_item_class_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Class *class = (Class *)data;
   if (!class->name)
     {
        ERR("It impossible, but it is occurred, class name is missing!");
        return NULL;
     }
   return strdup(class->name);
}

static void
_navi_gl_styles_pop(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   elm_naviframe_item_pop(nf);
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   evas_object_smart_callback_call (tabs, "wl,style,back", NULL);
}

static void
_navi_gl_parts_pop(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   elm_naviframe_item_pop(nf);
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);

   evas_object_smart_callback_call(tabs, "wl,part,back", NULL);
}

static void
_on_part_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Evas_Object *nf = (Evas_Object *)data;
   Part *_part = elm_object_item_data_get(glit);
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   evas_object_smart_callback_call(tabs, "wl,part,select", _part);
}

static void
_layout_add_cb(void *data,
               Evas_Object *obj,
               void *event_info __UNUSED__)
{
   Evas_Object *block = (Evas_Object *)data;
   Evas_Object *widget_list = evas_object_data_get(obj, WIDGETLIST_DATA_KEY);
   evas_object_smart_callback_call(block, "wl,layout,add", widget_list);
}

static void
_layout_del_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Evas_Object *block = (Evas_Object *)data;
   evas_object_smart_callback_call(block, "wl,layout,del", NULL);
}

static void
_unset_cur_style(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Project *project = (Project *)data;
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
   evas_object_smart_callback_call(block, "wl,style,add", NULL);
}

static void
_del_style_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *block =  elm_object_parent_widget_get(evas_object_data_get((Evas_Object *)data, TABS_DATA_KEY));
   evas_object_smart_callback_call(block, "wl,style,del", NULL);
}

static void
_del_part_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   evas_object_smart_callback_call(tabs, "wl,part,del", NULL);
}

static void
_add_part_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   evas_object_smart_callback_call(tabs, "wl,part,add", NULL);
}

static void
_above_part_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   evas_object_smart_callback_call(tabs, "wl,part,above", NULL);
}

static void
_past_part_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   evas_object_smart_callback_call(tabs, "wl,part,below", NULL);
}

static void
_part_reordered(Evas_Object *data,
                Evas_Object *obj __UNUSED__,
                Elm_Object_Item *item)
{
   Evas_Object *nf = (Evas_Object *)data;
   Evas_Object *tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   Part *part = NULL;
   Elm_Object_Item *rel = elm_genlist_item_next_get(item);
   elm_genlist_item_selected_set(item, true);
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
        elm_genlist_item_decorate_mode_set(dec_eoi, "eflete/mode", false);
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

   nf = evas_object_data_get(obj, NAVIFRAME_DATA_KEY);
   tabs = evas_object_data_get(nf, TABS_DATA_KEY);
   nf_widgets = evas_object_data_get(tabs, WIDGETS_NAVIFRAME_DATA_KEY);
   nf_layouts = evas_object_data_get(tabs, LAYOUTS_NAVIFRAME_DATA_KEY);
   _style = elm_object_item_data_get(glit);

   if ((_style->__type != STYLE) && (_style->__type != LAYOUT)) return;

   if ((nf == nf_widgets) && (eina_list_count(elm_naviframe_items_get(nf_layouts)) == 2))
      _navi_gl_parts_pop(nf_layouts, NULL, NULL);
   else if ((nf == nf_layouts) && (eina_list_count(elm_naviframe_items_get(nf_widgets)) == 3))
      _navi_gl_parts_pop(nf_widgets, NULL, NULL);

   clicked_style = _style;
   if (_style->isAlias)
     {
       if (!_style->main_group) return;
        _style = _style->main_group;
     }

   parts = _style->parts;

   evas_object_smart_callback_call(tabs, "wl,style,select", clicked_style);

   if (!_itc_part)
     {
        _itc_part = elm_genlist_item_class_new();
        _itc_part->item_style = "eflete/level4";
        _itc_part->func.text_get = _item_part_label_get;
        _itc_part->func.content_get = _item_part_content_get;
        _itc_part->func.state_get = NULL;
        _itc_part->func.del = NULL;
     }

   gl_parts = elm_genlist_add(nf);
   elm_genlist_longpress_timeout_set(gl_parts, 0.2);
   evas_object_data_set(gl_parts, NAVIFRAME_DATA_KEY, nf);
   evas_object_data_set(gl_parts, TABS_DATA_KEY, tabs);
   elm_object_style_set(gl_parts, "eflete/dark");
   elm_genlist_reorder_mode_set(gl_parts, true);
   elm_genlist_select_mode_set(gl_parts, ELM_OBJECT_SELECT_MODE_ALWAYS);
   pr->current_style = _style;
   evas_object_smart_callback_add(tabs, "wl,part,back", _unset_cur_style, pr);
   evas_object_size_hint_align_set(gl_parts, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_parts, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   EINA_INLIST_FOREACH(parts, _part)
     {
        eoi = elm_genlist_item_append(gl_parts, _itc_part, _part,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      _on_part_select, nf);
        elm_object_item_data_set(eoi, _part);
     }
   evas_object_smart_callback_add(gl_parts, "moved",
                                  (Evas_Smart_Cb)_part_reordered, nf);

   ICON_ADD(nf, ic, false, "icon-back");

   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   evas_object_smart_callback_add(bt, "clicked", _navi_gl_parts_pop, nf);
   elm_object_style_set(bt, "eflete/simple");
   evas_object_show(gl_parts);

   elm_naviframe_item_push(nf, clicked_style->full_group_name, bt, NULL, gl_parts, NULL);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, "icon-add");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add(button, "clicked", _add_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt3", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, "icon-remove");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _del_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt2", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, "icon-slideup");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _above_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt1", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, "icon-slidedown");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _past_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt0", button);
}

static Evas_Object *
_item_style_content_get(void *data,
                       Evas_Object *obj,
                       const char *part)
{
   Style *_style = (Style *)data;
   if ((!strcmp(part, "elm.swallow.end")) && (_style->isAlias))
     {
        Evas_Object *icon = NULL;
        GET_IMAGE(icon, obj, "alias_link");
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        return icon;
     }
   return NULL;
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

   if (_style->__type == LAYOUT)
     {
        _on_style_clicked_double(data, obj, event_info);
        return;
     }
   _style = NULL;

   nf = evas_object_data_get(obj, NAVIFRAME_DATA_KEY);
   _widget = elm_object_item_data_get(glit);
   classes = _widget->classes;

   if (!_itc_class)
     {
        _itc_class = elm_genlist_item_class_new();
        _itc_class->item_style = "eflete/level2";
        _itc_class->func.text_get = _item_class_label_get;
        _itc_class->func.content_get = NULL;
        _itc_class->func.state_get = NULL;
        _itc_class->func.del = NULL;
     }

   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "eflete/level3";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = _item_style_content_get;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }

   gl_class = elm_genlist_add(nf);
   evas_object_data_set(gl_class, NAVIFRAME_DATA_KEY, nf);
   elm_object_style_set(gl_class, "eflete/dark");
   elm_genlist_select_mode_set(gl_class, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_align_set(gl_class, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_class, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   EINA_INLIST_FOREACH(classes, _class)
     {
        glit_class = elm_genlist_item_append(gl_class, _itc_class, _class,
                                             NULL, ELM_GENLIST_ITEM_NONE,
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

   ICON_ADD(nf, ic, false, "icon-back");
   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "eflete/simple");
   evas_object_smart_callback_add(bt, "clicked", _navi_gl_styles_pop, nf);
   elm_naviframe_item_push(nf, _widget->name, bt, NULL, gl_class, NULL);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, "icon-remove");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _del_style_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt0", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, "icon-add");
   evas_object_size_hint_align_set(button, -1, EVAS_HINT_FILL);
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _add_style_cb, obj);
   elm_object_style_set(button, "eflete/simple");
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

   if (gl_classes) elm_genlist_clear(gl_classes);
   else return false;
   if (!classes) return false;

   if (!_itc_class)
     {
        _itc_class = elm_genlist_item_class_new();
        _itc_class->item_style = "eflete/level2";
        _itc_class->func.text_get = _item_class_label_get;
        _itc_class->func.content_get = NULL;
        _itc_class->func.state_get = NULL;
        _itc_class->func.del = NULL;
     }

   if (!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "eflete/level3";
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

   if (!parent) return NULL;

   if (!_itc_widget)
     {
        _itc_widget = elm_genlist_item_class_new();
        _itc_widget->item_style = "eflete/level1";
        _itc_widget->func.text_get = _item_widget_label_get;
        _itc_widget->func.content_get = NULL;
        _itc_widget->func.state_get = NULL;
        _itc_widget->func.del = NULL;
     }
   if (!_itc_layout)
     {
        _itc_layout = elm_genlist_item_class_new();
        _itc_layout->item_style = "eflete/level1";
        _itc_layout->decorate_item_style = "eflete/mode";
        _itc_layout->func.text_get = _item_layout_label_get;
        _itc_layout->func.content_get = _item_style_content_get;
        _itc_layout->func.state_get = NULL;
        _itc_layout->func.del = NULL;
     }
   tabs = ewe_tabs_add(parent);
   widgets_tab = ewe_tabs_item_append(tabs, NULL, "Themes", NULL);
   layouts_tab = ewe_tabs_item_append(tabs, NULL, "Layouts", NULL);

#define NAVI(TYPE, TEXT) \
   nf_##TYPE = elm_naviframe_add(tabs); \
   ewe_tabs_item_content_set(tabs, TYPE##_tab, nf_##TYPE); \
   evas_object_size_hint_align_set(nf_##TYPE, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(nf_##TYPE, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_object_style_set(nf_##TYPE, "eflete"); \
    \
   gl_##TYPE = elm_genlist_add(nf_##TYPE); \
   elm_object_style_set(gl_##TYPE, "eflete/dark"); \
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
   ICON_ADD(nf_layouts, ic, false, "icon-add");
   bt = elm_button_add(nf_layouts);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "eflete/simple");
   evas_object_show(bt);
   evas_object_data_set(bt, WIDGETLIST_DATA_KEY, tabs);
   evas_object_smart_callback_add (bt, "clicked", _layout_add_cb, parent);
   elm_object_item_part_content_set(it_layouts, "elm.swallow.bt1", bt);
   ICON_ADD(nf_layouts, ic, false, "icon-remove");
   bt = elm_button_add(nf_layouts);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "eflete/simple");
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

   if ((!object) || (!title)) return false;

   item_gl_widgets = elm_naviframe_bottom_item_get(evas_object_data_get(object,
                                                   WIDGETS_NAVIFRAME_DATA_KEY));
   elm_object_item_part_text_set(item_gl_widgets, "elm.text.title", title);
   item_gl_widgets = elm_naviframe_bottom_item_get(evas_object_data_get(object,
                                                   LAYOUTS_NAVIFRAME_DATA_KEY));
   elm_object_item_part_text_set(item_gl_widgets, "elm.text.title", title);

   return true;
}

static void
_expand_request_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, true);
}

static void
_contract_request_cb(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, false);
}

static void
_wl_expanded_cb(void *data, Evas_Object *obj, void *event_info)
{
   Style *_layout;
   Widget *_widget;
   Project *project = (Project *)data;
   Elm_Object_Item *eoi;
   Elm_Object_Item *tree_main = event_info;
   Evas_Object *gl_widgets = obj;
   char *type = NULL;

   type = elm_object_item_data_get(tree_main);
   if (!strcmp("Widgets", type))
     {
      EINA_INLIST_FOREACH(project->widgets, _widget)
        {
          eoi = elm_genlist_item_append(gl_widgets, _itc_widget, _widget,
                                        tree_main, ELM_GENLIST_ITEM_NONE,
                                        _wl_item_selected, NULL);
          elm_object_item_data_set(eoi, _widget);
        }
     }
   else
    {
       EINA_INLIST_FOREACH(project->layouts, _layout)
         {
            eoi = elm_genlist_item_append(gl_widgets, _itc_layout, _layout,
                                          tree_main, ELM_GENLIST_ITEM_NONE,
                                          _wl_item_selected, NULL);
            elm_object_item_data_set(eoi, _layout);
         }
    }
   elm_genlist_item_selected_set(tree_main, false);
}

static void
_wl_contracted_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

/* TODO: fix renaming
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
                  elm_genlist_item_decorate_mode_set(dec_eoi, "eflete/mode",
                                                     false);
                  elm_genlist_item_update(dec_eoi);
               }
             elm_genlist_item_decorate_mode_set(eoi, "eflete/mode", true);
          }
        return;
     }
   if ((!strcmp(ev->key, "Escape")) && (dec_eoi))
     {
        elm_genlist_item_decorate_mode_set(dec_eoi, "eflete/mode", false);
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
   if ((!gl_layouts) || (!project)) return false;

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

   if ((!object) || (!project)) return false;

   widget_list = project->widgets;
   nf_widgets = evas_object_data_get(object, WIDGETS_NAVIFRAME_DATA_KEY);
   gl_widgets = elm_object_item_part_content_get(_widget_list_get(nf_widgets),
                                                 "elm.swallow.content");
   elm_genlist_select_mode_set(gl_widgets, ELM_OBJECT_SELECT_MODE_ALWAYS);

   nf_layouts = evas_object_data_get(object, LAYOUTS_NAVIFRAME_DATA_KEY);
   gl_layouts = elm_object_item_part_content_get(_widget_list_get(nf_layouts),
                                                 "elm.swallow.content");
   elm_genlist_select_mode_set(gl_layouts, ELM_OBJECT_SELECT_MODE_ALWAYS);


   wm_widget_list_objects_load(widget_list,
                               evas_object_evas_get(gl_widgets),
                               project->dev);
   EINA_INLIST_FOREACH(widget_list, _widget)
     {
        eoi = elm_genlist_item_append(gl_widgets, _itc_widget, _widget,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      _wl_item_selected, NULL);
        elm_object_item_data_set(eoi, _widget);
     }

   wm_layouts_list_objects_load(project->layouts,
                               evas_object_evas_get(gl_widgets),
                               project->dev);

   EINA_INLIST_FOREACH(project->layouts, _layout)
     {
        eoi = elm_genlist_item_append(gl_layouts, _itc_layout, _layout,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      _wl_item_selected, NULL);
        elm_object_item_data_set(eoi, _layout);
     }

#define CALLBACKS(TYPE) \
   evas_object_smart_callback_add(gl_##TYPE, "expand,request", \
                                  _expand_request_cb, gl_##TYPE); \
   evas_object_smart_callback_add(gl_widgets, "contract,request", \
                                  _contract_request_cb, gl_##TYPE); \
   evas_object_smart_callback_add(gl_##TYPE, "expanded", \
                                  _wl_expanded_cb, project); \
   evas_object_smart_callback_add(gl_##TYPE, "contracted", \
                                  _wl_contracted_cb, gl_##TYPE); \
   evas_object_smart_callback_add(gl_##TYPE, "clicked,double", \
                                  _on_widget_clicked_double, project);

/* TODO: fix renaming
   evas_object_event_callback_add(gl_##TYPE, EVAS_CALLBACK_KEY_DOWN, \
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

   if ((!object) || (!style) || (!name)) return false;
   part = wm_part_add(style, name);

   if (!part) return false;
   nf = _current_naviframe_get(object);
   gl_parts = elm_object_item_part_content_get(_widget_list_get(nf),
                                               "elm.swallow.content");
   eoi = elm_genlist_item_append(gl_parts, _itc_part, part, NULL,
                                 ELM_GENLIST_ITEM_NONE, _on_part_select,
                                 nf);
   elm_object_item_data_set(eoi, part);
   elm_genlist_item_selected_set(eoi, EINA_TRUE);

   return true;
}

Eina_Bool
ui_widget_list_selected_part_del(Evas_Object *object, Style *style)
{
   Evas_Object *gl_parts;
   Elm_Object_Item *eoi, *next_eoi;
   Part *part;

   if ((!object) || (!style)) return false;

   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);

   if (!eoi)
     {
        NOTIFY_INFO(3, _("No part selected"));
        return false;
     }
   part = (Part *)elm_object_item_data_get(eoi);
   wm_part_del(style, part);

   next_eoi = elm_genlist_item_next_get(eoi);
   if (!next_eoi) next_eoi = elm_genlist_item_prev_get(eoi);
   if (next_eoi) elm_genlist_item_selected_set(next_eoi, true);
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
   Part *part = NULL;
   Eina_Inlist *prev_elm =  NULL;
   Eina_Inlist *next_elm =  NULL;
   Eina_Inlist *itr = NULL;
   Part *_part = NULL;
   Evas_Object *nf;

   if ((!object) || (!style)) return false;
   nf = _current_naviframe_get(object);
   gl_parts = elm_object_item_part_content_get(_widget_list_get(nf),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);
   if (!eoi)
     {
        NOTIFY_INFO(3, _("No part selected"));
        return false;
     }
   if (move_up)
     {
        prev_eoi = elm_genlist_item_prev_get(eoi);
        if (!prev_eoi)
          {
             NOTIFY_INFO(3, _("Selected part is currently on top of the list"));
             return false;
          }
     }
   else
     {
        next_eoi = elm_genlist_item_next_get(eoi);
        if (!next_eoi)
          {
             NOTIFY_INFO(3, _("Selected part is currently on bottom of the list"));
             return false;
          }
     }
   part = elm_object_item_data_get(eoi);
   new_eoi = (move_up) ? elm_genlist_item_insert_before(gl_parts, _itc_part, part, NULL,
                         prev_eoi, elm_genlist_item_type_get(eoi), _on_part_select, nf):
                         elm_genlist_item_insert_after(gl_parts, _itc_part, part, NULL,
                         next_eoi, elm_genlist_item_type_get(eoi),  _on_part_select, nf);
   eoi = elm_genlist_selected_item_get(gl_parts);
   elm_object_item_del(eoi);
   elm_genlist_item_selected_set(new_eoi, EINA_TRUE);

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

   if (!object) return NULL;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);
   if (!eoi) return NULL;
   part = (Part *)elm_object_item_data_get(eoi);
   if (!part) return NULL;

   return part;
}

Eina_Bool
ui_widget_list_part_selected_set(Evas_Object *object,
                                 const char *part,
                                 Eina_Bool selected)
{
   Evas_Object *gl_parts;

   if ((!object) || (!part)) return false;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");
   Elm_Object_Item *item = _genlist_find_item_by_name(gl_parts, part);
   if (item)
     {
        elm_genlist_item_selected_set(item, selected);
        elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
     }
   else return false;

   return true;
}

Eina_Bool
ui_widget_list_part_update(Evas_Object *object, const char *part)
{
   Evas_Object *gl_parts;
   Elm_Object_Item *item;

   if ((!object) || (!part)) return false;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");
   item = _genlist_find_item_by_name(gl_parts, part);
   if (item) elm_genlist_item_update(item);
   else return false;

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

   if (!object) return NULL;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(_current_naviframe_get(object)),
                                               "elm.swallow.content");
   selected = (Eina_List *)elm_genlist_selected_items_get(gl_parts);
   if (!selected) return NULL;

   EINA_LIST_FOREACH(selected, l, eoi)
     {
        part = (Part *)elm_object_item_data_get(eoi);
        parts = eina_list_append(parts, part);
     }

   return parts;
}
