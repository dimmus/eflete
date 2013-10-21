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
* along with this program; If not, see .
*/

#include "open_file_dialog.h"

struct _fs_entries
{
   char *project_name;
   Evas_Object *edc;
   Evas_Object *id;
   Evas_Object *fd;
   Evas_Object *sd;

   Evas_Object *parent;
};

typedef struct _fs_entries fs_entries;

static fs_entries *fs_ent = NULL;

static void
_on_part_back(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_data __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_part_back(ap);
}

static void
_on_group_back(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_data __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_group_back(ap);
}

static void
_on_state_selected(void *data,
                   Evas_Object *obj,
                   void *event_info)
{
   App_Data *ap = (App_Data *)data;
   Elm_Object_Item *eoit = (Elm_Object_Item *)event_info;
   Eina_Stringshare *state = elm_object_item_data_get(eoit);
   ui_state_select(ap, obj, state);
}

static void
_on_part_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_data)
{
   App_Data *ap = (App_Data *)data;
   Part *part = (Part *) event_data;
   Evas_Object *gl_states = ui_part_select(ap, part);

   if(gl_states)
     {
        evas_object_smart_callback_add(gl_states, "st,state,select",
                                       _on_state_selected, ap);
        evas_object_smart_callback_call(gl_states, "st,state,select",
                                        elm_genlist_first_item_get(gl_states));
     }
}
static void
_on_group_clicked(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_data)
{
   App_Data *ap = (App_Data *)data;
   Group *_group = (Group *)event_data;
   ui_group_clicked(ap, _group);
}

static void
_on_edj_done(void *data, Evas_Object *obj, void *event_info)
{
   App_Data *ap = (App_Data *)data;
   const char *selected = event_info;
   Evas_Object *wd_list = ui_edj_load_done(ap, obj, selected);
   evas_object_del(ap->inwin);
   ap->inwin = NULL;
   add_callbacks_wd(wd_list, ap);
}

static void
_on_mw_fileselector_close(void *data,
                       Evas *e __UNUSED__,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ap->inwin = NULL;
   evas_object_smart_callback_call(obj, "done", NULL);
}

Eina_Bool
open_edj_file(App_Data *ap)
{
   Evas_Object *fs, *box;

   if ((!ap) || (!ap->win)) return EINA_FALSE;

   if (!ap->inwin)
     ap->inwin = mw_add(ap->win);

   OPEN_DIALOG_ADD(ap->inwin, box, fs, "Open EDJ file dialog");

   evas_object_event_callback_add(ap->inwin, EVAS_CALLBACK_FREE,
                                  _on_mw_fileselector_close, ap);
   evas_object_smart_callback_add(fs, "done", _on_edj_done, ap);

   elm_win_inwin_activate(ap->inwin);

   return EINA_TRUE;
}

static void
_on_ok_cb(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *wd_list = NULL;

   const char *path_edc = elm_object_text_get(fs_ent->edc);
   const char *path_id = elm_object_text_get(fs_ent->id);
   const char *path_sd = elm_object_text_get(fs_ent->sd);
   const char *path_fd = elm_object_text_get(fs_ent->fd);

   if (!eina_str_has_suffix(path_edc, ".edc"))
     {
        NOTIFY_ERROR("The file must have an extension '.edc'");
        return;
     }

   wd_list = ui_edc_load_done(ap, fs_ent->project_name, path_edc,
                              path_id, path_sd, path_fd);

   add_callbacks_wd(wd_list, ap);
   free(fs_ent->project_name);
   free(fs_ent);
}

void
add_callbacks_wd(Evas_Object *wd_list, App_Data *ap)
{
   if(!wd_list)
     {
        return;
     }

   evas_object_smart_callback_add(wd_list, "wl,group,select",
                                     _on_group_clicked, ap);
   evas_object_smart_callback_add(wd_list, "wl,part,select",
                                       _on_part_selected, ap);
   evas_object_smart_callback_add(wd_list, "wl,part,back",
                                       _on_part_back, ap);
   evas_object_smart_callback_add(wd_list, "wl,group,back",
                                       _on_group_back, ap);
  if (ap->ws->groupspace) ui_groupspace_unset(ap->ws->groupspace);
  if (ap->demo) ui_demospace_unset(ap->demo, ap->project);
}

static void
_on_cancel_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;

   evas_object_del(elm_object_content_get(inwin));
   evas_object_hide(inwin);
}

