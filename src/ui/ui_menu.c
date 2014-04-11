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
* along with this program; If not, see http://www.gnu.org/licenses/gpl-2.0.html.
*/

#include "ui_main_window.h"
#include "open_file_dialog.h"
#include "save_file_dialog.h"
#include "style_editor.h"
#include "image_editor.h"
#include "program_editor.h"
#include "about_window.h"

static int _menu_delayed_event = 0;

struct _menu_event
{
   App_Data *ap;
   enum {
      OPEN_EDC = 0,
      OPEN_EDJ,
      EXPORT_EDC,
      SAVE_EDJ,
      SAVE_AS_EDJ
   } type;
};

typedef struct _menu_event Menu_Event;

/* TODO: Make delayed events from all menu callbacks. Otherwise menu will be
 * blocked after any long operation on week machines
 */

static Eina_Bool
_menu_event_handler_cb(void *data __UNUSED__,
                       int type __UNUSED__,
                       void *event)
{
   Menu_Event *menu_event = (Menu_Event *)event;
   ui_menu_locked_set(menu_event->ap->menu_hash, true);
   switch (menu_event->type)
      {
      case OPEN_EDC:
         open_edc_file(menu_event->ap);
      break;
      case OPEN_EDJ:
         open_edj_file(menu_event->ap);
      break;
      case EXPORT_EDC:
         save_as_edc_file(menu_event->ap);
      break;
      case SAVE_EDJ:
         if (pm_save_project_to_swap(menu_event->ap->project))
           {
              if (pm_save_project_edj(menu_event->ap->project))
                {
                   NOTIFY_INFO(3, _("Theme saved: %s"), menu_event->ap->project->edj)
                   live_view_widget_style_set(menu_event->ap->live_view, menu_event->ap->project,
                                              menu_event->ap->project->current_style);
                }
              else
                 NOTIFY_ERROR(_("Theme can not be saved: %s"), menu_event->ap->project->edj);
           }
      break;
      case SAVE_AS_EDJ:
         save_as_edj_file(menu_event->ap);
      break;
      }
   ui_menu_locked_set(menu_event->ap->menu_hash, false);
   return ECORE_CALLBACK_DONE;
}

static void
_on_close_project_cancel(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_hide(ap->popup);
   ui_menu_locked_set(ap->menu_hash, false);
}

static void
_on_close_project_save(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   evas_object_hide(ap->popup);
   if (!ap->project->edj)
     {
        ap->is_new = true;
        save_as_edj_file(ap);
     }
   else
     {
        if (pm_save_project_to_swap(ap->project))
          {
             if (pm_save_project_edj(ap->project))
                live_view_widget_style_set(ap->live_view, ap->project, ap->project->current_style);
             else
                NOTIFY_ERROR(_("Theme can not be saved: %s"), ap->project->edj);
          }
     }
}

static void
_project_not_save_new(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   evas_object_hide(ap->popup);

   if (pm_project_close(ap->project)) ap->project = NULL;

   new_theme_create(ap);
   ui_menu_locked_set(ap->menu_hash, false);
}


static void
_project_not_save_edc(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   ui_panes_hide(ap);

   if (pm_project_close(ap->project)) ap->project = NULL;

   ui_menu_base_disabled_set(ap->menu_hash, false);

   evas_object_hide(ap->popup);
   open_edc_file(ap);
   ui_menu_locked_set(ap->menu_hash, false);
}

static void
_project_not_save_edj(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   ui_panes_hide(ap);

   if (pm_project_close(ap->project)) ap->project = NULL;

   ui_menu_base_disabled_set(ap->menu_hash, false);

   evas_object_hide(ap->popup);
   open_edj_file(ap);
   ui_menu_locked_set(ap->menu_hash, false);
}


#define POPUP_CLOSE_PROJECT(MESSAGE, func_pro_not_save) \
   Evas_Object *btn, *label; \
   Eina_Stringshare *title; \
   ui_menu_locked_set(ap->menu_hash, true); \
   title = eina_stringshare_printf(_("Close project %s"), ap->project->name); \
   if (!ap->popup) \
     ap->popup = elm_popup_add(ap->win_layout); \
   elm_object_style_set(ap->popup, "eflete"); \
   elm_object_part_text_set(ap->popup, "title,text", title); \
   LABEL_ADD(ap->popup, label, MESSAGE) \
   elm_object_content_set(ap->popup, label); \
   BUTTON_ADD(ap->popup, btn, _("Save")) \
   evas_object_smart_callback_add(btn, "clicked", _on_close_project_save, ap); \
   elm_object_part_content_set(ap->popup, "button1", btn); \
   BUTTON_ADD(ap->popup, btn, _("Don't save")) \
   evas_object_smart_callback_add(btn, "clicked", func_pro_not_save, ap); \
   elm_object_part_content_set(ap->popup, "button2", btn); \
   BUTTON_ADD(ap->popup, btn, _("Cancel")) \
   evas_object_smart_callback_add(btn, "clicked", _on_close_project_cancel, ap); \
   elm_object_part_content_set(ap->popup, "button3", btn); \
   evas_object_show(ap->popup);  \
   eina_stringshare_del(title);

