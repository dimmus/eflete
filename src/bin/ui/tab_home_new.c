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

struct _Widget_Item_Data
{
   const char *name;
   Eina_Bool check;
};
typedef struct _Widget_Item_Data Widget_Item_Data;

static Widget_Item_Data widget_item_data[] =
   {
     { N_("access"),           false },
     { N_("actionslider"),     false },
     { N_("bg"),               false },
     { N_("border"),           false },
     { N_("bubble"),           false },
     { N_("button"),           false },
     { N_("calendar"),         false },
     { N_("check"),            false },
     { N_("clock"),            false },
     { N_("colorsel"),         false },
     { N_("conform"),          false },
     { N_("ctxpopup"),         false },
     { N_("cursor"),           false },
     { N_("datetime"),         false },
     { N_("dayselector"),      false },
     { N_("diskselector"),     false },
     { N_("entry"),            false },
     { N_("fileselector"),     false },
     { N_("flipselector"),     false },
     { N_("focus"),            false },
     { N_("frame"),            false },
     { N_("gengrid"),          false },
     { N_("genlist"),          false },
     { N_("hover"),            false },
     { N_("icon"),             false },
     { N_("index"),            false },
     { N_("label"),            false },
     { N_("layout"),           false },
     { N_("list"),             false },
     { N_("map"),              false },
     { N_("menu"),             false },
     { N_("multibuttonentry"), false },
     { N_("naviframe"),        false },
     { N_("notify"),           false },
     { N_("panel"),            false },
     { N_("panes"),            false },
     { N_("photo"),            false },
     { N_("photocam"),         false },
     { N_("player"),           false },
     { N_("pointer"),          false },
     { N_("popup"),            false },
     { N_("progress"),         false },
     { N_("radio"),            false },
     { N_("scroller"),         false },
     { N_("segment_control"),  false },
     { N_("separator"),        false },
     { N_("slider"),           false },
     { N_("slideshow"),        false },
     { N_("spinner"),          false },
     { N_("thumb"),            false },
     { N_("toolbar"),          false },
     { N_("tooltip"),          false },
     { N_("video"),            false },
     { N_("win"),              false },
     { NULL,                   false }
   };

struct _Tab_Home_New
{
   Evas_Object *layout;
   Evas_Object *btn_create;
   Elm_Validator_Regexp *name_validator;

   Evas_Object *name;
   Evas_Object *path;
   Meta_Data_Controls meta;

   Evas_Object *ch_all;
   Evas_Object *genlist;
   Eina_Stringshare *tmp_dir_path;
};

typedef struct _Tab_Home_New Tab_Home_New;
static Tab_Home_New tab_new;

/* CHECK ALL AND NOT ALL */
static void
_checks_set(Eina_Bool check_val)
{
   Widget_Item_Data *widget_item_data_iterator = widget_item_data;

   while (widget_item_data_iterator->name)
     {
        if (widget_item_data_iterator->check != check_val)
          widget_item_data_iterator->check = check_val;
        widget_item_data_iterator++;
     }
   elm_genlist_realized_items_update(tab_new.genlist);
}

