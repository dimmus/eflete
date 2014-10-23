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
 * along with this program; If not, see http://www.gnu.org/licenses/lgpl.html.
 */
#include "compile_dialog.h"

struct _fs_entries
{
   char *project_name;
   Evas_Object *edc;
   Evas_Object *edj;
   Evas_Object *id;
   Evas_Object *fd;
   Evas_Object *sd;
   Evas_Object *log;

   Evas_Object *parent;
   Evas_Object *win;
};

typedef struct _fs_entries fs_entries;

static fs_entries *fs_ent = NULL;

static void
_on_cancel_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ap)
     {
        if (ap->project)
          {
             ui_menu_base_disabled_set(ap->menu_hash, false);
             ui_menu_locked_set(ap->menu_hash, true);
             ui_menu_disable_set(ap->menu_hash, _("Save project"), false);
          }
        else
          {
             ui_menu_base_disabled_set(ap->menu_hash, true);
             ui_menu_locked_set(ap->menu_hash, false);
             ui_menu_disable_set(ap->menu_hash, _("Save project"), true);
          }
     }
   ecore_main_loop_quit();
}

static void
_on_edj_name_changed_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *bt_open = data;
   elm_object_disabled_set(bt_open, true);
}

static void
_on_open_edj_cb(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   const char *path_edj = elm_object_text_get(fs_ent->edj);

   if (ui_edj_load(ap, path_edj))
     {
        evas_object_hide(elm_object_parent_widget_get(obj));
        ecore_main_loop_quit();
     }
}

static void
_log_cb(time_t time,
       Eina_Stringshare* msg,
       int type __UNUSED__)
{
   elm_entry_cursor_end_set(fs_ent->log);
   static char buf[BUFF_MAX];
   struct tm *timeinfo;
   timeinfo = localtime(&time);
   if (type == 14) /* stderr (almost all output from edje_cc) */
      snprintf(buf, BUFF_MAX, "<br> %02d:%02d:%02d:    %s",
               timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, msg);
   else /* stdout (SUMMARY) */
      snprintf(buf, BUFF_MAX, "<br>     <b>%s</b>", msg);
   elm_entry_entry_insert(fs_ent->log, buf);
}

static void
_on_compile_cb(void *data,
          Evas_Object *obj,
          void *event_info __UNUSED__)
{
   Evas_Object *bt_open = (Evas_Object *)data;

   const char *path_edc = elm_object_text_get(fs_ent->edc);
   const char *path_edj = elm_object_text_get(fs_ent->edj);
   const char *path_id = elm_object_text_get(fs_ent->id);
   const char *path_sd = elm_object_text_get(fs_ent->sd);
   const char *path_fd = elm_object_text_get(fs_ent->fd);

   if (!eina_str_has_suffix(path_edc, ".edc"))
     {
        WIN_NOTIFY_ERROR(fs_ent->win, _("Input file must have an extension '.edc'"));
        return;
     }
   if (!eina_str_has_suffix(path_edj, ".edj"))
     {
        WIN_NOTIFY_ERROR(fs_ent->win, _("Output file must have an extension '.edj'"));
        return;
     }
   elm_object_disabled_set(obj, true);
   elm_object_disabled_set(bt_open, true);

   elm_entry_cursor_end_set(fs_ent->log);
   elm_entry_entry_insert(fs_ent->log, _("<b>Compilation started...</b>"));
   int exit_code = compile(path_edc, path_edj, path_id, path_fd, path_sd, _log_cb);
   elm_entry_cursor_end_set(fs_ent->log);
   elm_entry_entry_insert(fs_ent->log, "<br><br>");
   elm_object_disabled_set(obj, false);
   if (exit_code == 0) elm_object_disabled_set(bt_open, false);
   DBG("Compile exit code: %d", exit_code);
}

