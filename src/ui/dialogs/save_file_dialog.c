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

#include "save_file_dialog.h"

struct _cb_data
{
   Project *project;
   char *path;
   void *popup;
};

typedef struct _cb_data cb_data;

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

   const char *selected = cbdata->path;
   if (pm_save_project_to_swap(cbdata->project))
     {
        if (pm_save_as_project_edj(cbdata->project, selected))
          NOTIFY_INFO(3, "Theme saved: %s", cbdata->path)
        else
          NOTIFY_ERROR("Theme can not be saved: %s", selected);
     }
   evas_object_del(cbdata->popup);
   free(cbdata);
   ecore_main_loop_quit();
}

static void
_on_edj_done(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   /*TODO: change a project name and set to ui widget list */
   App_Data *ap = (App_Data *)data;
   const char *selected = event_info;
   if ((!selected) || (!strcmp(selected, "")))
     {
        ecore_main_loop_quit();
        return;
     }
   if (ecore_file_exists(selected))
     {
        if (eina_str_has_suffix(selected, ".edj"))
          {
             cb_data *d_data = mem_malloc(sizeof(cb_data));

             Evas_Object *popup, *btn1, *btn2;
             popup = elm_popup_add(ap->win_layout);
             elm_object_style_set(popup, "eflete");

             d_data->popup = popup;
             d_data->path = event_info;
             d_data->project = ap->project;

             elm_object_text_set(popup, "File is already exist. Would"
                                 "you like to replace it?");
             BUTTON_ADD(popup, btn1, "OK");
             elm_object_part_content_set(popup, "button1", btn1);
             evas_object_smart_callback_add(btn1, "clicked", _ok_cb, d_data);

             BUTTON_ADD(popup, btn2, "Cancel");
             elm_object_part_content_set(popup, "button2", btn2);
             evas_object_smart_callback_add(btn2, "clicked", _cancel_cb, d_data);

             evas_object_show(popup);
          }
        else
          NOTIFY_ERROR("The file must have a extension '.edj'");
     }
   else
     {
        if (eina_str_has_suffix(selected, ".edj"))
          {
             if (pm_save_project_to_swap(ap->project))
               {
                  if (pm_save_as_project_edj(ap->project, selected))
                    NOTIFY_INFO(3, "Theme saved: %s", selected)
                  else
                    NOTIFY_ERROR("Theme can not be saved: %s", selected);
               }
             ecore_main_loop_quit();
          }
        else
           NOTIFY_ERROR("The file must have a extension '.edj'");
     }
   if (ap->is_new) new_theme_create(ap);
}

static void
_on_inwin_delete(void *data,
                       Evas *e __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   Eina_Bool *dialog_deleted = (Eina_Bool *)data;
   if (!*dialog_deleted) ecore_main_loop_quit();
   *dialog_deleted = true;
}

Eina_Bool
save_as_edj_file(App_Data *ap)
{
   Evas_Object *fs;
   Eina_Bool dialog_deleted = false;

   if ((!ap) || (!ap->win) || (!ap->project)) return EINA_FALSE;

   Evas_Object *inwin = mw_add(ap->win);
   evas_object_event_callback_add(inwin, EVAS_CALLBACK_FREE,
                                  _on_inwin_delete, &dialog_deleted);
   OPEN_DIALOG_ADD(inwin, fs, "Save as EDJ file");
   elm_fileselector_is_save_set(fs, EINA_TRUE);
   evas_object_smart_callback_add(fs, "done", _on_edj_done, ap);
   evas_object_smart_callback_add(fs, "activated", _on_edj_done, ap);

   elm_win_inwin_activate(inwin);

   ecore_main_loop_begin();

   if (!dialog_deleted)
     {
        dialog_deleted = true;
        evas_object_del(fs);
        evas_object_del(inwin);
     }

   return EINA_TRUE;
}

/* TO BE IMPLEMENTED. */
Eina_Bool
save_as_edc_file(App_Data *ap)
{
   if (!ap)
     {
        ERR("App Data missing!");
        return EINA_FALSE;
     }
   if (!ap->project)
     {
        ERR("Project missing!");
        return EINA_FALSE;
     }

   pm_save_project_edc(ap->project);

   return EINA_TRUE;
}
