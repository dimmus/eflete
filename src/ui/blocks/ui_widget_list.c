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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "ui_widget_list.h"
#include "widget_manager.h"

static Elm_Genlist_Item_Class *_itc_widget = NULL;
static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;

static inline Elm_Object_Item *
_widget_list_get(Evas_Object *naviframe)
{
   Elm_Object_Item *item_gl_widgets = elm_naviframe_top_item_get(naviframe);

   return item_gl_widgets;
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
   else if (!strcmp(part, "elm.text.sub"))
     {
        return strdup(wm_part_type_get(p->type));
     }
   return "";
}

static void
_on_icon_click(void *data,
               Evas_Object *obj,
               void *event_data __UNUSED__)
{
   Evas_Object *gl_parts, *nf;
   Part *_part = (Part *)data;
   _part->show = !_part->show;

   gl_parts = evas_object_data_get(obj, "gl_parts");
   nf = evas_object_data_get(gl_parts, "naviframe");
   DBG("%s", evas_object_type_get(gl_parts));

   if (_part->show)
     {
        elm_image_file_set(obj, TET_IMG_PATH"eye_open.png", NULL);
        evas_object_smart_callback_call(nf, "wl,part,show", (void *)_part->name);
     }
   else
     {
        elm_image_file_set(obj, TET_IMG_PATH"eye_close.png", NULL);
        evas_object_smart_callback_call(nf, "wl,part,hide", (void *)_part->name);
     }
}

static Evas_Object *
_item_part_content_get(void *data,
                       Evas_Object *obj,
                       const char *part)
{
   Part *_part = (Part *) data;
   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *icon = elm_icon_add(obj);
        if (_part->show)
          elm_image_file_set(icon, TET_IMG_PATH"eye_open.png", NULL);
        else
          elm_image_file_set(icon, TET_IMG_PATH"eye_close.png", NULL);
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_smart_callback_add(icon, "clicked", _on_icon_click, _part);
        evas_object_data_set(icon, "gl_parts", obj);
        return icon;
     }
   if (!strcmp(part, "elm.swallow.end"))
     {
        Evas_Object *icon = elm_icon_add(obj);
        if (_part->type == EDJE_PART_TYPE_RECTANGLE)
          elm_image_file_set(icon, TET_IMG_PATH"icon-rectangle.png", NULL);
        if (_part->type == EDJE_PART_TYPE_IMAGE)
          elm_image_file_set(icon, TET_IMG_PATH"icon-image.png", NULL);
        if (_part->type == EDJE_PART_TYPE_SPACER)
          elm_image_file_set(icon, TET_IMG_PATH"icon-spacer.png", NULL);
        if (_part->type == EDJE_PART_TYPE_SWALLOW)
          elm_image_file_set(icon, TET_IMG_PATH"icon-swallow.png", NULL);
        if (_part->type == EDJE_PART_TYPE_TEXT)
          elm_image_file_set(icon, TET_IMG_PATH"icon-text.png", NULL);
        if (_part->type == EDJE_PART_TYPE_TEXTBLOCK)
          elm_image_file_set(icon, TET_IMG_PATH"icon-textblock.png", NULL);
        if (_part->type == EDJE_PART_TYPE_GROUP)
          elm_image_file_set(icon, TET_IMG_PATH"icon-group.png", NULL);
        elm_image_resizable_set(icon, EINA_FALSE, EINA_FALSE);
        evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        return icon;
     }
   return NULL;
}

static char *
_item_group_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Group *g = (Group *)data;
   if (!g->group_name)
     {
        ERR("It impossible, but it is occurred, group name is missing!");
        return NULL;
     }
   return strdup(g->group_name);
}

static char *
_item_widget_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char *part __UNUSED__)
{
   Widget *w = (Widget *)data;
   if (!w->widget_name)
     {
        ERR("It impossible, but it is occurred, widget name is missing!");
        return NULL;
     }
   return strdup(w->widget_name);
}

static char *
_item_style_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Style *st = (Style *)data;
   if (!st->style_name)
     {
        ERR("It impossible, but it is occurred, style name is missing!");
        return NULL;
     }
   return strdup(st->style_name);
}

static void
_navi_gl_styles_pop(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   elm_naviframe_item_pop(nf);

   evas_object_smart_callback_call (nf, "wl,group,back", NULL);
}

