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

#include "style_dialog.h"
#include "common_macro.h"
#include "string_macro.h"

static Evas_Object *entry_class;
static Evas_Object *entry_style;

static Widget *
_widget_from_ap_get(App_Data *ap)
{
   Widget *widget = NULL;
   Evas_Object *gl_widget = NULL;
   Evas_Object *nf = NULL;
   Elm_Object_Item *eoi = NULL;

   nf = ui_block_widget_list_get(ap);
   eoi = elm_naviframe_bottom_item_get(nf);
   gl_widget = elm_object_item_part_content_get(eoi, NULL);
   eoi = elm_genlist_selected_item_get(gl_widget);
   if (!eoi)
     {
         NOTIFY_ERROR(_("Couldn't add new state/class without opened widget"));
         return NULL;
     }
   widget = elm_object_item_data_get(eoi);
   return widget;
}

static void
_reload_classes(App_Data *ap, Eina_Inlist *classes)
{
   Evas_Object *gl_classes = NULL;
   Elm_Object_Item *eoi = NULL;

   eoi = elm_naviframe_top_item_get(ui_block_widget_list_get(ap));
   gl_classes = elm_object_item_part_content_get(eoi, NULL);

   ui_widget_list_class_data_reload(gl_classes, classes);
}

static Class *
_class_from_ap_get(App_Data *ap)
{
   Evas_Object *gl_class = NULL;
   Evas_Object *nf = NULL;
   Elm_Object_Item *eoi = NULL;
   Elm_Object_Item *parent_eoi = NULL;

   nf = ui_block_widget_list_get(ap);
   eoi = elm_naviframe_top_item_get(nf);
   gl_class = elm_object_item_part_content_get(eoi, NULL);
   eoi = elm_genlist_selected_item_get(gl_class);
   if (!eoi)
     {
        eoi = elm_genlist_first_item_get(gl_class);
        if (!eoi) return NULL;
     }
   parent_eoi = elm_genlist_item_parent_get(eoi);
   if(parent_eoi) eoi = parent_eoi;
   return elm_object_item_data_get(eoi);
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
_on_popup_btn_yes(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Eina_Stringshare *style_name = NULL;
   Eina_Stringshare *class_name = NULL;
   Eina_Stringshare *source_file = NULL;
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
   Eina_Inlist *source_widgets = NULL;
   Eina_List *styles = NULL;
   Evas *canvas = NULL;

#define STRING_CLEAR\
        eina_stringshare_del(source_file);\
        eina_stringshare_del(style_name);\
        eina_stringshare_del(class_name);\


   widget = _widget_from_ap_get(ap);
   if (!widget) return;

   source_file = eina_stringshare_add(EFLETE_EDJ_PATH"template.edj");
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

   source_widgets = wm_widget_list_new(source_file);
   EINA_INLIST_FOREACH_SAFE(source_widgets, l, source_wdg)
     {
        if (!strcmp(source_wdg->name, widget->name)) break;
     }

   if (!source_wdg)
     {
        NOTIFY_ERROR(_("Sorry, there are no templates for [%s]"), widget->name);
        evas_object_smart_callback_call(obj, "close,popup", NULL);
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

   canvas = evas_object_evas_get(ap->workspace);

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
        if (!source_style)
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
        dest_class = wm_class_add(class_name, styles);
        dest_wdg->classes = eina_inlist_append(dest_wdg->classes,
                                                  EINA_INLIST_GET(dest_class));
        style = EINA_INLIST_CONTAINER_GET(dest_class->styles, Style);
     }
   else
     {
        style = wm_style_add(style_name, full_name, STYLE);
        dest_class->styles = eina_inlist_append(dest_class->styles,
                                                EINA_INLIST_GET(style));
     }

   if (!dest_style)
       dest_style = EINA_INLIST_CONTAINER_GET(dest_class->styles, Style);


   /* Load data into source style, for copy this params into destination style*/
   wm_style_data_load(source_style, canvas, source_file);

   if (dest_style->isAlias) dest_style = dest_style->main_group;
   /* call method, which copy all parts and their params into new style */
   if (wm_style_copy(dest_style->obj, source_style->obj, full_name,
                     ap->project->swapfile, style))
     {
        wm_style_data_load(style, canvas, ap->project->swapfile);
        _reload_classes(ap, dest_wdg->classes);
     }

   if (!wm_widget_list_free(source_widgets))
     ERR("Failed free template widget list");

   STRING_CLEAR;
   eina_stringshare_del(full_name);
   evas_object_smart_callback_call(obj, "close,popup", NULL);
#undef STRING_CLEAR
}

static void
_popup_close(void *data, Evas_Object *obj __UNUSED__, void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}


/* FIXME: change name to class_dialog_add */
void
style_dialog_add(App_Data *ap)
{
   Evas_Object *popup, *box, *button;
   Widget *widget = NULL;
   Class *class_st = NULL;
   Eina_Stringshare *title = NULL;
   Eina_Stringshare *entry_text = NULL;

   if (!ap) return;
   widget = _widget_from_ap_get(ap);
   if (!widget) return;
   title = eina_stringshare_printf(_("Add style/class for \"%s\" widget"),
                                   widget->name);
   popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(popup, "eflete");
   elm_object_part_text_set(popup, "title,text", title);
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(popup, box, false, false);

   EWE_ENTRY_ADD(box, entry_style, true, DEFAULT_STYLE);
   ewe_entry_label_visible_set(entry_style, EINA_TRUE);
   ewe_entry_label_text_set(entry_style, "Style name:");
   elm_object_part_text_set(entry_style, "guide", _("Type a new style name."));
   ewe_entry_regex_set(entry_style, STYLE_NAME_REGEX, EWE_REG_ICASE);
   ewe_entry_regex_glow_set(entry_style, EINA_TRUE);
   ewe_entry_regex_autocheck_set(entry_style, EINA_TRUE);

   class_st = _class_from_ap_get(ap);
   if (!class_st) return;
   entry_text = eina_stringshare_add(class_st->name);

   EWE_ENTRY_ADD(box, entry_class, true, DEFAULT_STYLE);
   ewe_entry_label_visible_set(entry_class, EINA_TRUE);
   ewe_entry_label_text_set(entry_class, "Class name:");
   elm_entry_entry_set(entry_class, entry_text);
   elm_object_part_text_set(entry_class, "guide", _("Type a new class name."));
   ewe_entry_regex_set(entry_class, STYLE_NAME_REGEX, EWE_REG_ICASE);
   ewe_entry_regex_glow_set(entry_class, EINA_TRUE);
   ewe_entry_regex_autocheck_set(entry_class, EINA_TRUE);

   elm_box_pack_end(box, entry_style);
   elm_box_pack_end(box, entry_class);
   elm_object_content_set(popup, box);

   BUTTON_ADD(popup, button, _("Add"));
   evas_object_smart_callback_add(button, "close,popup", _popup_close, popup);
   evas_object_smart_callback_add(button, "pressed", _on_popup_btn_yes, ap);
   elm_object_part_content_set(popup, "button1", button);

   BUTTON_ADD(popup, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _popup_close, popup);
   elm_object_part_content_set(popup, "button2", button);

   evas_object_show(popup);
   eina_stringshare_del(title);
   eina_stringshare_del(entry_text);
}