static void
_on_new_theme_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ap->project)
     {
        POPUP_CLOSE_PROJECT(_("You want to create a new theme, but now you have<br/>"
                            "open project. If you dont save the open project<br/>"
                            "all your changes will be lost!"),
                            _project_not_save_new);
     }
   else new_theme_create(ap);
}

static void
_on_edc_open_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Menu_Event *menu_event;
   if (ap->project)
     {
        POPUP_CLOSE_PROJECT(_("You want to open new theme, but now you have<br/>"
                            "open project. If you dont save the open project<br/>"
                            "all your changes will be lost!"),
                            _project_not_save_edc);
     }
   else
     {
        menu_event = mem_malloc(sizeof(Menu_Event));
        menu_event->ap = ap;
        menu_event->type = OPEN_EDC;
        ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
     }
}

static void
_on_edj_open_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Menu_Event *menu_event;
   if (ap->project)
     {
        POPUP_CLOSE_PROJECT(_("You want to open new theme, but now you have<br/>"
                            "open project. If you dont save the open project<br/>"
                            "all your changes will be lost!"),
                            _project_not_save_edj);
     }
   else
     {
        menu_event = mem_malloc(sizeof(Menu_Event));
        menu_event->ap = ap;
        menu_event->type = OPEN_EDJ;
        ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
     }
}

static void
_on_save_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Menu_Event *menu_event;
   if ((!ap) || (!ap->project))
     {
        ERR("Project coud'nt be saved");
        return;
     }
   menu_event = mem_malloc(sizeof(Menu_Event));
   menu_event->ap = ap;
   if (!ap->project->edj)
      menu_event->type = SAVE_AS_EDJ;
   else
      menu_event->type = SAVE_EDJ;
   ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
}

static void
_on_save_as_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Menu_Event *menu_event = mem_malloc(sizeof(Menu_Event));
   menu_event->ap = (App_Data *)data;
   menu_event->type = SAVE_AS_EDJ;
   ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
}

static void
_on_export_edc_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Menu_Event *menu_event = mem_malloc(sizeof(Menu_Event));
   menu_event->ap = (App_Data *)data;
   menu_event->type = EXPORT_EDC;
   ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
}

static void
_on_exit_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_main_window_del(ap);
}

static void
_on_view_separate(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_disable_set(ap->menu_hash, _("Show/Hide object area"), true);
}

static void
_on_view_zoom_in(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   double current_factor = workspace_zoom_factor_get(ap->workspace);
   workspace_zoom_factor_set(ap->workspace, current_factor + 0.1);
}

static void
_on_view_zoom_out(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   double current_factor = workspace_zoom_factor_get(ap->workspace);
   workspace_zoom_factor_set(ap->workspace, current_factor - 0.1);
}

static void
_on_view_ruler_hor(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,hide,hor", strdup("hor"));
}

static void
_on_view_ruler_ver(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,hide,ver", strdup("ver"));
}

static void
_on_view_legend(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   workspace_legend_visible_set(ap->workspace);
}

static void
_on_view_highlight(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "highlight,visible", NULL);
}

static void
_on_view_ruler_rel(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,hide,hor", strdup("rel"));
}

static void
_on_view_ruler_abs(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,hide,hor", strdup("abs"));
}

static void
_on_style_window_menu(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   style_editor_window_add(ap->project);
}

static void
_on_image_editor_menu(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   image_editor_window_add(ap->project, MULTIPLE);
}

static void
_on_ccl_viewer_menu(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   colorclass_viewer_add(ap->project);
}

static void
_on_prog_editor_menu(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (!ap->project->current_style)
     NOTIFY_WARNING(_("Please open the widget style for editing style programs!"))
   else
     program_editor_window_add(ap->project->current_style);
}

static void
_on_about_window_menu(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   about_window_add();
}

