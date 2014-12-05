/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "style_dialog.h"
#include "common_macro.h"
#include "string_macro.h"

static Evas_Object *entry_class;
static Evas_Object *entry_style;

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = STYLE_NAME_BANNED_SYMBOLS
};

static void
_reload_classes(App_Data *ap, Eina_Inlist *classes)
{
   Evas_Object *gl_classes = NULL;
   const Evas_Object *nf = NULL;
   Elm_Object_Item *eoi = NULL;
   Ewe_Tabs_Item *class_tab = NULL;
   Evas_Object *tabs = NULL;

   tabs = ui_block_widget_list_get(ap);
   class_tab = ewe_tabs_active_item_get(tabs);
   nf = ewe_tabs_item_content_get(tabs, class_tab);
   eoi = elm_naviframe_top_item_get(nf);
   gl_classes = elm_object_item_part_content_get(eoi, NULL);

   ui_widget_list_class_data_reload(gl_classes, classes);
}

int
_sort_class_add_cb(const void *data1, const void *data2)
{
   Style *style_1 = NULL;
   Style *style_2 = NULL;
   int cmp = 0;

   if (!data1) return 1;
   if (!data2) return -1;

   style_1 = EINA_INLIST_CONTAINER_GET(data1, Style);
   style_2 = EINA_INLIST_CONTAINER_GET(data2, Style);

   cmp = strcmp(style_1->name, style_2->name);

   if (cmp < 0) return -1;
   if (cmp > 0) return 1;
   return 0;
}

