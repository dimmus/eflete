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

#include "wizard.h"
#include "wizard_common.h"

/*
                          TODO:
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 !!   This submodule required to be refactored ASAP,  !!
 !!    after theme specification will be released.    !!
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*/

struct _Item_Mod_Callback_Data
{
   Evas_Object *obj;
   Evas_Object *box;
   Evas_Object *entry;
   Wizard_Import_Edj_Win *wiew;
};

typedef struct _Item_Mod_Callback_Data Item_Mod_Callback_Data;

struct _Widget_Item_Data
{
   const char *name;
   Eina_Bool check;
};

typedef struct _Widget_Item_Data Widget_Item_Data;

static Widget_Item_Data widget_item_data[] =
                                { { N_("access"),           false },
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

static void
_on_button_add_clicked_cb(void *data, Evas_Object *obj, void *event_info);

static void
_on_button_del_clicked_cb(void *data, Evas_Object *obj, void *event_info);

static void
_widget_item_data_array_checks_set(Evas_Object *genlist, Eina_Bool check_val)
{
   Widget_Item_Data *widget_item_data_iterator = widget_item_data;
   Eina_Bool is_changed = false;

   while (widget_item_data_iterator->name)
     {
        if (widget_item_data_iterator->check != check_val)
          {
             is_changed = true;
             widget_item_data_iterator->check = check_val;
          }
        widget_item_data_iterator++;
     }
   if ((is_changed) && (genlist))
     elm_genlist_realized_items_update(genlist);
}

static void
_del_item_callback_data(void * data,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Item_Mod_Callback_Data *c_data = (Item_Mod_Callback_Data*)data;
   free(c_data);
}

static Eina_Bool
_edc_filter(const char *path,
            Eina_Bool dir,
            void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".edc"))
     return true;
   return false;
}
FILESELCTOR_WINDOW(_on_edc_bt, _("Select edc file for import"), false, true, _edc_filter, edj)

static void
_on_directory_bt_done(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Item_Mod_Callback_Data *c_data = (Item_Mod_Callback_Data*)data;
   const char *selected;

   selected = event_info;
   if ((selected) && (strcmp(selected, "")))
     elm_entry_entry_set(c_data->entry, selected);

   _fs_close(NULL, c_data->wiew->fs, NULL);
}

static inline const char *
item_entry_entry_get(Evas_Object *item)
{
   Evas_Object *entry = elm_object_part_content_get(item, "swallow.entry");
   return elm_entry_entry_get(entry);
}

static void
_directories_include_flags_build(Evas_Object *box,
                                 const char *dir_name,
                                 Eina_Strbuf *str)
{
   Eina_List *box_items, *l;
   Evas_Object *item;
   const char *path;

   box_items = elm_box_children_get(box);
   EINA_LIST_FOREACH(box_items, l, item)
     {
        path = item_entry_entry_get(item);
        if (path)
          eina_strbuf_append_printf(str, "-%s \"%s\" ", dir_name, path);
     }
}

static inline Evas_Object *
_directories_box_get(Evas_Object *layout, const char *part_name)
{
   Evas_Object *scr = elm_object_part_content_get(layout, part_name);
   return elm_object_content_get(scr);
}

static Eina_Strbuf *
_edje_cc_flags_create(Wizard_Import_Edj_Win *wiew)
{
   Eina_Strbuf *flags = eina_strbuf_new();

   _directories_include_flags_build(_directories_box_get(wiew->layout,
                                    "swallow.directories_img"), "id", flags);
   _directories_include_flags_build(_directories_box_get(wiew->layout,
                                    "swallow.directories_fnt"), "fd", flags);
   _directories_include_flags_build(_directories_box_get(wiew->layout,
                                    "swallow.directories_snd"), "sd", flags);
   _directories_include_flags_build(_directories_box_get(wiew->layout,
                                    "swallow.directories_vbr"), "vd", flags);
   _directories_include_flags_build(_directories_box_get(wiew->layout,
                                    "swallow.directories_data"), "dd", flags);

   eina_strbuf_append(flags, "-v");
   return flags;
}