static void
_navi_gl_parts_pop(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   elm_naviframe_item_pop(nf);

   evas_object_smart_callback_call(nf, "wl,part,back", NULL);
}

static void
_wl_part_add(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *eoi = NULL;
   Evas_Object *glist = (Evas_Object *)data;
   Part *part = (Part *)event_info;

   part->show = EINA_TRUE;
   eoi = elm_genlist_item_append(glist, _itc_part, part, NULL,
                                 ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_object_item_data_set(eoi, part);
   elm_genlist_item_selected_set(eoi, EINA_TRUE);
}

static void
_on_part_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Evas_Object *nf = (Evas_Object *)data;
   Part *_part = elm_object_item_data_get(glit);

   evas_object_smart_callback_call (nf, "wl,part,select", _part);
}
static void
_unset_cur_group(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Project *pr = (Project *)data;
   if (pr->current_group)
     evas_object_smart_callback_del(pr->current_group->obj, "edit_obj,part,add",
                                    _wl_part_add);
   pr->current_group = NULL;

}
static void
_add_style_unpress(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *block =  elm_object_parent_widget_get(data);
   evas_object_smart_callback_call(block, "gs,style,add", NULL);
}

static void
_del_style_unpress(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *block =  elm_object_parent_widget_get(data);
   evas_object_smart_callback_call(block, "gs,style,del", NULL);
}

static void
_del_part_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   evas_object_smart_callback_call(nf, "wl,part,del", NULL);
}

static void
_add_part_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   evas_object_smart_callback_call(nf, "wl,part,add", NULL);
}

static void
_above_part_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   evas_object_smart_callback_call(nf, "wl,part,above", NULL);
}

static void
_past_part_cb(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data;
   evas_object_smart_callback_call(nf, "wl,part,below", NULL);
}

static void
_on_group_clicked_double(void *data,
                         Evas_Object *obj,
                         void *event_info)
{
   Project *pr = (Project *)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Elm_Object_Item *eoi;
   Evas_Object *nf, *gl_parts, *bt, *ic;
   Eina_Inlist *parts;
   Group *_group;
   Part *_part;
   Evas_Object *button = NULL;
   Evas_Object *_icon = NULL;

   nf = evas_object_data_get(obj, "naviframe");
   _group = elm_object_item_data_get(glit);
   if (_group->__type != GROUP) return;
   if (_group->isAlias)
        _group = _group->main_group;

   parts = _group->parts;

   evas_object_smart_callback_call (nf, "wl,group,select", _group);

   if (!_itc_part)
     {
        _itc_part = elm_genlist_item_class_new();
        _itc_part->item_style = "small/double_label";
        _itc_part->func.text_get = _item_part_label_get;
        _itc_part->func.content_get = _item_part_content_get;
        _itc_part->func.state_get = NULL;
        _itc_part->func.del = NULL;
     }

   gl_parts = elm_genlist_add(nf);
   elm_object_style_set(gl_parts, DEFAULT_STYLE);
   pr->current_group = _group;
   evas_object_smart_callback_add(nf, "wl,part,back", _unset_cur_group, pr);
   evas_object_size_hint_align_set(gl_parts,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_parts,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   EINA_INLIST_FOREACH(parts, _part)
     {
        eoi = elm_genlist_item_append(gl_parts, _itc_part,
                                      _part,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      NULL, NULL);
        elm_object_item_data_set(eoi, _part);
     }
   evas_object_smart_callback_add(gl_parts, "selected", _on_part_select, nf);
   evas_object_data_set(gl_parts, "naviframe", nf);

   ICON_ADD(nf, ic, false, TET_IMG_PATH"icon-back.png");

   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   evas_object_smart_callback_add(bt, "clicked", _navi_gl_parts_pop, nf);
   elm_object_style_set(bt, "eflete/default");
   evas_object_show(gl_parts);

   elm_naviframe_item_push(nf, _group->full_group_name, bt, NULL, gl_parts, NULL);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, TET_IMG_PATH"icon-add.png");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add(button, "clicked", _add_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt3", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, TET_IMG_PATH"icon-remove.png");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _del_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt2", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, TET_IMG_PATH"icon-slideup.png");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _above_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt1", button);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, TET_IMG_PATH"icon-slidedown.png");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "clicked", _past_part_cb, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt0", button);
}

