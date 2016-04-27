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

#include "tabs_private.h"
#include "tabs.h"
#include "main_window.h"
#include "project_common.h"
#include "config.h"

struct _Widget_Item_Data
{
   const char *name;
   Eina_Bool check;
};
typedef struct _Widget_Item_Data Widget_Item_Data;

Eina_List *widget_list = NULL;
const char *prev_edj_path = NULL;

struct _Tab_Home_Edj
{
   Evas_Object *layout;
   Evas_Object *btn_create;
   Elm_Validator_Regexp *name_validator;

   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edj;
   Meta_Data_Controls meta;

   Evas_Object *ch_all;
   Evas_Object *themes;
   Evas_Object *genlist;
};

typedef struct _Tab_Home_Edj Tab_Home_Edj;
static Tab_Home_Edj tab_edj;

static char *_genlist_label_get(void *data,
                                Evas_Object *obj,
                                const char  *part);

static Evas_Object *_genlist_content_get(void *data,
                                         Evas_Object *obj,
                                         const char *part);


/* CHECK ALL AND NOT ALL */
static void
_checks_set(Eina_Bool check_val)
{
   Widget_Item_Data *widget = NULL;
   Eina_List *l;

   EINA_LIST_FOREACH(widget_list, l, widget)
     {
        if (widget->check != check_val)
          widget->check = check_val;
     }
   elm_genlist_realized_items_update(tab_edj.genlist);
}

static Eina_Bool
_checked_get(void)
{
   Widget_Item_Data *widget = NULL;
   Eina_List *l;

   EINA_LIST_FOREACH(widget_list, l, widget)
     {
        if (widget->check)
          return EINA_TRUE;
     }
   return EINA_FALSE;
}

static int
_string_compare(const void *data1, const void *data2)
{
   Widget_Item_Data *d1, *d2;
   d1 = (Widget_Item_Data *)data1;
   d2 = (Widget_Item_Data *)data2;

   if (!strcmp(d1->name, d2->name)) return 0;

   return -1;
}

static void
_validate()
{
   if ((elm_validator_regexp_status_get(tab_edj.name_validator) != ELM_REG_NOERROR) ||
       !eina_str_has_extension(elm_entry_entry_get(tab_edj.edj), ".edj") ||
       !ecore_file_exists(elm_entry_entry_get(tab_edj.edj)) ||
       (widget_list && !_checked_get()))
     elm_object_disabled_set(tab_edj.btn_create, true);
   else
     elm_object_disabled_set(tab_edj.btn_create, false);
}

static void
_name_changed_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   _validate();
}

static void
_edj_changed_cb(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   const char *widget_name;
   Eina_List *collections, *l;
   Eina_Stringshare *group_name;
   Widget_Item_Data *widget = NULL;
   Elm_Genlist_Item_Class *itc = NULL;

   if (prev_edj_path && !strcmp(prev_edj_path, elm_entry_entry_get(tab_edj.edj))) return;
   prev_edj_path = elm_entry_entry_get(tab_edj.edj);

   elm_genlist_clear(tab_edj.genlist);

   EINA_LIST_FREE(widget_list, widget)
     {
        free(widget);
     }
   widget_list = NULL;

   if (eina_str_has_extension(elm_entry_entry_get(tab_edj.edj), ".edj") &&
       ecore_file_exists(elm_entry_entry_get(tab_edj.edj)))
     {
        collections = edje_file_collection_list(elm_entry_entry_get(tab_edj.edj));

        assert(collections != NULL);

        collections = eina_list_sort(collections, eina_list_count(collections), (Eina_Compare_Cb) strcmp);
        EINA_LIST_FOREACH(collections, l, group_name)
          {
             if (!strcmp(group_name, EFLETE_INTERNAL_GROUP_NAME)) continue;
             widget_name = widget_name_get(group_name);
             if (!widget_name) continue;

             widget = mem_calloc(1, sizeof(Widget_Item_Data));
             widget->name = widget_name;
             widget->check = false;
             if (!eina_list_search_unsorted(widget_list, _string_compare, (void *)widget))
               widget_list = eina_list_append(widget_list, widget);
          }

        itc = elm_genlist_item_class_new();
        itc->item_style = "default";
        itc->func.text_get = _genlist_label_get;
        itc->func.content_get = _genlist_content_get;

        EINA_LIST_FOREACH(widget_list, l, widget)
          {
             elm_genlist_item_append(tab_edj.genlist, itc, widget,
                                     NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
          }
        elm_genlist_item_class_free(itc);
     }

   _validate();
}

static void
_edj_set()
{
   Ewe_Combobox_Item *item = ewe_combobox_select_item_get(tab_edj.themes);
   char buf[256];
   if (item)
     {
        snprintf(buf, sizeof(buf), "%s/%s", EFLETE_TEMPLATE_EDJ_PATH, item->title);
        elm_entry_entry_set(tab_edj.edj, buf);
     }
   else elm_entry_entry_set(tab_edj.edj, "");

   _validate();
}

static void
_on_check_all(void *data __UNUSED__,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   _checks_set(elm_check_state_get(obj));
   _validate();
}

/*  GENLIST  */
static void
_check_widget(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Widget_Item_Data *widget_data = (Widget_Item_Data *)data;
   assert(widget_data != NULL);
   widget_data->check = elm_check_state_get(obj);
   _validate();
}

static char *
_genlist_label_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char  *part __UNUSED__)
{
   Widget_Item_Data *widget_data = (Widget_Item_Data *)data;
   assert(widget_data != NULL);
   return strdup(widget_data->name);
}

