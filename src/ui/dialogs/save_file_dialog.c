/**
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

#include "save_file_dialog.h"

struct _cb_data
{
   Project *project;
   char *path;
   void *popup;
   Eina_Bool *cancel;
};

typedef struct _cb_data cb_data;

static Eina_Bool
_save_internal(Project *project, const char *path)
{
   if (pm_save_project_to_swap(project))
     {
        if (path)
          {
             if (pm_save_as_project_edj(project, path))
               {
                  NOTIFY_INFO(3, _("Theme saved: %s"), path);
                  return true;
               }
             else NOTIFY_ERROR(_("Theme can not be saved: %s"), path)
          }
        else
          {
             if (pm_save_project_edj(project))
               {
                  NOTIFY_INFO(3, _("Theme saved"));
                  return true;
               }
             else NOTIFY_ERROR(_("Theme can not be saved"))
          }
     }
   return false;
}

static void
_cancel_cb(void *data,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   cb_data *cbdata = (cb_data *)data;
   evas_object_del(cbdata->popup);
   free(cbdata);
}

static void
_ok_cb(void *data,
       Evas_Object *obj __UNUSED__,
       void *event_info __UNUSED__)
{
   cb_data *cbdata = (cb_data *)data;

   *cbdata->cancel = !_save_internal(cbdata->project, cbdata->path);

   evas_object_del(cbdata->popup);
   free(cbdata);
   ecore_main_loop_quit();
}

static void
_on_cancel_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Eina_Bool *cancel = data;
   *cancel = true;
   ecore_main_loop_quit();
}

static void
_on_edj_done(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   /*TODO: change a project name and set to ui widget list */
   App_Data *ap = app_data_get();
   Eina_Bool *cancel = data;
   const char *selected = event_info;
   if ((!data) || (!ap) || (!selected) || (!strcmp(selected, "")))
     {
        ecore_main_loop_quit();
        return;
     }

   if (!eina_str_has_suffix(selected, ".edj"))
     {
        Evas_Object *win = elm_object_parent_widget_get(obj);
        WIN_NOTIFY_ERROR(win, _("The file must have an extension '.edj'"));
     }
   else
     {
        if (ecore_file_exists(selected))
          {
             cb_data *d_data = mem_malloc(sizeof(cb_data));

             Evas_Object *popup, *btn1, *btn2;
             Evas_Object *win = elm_object_parent_widget_get(obj);
             popup = elm_popup_add(win);
             elm_object_style_set(popup, "eflete");

             d_data->popup = popup;
             d_data->path = event_info;
             d_data->project = ap->project;
             d_data->cancel = cancel;

             elm_object_part_text_set(popup, "title,text", _("Warning"));
             elm_object_text_set(popup, _("File is already exist. Would"
                                          "you like to replace it?"));
             BUTTON_ADD(popup, btn1, _("OK"));
             elm_object_part_content_set(popup, "button1", btn1);
             evas_object_smart_callback_add(btn1, "clicked", _ok_cb, d_data);

             BUTTON_ADD(popup, btn2, _("Cancel"));
             elm_object_part_content_set(popup, "button2", btn2);
             evas_object_smart_callback_add(btn2, "clicked", _cancel_cb, d_data);

             evas_object_show(popup);
          }
        else
          {
             *cancel = !_save_internal(ap->project, selected);
             ecore_main_loop_quit();
          }
     }
}

static void
_on_edc_done(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   App_Data *ap = app_data_get();
   Eina_Bool *cancel = data;
   const char *selected = (const char *)event_info;
   if ((!data) || (!selected) || (!strcmp(selected, "")))
     {
        ecore_main_loop_quit();
        return;
     }
   *cancel = !pm_export_to_edc(ap->project, selected, NULL);
   ecore_main_loop_quit();
}

static Eina_Bool
_save_as_edx_file(App_Data *ap,
                  const char* title,
                  Evas_Smart_Cb done_cb,
                  Eina_Bool folder_only)
{
   Evas_Object *win, *bg, *fs;
   Eina_Bool cancel = false;

   if ((!ap->win) || (!ap->project)) return false;

   MODAL_WINDOW_ADD(win, main_window_get(), title, _on_cancel_cb, &cancel);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);

   FILESELECTOR_ADD(fs, win, done_cb, &cancel);
   elm_fileselector_is_save_set(fs, true);
   elm_fileselector_folder_only_set(fs, folder_only);
   elm_fileselector_path_set(fs, getenv("HOME"));
   elm_win_resize_object_add(win, fs);

   ecore_main_loop_begin();

   evas_object_del(win);

   return !cancel;
}

Eina_Bool
save_as_edj_file(App_Data *ap)
{
   if (!ap) return false;
   return _save_as_edx_file(ap, _("Save as EDJ file"), _on_edj_done, false);
}

Eina_Bool
save_as_edc_file(App_Data *ap)
{
   if (!ap) return false;
   return _save_as_edx_file(ap, _("Export to EDC"), _on_edc_done, true);
}

Eina_Bool
save_edj_file(App_Data *ap)
{
   if (!ap) return false;
   if (ap->project->is_new)
     return _save_as_edx_file(ap, _("Save as EDJ file"), _on_edj_done, false);
   else
     return _save_internal(ap->project, NULL);
}