static void
_file_to_swap_copy(Eina_Stringshare *path, const char *widget_name)
{
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
   int ret;
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
_edc_code_generate(Eina_Stringshare *path, Wizard_Import_Edj_Win *wiew __UNUSED__)
{
   Eina_Strbuf *edc = eina_strbuf_new();
   Eina_Strbuf *dep_message = eina_strbuf_new();
   Eina_Strbuf *dep_edc = eina_strbuf_new();
   int deps_count;
   Widget_Item_Data *widget_item_data_iterator = widget_item_data;
   Eina_Bool are_widgets_included = false;
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
   eina_strbuf_append(edc, "   #include \"fonts.edc\"\n");
   eina_strbuf_append(edc, "   #include \"colorclasses.edc\"\n");
   eina_strbuf_append(edc, "   #include \"macros.edc\"\n");

   _file_to_swap_copy(path, "fonts");
   _file_to_swap_copy(path, "colorclasses");
   _file_to_swap_copy(path, "macros");

   are_widgets_included = _widgets_dependencies_generate(path, dep_edc);
   eina_strbuf_append(edc, eina_strbuf_string_get(dep_edc));
   eina_strbuf_free(dep_edc);

   while (widget_item_data_iterator->name)
     {
        if (widget_item_data_iterator->check)
          {
             eina_strbuf_append_printf(edc, "   #include \"%s.edc\"\n",
                                       widget_item_data_iterator->name);
             _file_to_swap_copy(path, widget_item_data_iterator->name);
             are_widgets_included = true;
          }
        widget_item_data_iterator++;
     }

   if (!are_widgets_included)
     {
        eina_strbuf_append(edc, "   group {\n");
        eina_strbuf_append(edc, "      name: \"new/layout/0\";\n");
        eina_strbuf_append(edc, "   }\n");
     }

   eina_strbuf_append(edc, "}\n");
   return edc;
}

static Eina_Bool
_splash_setup_import_edc(void *data, Splash_Status status __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew = (Wizard_Import_Edj_Win *)data;
   Eina_Strbuf *flags = _edje_cc_flags_create(wiew);
   wiew->progress_log = eina_strbuf_new();

   wiew->thread = pm_project_import_edc(elm_entry_entry_get(wiew->name),
                                        elm_entry_entry_get(wiew->path),
                                        elm_entry_entry_get(wiew->edj),
                                        eina_strbuf_string_get(flags),
                                        _progress_print,
                                        _progress_end,
                                        wiew);

   eina_strbuf_free(flags);
   return false;
}

Eina_Bool
_new_project_file_create(Eina_Stringshare *path, const char *edc)
{
   FILE *fp = fopen(path, "w");
   if (!fp)
     {
        ERR("Failed to open file \"%s\"", path);
        return false;
     }

   fputs(edc, fp);
   fclose(fp);
   return true;
}

static Eina_Bool
_splash_setup_new_project(void *data, Splash_Status status __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew = (Wizard_Import_Edj_Win *)data;
   Eina_Strbuf *edc;
   Eina_Strbuf *flags;
   Eina_Tmpstr *tmp_dir;
   Eina_Stringshare *edc_path;
   if (!eina_file_mkdtemp("eflete_project_XXXXXX", &tmp_dir))
     {
        ERR("can not create tmp dir");
        return false;
     }
   wiew->tmp_dir_path = eina_stringshare_add(tmp_dir);
   eina_tmpstr_del(tmp_dir);

   edc_path = eina_stringshare_printf("%s/new_project_tmp.edc", wiew->tmp_dir_path);
   edc = _edc_code_generate(wiew->tmp_dir_path, wiew);
   _new_project_file_create(edc_path, eina_strbuf_string_get(edc));

   flags = eina_strbuf_new();

   eina_strbuf_append_printf(flags, "-id \"%s\" -sd \"%s\" -v",
                             EFLETE_TEMPLATE_IMAGES_PATH,
                             EFLETE_TEMPLATE_SOUNDS_PATH);

   wiew->thread = pm_project_import_edc(elm_entry_entry_get(wiew->name),
                                        elm_entry_entry_get(wiew->path),
                                        edc_path,
                                        eina_strbuf_string_get(flags),
                                        _progress_print,
                                        _progress_end,
                                        wiew);
   wiew->progress_log = eina_strbuf_new();

   eina_strbuf_free(edc);
   eina_strbuf_free(flags);
   eina_stringshare_del(edc_path);

   return false;
}

static void
_on_directory_bt(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *bg, *fs;
   const char *path;
   Item_Mod_Callback_Data *c_data = (Item_Mod_Callback_Data*)data;

   MODAL_WINDOW_ADD(c_data->wiew->fs, main_window_get(),
                    "Select Directory", _fs_close, NULL);
   bg = elm_bg_add(c_data->wiew->fs);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(c_data->wiew->fs, bg);
   FILESELECTOR_ADD(fs, c_data->wiew->fs, _on_directory_bt_done, data);
   elm_fileselector_folder_only_set(fs, true);
   path = elm_entry_entry_get(c_data->entry);
   if ((path) && (ecore_file_is_dir(path))) elm_fileselector_path_set(fs, path);
   elm_win_resize_object_add(c_data->wiew->fs, fs);
}

static Evas_Object *
_dir_item_add(Evas_Object *parent, Wizard_Import_Edj_Win *wiew)
{
   Evas_Object *item, *button, *entry, *icon;
   Item_Mod_Callback_Data *c_data = malloc(sizeof(Item_Mod_Callback_Data));
   if (!c_data) return NULL;

   item = elm_layout_add(parent);
   elm_layout_theme_set(item, "layout", "wizard", "directories_item");
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   evas_object_show(item);

   c_data->box = parent;
   c_data->obj = item;
   c_data->wiew = wiew;

   BUTTON_ADD(item, button, NULL);
   elm_object_style_set(button, "btn");
   ICON_ADD(button, icon, true, "icon-add");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, c_data);

   elm_object_part_content_set(item, "swallow.button_add", button);

   BUTTON_ADD(item, button, NULL);
   elm_object_style_set(button, "btn");
   ICON_ADD(button, icon, true, "icon-remove");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_del_clicked_cb, c_data);

   elm_object_part_content_set(item, "swallow.button_del", button);

   ENTRY_ADD(item, entry, true)
   button = elm_button_add(item);
   elm_object_style_set(button, "elipsis");
   evas_object_show(button);
   evas_object_smart_callback_add(button, "clicked", _on_directory_bt, c_data);
   elm_object_part_content_set(entry, "elm.swallow.end", button);
   elm_object_part_content_set(item, "swallow.entry", entry);
   c_data->entry = entry;

   evas_object_event_callback_add(item, EVAS_CALLBACK_DEL,
                                  _del_item_callback_data, c_data);

   return item;
}