static void
_on_path_select_done(void *data,
                     Evas_Object *obj,
                     void *event_info)
{
   char *edj_name;
   char tmp_path[PATH_MAX];
   Eina_Array *array;
   int i, j;

   const char *selected = event_info;
   Evas_Object *entry = (Evas_Object *)data;
   Eina_Bool edc = entry == fs_ent->edc;
   Eina_Bool edj = entry == fs_ent->edj;

   if ((!entry) || (!selected))
     {
        ecore_main_loop_quit();
        return;
     }

   if (edc)
     {
        if (eina_str_has_suffix(selected, ".edc"))
          {
             array = eina_file_split(strdup(selected));

             i = eina_array_count(array);
             eina_strlcpy(tmp_path, "/", sizeof("/"));
             for (j = 0; j < i - 1; j++)
               {
                  eina_strlcat(tmp_path, eina_array_data_get(array, j), PATH_MAX);
                  eina_strlcat(tmp_path, "/", PATH_MAX);
               }
             fs_ent->project_name = strdup(eina_array_data_get(array, i - 1));
             eina_array_free(array);

             elm_object_text_set(fs_ent->id, tmp_path);
             elm_object_text_set(fs_ent->sd, tmp_path);
             elm_object_text_set(fs_ent->fd, tmp_path);

             elm_object_text_set(fs_ent->edc, selected);
             edj_name = strdup(selected);
             edj_name[strlen(selected) - 1] = 'j';
             elm_object_text_set(fs_ent->edj, edj_name);
             free(edj_name);

             ecore_main_loop_quit();
          }
        else
          {
             Evas_Object *win = elm_object_parent_widget_get(obj);
             WIN_NOTIFY_ERROR(win, _("The file must have an extension '.edc'"));
          }
     }
   else if (edj)
     {
        if (eina_str_has_suffix(selected, ".edj"))
          {
             elm_object_text_set(fs_ent->edj, selected);
             ecore_main_loop_quit();
          }
        else
          {
             Evas_Object *win = elm_object_parent_widget_get(obj);
             WIN_NOTIFY_ERROR(win, _("The file must have an extension '.edj'"));
          }
     }
   else /* folder selection */
     {
        elm_object_text_set(entry, selected);
        ecore_main_loop_quit();
     }
}

static void
_path_select(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *win, *bg, *fs;
   Evas_Object *entry = (Evas_Object *)data;
   Eina_Bool edc = entry == fs_ent->edc;
   Eina_Bool edj = entry == fs_ent->edj;

   const char *path = elm_object_text_get(entry);

   if (!ecore_file_exists(path))
      path = getenv("HOME");

   if (!fs_ent->parent) return;

   MODAL_WINDOW_ADD(win, main_window_get(), evas_object_data_get(entry, FS_TITLE), _on_path_select_done, NULL);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);

   FILESELECTOR_ADD(fs, win, _on_path_select_done, entry);
   if (edj)
     elm_fileselector_is_save_set(fs, true);
   else if (!edc) /* folder selection */
     elm_fileselector_folder_only_set(fs, EINA_TRUE);

   elm_fileselector_selected_set(fs, path);
   elm_win_resize_object_add(win, fs);

   ecore_main_loop_begin();

   evas_object_del(win);
}

static void
_clean_log(void *data,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   Evas_Object *entry = (Evas_Object *)data;
   elm_object_text_set(entry, "");
}