static Evas_Object *
_item_group_content_get(void *data,
                       Evas_Object *obj,
                       const char *part __UNUSED__)
{
   Group *_group = (Group *) data;
   if ((!strcmp(part, "elm.swallow.end")) && (_group->isAlias))
     {
        Evas_Object *icon = elm_icon_add(obj);
        elm_image_file_set(icon, TET_IMG_PATH"alias_link.png", NULL);
        elm_image_resizable_set(icon, EINA_FALSE, EINA_FALSE);
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
   Elm_Object_Item *glit_style, *glit_group;
   Evas_Object *nf, *gl_styles, *bt, *ic;
   Eina_Inlist *styles, *groups;
   Widget *_widget;
   Style *_style;
   Group *_group;
   Evas_Object *button = NULL;
   Evas_Object *_icon = NULL;

   nf = evas_object_data_get(obj, "naviframe");
   _widget = elm_object_item_data_get(glit);
   styles = _widget->styles;


   if(!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "eflete/default";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = NULL;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }

   if (!_itc_group)
     {
        _itc_group = elm_genlist_item_class_new();
        _itc_group->item_style = "eflete/default";
        _itc_group->func.text_get = _item_group_label_get;
        _itc_group->func.content_get = _item_group_content_get;
        _itc_group->func.state_get = NULL;
        _itc_group->func.del = NULL;
     }

   gl_styles = elm_genlist_add(nf);
   evas_object_data_set(gl_styles, "naviframe", nf);
   elm_object_style_set(gl_styles, "eflete/default");
   evas_object_size_hint_align_set(gl_styles,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_styles,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   EINA_INLIST_FOREACH(styles, _style)
     {
        glit_style = elm_genlist_item_append(gl_styles, _itc_style,
                                             _style,
                                             NULL, ELM_GENLIST_ITEM_NONE,
                                             NULL, NULL);
        groups = _style->groups;

        EINA_INLIST_FOREACH(groups, _group)
          {
             glit_group = elm_genlist_item_append(gl_styles, _itc_group,
                             _group, glit_style, ELM_GENLIST_ITEM_NONE,
                             NULL, NULL);
             elm_object_item_data_set(glit_group, _group);
          }
     }

   ICON_ADD(nf, ic, false, TET_IMG_PATH"icon-back.png");
   bt = elm_button_add(nf);
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "eflete/default");
   evas_object_smart_callback_add(bt, "clicked", _navi_gl_styles_pop, nf);
   evas_object_smart_callback_add(gl_styles, "clicked,double",
                                  _on_group_clicked_double, data);

   elm_naviframe_item_push(nf, _widget->widget_name, bt, NULL, gl_styles, NULL);

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, TET_IMG_PATH"icon-remove.png");
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "unpressed", _del_style_unpress, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt0", button);
   /* temporary solution, while deleting aliases is not working. */
   elm_object_disabled_set(button, true);
   /*************************************************************/

   BUTTON_ADD(nf, button, NULL)
   ICON_ADD(button, _icon, true, TET_IMG_PATH"icon-add.png");
   evas_object_size_hint_align_set(button, -1, EVAS_HINT_FILL);
   elm_object_part_content_set(button, NULL, _icon);
   evas_object_smart_callback_add (button, "unpressed", _add_style_unpress, nf);
   elm_object_style_set(button, "eflete/simple");
   elm_object_part_content_set(nf, "elm.swallow.bt1", button);
   evas_object_show(gl_styles);
}

void
ui_widget_list_style_data_reload(Evas_Object *gl_styles, Eina_Inlist *styles)
{
   Style *_style = NULL;
   Group *_group = NULL;
   Eina_Inlist *groups = NULL;
   Elm_Object_Item *glit_style = NULL;
   Elm_Object_Item *glit_group = NULL;

   if((!gl_styles) || (!styles)) return;
   elm_genlist_clear(gl_styles);

   if(!_itc_style)
     {
        _itc_style = elm_genlist_item_class_new();
        _itc_style->item_style = "eflete/default";
        _itc_style->func.text_get = _item_style_label_get;
        _itc_style->func.content_get = NULL;
        _itc_style->func.state_get = NULL;
        _itc_style->func.del = NULL;
     }

   if (!_itc_group)
     {
        _itc_group = elm_genlist_item_class_new();
        _itc_group->item_style = "eflete/default";
        _itc_group->func.text_get = _item_group_label_get;
        _itc_group->func.content_get = NULL;
        _itc_group->func.state_get = NULL;
        _itc_group->func.del = NULL;
     }

   EINA_INLIST_FOREACH(styles, _style)
     {
        glit_style = elm_genlist_item_append(gl_styles, _itc_style,
                                             _style,
                                             NULL, ELM_GENLIST_ITEM_NONE,
                                             NULL, NULL);
        groups = _style->groups;

        EINA_INLIST_FOREACH(groups, _group)
          {
             glit_group = elm_genlist_item_append(gl_styles, _itc_group,
                             _group, glit_style, ELM_GENLIST_ITEM_NONE,
                             NULL, NULL);
             elm_object_item_data_set(glit_group, _group);
          }
     }
}

