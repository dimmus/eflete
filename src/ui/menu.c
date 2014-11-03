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

#include "main_window.h"
#include "compile_dialog.h"
#include "open_file_dialog.h"
#include "save_file_dialog.h"
#include "preference.h"
#include "style_editor.h"
#include "image_editor.h"
#include "sound_editor.h"
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
      SAVE_AS_EDJ,
      NEW_THEME,
      MENU_EXIT
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

   Evas_Object *nf;

   switch (menu_event->type)
     {
      case OPEN_EDC:
        {
           compile_dialog(menu_event->ap);
           break;
        }
      case OPEN_EDJ:
        {
           open_edj_file(menu_event->ap);
           break;
        }
      case EXPORT_EDC:
        {
           save_as_edc_file(menu_event->ap);
           break;
        }
      case SAVE_EDJ:
         if (save_edj_file(menu_event->ap))
           {
              nf = ui_block_widget_list_get(menu_event->ap);
              ui_widget_list_title_set(nf, menu_event->ap->project->name);
              STATUSBAR_PROJECT_PATH(menu_event->ap, menu_event->ap->project->edj);
           }
         break;
      case SAVE_AS_EDJ:
         if (save_as_edj_file(menu_event->ap))
           {
              nf = ui_block_widget_list_get(menu_event->ap);
              ui_widget_list_title_set(nf, menu_event->ap->project->name);
              STATUSBAR_PROJECT_PATH(menu_event->ap, menu_event->ap->project->edj);
           }
         break;
      case NEW_THEME:
        {
           new_theme_create(menu_event->ap);
           break;
        }
      case MENU_EXIT:
        {
           ui_main_window_del(menu_event->ap);
        }
     }
   ui_menu_locked_set(menu_event->ap->menu_hash, false);
   return ECORE_CALLBACK_DONE;
}

#define DELAYED_CB(NAME, EVENT) \
static void \
NAME(void *data, \
     Evas_Object *obj __UNUSED__, \
     void *event_info __UNUSED__) \
{ \
   Menu_Event *menu_event = mem_malloc(sizeof(Menu_Event)); \
   menu_event->ap = data; \
   menu_event->type = EVENT; \
   ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL); \
} \

DELAYED_CB(_on_new_theme_menu, NEW_THEME);
DELAYED_CB(_on_edc_open_menu, OPEN_EDC);
DELAYED_CB(_on_edj_open_menu, OPEN_EDJ);
DELAYED_CB(_on_save_menu, SAVE_EDJ);
DELAYED_CB(_on_save_as_menu, SAVE_AS_EDJ);
DELAYED_CB(_on_export_edc_menu, EXPORT_EDC);
DELAYED_CB(_on_exit_menu, MENU_EXIT);

#undef DELAYED_CB

static void
_on_preferences_window_menu(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   preferences_window_add(ap->project);
}

static void
_on_view_separate(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Eina_Bool sep = workspace_separate_mode_get(ap->workspace);
   workspace_separate_mode_set(ap->workspace, !sep);
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
_on_view_rulers(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("rulers"));
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
   evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("rel"));
}

static void
_on_view_ruler_abs(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("abs"));
}

static void
_on_view_ruler_both(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("abs&rel"));
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
_on_sound_editor_menu(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   sound_editor_window_add(ap->project, SOUND_EDITOR_SINGLE);
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

   ITEM_MENU_ADD(menu, NULL, NULL, _("Edit"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Preferences"), _on_preferences_window_menu, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, _("View"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Workspace"), NULL, NULL, sub_menu);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Zoom in"), _on_view_zoom_in, ap, it);
   elm_object_item_disabled_set(it, true);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Zoom out"), _on_view_zoom_out, ap, it);
   elm_object_item_disabled_set(it, true);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Separate"), _on_view_separate, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Rulers"), NULL, NULL, sub_menu);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Show/Hide rulers"), _on_view_rulers, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Absolute scale"), _on_view_ruler_abs, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Relative scale"), _on_view_ruler_rel, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, _("Both"), _on_view_ruler_both, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Show/Hide object area"), _on_view_highlight, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, _("Editors"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Styles"), _on_style_window_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Images"), _on_image_editor_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Sounds"), _on_sound_editor_menu, ap, it);
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

#define ITEM_TB_ADD(toolbar_obj, icon, label, callback, data, ret) \
   ret = elm_toolbar_item_append(toolbar_obj, icon, label, callback, data); \
   eina_hash_add(menu_elms_hash, label, ret);

   ITEM_TB_ADD(toolbar, EFLETE_IMG_PATH"icon-new_project.png", _("New project"), _on_new_theme_menu, ap, it);
   ITEM_TB_ADD(toolbar, EFLETE_IMG_PATH"icon-open_project.png", _("Open project"), _on_edj_open_menu, ap, it);
   ITEM_TB_ADD(toolbar, EFLETE_IMG_PATH"icon_save.png", _("Save project"), _on_save_menu, ap, it);

   elm_object_item_disabled_set(it, true);

#undef ITEM_TB_ADD
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
   if (!menu_hash)
     {
        ERR("Hash is NULL");
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
   if (!menu_hash) return false;

   Eina_Bool result = true;
   result = ui_menu_disable_set(menu_hash, _("Save"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Save as..."), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Export to edc..."), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Workspace"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Separate"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Rulers"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Show/Hide object area"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Styles"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Images"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Sounds"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Colorclasses"), flag) && result;
   return result;
}

Eina_Bool
ui_menu_locked_set(Eina_Hash *menu_hash, Eina_Bool flag)
{
   if (!menu_hash) return false;

   Eina_Bool result = true;
   result = ui_menu_disable_set(menu_hash, _("File"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Edit"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("View"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Editors"), flag) && result;
   result = ui_menu_disable_set(menu_hash, _("Help"), flag) && result;
   return result;
}