static void
_on_check_all(void *data __UNUSED__,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   _checks_set(elm_check_state_get(obj));
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

/* GENERATE SOURCE */
#define BTN_WD       (widget_item_data + 5)
#define SCROLLER_WD  (widget_item_data + 43)
#define ENTRY_WD     (widget_item_data + 16)
#define LABEL_WD     (widget_item_data + 26)
#define GENLIST_WD   (widget_item_data + 22)
#define LIST_WD      (widget_item_data + 28)
#define PHOTOCAM_WD  (widget_item_data + 37)
#define NOTIFY_WD    (widget_item_data + 33)
#define MAP_WD       (widget_item_data + 29)
#define POPUP_WD     (widget_item_data + 40)
#define GENGRID_WD   (widget_item_data + 21)

static int
_widgets_dependencies_setup(Widget_Item_Data *item, Eina_Strbuf *dep_message)
{
   assert(item != NULL);
   assert(dep_message != NULL);

   int ret  = 0;
   if (item->name)
     ret += _widgets_dependencies_setup(item + 1, dep_message);
   else
     return 0;

   if (!item->check)
     return ret;

   if ((item == ENTRY_WD) || (item == GENLIST_WD) ||
       (item == PHOTOCAM_WD) || (item == LIST_WD))
     {
        if (!SCROLLER_WD->check)
          {
             SCROLLER_WD->check = true;
             eina_strbuf_append(dep_message, _("Scroller<br>"));
             ret++;
             ret += _widgets_dependencies_setup(SCROLLER_WD, dep_message);
          }
     }
   else if ((item == MAP_WD) && (!PHOTOCAM_WD->check))
     {
        PHOTOCAM_WD->check = true;
        eina_strbuf_append(dep_message, _("Photocam<br>"));
        ret++;
        ret += _widgets_dependencies_setup(PHOTOCAM_WD, dep_message);
     }
   else if ((item == GENGRID_WD) && (!GENLIST_WD->check))
     {
        GENLIST_WD->check = true;
        eina_strbuf_append(dep_message, _("Genlist<br>"));
        ret++;
        ret += _widgets_dependencies_setup(GENLIST_WD, dep_message);
     }
   else if (item == POPUP_WD)
     {
        if (!NOTIFY_WD->check)
          {
             NOTIFY_WD->check = true;
             eina_strbuf_append(dep_message, _("Notify<br>"));
             ret++;
             ret += _widgets_dependencies_setup(NOTIFY_WD, dep_message);
          }
        if (!BTN_WD->check)
          {
             BTN_WD->check = true;
             eina_strbuf_append(dep_message, _("Button<br>"));
             ret++;
             ret += _widgets_dependencies_setup(BTN_WD, dep_message);
          }
        if (!LIST_WD->check)
          {
             LIST_WD->check = true;
             eina_strbuf_append(dep_message, _("List<br>"));
             ret++;
             ret += _widgets_dependencies_setup(LIST_WD, dep_message);
          }
        if (!LABEL_WD->check)
          {
             LABEL_WD->check = true;
             eina_strbuf_append(dep_message, _("Label<br>"));
             ret++;
             ret += _widgets_dependencies_setup(LABEL_WD, dep_message);
          }
     }

   return ret;
}

static void
_file_to_swap_copy(Eina_Stringshare *path, const char *widget_name)
{
   assert(path != NULL);
   assert(widget_name != NULL);

   Eina_Stringshare *path_to =
      eina_stringshare_printf("%s/%s.edc", path, widget_name);
   Eina_Stringshare *path_from =
      eina_stringshare_printf(EFLETE_TEMPLATE_EDC_PATH"/%s.edc", widget_name);
   int ch;

   FILE *fp_from = fopen(path_from, "r");
   if (!fp_from)
     {
        ERR("Failed to open file \"%s\"", path_from);
        eina_stringshare_del(path_to);
        eina_stringshare_del(path_from);
        return;
     }
   FILE *fp_to = fopen(path_to, "w");
   if (!fp_to)
     {
        ERR("Failed to open file \"%s\"", path_to);
        eina_stringshare_del(path_to);
        eina_stringshare_del(path_from);
        fclose(fp_from);
        return;
     }

   while ((ch = fgetc(fp_from)) != EOF)
     fputc(ch, fp_to);

   fclose(fp_from);
   fclose(fp_to);

   eina_stringshare_del(path_to);
   eina_stringshare_del(path_from);
}

#define DEPENDENCE_INCLUDE(Widget) \
   if (Widget->check) \
     { \
        Widget->check = false; \
        eina_strbuf_append_printf(dep_edc, "   #include \"%s.edc\"\n", \
                                  Widget->name); \
        _file_to_swap_copy(path, Widget->name); \
        are_widgets_included = true; \
     }

static Eina_Bool
_widgets_dependencies_generate(Eina_Stringshare *path, Eina_Strbuf *dep_edc)
{
   Eina_Bool are_widgets_included = false;

   assert(path != NULL);
   assert(dep_edc != NULL);

   DEPENDENCE_INCLUDE(BTN_WD);
   DEPENDENCE_INCLUDE(SCROLLER_WD);
   DEPENDENCE_INCLUDE(ENTRY_WD);
   DEPENDENCE_INCLUDE(LABEL_WD);
   DEPENDENCE_INCLUDE(GENLIST_WD);
   DEPENDENCE_INCLUDE(LIST_WD);
   DEPENDENCE_INCLUDE(PHOTOCAM_WD);
   DEPENDENCE_INCLUDE(NOTIFY_WD);
   DEPENDENCE_INCLUDE(MAP_WD);
   DEPENDENCE_INCLUDE(POPUP_WD);
   DEPENDENCE_INCLUDE(GENGRID_WD);

   return are_widgets_included;
}

#undef DEPENDENCE_INCLUDE

#undef BTN_WD
#undef SCROLLER_WD
#undef ENTRY_WD
#undef LABEL_WD
#undef GENLIST_WD
#undef LIST_WD
#undef PHOTOCAM_WD
#undef NOTIFY_WD
#undef MAP_WD
#undef POPUP_WD
#undef GENGRID_WD

static Eina_Strbuf *
_edc_code_generate(Eina_Stringshare *path)
{
   assert(path != NULL);

   Eina_Strbuf *edc = eina_strbuf_new();
   Eina_Strbuf *dep_message = eina_strbuf_new();
   Eina_Strbuf *dep_edc = eina_strbuf_new();
   int deps_count;
   Widget_Item_Data *widget_item_data_iterator = widget_item_data;
   deps_count = _widgets_dependencies_setup(widget_item_data, dep_message);

   if (deps_count)
     {
        eina_strbuf_prepend_printf(dep_message,
                                   NGETTEXT("%d widget included due to dependencies:<br><br>",
                                            "%d widgets included due to dependencies:<br><br>",
                                            deps_count), deps_count);

        NOTIFY_INFO(3, "%s", eina_strbuf_string_get(dep_message));
     }
   eina_strbuf_free(dep_message);
   eina_strbuf_append(edc, "data.item: \"version\" \"110\";\n\n");
   eina_strbuf_append(edc, "collections {\n");

   eina_strbuf_append(edc, "   group {\n");
   eina_strbuf_append(edc, "      name: \"" EFLETE_INTERNAL_GROUP_NAME "\";\n");
   eina_strbuf_append(edc, "   }\n");

   TODO("move fonts, colorclasses and macros to widgets where they are used");
   eina_strbuf_append(edc, "   #include \"fonts.edc\"\n");
   eina_strbuf_append(edc, "   #include \"colorclasses.edc\"\n");
   eina_strbuf_append(edc, "   #include \"macros.edc\"\n");

   _file_to_swap_copy(path, "fonts");
   _file_to_swap_copy(path, "colorclasses");
   _file_to_swap_copy(path, "macros");

   _widgets_dependencies_generate(path, dep_edc);
   eina_strbuf_append(edc, eina_strbuf_string_get(dep_edc));
   eina_strbuf_free(dep_edc);

   while (widget_item_data_iterator->name)
     {
        if (widget_item_data_iterator->check)
          {
             eina_strbuf_append_printf(edc, "   #include \"%s.edc\"\n",
                                       widget_item_data_iterator->name);
             _file_to_swap_copy(path, widget_item_data_iterator->name);
          }
        widget_item_data_iterator++;
     }

   eina_strbuf_append(edc, "}\n");
   return edc;
}

/* SPLASH */

static void
_progress_end(void *data, PM_Project_Result result)
{
   if (PM_PROJECT_SUCCESS == result)
     {
        elm_entry_entry_set(tab_new.name, NULL);
        elm_entry_entry_set(tab_new.path, profile_get()->general.projects_folder);
        elm_entry_entry_set(tab_new.meta.version, NULL);
        elm_entry_entry_set(tab_new.meta.authors, NULL);
        elm_entry_entry_set(tab_new.meta.licenses, NULL);
        elm_entry_entry_set(tab_new.meta.comment, N_("Created with Eflete!"));
        _checks_set(false);
     }
   _tabs_progress_end(data, result);
}

static Eina_Bool
_setup_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   Eina_Tmpstr *tmp_dir;
   Eina_Strbuf *edc, *flags;
   Eina_Stringshare *edc_path;
   FILE *fp;

   if (!eina_file_mkdtemp("eflete_project_XXXXXX", &tmp_dir))
     {
        ERR("can not create tmp dir");
        return false;
     }
   tab_new.tmp_dir_path = eina_stringshare_add(tmp_dir);
   eina_tmpstr_del(tmp_dir);

   edc_path = eina_stringshare_printf("%s/new_project_tmp.edc", tab_new.tmp_dir_path);
   edc = _edc_code_generate(tab_new.tmp_dir_path);

   /* create and write edc file */
   fp = fopen(edc_path, "w");
   if (!fp)
     {
        ERR("Failed to open file \"%s\"", edc_path);
        return false;
     }
   fputs(eina_strbuf_string_get(edc), fp);
   fclose(fp);

   flags = eina_strbuf_new();
   eina_strbuf_append_printf(flags, "-id \"%s\" -sd \"%s\" -v",
                             EFLETE_TEMPLATE_IMAGES_PATH,
                             EFLETE_TEMPLATE_SOUNDS_PATH);

   pm_project_import_edc(elm_entry_entry_get(tab_new.name),
                         elm_entry_entry_get(tab_new.path),
                         edc_path,
                         eina_strbuf_string_get(flags),
                         progress_print,
                         _progress_end,
                         &tab_new.meta);

   return true;
}