Evas_Object *
ui_widget_list_add(Evas_Object *parent)
{
   Evas_Object *nf;
   Evas_Object *gl_widgets = NULL;
   Elm_Object_Item *it;

   if (!_itc_widget)
     {
        _itc_widget = elm_genlist_item_class_new();
        _itc_widget->item_style = "eflete/default";
        _itc_widget->func.text_get = _item_widget_label_get;
        _itc_widget->func.content_get = NULL;
        _itc_widget->func.state_get = NULL;
        _itc_widget->func.del = NULL;
     }

   nf = elm_naviframe_add(parent);
   evas_object_size_hint_align_set(nf, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_style_set(nf, "eflete");

   gl_widgets = elm_genlist_add(nf);
   elm_object_style_set(gl_widgets, "eflete/default");
   evas_object_size_hint_align_set(gl_widgets,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_widgets,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   evas_object_data_set(gl_widgets, "naviframe", nf);

   it = elm_naviframe_item_push(nf, NULL, NULL, NULL, gl_widgets, NULL);
   elm_object_item_part_text_set(it, "subtitle", "Widget list");

   return nf;
}

void
ui_widget_list_title_set(Evas_Object *object, const char *title)
{
   Elm_Object_Item *item_gl_widgets;

   if (!object) return;

   item_gl_widgets = _widget_list_get(object);
   elm_object_item_part_text_set(item_gl_widgets, "elm.text.title", title);
}

Eina_Bool
ui_widget_list_data_set(Evas_Object *object, Project *project)
{
   Widget *_widget;
   Elm_Object_Item *eoi;
   Evas_Object *gl_widgets;
   Eina_Inlist *widget_list = project->widgets;

   if ((!object) || (!widget_list)) return EINA_FALSE;

   gl_widgets = elm_object_item_part_content_get(_widget_list_get(object),
                                                 "elm.swallow.content");
   wm_widget_list_objects_load(widget_list,
                               evas_object_evas_get(gl_widgets),
                               project->swapfile);

   EINA_INLIST_FOREACH(widget_list, _widget)
     {
        eoi = elm_genlist_item_append(gl_widgets, _itc_widget,
                                      _widget,
                                      NULL, ELM_GENLIST_ITEM_NONE,
                                      NULL, NULL);
        /* maybe add a ref to _widget */
        elm_object_item_data_set(eoi, _widget);
     }

   evas_object_smart_callback_add(gl_widgets, "clicked,double",
                                  _on_widget_clicked_double, project);

   return EINA_TRUE;
}

Eina_Bool
ui_widget_list_part_add(Evas_Object *object, Group *group, const char *name)
{
   Evas_Object *gl_parts;
   Elm_Object_Item *eoi;
   Part *part;

   if ((!object) || (!group) || (!name)) return false;
   part = wm_part_add(group, name);

   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
                                               "elm.swallow.content");
   eoi = elm_genlist_item_append(gl_parts, _itc_part, part, NULL,
                                 ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_object_item_data_set(eoi, part);
   elm_genlist_item_selected_set(eoi, EINA_TRUE);

   return false;
}

Eina_Bool
ui_widget_list_selected_part_del(Evas_Object *object, Group *group)
{
   Evas_Object *gl_parts;
   Elm_Object_Item *eoi, *next_eoi;
   Part *part;

   if ((!object) || (!group)) return false;

   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);

   if (!eoi)
     {
        WARN("None one part does'nt selected");
        return false;
     }
   part = (Part *)elm_object_item_data_get(eoi);
   wm_part_del(group, part);

   next_eoi = elm_genlist_item_next_get(eoi);
   if (!next_eoi) next_eoi = elm_genlist_item_prev_get(eoi);
   elm_genlist_item_selected_set(next_eoi, true);
   elm_object_item_del(eoi);

   return true;
}

Eina_Bool
ui_widget_list_selected_part_above(Evas_Object *object, Group *group)
{
   Evas_Object *gl_parts = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *new_eoi = NULL;
   Elm_Object_Item *prev_eoi = NULL;
   Part *part = NULL;
   Eina_Inlist *prev_elm =  NULL;
   Eina_Inlist *itr = NULL;
   Part *_part = NULL;


   if ((!object) || (!group)) return false;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);
   if (!eoi)
     {
        NOTIFY_INFO(3, "None one part does'nt selected");
        return false;
     }
   prev_eoi = elm_genlist_item_prev_get(eoi);
   if (!prev_eoi)
     {
        NOTIFY_INFO(3, "Selected part currently on top in list");
        return false;
     }
   part = elm_object_item_data_get(eoi);
   new_eoi = elm_genlist_item_insert_before(gl_parts, _itc_part, part, NULL,
                         prev_eoi, elm_genlist_item_type_get(eoi), NULL, NULL);
   eoi = elm_genlist_selected_item_get(gl_parts);
   elm_object_item_del(eoi);
   elm_genlist_item_selected_set(new_eoi, EINA_TRUE);

   for(itr = group->parts; itr != NULL; itr = itr->next)
     {
        _part = EINA_INLIST_CONTAINER_GET(itr, Part);
        if (_part == part)
          {
             prev_elm = itr->prev;
             group->parts = eina_inlist_remove(group->parts, itr);
             group->parts = eina_inlist_prepend_relative(group->parts,
                                                         itr, prev_elm);
             break;
          }
     }

   return true;
}