Evas_Object *
ui_menu_add(App_Data *ap)
{
   Evas_Object *menu, *toolbar;
   Elm_Object_Item *it, *menu_it, *sub_menu;
   Eina_Hash *menu_elms_hash = NULL;
   if (!ap)
     {
        ERR("ap is NULL");
        return NULL;
     }
   if ((!ap->win) || (!ap->win_layout)) return NULL;
   _menu_delayed_event = ecore_event_type_new();

   ecore_event_handler_add(_menu_delayed_event, _menu_event_handler_cb, NULL);
   menu = elm_win_main_menu_get(ap->win);
   menu_elms_hash = eina_hash_string_small_new(NULL);

#define ITEM_MENU_ADD(menu_obj, parent_menu, icon, label, callback, data, ret) \
   ret = elm_menu_item_add(menu_obj, parent_menu, icon, label, callback, data); \
   eina_hash_add(menu_elms_hash, label, ret);

   ITEM_MENU_ADD(menu, NULL, NULL, _("File"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("New theme"), _on_new_theme_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Open edc-file"), _on_edc_open_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Open edj-file"), _on_edj_open_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Save"), _on_save_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Save as..."), _on_save_as_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Export to edc..."), _on_export_edc_menu, ap, it);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Exit"), _on_exit_menu, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, _("View"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Workspace"), NULL, NULL, sub_menu);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Zoom in"), _on_view_zoom_in, ap, it);
   elm_object_item_disabled_set(it, true);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Zoom out"), _on_view_zoom_out, ap, it);
   elm_object_item_disabled_set(it, true);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Separate"), _on_view_separate, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Legend"), _on_view_legend, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Rulers"), NULL, NULL, sub_menu);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Show/Hide hor."), _on_view_ruler_hor, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Show/Hide ver."), _on_view_ruler_ver, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Absolute scale"), _on_view_ruler_abs, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Relative scale"), _on_view_ruler_rel, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Show/Hide object area"), _on_view_highlight, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, _("Editors"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Styles"), _on_style_window_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Images"), _on_image_editor_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Colorclasses"), _on_ccl_viewer_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Programs"), _on_prog_editor_menu, ap, it);
   elm_object_item_disabled_set(it, true);

   ITEM_MENU_ADD(menu, NULL, NULL, _("Help"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("About"), _on_about_window_menu , ap, it);

#undef ITEM_MENU_ADD

   toolbar = elm_toolbar_add(ap->win);
   elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_MENU);
   /*TODO: for this select mode need new style */
   elm_toolbar_select_mode_set(toolbar, ELM_OBJECT_SELECT_MODE_NONE);
   elm_object_style_set(toolbar, DEFAULT_STYLE);
   elm_toolbar_align_set(toolbar, 0.0);
   evas_object_size_hint_weight_set(toolbar, 0.0, 0.0);
   evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, 0.0);
   elm_object_part_content_set(ap->win_layout, "eflete.swallow.toolbar", toolbar);
   evas_object_show(toolbar);

   elm_toolbar_item_append(toolbar, EFLETE_IMG_PATH"icon-new_project.png", _("New project"), _on_new_theme_menu, ap);
   elm_toolbar_item_append(toolbar, EFLETE_IMG_PATH"icon-open_project.png", _("Open project"), _on_edj_open_menu, ap);
   elm_toolbar_item_append(toolbar, EFLETE_IMG_PATH"icon_save.png", _("Save project"), _on_save_menu, ap);

   ap->menu_hash = menu_elms_hash;
   ui_menu_base_disabled_set(ap->menu_hash, true);

   return menu;
}

Eina_Bool
ui_menu_disable_set(Eina_Hash *menu_hash, const char *name, Eina_Bool flag)
{
   Elm_Object_Item *item = NULL;

   if (!name)
     {
        ERR("Name is NULL");
        return false;
     }
   item = eina_hash_find(menu_hash, name);
   if (!item)
     {
        WARN("Coud'nt find menu item [%s] in hash", name);
        return false;
     }
   elm_object_item_disabled_set(item, flag);
   return  elm_object_item_disabled_get(item) == flag;
}

Eina_Bool
ui_menu_base_disabled_set(Eina_Hash *menu_hash, Eina_Bool flag)
{
   Eina_Bool result = true;
   result = ui_menu_disable_set(menu_hash, _("Save"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Save as..."), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Export to edc..."), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Workspace"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Separate"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Legend"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Rulers"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Show/Hide object area"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Styles"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Images"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Colorclasses"), flag) && result;
   return result;
}

Eina_Bool
ui_menu_locked_set(Eina_Hash *menu_hash, Eina_Bool flag)
{
   Eina_Bool result = true;
   result = ui_menu_disable_set(menu_hash, _("File"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("View"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Editors"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Help"), flag) && result;
   return result;
}