Eina_Bool
compile_dialog(App_Data *ap)
{
   Evas_Object *button_box, *layout;
   Evas_Object *label, *fs_entry;
   Evas_Object *bt, *bt_c, *bt_o;
   Evas_Object *win, *bg;

   if ((!ap) || (!ap->win))
     {
        ERR("App Data or Window is NULL. Something is wrong.");
        return EINA_FALSE;
     }

   if (!ui_close_project_request(ap,
                                 _("You want to compile and open new theme, but now <br/>"
                                   "you have opened project. If you dont save opened<br/>"
                                   "project before opening compiled one "
                                   "all your changes will be lost!")))
     return false;

   MODAL_WINDOW_ADD(win, ap->win, _("Compile EDC file"), _on_cancel_cb, ap);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);

   if (!fs_ent)
     fs_ent = mem_malloc(sizeof(fs_entries));
   fs_ent->parent = ap->win;
   fs_ent->win = win;
   fs_ent->project_name = NULL;

   layout = elm_layout_add(win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, EFLETE_EDJ, "compile_dialog");

   #define _BUTTON_ADD(button, parent, text, func, data) \
      button = elm_button_add(parent); \
      elm_object_text_set(button, text); \
      elm_object_style_set(button, DEFAULT_STYLE); \
      evas_object_smart_callback_add(button, "clicked", func, data); \
      evas_object_show(button);

   #define _ITEM_ADD(parent, label_text, button_text, title, _fs, func, data) \
      LABEL_ADD(parent, label, label_text) \
      fs_entry = elm_entry_add(parent); \
      elm_entry_single_line_set(fs_entry, true); \
      elm_object_style_set(fs_entry, DEFAULT_STYLE); \
      evas_object_data_set(fs_entry, FS_TITLE, title); \
      elm_object_text_set(fs_entry, getenv("HOME")); \
      evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0); \
      evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, EVAS_HINT_FILL); \
      elm_entry_scrollable_set(fs_entry, true); \
      _fs = fs_entry; \
      evas_object_show(fs_entry); \
      _BUTTON_ADD(bt, parent, button_text, func, data);

   _ITEM_ADD(win, _("Path to EDC:"), _("Select"), _("Select a EDC file."),
         fs_ent->edc, _path_select, fs_ent->edc);
   elm_object_part_content_set(layout, "label_edc", label);
   elm_object_part_content_set(layout, "button_edc", bt);
   elm_object_part_content_set(layout, "entry_edc", fs_ent->edc);

   _ITEM_ADD(win, _("Path to EDJ:"), _("Select"), _("Select a EDJ file."),
         fs_ent->edj, _path_select, fs_ent->edj);
   elm_object_part_content_set(layout, "label_edj", label);
   elm_object_part_content_set(layout, "button_edj", bt);
   elm_object_part_content_set(layout, "entry_edj", fs_ent->edj);

   _ITEM_ADD(win, _("Image directory:"), _("Select"), _("Select a images directory."),
         fs_ent->id, _path_select, fs_ent->id);
   elm_object_part_content_set(layout, "label_img", label);
   elm_object_part_content_set(layout, "button_img", bt);
   elm_object_part_content_set(layout, "entry_img", fs_ent->id);

   _ITEM_ADD(win, _("Sound directory:"), _("Select"), _("Select a sounds directory."),
         fs_ent->sd, _path_select, fs_ent->sd);
   elm_object_part_content_set(layout, "label_snd", label);
   elm_object_part_content_set(layout, "button_snd", bt);
   elm_object_part_content_set(layout, "entry_snd", fs_ent->sd);

   _ITEM_ADD(win, _("Font directory:"), _("Select"), _("Select a fonts directory."),
         fs_ent->fd, _path_select, fs_ent->fd);
   elm_object_part_content_set(layout, "label_fnt", label);
   elm_object_part_content_set(layout, "button_fnt", bt);
   elm_object_part_content_set(layout, "entry_fnt", fs_ent->fd);

   _ITEM_ADD(win, _("Log:"), _("Clean"), "Clean the log messages.",
         fs_ent->log, _clean_log, fs_ent->log);
   elm_object_text_set(fs_entry, "");
   elm_object_part_content_set(layout, "label_log", label);
   elm_object_part_content_set(layout, "button_log", bt);
   elm_object_part_content_set(layout, "entry_log", fs_ent->log);

   elm_entry_single_line_set(fs_ent->log, false);
   elm_entry_editable_set(fs_ent->log, false);
   elm_scroller_policy_set(fs_ent->log, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_ON);
   ewe_entry_entry_set(fs_ent->log, "");
   elm_entry_cursor_end_set(fs_ent->log);
   #undef _ITEM_ADD

   BOX_ADD(win, button_box, true, true);

   _BUTTON_ADD(bt_o, button_box, _("Open EDJ"), _on_open_edj_cb, ap);
   elm_object_disabled_set(bt_o, true);
   evas_object_smart_callback_add(fs_ent->edj, "changed", _on_edj_name_changed_cb, bt_o);
   _BUTTON_ADD(bt, button_box, _("Compile"), _on_compile_cb, bt_o);
   _BUTTON_ADD(bt_c, button_box, _("Cancel"), _on_cancel_cb, ap);
   elm_box_pack_end(button_box, bt);
   elm_box_pack_end(button_box, bt_o);
   elm_box_pack_end(button_box, bt_c);
   #undef _BUTTON_ADD

   elm_object_part_content_set(layout, "button_box", button_box);

   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, layout);
   evas_object_show(layout);

   ecore_main_loop_begin();

   free(fs_ent->project_name);
   free(fs_ent);
   fs_ent = NULL;
   evas_object_del(win);

   return EINA_TRUE;
}