Eina_Bool
ui_widget_list_selected_part_below(Evas_Object *object, Group *group)
{

   Evas_Object *gl_parts = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *new_eoi = NULL;
   Elm_Object_Item *next_eoi = NULL;
   Part *part = NULL;
   Eina_Inlist *next_elm =  NULL;
   Eina_Inlist *itr = NULL;
   Part *_part = NULL;

   if ((!object) || (!group)) return false;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);
   if (!eoi)
     {
        NOTIFY_INFO(3, "None one part does'nt selected");
        return false;
     }
   next_eoi = elm_genlist_item_next_get(eoi);
   if (!next_eoi)
     {
        NOTIFY_INFO(3, "Selected part currently on bottom in list");
        return false;
     }
   part = elm_object_item_data_get(eoi);
   new_eoi = elm_genlist_item_insert_after(gl_parts, _itc_part, part, NULL,
                         next_eoi, elm_genlist_item_type_get(eoi), NULL, NULL);
   eoi = elm_genlist_selected_item_get(gl_parts);
   elm_object_item_del(eoi);
   elm_genlist_item_selected_set(new_eoi, EINA_TRUE);

   for (itr = group->parts; itr != NULL; itr = itr->next)
     {
        _part = EINA_INLIST_CONTAINER_GET(itr, Part);
        if (_part == part)
          {
             next_elm = itr->next;
             group->parts = eina_inlist_remove(group->parts, itr);
             group->parts = eina_inlist_append_relative(group->parts,
                                                        itr, next_elm);
             break;
          }
     }
   return true;
}

Part *
ui_widget_list_selected_part_get(Evas_Object *object)
{
   Evas_Object *gl_parts;
   Part *part;
   Elm_Object_Item *eoi;

   if (!object) return NULL;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
                                               "elm.swallow.content");
   eoi = elm_genlist_selected_item_get(gl_parts);
   part = (Part *)elm_object_item_data_get(eoi);
   if (!part) return NULL;

   return part;
}

Eina_Bool
ui_widget_list_part_selected_set(Evas_Object *object, const char *part)
{
   Evas_Object *gl_parts;

   if (!object) return false;
   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
                                               "elm.swallow.content");
   Elm_Object_Item *item = _genlist_find_item_by_name(gl_parts, part);
   if (item)
     {
        elm_genlist_item_selected_set(item, true);
        elm_genlist_item_bring_in(item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
     }
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
   gl_parts = elm_object_item_part_content_get(_widget_list_get(object),
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

void
ui_widget_list_callback_add(Evas_Object *object,
                            const char *event,
                            Edje_Signal_Cb func,
                            void *data)
{
   if (!object) return;
   elm_object_signal_callback_add(object, event, "", func, data);
}