static Eina_Bool
_teardown_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_free();
   ecore_file_recursive_rm(tab_new.tmp_dir_path);
   eina_stringshare_del(tab_new.tmp_dir_path);
   tab_new.tmp_dir_path = NULL;

   return true;
}

static Eina_Bool
_cancel_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_cancel();
   return true;
}

/* TAB_HOME_NEW LAYOUT */
static void
_on_create(void *data __UNUSED__,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   Eina_Strbuf *buf;

   if (ap.project)
     if (!project_close())
       return;

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf,
                            _("<font_size=16>A project folder named '%s' already exist."
                              "Do you want to replace it?</font_size><br>"
                              "The project folder '%s' already exist in '%s'. Replacing it will overwrite"
                              "<b>all</b> contents."),
                            elm_entry_entry_get(tab_new.name),
                            elm_entry_entry_get(tab_new.name),
                            elm_entry_entry_get(tab_new.path));

   if (!exist_permission_check(elm_entry_entry_get(tab_new.path),
                               elm_entry_entry_get(tab_new.name),
                               _("New project"), eina_strbuf_string_get(buf)))
     return;
   eina_strbuf_free(buf);
   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          NULL);
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
   elm_check_state_set(tab_new.ch_all, false);
}

static void
_validate(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   if (elm_validator_regexp_status_get(tab_new.name_validator) != ELM_REG_NOERROR)
     elm_object_disabled_set(tab_new.btn_create, true);
   else
     elm_object_disabled_set(tab_new.btn_create, false);
}