static void
_job_popup_close(void *data)
{
   App_Data *ap = (App_Data *)data;
   evas_object_del(ap->popup);
   ap->popup = NULL;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_popup_close_cb(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   ecore_job_add(_job_popup_close, data);
}

static void
_on_popup_btn_yes(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Eina_Stringshare *style_name = NULL;
   Eina_Stringshare *class_name = NULL;
   Eina_Stringshare *full_name = NULL;

   Widget *dest_wdg = NULL;
   Class  *dest_class = NULL;
   Style  *dest_style = NULL;

   Widget *source_wdg = NULL;
   Class  *source_class = NULL;
   Style  *source_style = NULL;

   Widget *widget = NULL;
   Style *style = NULL;

   Eina_Inlist *l =NULL;
   Eina_List *styles = NULL;

#define STRING_CLEAR\
        eina_stringshare_del(style_name);\
        eina_stringshare_del(class_name);\


   widget = ui_widget_from_ap_get(ap);
   if (!widget) return;
   style_name = eina_stringshare_add(elm_entry_entry_get(entry_style));
   class_name = eina_stringshare_add(elm_entry_entry_get(entry_class));

   if (eina_stringshare_strlen(style_name) <= 0)
     {
        NOTIFY_WARNING(_("Please type style name"));
        STRING_CLEAR;
        return;
     }

   if (eina_stringshare_strlen(class_name) <= 0)
     {
        NOTIFY_WARNING(_("Please type class name"));
        STRING_CLEAR;
        return;
     }

   full_name = eina_stringshare_printf("elm/%s/%s/%s", widget->name, class_name,
                                       style_name);

   if (wm_style_object_find(ap->project->widgets, full_name))
     {
        NOTIFY_WARNING(_("[%s] alredy exist. Type another style name."),
                      full_name);
        STRING_CLEAR;
        eina_stringshare_del(full_name);
        return;
     }

   EINA_INLIST_FOREACH_SAFE(ap->project->widgets, l, source_wdg)
     {
        if (!strcmp(source_wdg->name, widget->name)) break;
     }

   if (!source_wdg)
     {
        NOTIFY_ERROR(_("Sorry, there are no templates for [%s]"), widget->name);
        ecore_job_add(_job_popup_close, ap);
        STRING_CLEAR;
        eina_stringshare_del(full_name);
        return;
     }
   EINA_INLIST_FOREACH_SAFE(ap->project->widgets, l, dest_wdg)
     {
        if (!strcmp(dest_wdg->name, widget->name)) break;
     }

   if (!dest_wdg)
     {
        STRING_CLEAR;
        eina_stringshare_del(full_name);
        return;
     }

   /* Trying search same class in template with default style */
   EINA_INLIST_FOREACH_SAFE(source_wdg->classes, l, source_class)
     {
        if (!strcmp(source_class->name, class_name))
          {
             EINA_INLIST_FOREACH_SAFE(source_class->styles, l, source_style)
                {
                   if (!strcmp(source_style->name, "default")) break;
                   source_style = NULL;
                }
             break;
          }
        source_class = NULL;
     }

   /* If not found same class in template, trying search base/default style
    * for widget.*/
   if (!source_class)
     {
        EINA_INLIST_FOREACH_SAFE(source_wdg->classes, l, source_class)
          {
             if (!strcmp(source_class->name, "base"))
               {
                  EINA_INLIST_FOREACH_SAFE(source_class->styles, l, source_style)
                     {
                        if (!strcmp(source_style->name, "default")) break;
                        source_style = NULL;
                    }
                  break;
               }
             source_class = NULL;
          }
     }

   /* If failed get base/default style from template file, get first style in class*/
   if (!source_style)
     {
        if (!source_class)
          source_class = EINA_INLIST_CONTAINER_GET(source_wdg->classes,
                                                    Class);
        source_style = EINA_INLIST_CONTAINER_GET(source_class->styles,
                                                    Style);
        if ((!source_style) || (source_style->isAlias))
          {
             STRING_CLEAR;
             eina_stringshare_del(full_name);
             return;
          }
     }


    /* Trying search class in widget from current loaded theme. If that class
     * does'nt exist, will create new class with name, which user type.       */
   EINA_INLIST_FOREACH_SAFE(widget->classes, l, dest_class)
    {
       if (!strcmp(dest_class->name, class_name)) break;
       dest_class = NULL;
    }

   /* Trying create new class*/
   if (!dest_class)
     {
        dest_style = EINA_INLIST_CONTAINER_GET(
                        EINA_INLIST_CONTAINER_GET(widget->classes, Class)->styles,
                        Style);
        styles = eina_list_append(styles, full_name);
        dest_class = wm_class_add(class_name, styles, dest_wdg);
        dest_wdg->classes = eina_inlist_append(dest_wdg->classes,
                                                  EINA_INLIST_GET(dest_class));
        style = EINA_INLIST_CONTAINER_GET(dest_class->styles, Style);
     }
   else
     {
        style = wm_style_add(style_name, full_name, STYLE, dest_class);
        dest_class->styles = eina_inlist_append(dest_class->styles,
                                                EINA_INLIST_GET(style));
     }

   if (!dest_style)
       dest_style = EINA_INLIST_CONTAINER_GET(dest_class->styles, Style);


   if (dest_style->isAlias) dest_style = dest_style->main_group;
   /* call method, which copy all parts and their params into new style */
   if (wm_style_copy(dest_style->obj, source_style->full_group_name, full_name,
                     ap->project->dev, style))
     {
        wm_widgets_list_objects_load(ap->project->widgets,
                                     evas_object_evas_get(ap->win),
                                     ap->project->dev);
        _reload_classes(ap, dest_wdg->classes);
        style->isModify = true;
     }

   project_changed();

   STRING_CLEAR;
   eina_stringshare_del(full_name);
   ecore_job_add(_job_popup_close, ap);
#undef STRING_CLEAR
}

/* FIXME: change name to class_dialog_add */
Eina_Bool
style_dialog_add(App_Data *ap)
{
   Evas_Object *box, *item, *button;
   Widget *widget = NULL;
   Class *class_st = NULL;
   Eina_Stringshare *title = NULL;
   Eina_Stringshare *entry_text = NULL;
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = NULL;

   if (!ap) return false;
   widget = ui_widget_from_ap_get(ap);
   if (!widget) return false;

   /* Checking if the source style is an alias.
      We can't clone aliases right now, it need lots of difficult code for that.
   */
   nf = ui_block_widget_list_get(ap);
   nf = evas_object_data_get(nf, "nf_widgets");
   nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                         "elm.swallow.content");
   glit = elm_genlist_selected_item_get(nf);
   if (glit)
     {
        _style = elm_object_item_data_get(glit);
        if (_style->isAlias) return false;
     }

   title = eina_stringshare_printf(_("Add style/class for \"%s\" widget"),
                                   widget->name);
   ap->popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(ap->popup, "eflete");
   elm_object_part_text_set(ap->popup, "title,text", title);
   elm_popup_orient_set(ap->popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(ap->popup, box, false, false);

   ITEM_ADD(box, item, "Style name:", "eflete/property/item/default")
   EWE_ENTRY_ADD(item, entry_style, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_style, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry_style, "guide", _("Type a new style name."));
   elm_object_part_content_set(item, "elm.swallow.content", entry_style);
   elm_box_pack_end(box, item);

   class_st = ui_class_from_ap_get(ap);
   if (!class_st) return false;
   entry_text = eina_stringshare_add(class_st->name);

   ITEM_ADD(box, item, "Class name:", "eflete/property/item/default")
   EWE_ENTRY_ADD(box, entry_class, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_class, elm_entry_filter_accept_set, &accept_name);
   ewe_entry_entry_set(entry_class, entry_text);
   elm_object_part_text_set(entry_class, "guide", _("Type a new class name."));
   elm_object_part_content_set(item, "elm.swallow.content", entry_class);
   elm_box_pack_end(box, item);

   elm_object_content_set(ap->popup, box);

   BUTTON_ADD(ap->popup, button, _("Add"));
   evas_object_smart_callback_add(button, "clicked", _on_popup_btn_yes, ap);
   elm_object_part_content_set(ap->popup, "button1", button);

   BUTTON_ADD(ap->popup, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _popup_close_cb, ap);
   elm_object_part_content_set(ap->popup, "button2", button);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);

   evas_object_show(ap->popup);
   eina_stringshare_del(title);
   eina_stringshare_del(entry_text);

   return true;
}
