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
#include "wizard.h"
#include "save_file_dialog.h"
#include "preference.h"
#include "style_editor.h"
#include "image_editor.h"
#include "sound_editor.h"
#include "program_editor.h"
#include "about_window.h"

static int _menu_delayed_event = 0;

static void
_on_new_project(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{

}

static void
_on_open_project(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   project_open();
}

static void
_on_import_edj(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   wizard_import_edj_add(ap);
}

static void
_on_import_edc(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{

}

static void
_on_save(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   project_save();
}

static void
_on_save_as(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{

}

static void
_on_export_to_edc(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{

}

static void
_on_close_project(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap;

   ap = (App_Data *)data;
   if (!ap->project) return;

   pm_project_close(ap->project);
   blocks_hide(ap);
   ui_menu_base_disabled_set(ap->menu_hash, true);
   ui_menu_disable_set(ap->menu_hash, _("Save project"), true);
   ui_menu_disable_set(ap->menu_hash, _("Close project"), true);
}

static void
_on_exit(void *data,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_main_window_del(ap);
}

static void
_on_preference(void *data,
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
_on_style_window(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   style_editor_window_add(ap->project);
}

static void
_on_image_editor(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   image_editor_window_add(ap->project, MULTIPLE);
}

static void
_on_sound_editor(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   sound_editor_window_add(ap->project, SOUND_EDITOR_SINGLE);
}

static void
_on_ccl_editor(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   colorclass_viewer_add(ap->project);
}

static void
_on_prog_editor(void *data __UNUSED__,
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
_on_about_window(void *data __UNUSED__,
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

   //ecore_event_handler_add(_menu_delayed_event, _menu_event_handler_cb, NULL);
   menu = elm_win_main_menu_get(ap->win);
   menu_elms_hash = eina_hash_string_small_new(NULL);

#define ITEM_MENU_ADD(menu_obj, parent_menu, icon, label, callback, data, ret) \
   ret = elm_menu_item_add(menu_obj, parent_menu, icon, label, callback, data); \
   eina_hash_add(menu_elms_hash, label, ret);

   ITEM_MENU_ADD(menu, NULL, NULL, _("File"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("New project"), _on_new_project, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Open project"), _on_open_project, ap, it);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Import edj-file"), _on_import_edj, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Import edc-file"), _on_import_edc, ap, it);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Save"), _on_save, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Save as..."), _on_save_as, ap, it);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Export to edc..."), _on_export_to_edc, ap, it);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Close project"), _on_close_project, ap, it);
   elm_object_item_disabled_set(it, true);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Exit"), _on_exit, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, _("Edit"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Preference"), _on_preference, ap, it);

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
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Styles"), _on_style_window, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Images"), _on_image_editor, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Sounds"), _on_sound_editor, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Colorclasses"), _on_ccl_editor, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("Programs"), _on_prog_editor, ap, it);
   elm_object_item_disabled_set(it, true);

   ITEM_MENU_ADD(menu, NULL, NULL, _("Help"), NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, _("About"), _on_about_window, ap, it);

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

   ITEM_TB_ADD(toolbar, EFLETE_IMG_PATH"icon-new_project.png", _("New project"), _on_new_project, ap, it);
   ITEM_TB_ADD(toolbar, EFLETE_IMG_PATH"icon-open_project.png", _("Open project"), _on_open_project, ap, it);
   ITEM_TB_ADD(toolbar, EFLETE_IMG_PATH"icon_save.png", _("Save project"), _on_save, ap, it);

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