static void
_on_button_add_clicked_cb(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Item_Mod_Callback_Data *c_data = (Item_Mod_Callback_Data *)data;
   Eina_List *box_items = elm_box_children_get(c_data->box);
   if (!eina_list_count(box_items))
     elm_box_pack_end(c_data->box, _dir_item_add(c_data->box, c_data->wiew));
   else
     elm_box_pack_after(c_data->box,
                        _dir_item_add(c_data->box, c_data->wiew), c_data->obj);
   return;
}

static void
_on_button_del_clicked_cb(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Item_Mod_Callback_Data *c_data = (Item_Mod_Callback_Data*)data;

   Eina_List *box_items = elm_box_children_get(c_data->box);
   if (eina_list_count(box_items) <= 1)
     {
        elm_entry_entry_set(c_data->entry, "");
        return;
     }

   elm_box_unpack(c_data->box, c_data->obj);
   evas_object_del(c_data->obj);
   return;
}

static Evas_Object *
_directories_box_add(Wizard_Import_Edj_Win *wiew)
{
   Evas_Object *scr;
   Evas_Object *box;

   SCROLLER_ADD(wiew->layout, scr);
   BOX_ADD(wiew->layout, box, false, false);
   elm_box_pack_end(box, _dir_item_add(box, wiew));
   elm_object_content_set(scr, box);
   return scr;
}

Evas_Object *
wizard_import_edc_add(App_Data *ap __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;
   wiew = wizard_import_common_add("import_edc");
   if (!wiew) return NULL;

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   mw_title_set(wiew->win, _("Wizard: import edc"));
   project_path_item_add(wiew, _("Path to edc file:"), _on_edc_bt);

   //labels setup
   elm_object_part_text_set(wiew->layout, "label.directories_img", _("Images directories:"));
   elm_object_part_text_set(wiew->layout, "label.directories_fnt", _("Fonts directories:"));
   elm_object_part_text_set(wiew->layout, "label.directories_snd", _("Sounds directories:"));
   elm_object_part_text_set(wiew->layout, "label.directories_vbr", _("Vibrations directories:"));
   elm_object_part_text_set(wiew->layout, "label.directories_data", _("Data directories:"));

   elm_object_part_content_set(wiew->layout, "swallow.directories_img",
                               _directories_box_add(wiew));
   elm_object_part_content_set(wiew->layout, "swallow.directories_fnt",
                               _directories_box_add(wiew));
   elm_object_part_content_set(wiew->layout, "swallow.directories_snd",
                               _directories_box_add(wiew));
   elm_object_part_content_set(wiew->layout, "swallow.directories_vbr",
                               _directories_box_add(wiew));
   elm_object_part_content_set(wiew->layout, "swallow.directories_data",
                               _directories_box_add(wiew));

   wiew->splash_setup_func = _splash_setup_import_edc;

   return wiew->win;
}