static void
_on_edc_done(void *data __UNUSED__,
             Evas_Object *obj,
             void *event_info)
{
   const char *selected = event_info;
   char *images, *sounds, *fonts;
   char tmp_path[PATH_MAX];
   Eina_Array *array;
   int i, j;

   if (!selected) return;

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

        #define CREATE_PATH(target, prefix, prefix_size, suffix, suffix_size) \
           size_t size_##target = prefix_size + prefix_size + 1; \
           target = mem_malloc(size_##target * sizeof(char)); \
           eina_strlcpy(target, prefix, size_##target); \
           eina_strlcat(target, suffix, size_##target);

        CREATE_PATH(images, tmp_path, strlen(tmp_path), "images/", strlen("images/"));
        elm_object_text_set(fs_ent->id, images);

        CREATE_PATH(sounds, tmp_path, strlen(tmp_path), "sounds/", strlen("sounds/"));
        elm_object_text_set(fs_ent->sd, sounds);

        CREATE_PATH(fonts, tmp_path, strlen(tmp_path), "fonts/", strlen("fonts/"));
        elm_object_text_set(fs_ent->fd, fonts);
        elm_object_text_set(fs_ent->edc, selected);

        free(images);
        free(sounds);
        free(fonts);
        #undef CREATE_PATH

        evas_object_hide(elm_object_parent_widget_get(obj));
        evas_object_del(obj);
     }
   else
     NOTIFY_ERROR("The file must have an extension '.edj'");
}

static void
_edc_select(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Evas_Object *fs;
   const char *path = elm_object_text_get(fs_ent->edc);

   if (!ecore_file_exists(path))
     path = getenv("HOME");

   if (!fs_ent->parent) return;

   Evas_Object *inwin = mw_add(fs_ent->parent);
   mw_title_set(inwin, "Open EDC file dialog");
   evas_object_focus_set(inwin, EINA_TRUE);

   fs = elm_fileselector_add(inwin);

   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_fileselector_path_set(fs, path);
   elm_fileselector_buttons_ok_cancel_set(fs, EINA_TRUE);
   elm_fileselector_expandable_set(fs, EINA_FALSE);
   elm_fileselector_mode_set(fs, ELM_FILESELECTOR_LIST);
   evas_object_smart_callback_add(fs, "done", _on_edc_done, NULL);

   elm_win_inwin_content_set(inwin, fs);

   evas_object_show(fs);
   elm_win_inwin_activate(inwin);
}

static void
_on_path_done(void *data,
             Evas_Object *obj,
             void *event_info)
{
   const char *selected = event_info;
   Evas_Object *entry = (Evas_Object *)data;
   if (selected)
     elm_object_text_set(entry, selected);

   evas_object_hide(elm_object_parent_widget_get(obj));
   evas_object_del(obj);
}

static void
_path_select(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *fs;
   Evas_Object *entry = (Evas_Object *)data;
   const char *path = elm_object_text_get(entry);

   if (!ecore_file_exists(path))
     path = getenv("HOME");

   if (!fs_ent->parent) return;

   Evas_Object *inwin = mw_add(fs_ent->parent);
   mw_title_set(inwin, "Choose path dialog");
   evas_object_focus_set(inwin, EINA_TRUE);

   fs = elm_fileselector_add(inwin);

   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_fileselector_path_set(fs, path);
   elm_fileselector_buttons_ok_cancel_set(fs, EINA_TRUE);
   elm_fileselector_expandable_set(fs, EINA_FALSE);
   elm_fileselector_mode_set(fs, ELM_FILESELECTOR_LIST);
   elm_fileselector_folder_only_set(fs, EINA_TRUE);
   evas_object_smart_callback_add(fs, "done", _on_path_done, entry);

   elm_win_inwin_content_set(inwin, fs);

   evas_object_show(fs);
   elm_win_inwin_activate(inwin);
}

Eina_Bool
open_edc_file(App_Data *ap)
{
   Evas_Object *box, *box_item;
   Evas_Object *label, *fs_entry;
   Evas_Object *bt;

   if ((!ap) || (!ap->win))
     {
        ERR("App Data or Window is NULL. Something is wrong.");
        return EINA_FALSE;
     }

   if (!ap->inwin)
     ap->inwin = mw_add(ap->win);
   mw_title_set(ap->inwin, "Open EDC file dialog");
   evas_object_event_callback_add(ap->inwin, EVAS_CALLBACK_FREE,
                                  _on_mw_fileselector_close, ap);
   evas_object_focus_set(ap->inwin, EINA_TRUE);

   if (!fs_ent)
     fs_ent = mem_malloc(sizeof(fs_entries));
   fs_ent->parent = ap->win;

   box = elm_box_add(ap->inwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(box);

   #define _BUTTON_ADD(box, text, func, data) \
      bt = elm_button_add(box); \
      elm_object_text_set(bt, text); \
      elm_object_style_set(bt, DEFAULT_STYLE); \
      evas_object_smart_callback_add(bt, "clicked", func, data); \
      evas_object_show(bt); \
      elm_box_pack_end(box, bt);

   #define _ITEM_ADD(box, label_text, button_text, _fs, func, data) \
      BOX_ADD(box, box_item, EINA_TRUE, EINA_FALSE) \
      label = elm_label_add(box_item); \
      elm_object_text_set(label, label_text); \
      evas_object_show(label); \
      elm_box_pack_end(box_item, label); \
      fs_entry = elm_entry_add(box_item); \
      elm_object_style_set(fs_entry, DEFAULT_STYLE); \
      elm_object_text_set(fs_entry, getenv("HOME")); \
      evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0); \
      evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, EVAS_HINT_FILL); \
      elm_entry_scrollable_set(fs_entry, EINA_TRUE); \
      _fs = fs_entry; \
      evas_object_show(fs_entry); \
      elm_box_pack_end(box_item, fs_entry); \
      _BUTTON_ADD(box_item, button_text, func, data); \
      elm_box_pack_end(box, box_item);

   _ITEM_ADD(box, "Path to EDC:", "[Select]", fs_ent->edc, _edc_select,
             NULL);
   _ITEM_ADD(box, "Image directory:", "[Select]", fs_ent->id, _path_select,
             fs_ent->id);
   _ITEM_ADD(box, "Sound directory:", "[Select]", fs_ent->sd, _path_select,
             fs_ent->sd);
   _ITEM_ADD(box, "Font directory:", "[Select]", fs_ent->fd, _path_select,
             fs_ent->fd);
   #undef _ITEM_ADD

   box_item = elm_box_add(box);
   evas_object_size_hint_weight_set(box_item, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box_item, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(box_item, EINA_TRUE);
   evas_object_show(box_item);

   _BUTTON_ADD(box_item, "Ok", _on_ok_cb, ap);
   _BUTTON_ADD(box_item, "Cancel", _on_cancel_cb, ap->inwin);
   #undef _BUTTON_ADD
   elm_box_pack_end(box, box_item);

   elm_win_inwin_content_set(ap->inwin, box);
   elm_win_inwin_activate(ap->inwin);

   return EINA_TRUE;
}
