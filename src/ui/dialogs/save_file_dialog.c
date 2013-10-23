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
   void *obj;
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
   evas_object_hide(elm_object_parent_widget_get(cbdata->obj));
   evas_object_del(cbdata->obj);
   evas_object_del(cbdata->popup);
   free(cbdata);
}

static void
_on_edj_done(void *data,
             Evas_Object *obj,
             void *event_info)
{
   /*TODO: change a project name and set to ui widget list */
   App_Data *ap = (App_Data *)data;
   const char *selected = event_info;
   if (ecore_file_exists(selected))
     {
        if (eina_str_has_suffix(selected, ".edj"))
          {
             cb_data *d_data = mem_malloc(sizeof(cb_data));

             Evas_Object *popup, *btn1, *btn2;
             popup = elm_popup_add(ap->win_layout);
             elm_object_style_set(popup, "eflete/popup");

             d_data->obj = obj;
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
             evas_object_hide(elm_object_parent_widget_get(obj));
             evas_object_del(obj);
          }
        else if (selected)
          {
             NOTIFY_ERROR("The file must have a extension '.edj'");
          }
        else if (!selected)
          {
             evas_object_hide(elm_object_parent_widget_get(obj));
             evas_object_del(obj);
          }
     }
}

static void
__on_mw_fileselector_close(void *data,
                       Evas *e __UNUSED__,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ap->inwin = NULL;
   evas_object_smart_callback_call(obj, "done", NULL);
}

Eina_Bool
save_as_edj_file(App_Data *ap)
{
   Evas_Object *fs;

   if ((!ap) || (!ap->win) || (!ap->project)) return EINA_FALSE;

   if (!ap->inwin)
     ap->inwin = mw_add(ap->win);
   mw_title_set(ap->inwin, "Save as EDJ file");
   evas_object_event_callback_add(ap->inwin, EVAS_CALLBACK_FREE,
                                  __on_mw_fileselector_close, ap);
   evas_object_focus_set(ap->inwin, EINA_TRUE);

   fs = elm_fileselector_add(ap->inwin);

   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_fileselector_path_set(fs, getenv("HOME"));
   elm_fileselector_buttons_ok_cancel_set(fs, EINA_TRUE);
   elm_fileselector_expandable_set(fs, EINA_FALSE);
   elm_fileselector_mode_set(fs, ELM_FILESELECTOR_LIST);
   elm_fileselector_is_save_set(fs, EINA_TRUE);
   evas_object_smart_callback_add(fs, "done", _on_edj_done, ap);

   elm_win_inwin_content_set(ap->inwin, fs);

   evas_object_show(fs);
   elm_win_inwin_activate(ap->inwin);

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
