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

struct _Item_Mod_Callback_Data
{
   Evas_Object *obj;
   Evas_Object *box;
   Evas_Object *entry;
   Wizard_Import_Edj_Win *wiew;
};

typedef struct _Item_Mod_Callback_Data Item_Mod_Callback_Data;

static const char *widget_names_list[] = { N_("access"),
                                           N_("actionslider"),
                                           N_("bg"),
                                           N_("border"),
                                           N_("bubble"),
                                           N_("button"),
                                           N_("calendar"),
                                           N_("check"),
                                           N_("clock"),
                                           N_("colorselector"),
                                           N_("conformant"),
                                           N_("ctxpopup"),
                                           N_("cursor"),
                                           N_("datetime"),
                                           N_("dayselector"),
                                           N_("diskselector"),
                                           N_("entry"),
                                           N_("ews"),
                                           N_("fileselector"),
                                           N_("fileselector_entry"),
                                           N_("flipselector"),
                                           N_("focus_hithlight"),
                                           N_("frame"),
                                           N_("gengrid"),
                                           N_("genlist"),
                                           N_("genscroller"),
                                           N_("hover"),
                                           N_("icon"),
                                           N_("index"),
                                           N_("label"),
                                           N_("leyout"),
                                           N_("list"),
                                           N_("map"),
                                           N_("menu"),
                                           N_("multibuttonentry"),
                                           N_("naviframe"),
                                           N_("notify"),
                                           N_("panel"),
                                           N_("panes"),
                                           N_("photo"),
                                           N_("photocam"),
                                           N_("player"),
                                           N_("pointer"),
                                           N_("popup"),
                                           N_("progressbar"),
                                           N_("radio"),
                                           N_("scroller"),
                                           N_("segment_controll"),
                                           N_("separator"),
                                           N_("slider"),
                                           N_("slideshow"),
                                           N_("spinner"),
                                           N_("thumb"),
                                           N_("toolbar"),
                                           N_("tooltip"),
                                           N_("video"),
                                           NULL};

static void
_on_button_add_clicked_cb(void *data, Evas_Object *obj, void *event_info);

static void
_on_button_del_clicked_cb(void *data, Evas_Object *obj, void *event_info);

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

static Eina_Bool
_setup_splash(void *data)
{
   Wizard_Import_Edj_Win *wiew = (Wizard_Import_Edj_Win *)data;
   Eina_Strbuf *flags = _edje_cc_flags_create(wiew);
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
   elm_layout_file_set(item,
                       EFLETE_EDJ, "elm/layout/wizard/directories_item");
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   evas_object_show(item);

   c_data->box = parent;
   c_data->obj = item;
   c_data->wiew = wiew;

   BUTTON_ADD(item, button, NULL);
   elm_object_style_set(button, "eflete/btn");
   ICON_ADD(button, icon, true, "icon-add");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, c_data);

   elm_object_part_content_set(item, "swallow.button_add", button);

   BUTTON_ADD(item, button, NULL);
   elm_object_style_set(button, "eflete/btn");
   ICON_ADD(button, icon, true, "icon-remove");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_del_clicked_cb, c_data);

   elm_object_part_content_set(item, "swallow.button_del", button);

   ENTRY_ADD(item, entry, true, DEFAULT_STYLE)
   button = elm_button_add(item);
   elm_object_style_set(button, "eflete/elipsis");
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
   wiew = wizard_import_common_add("elm/layout/wizard/import_edc");
   if (!wiew) return NULL;

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

   wiew->splash_setup_func = _setup_splash;

   return wiew->win;
}

static char *
_genlist_label_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char  *part __UNUSED__)
{
   return strdup(data);
}

static Evas_Object *
_genlist_content_get(void *data __UNUSED__,
                     Evas_Object *obj,
                     const char *part)
{
   Evas_Object *check;
   if (strcmp(part, "elm.swallow.icon"))
     return NULL;

   CHECK_ADD(obj, check, "eflete/live_view");
   elm_object_focus_allow_set(check, false);
   return check;
}

static Evas_Object *
_wizart_widget_list_add(Evas_Object *parent)
{
   Evas_Object *genlist;
   Elm_Genlist_Item_Class *itc = NULL;
   const char **widget_names_list_iterator = widget_names_list;

   genlist = elm_genlist_add(parent);
   elm_object_style_set(genlist, "eflete/dark");

   itc = elm_genlist_item_class_new();
   itc->item_style = "eflete/level1";
   itc->func.text_get = _genlist_label_get;
   itc->func.content_get = _genlist_content_get;
   itc->func.state_get = NULL;
   itc->func.del = NULL;

   while (*widget_names_list_iterator)
     {
        elm_genlist_item_append(genlist, itc,
                                *widget_names_list_iterator,
                                NULL, ELM_GENLIST_ITEM_NONE,
                                NULL, NULL);
        widget_names_list_iterator++;
     }

   elm_genlist_item_class_free(itc);
   return genlist;
}

Evas_Object *
wizard_new_project_add(App_Data *ap __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;
   Evas_Object *btn;
   wiew = wizard_import_common_add("elm/layout/wizard/new_project");
   if (!wiew) return NULL;

   mw_title_set(wiew->win, _("Wizard: new project"));

   elm_object_part_text_set(wiew->layout, "label.widgets", _("Widgets:"));

   elm_object_part_content_set(wiew->layout, "swallow.widgets",
                               _wizart_widget_list_add(wiew->layout));

   // to be deleted during further implementation
   btn = elm_object_part_content_get(wiew->layout, "swallow.button1");
   elm_object_disabled_set(btn, true);

   wiew->splash_setup_func = NULL;

   return wiew->win;
}

#undef FILESELCTOR_WINDOW