static void
_elipsis(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   popup_fileselector_folder_helper(NULL,
                                    tab_new.path,
                                    elm_entry_entry_get(tab_new.path),
                                    entry_path_set,
                                    tab_new.path,
                                    false,
                                    false);
}

Evas_Object *
_tab_new_project_add(void)
{
   Elm_Genlist_Item_Class *itc = NULL;
   Widget_Item_Data *widget_item_data_iterator = widget_item_data;

   tab_new.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   tab_new.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab_new.layout, "layout", "tab_home", "new_project");

   BUTTON_ADD(tab_new.layout, tab_new.btn_create, _("Create"))
   elm_object_part_content_set(tab_new.layout, "elm.swallow.btn_create", tab_new.btn_create);
   evas_object_smart_callback_add(tab_new.btn_create, "clicked", _on_create, NULL);
   elm_object_disabled_set(tab_new.btn_create, true);

   /* label.name */
   elm_object_part_text_set(tab_new.layout, "label.name", _("Project name:"));
   ENTRY_ADD(tab_new.layout, tab_new.name, true)
   eo_do(tab_new.name, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, tab_new.name_validator));
   evas_object_smart_callback_add(tab_new.name, "changed", _validate, NULL);
   elm_object_part_content_set(tab_new.layout, "swallow.name", tab_new.name);
   /* label.path */
   elm_object_part_text_set(tab_new.layout, "label.path", _("Path to project:"));
   ENTRY_ADD(tab_new.layout, tab_new.path, true)
   elm_object_part_content_set(tab_new.layout, "swallow.path", tab_new.path);
   elm_entry_entry_set(tab_new.path, profile_get()->general.projects_folder);
   elipsis_btn_add(tab_new.path, _elipsis, NULL);

   meta_controls_add(tab_new.layout, &tab_new.meta);

   /* check all */
   CHECK_ADD(tab_new.layout, tab_new.ch_all);
   evas_object_smart_callback_add(tab_new.ch_all, "changed", _on_check_all, NULL);
   elm_object_part_content_set(tab_new.layout, "swallow.all_widgets_check", tab_new.ch_all);
   elm_object_part_text_set(tab_new.layout, "label.widgets", _("Widgets:"));

   /* genlist */
   tab_new.genlist = elm_genlist_add(ap.win);
   evas_object_smart_callback_add(tab_new.genlist, "activated", _on_item_activated, NULL);
   itc = elm_genlist_item_class_new();
   itc->item_style = "default";
   itc->func.text_get = _genlist_label_get;
   itc->func.content_get = _genlist_content_get;

   while (widget_item_data_iterator->name)
     {
        elm_genlist_item_append(tab_new.genlist, itc, widget_item_data_iterator,
                                NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
        widget_item_data_iterator++;
     }
   elm_genlist_item_class_free(itc);
   elm_object_part_content_set(tab_new.layout, "swallow.widgets", tab_new.genlist);

   return tab_new.layout;
}