static char *
_genlist_label_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char  *part __UNUSED__)
{
   Widget_Item_Data *widget_data = (Widget_Item_Data *)data;
   return strdup(widget_data->name);
}

static void
_on_widget_include_check_changed(void *data,
                                 Evas_Object *obj,
                                 void *ei __UNUSED__)
{
   Widget_Item_Data *widget_data = (Widget_Item_Data *)data;
   widget_data->check = elm_check_state_get(obj);
}

static void
_on_widget_include_all_check_changed(void *data,
                                     Evas_Object *obj,
                                     void *ei __UNUSED__)
{
   Evas_Object *genlist = (Evas_Object *)data;
   _widget_item_data_array_checks_set(genlist, elm_check_state_get(obj));
}

static void
_on_genlist_item_activated(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *ei)
{
   Elm_Object_Item *it = (Elm_Object_Item *)ei;
   Widget_Item_Data *widget_data = elm_object_item_data_get(it);
   widget_data->check = !widget_data->check;
   elm_genlist_item_update(it);
}

static Evas_Object *
_genlist_content_get(void *data,
                     Evas_Object *obj,
                     const char *part)
{
   Evas_Object *check;
   Widget_Item_Data *widget_data = (Widget_Item_Data *)data;
   if (strcmp(part, "elm.swallow.icon"))
     return NULL;

   CHECK_ADD(obj, check);
   elm_object_style_set(check, "live_view");
   elm_object_focus_allow_set(check, false);
   elm_check_state_set(check, widget_data->check);
   evas_object_smart_callback_add(check, "changed",
                                  _on_widget_include_check_changed, data);
   return check;
}

static Evas_Object *
_wizart_widget_list_add(Evas_Object *parent)
{
   Evas_Object *genlist;
   Elm_Genlist_Item_Class *itc = NULL;
   Widget_Item_Data *widget_item_data_iterator = widget_item_data;

   genlist = elm_genlist_add(parent);
   elm_object_style_set(genlist, "dark");

   itc = elm_genlist_item_class_new();
   itc->item_style = "wizard_widgetlist";
   itc->func.text_get = _genlist_label_get;
   itc->func.content_get = _genlist_content_get;
   itc->func.state_get = NULL;
   itc->func.del = NULL;

   while (widget_item_data_iterator->name)
     {
        elm_genlist_item_append(genlist, itc,
                                       widget_item_data_iterator,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL,
                                       NULL);
        widget_item_data_iterator++;
     }

   elm_genlist_item_class_free(itc);
   return genlist;
}

Evas_Object *
wizard_new_project_add(App_Data *ap __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;
   Evas_Object *genlist, *check;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   wiew = wizard_import_common_add("new_project");
   if (!wiew) return NULL;

   mw_title_set(wiew->win, _("Wizard: new project"));

   elm_object_part_text_set(wiew->layout, "label.widgets", _("Widgets:"));

   _widget_item_data_array_checks_set(NULL, false);

   genlist = _wizart_widget_list_add(wiew->layout);
   elm_object_part_content_set(wiew->layout, "swallow.widgets", genlist);

   CHECK_ADD(wiew->layout, check);
   elm_object_style_set(check, "live_view");
   evas_object_smart_callback_add(check, "changed",
                                  _on_widget_include_all_check_changed,
                                  genlist);
   evas_object_smart_callback_add(genlist, "activated",
                                  _on_genlist_item_activated,
                                  NULL);
   elm_object_part_content_set(wiew->layout,
                               "swallow.all_widgets_check", check);

   wiew->splash_setup_func = _splash_setup_new_project;
   wiew->edj = NULL;

   return wiew->win;
}

#undef FILESELCTOR_WINDOW