static Evas_Object *
_genlist_content_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part)
{
   Evas_Object *check;
   Widget_Item_Data *widget_data = (Widget_Item_Data *)data;
   if (strcmp(part, "elm.swallow.icon")) return NULL;

   /* the old hack. sometimes edje get wrong style, from system defalt theme,
    * for changed widget if widget is hidden */
   TODO("find why load wrong style");
   CHECK_ADD(ap.win, check);
   elm_object_focus_allow_set(check, false);
   elm_check_state_set(check, widget_data->check);
   evas_object_smart_callback_add(check, "changed", _check_widget, data);
   return check;
}

static void
_on_item_activated(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *it = (Elm_Object_Item *)event_info;
   Widget_Item_Data *widget_data = elm_object_item_data_get(it);

   assert(widget_data != NULL);

   widget_data->check = !widget_data->check;
   elm_genlist_item_update(it);
}

static void
_template_theme_changed(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   _edj_set();

}

static void
_progress_end(void *data, PM_Project_Result result)
{
   if (PM_PROJECT_SUCCESS == result)
     {
        elm_entry_entry_set(tab_edj.name, NULL);
        elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);
        elm_entry_entry_set(tab_edj.edj, NULL);
        elm_entry_entry_set(tab_edj.meta.version, NULL);
        elm_entry_entry_set(tab_edj.meta.authors, NULL);
        elm_entry_entry_set(tab_edj.meta.licenses, NULL);
        elm_entry_entry_set(tab_edj.meta.comment, N_("Created with Eflete!"));
     }

   _tabs_progress_end(data, result);
}

Eina_List *
_checked_widget_list_get()
{
   Widget_Item_Data *widget = NULL;
   Eina_List *list = NULL, *l;

   EINA_LIST_FOREACH(widget_list, l, widget)
     {
        if (widget->check)
          list = eina_list_append(list, widget->name);
     }

   return list;
}

static Eina_Bool
_setup_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   Eina_List *checked_widgets = _checked_widget_list_get();

   pm_project_import_edj(elm_entry_entry_get(tab_edj.name),
                         elm_entry_entry_get(tab_edj.path),
                         elm_entry_entry_get(tab_edj.edj),
                         checked_widgets,
                         progress_print,
                         _progress_end,
                         &tab_edj.meta);

   return true;
}

static Eina_Bool
_teardown_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_free();
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
   return true;
}

static Eina_Bool
_cancel_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_cancel();
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
   return true;
}

static void
_import(void *data __UNUSED__,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   Eina_Strbuf *buf;

   if (ap.project)
     if (!project_close())
       return;

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s/%s.pro",
                             elm_entry_entry_get(tab_edj.path),
                             elm_entry_entry_get(tab_edj.name));
   if (!pm_lock_check(eina_strbuf_string_get(buf)))
     {
       popup_want_action(_("Import EDJ-file"), _("The given file is locked by another application"),
                         NULL, NULL, BTN_OK, NULL, NULL);
       return;
     }

   eina_strbuf_reset(buf);
   eina_strbuf_append_printf(buf,
                            _("<font_size=16>A project folder named '%s' already exist."
                              "Do you want to replace it?</font_size><br>"
                              "The project folder '%s' already exist in '%s'. Replacing it will overwrite"
                              "<b>all</b> contents."),
                            elm_entry_entry_get(tab_edj.name),
                            elm_entry_entry_get(tab_edj.name),
                            elm_entry_entry_get(tab_edj.path));

   if (!exist_permission_check(elm_entry_entry_get(tab_edj.path),
                               elm_entry_entry_get(tab_edj.name),
                               _("Import edj-file"), eina_strbuf_string_get(buf)))
     return;
   eina_strbuf_free(buf);
   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          NULL);
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

static void
_elipsis(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   popup_fileselector_folder_helper(NULL,
                                    tab_edj.path,
                                    elm_entry_entry_get(tab_edj.path),
                                    entry_path_set,
                                    tab_edj.path,
                                    false,
                                    false);
}

static void
_elipsis_edj(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   popup_fileselector_edj_helper(NULL,
                                 tab_edj.edj,
                                 NULL,
                                 entry_path_set,
                                 tab_edj.edj,
                                 false,
                                 false);
}

Evas_Object *
_tab_import_edj_add(void)
{
   Eina_List *themes = NULL, *l = NULL;
   char *theme;

   tab_edj.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   tab_edj.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab_edj.layout, "layout", "tab_home", "import_edj");

   BUTTON_ADD(tab_edj.layout, tab_edj.btn_create, _("Create"))
   elm_object_part_content_set(tab_edj.layout, "elm.swallow.btn_create", tab_edj.btn_create);
   evas_object_smart_callback_add(tab_edj.btn_create, "clicked", _import, NULL);
   elm_object_disabled_set(tab_edj.btn_create, true);

   /* label.name */
   elm_object_part_text_set(tab_edj.layout, "label.name", _("Project name:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.name, true)
   eo_event_callback_add(tab_edj.name, ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, tab_edj.name_validator);
   evas_object_smart_callback_add(tab_edj.name, "changed", _name_changed_cb, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.name", tab_edj.name);
   /* label.path */
   elm_object_part_text_set(tab_edj.layout, "label.path", _("Path to project:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.path, true)
   elm_object_part_content_set(tab_edj.layout, "swallow.path", tab_edj.path);
   elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);
   elipsis_btn_add(tab_edj.path, _elipsis, NULL);

   /* label.path */
   elm_object_part_text_set(tab_edj.layout, "label.edj", _("Path to edj-file:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.edj, true)
   evas_object_smart_callback_add(tab_edj.edj, "changed", _edj_changed_cb, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.edj", tab_edj.edj);
   elipsis_btn_add(tab_edj.edj, _elipsis_edj, NULL);

   meta_controls_add(tab_edj.layout, &tab_edj.meta);

   evas_object_event_callback_add(tab_edj.layout, EVAS_CALLBACK_SHOW, _tab_default_focus, tab_edj.name);

   /* check all */
   CHECK_ADD(tab_edj.layout, tab_edj.ch_all);
   evas_object_smart_callback_add(tab_edj.ch_all, "changed", _on_check_all, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.all_widgets_check", tab_edj.ch_all);
   elm_object_part_text_set(tab_edj.layout, "label.widgets", _("Widgets:"));

   /* template themes */
   EWE_COMBOBOX_ADD(tab_edj.layout, tab_edj.themes);
   elm_object_part_content_set(tab_edj.layout, "swallow.template_themes", tab_edj.themes);
   ewe_combobox_text_set(tab_edj.themes, "template themes");
   themes = ecore_file_ls(EFLETE_TEMPLATE_EDJ_PATH);
   EINA_LIST_FOREACH(themes, l, theme)
      ewe_combobox_item_add(tab_edj.themes, theme);
   evas_object_smart_callback_add(tab_edj.themes, "selected", _template_theme_changed, NULL);

   /* genlist */
   tab_edj.genlist = elm_genlist_add(ap.win);
   evas_object_smart_callback_add(tab_edj.genlist, "activated", _on_item_activated, NULL);

   elm_object_part_content_set(tab_edj.layout, "swallow.widgets", tab_edj.genlist);

   return tab_edj.layout;
}

static void
_delayed_popup(void *data)
{
   char *msg = data;
   popup_want_action(_("Import edj-file"), msg, NULL, NULL, BTN_OK, NULL, NULL);
   free(msg);
}

void
_tab_import_edj_data_set(const char *name, const char *path, const char *edj, const Eina_List *widgets)
{
   const Eina_List *l, *wl;
   const char *str;
   Widget_Item_Data *widget_item_data_iterator;
   Eina_Strbuf *buf = eina_strbuf_new();
   Eina_Bool first_not_found = true;
   Widget_Item_Data *widget = NULL;

   assert(tab_edj.layout != NULL);

   elm_entry_entry_set(tab_edj.name, name);

   if (path) elm_entry_entry_set(tab_edj.path, path);
   else elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);

   elm_entry_entry_set(tab_edj.edj, edj);
   edje_message_signal_process();

   EINA_LIST_FOREACH(widgets, l, str)
     {
        EINA_LIST_FOREACH(widget_list, wl, widget)
          {
             if (!strcasecmp(str, widget->name))
               {
                  widget->check = true;
                  break;
               }
          }
        if (!widget)
          {
             eina_strbuf_append_printf(buf, first_not_found ? "%s" : ", %s", str);
             first_not_found = false;
          }
     }
   elm_genlist_realized_items_update(tab_edj.genlist);
   if (eina_strbuf_length_get(buf))
     {
        eina_strbuf_prepend(buf, _("Following widgets were not found and ignored: "));
        ERR("%s", eina_strbuf_string_get(buf));
        ecore_job_add(_delayed_popup, eina_strbuf_string_steal(buf));
     }
   eina_strbuf_free(buf);

   _validate();
}
