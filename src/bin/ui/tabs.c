/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "tabs_private.h"
#include "workspace.h"
#include "tabs.h"
#include "new_history.h"
#include "signals.h"

#include "style_editor.h"
#include "image_editor.h"
#include "sound_editor.h"
#include "animator.h"

struct _Tabs_Item {
   Group *group;
   Elm_Object_Item *toolbar_item;
   Evas_Object *content;
   Eina_Bool need_recalc : 1;
};

typedef struct _Tabs_Item Tabs_Item;

struct _Tabs {
   Evas_Object *layout;
   Evas_Object *toolbar;
   Evas_Object *toolbar_editors;
   Evas_Object *box;
   Elm_Object_Item *selected;
   Eina_List *items;
   Evas_Object *current_workspace;
   Group *current_group;
   struct {
      Elm_Object_Item *item_home;
      Elm_Object_Item *item_sound;
      Elm_Object_Item *item_text;
      Elm_Object_Item *item_image;
      Elm_Object_Item *item_colorclass;
      /* those are used for showing home params */
      Evas_Object *content;
      Evas_Object *tabs;
      Elm_Object_Item *tab_project_info;
      Evas_Object *content_project_info;
      Elm_Object_Item *tab_open_project;
      Evas_Object *content_open_project;
      Elm_Object_Item *tab_new_project;
      Evas_Object *content_new_project;
      Elm_Object_Item *tab_import_edj;
      Evas_Object *content_import_edj;
      Elm_Object_Item *tab_import_edc;
      Evas_Object *content_import_edc;
      /* editors */
      Evas_Object *content_image_editor;
      Evas_Object *content_sound_editor;
      Evas_Object *content_text_editor;
      Evas_Object *content_colorclass_editor;
   } menu;
};

typedef struct _Tabs Tabs;

Tabs tabs;

static void
_content_unset(void)
{
   Evas_Object *content;

   assert(tabs.layout != NULL);
   tabs.current_workspace = NULL;
   tabs.current_group = NULL;
   content = elm_layout_content_unset(tabs.layout, NULL);
   evas_object_hide(content);
}

static void
_content_set(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Tabs_Item *item = (Tabs_Item *)data;
   Elm_Object_Item *toolbar_item = event_info;

   assert(tabs.layout != NULL);
   if (tabs.selected == toolbar_item) return;
   elm_toolbar_item_selected_set(tabs.selected, false);
   tabs.selected = toolbar_item;

   if (item)
     {
        _content_unset();
        elm_layout_content_set(tabs.layout, NULL, item->content);
        tabs.current_workspace = item->content;
        tabs.current_group = item->group;
        if (ap.project)
          ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, false);
        if (item->need_recalc)
          {
             workspace_edit_object_recalc(tabs.current_workspace);
             item->need_recalc = false;
          }
     }
   else
     {
        tabs.current_workspace = NULL;
        tabs.current_group = NULL;
        if (ap.project)
          ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

        if (toolbar_item == tabs.menu.item_sound)
          {
             tabs_menu_tab_open(TAB_SOUND_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_SOUND_EDITOR_TAB_CLICKED, NULL);
          }
        else if (toolbar_item == tabs.menu.item_text)
          {
             tabs_menu_tab_open(TAB_STYLE_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_STYLE_EDITOR_TAB_CLICKED, NULL);
          }
        else if (toolbar_item == tabs.menu.item_image)
          {
             tabs_menu_tab_open(TAB_IMAGE_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_EDITOR_TAB_CLICKED, NULL);
          }
        else if (toolbar_item == tabs.menu.item_colorclass)
          {
             tabs_menu_tab_open(TAB_COLORCLASS_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_COLOR_EDITOR_TAB_CLICKED, NULL);
          }
        else
          {
             tabs_menu_tab_open(TAB_LAST);
             evas_object_smart_callback_call(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, NULL);
          }
     }

   /* call 'tab,changed' on tab click, and sent Group accociated with clicked
    * tab or NULL, if click on 'home' tab */
   evas_object_smart_callback_call(ap.win, SIGNAL_TAB_CHANGED,
                                   item ? item->group : NULL);
}

static Tabs_Item *
_find_tab(Group *group)
{
   Eina_List *l;
   Tabs_Item *item;

   EINA_LIST_FOREACH(tabs.items, l, item)
     {
        if (item->group == group)
          return item;
     }
   return NULL;
}

static void
_del_tab(Tabs_Item *item)
{
   elm_object_item_del(item->toolbar_item);
   gm_group_edit_object_unload(item->group);
   /* delete pans with workspace and liveview */
   evas_object_del(item->content);
   history_del(item->group->history);
   item->group->history = NULL;
   evas_object_smart_callback_call(ap.win, SIGNAL_TAB_CLOSE, item->group);
   free(item);
}

static void
_home_tab_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   evas_object_hide(elm_layout_content_unset(tabs.menu.content, NULL));
   elm_layout_content_set(tabs.menu.content, NULL, data);
}

static void
_property_attribute_changed(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *ei __UNUSED__)
{
   assert(tabs.current_workspace != NULL);
   workspace_edit_object_recalc(tabs.current_workspace);
}

static void
_part_renamed(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Rename *ren = ei;
   Part_ *part;

   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);
   assert(ren != NULL);

   part = pm_resource_unsorted_get(tabs.current_group->parts, ren->old_name);
   gm_part_rename(part, ren->new_name);
   workspace_group_navigator_update_part(tabs.current_workspace, part);
   TODO("update live_view here");
}

static void
_part_unselected(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_highlight_unset(tabs.current_workspace);
}

static void
_project_changed(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   /* project may be changed in editors for example */
}

static void
_editor_saved(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *ei __UNUSED__)
{
   Eina_List *l;
   Tabs_Item *item;

   pm_dev_file_reload(ap.project);
   EINA_LIST_FOREACH(tabs.items, l, item)
     {
        edje_object_mmap_set(item->group->edit_object, ap.project->mmap_file, item->group->name);
        if (item->content == tabs.current_workspace)
          {
             workspace_edit_object_recalc(tabs.current_workspace);
          }
        else
          item->need_recalc = true;
     }
}

static void
_project_opened(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{
   tabs.menu.content_image_editor = image_editor_window_add(MULTIPLE);
   tabs.menu.content_sound_editor = sound_editor_window_add(SOUND_EDITOR_EDIT);
   tabs.menu.content_text_editor = style_editor_window_add();
   tabs.menu.content_colorclass_editor = colorclass_manager_add();

   elm_object_item_disabled_set(tabs.menu.item_image, false);
   elm_object_item_disabled_set(tabs.menu.item_sound, false);
   elm_object_item_disabled_set(tabs.menu.item_text, false);
   elm_object_item_disabled_set(tabs.menu.item_colorclass, false);
}

static void
_project_closed(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{
   evas_object_del(tabs.menu.content_image_editor);
   evas_object_del(tabs.menu.content_sound_editor);
   evas_object_del(tabs.menu.content_text_editor);
   evas_object_del(tabs.menu.content_colorclass_editor);

   tabs.menu.content_image_editor = NULL;
   tabs.menu.content_sound_editor = NULL;
   tabs.menu.content_text_editor = NULL;
   tabs.menu.content_colorclass_editor = NULL;

   elm_object_item_disabled_set(tabs.menu.item_image, true);
   elm_object_item_disabled_set(tabs.menu.item_sound, true);
   elm_object_item_disabled_set(tabs.menu.item_text, true);
   elm_object_item_disabled_set(tabs.menu.item_colorclass, true);

   tabs_menu_tab_open(TAB_LAST);
}

static void
_editor_part_added_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Eina_Stringshare *part_name = event_info;

   assert(part_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_add(tabs.current_workspace, part_name);
}

static void
_editor_part_deleted_cb(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Eina_Stringshare *part_name = event_info;

   assert(part_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_del(tabs.current_workspace, part_name);
}

static void
_editor_part_item_added_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   const Editor_Item *editor_item = event_info;

   assert(editor_item != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_item_add(tabs.current_workspace, editor_item->part_name, editor_item->item_name);
}

Evas_Object *
tabs_add(void)
{
   assert (ap.win != NULL);

   tabs.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tabs.layout, "layout", "tabs", "default");
   BOX_ADD(tabs.layout, tabs.box, true, false);
   evas_object_size_hint_weight_set(tabs.box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_align_set(tabs.box, 0.0, 0.5);
   elm_layout_content_set(tabs.layout, "elm.swallow.toolbar", tabs.box);
   evas_object_show(tabs.box);

   /* adding toolbar for editors */
   tabs.toolbar_editors = elm_toolbar_add(tabs.layout);
   elm_object_style_set(tabs.toolbar_editors, "editor_tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar_editors, ELM_TOOLBAR_SHRINK_NONE);
   elm_toolbar_select_mode_set(tabs.toolbar_editors, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar_editors, 0.0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar_editors, 0.0, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar_editors, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar_editors);
   evas_object_show(tabs.toolbar_editors);
   /* addind two different toolbars */
   tabs.toolbar = elm_toolbar_add(tabs.layout);
   elm_object_style_set(tabs.toolbar, "tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.toolbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar);
   evas_object_show(tabs.toolbar);

   tabs.menu.content = elm_layout_add(ap.win);
   elm_layout_theme_set(tabs.menu.content, "layout", "tab_home", "default");
   tabs.menu.tabs = elm_toolbar_add(tabs.menu.content);
   elm_layout_content_set(tabs.menu.content, "elm.swallow.toolbar", tabs.menu.tabs);
   elm_toolbar_horizontal_set(tabs.menu.tabs, false);
   elm_object_style_set(tabs.menu.tabs, "tabs_vertical");
   elm_toolbar_shrink_mode_set(tabs.menu.tabs, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.menu.tabs, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(tabs.menu.tabs, 0.0);

   tabs.menu.content_open_project = _tab_open_project_add();
   tabs.menu.content_new_project = _tab_new_project_add();
   tabs.menu.content_import_edj = _tab_import_edj_add();
   tabs.menu.content_import_edc = _tab_import_edc_add();
   tabs.menu.content_project_info = _tab_project_info_add();

   tabs.menu.tab_open_project =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Open project"), _home_tab_change, tabs.menu.content_open_project);
   tabs.menu.tab_new_project =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("New project"), _home_tab_change, tabs.menu.content_new_project);
   tabs.menu.tab_import_edj =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Import edj-file"), _home_tab_change, tabs.menu.content_import_edj);
   tabs.menu.tab_import_edc =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Import edc-file"), _home_tab_change, tabs.menu.content_import_edc);
   tabs.menu.tab_project_info =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Project info"), _home_tab_change, tabs.menu.content_project_info);

   tabs.menu.item_home = elm_toolbar_item_append(tabs.toolbar_editors, "home", NULL,
                                                 _content_set, NULL);
   tabs.menu.item_image = elm_toolbar_item_append(tabs.toolbar_editors, "image2", NULL,
                                                  _content_set, NULL);
   tabs.menu.item_sound = elm_toolbar_item_append(tabs.toolbar_editors, "sound2", NULL,
                                                  _content_set, NULL);
   tabs.menu.item_text = elm_toolbar_item_append(tabs.toolbar_editors, "text2", NULL,
                                                 _content_set, NULL);
   tabs.menu.item_colorclass = elm_toolbar_item_append(tabs.toolbar_editors, "color", NULL,
                                                       _content_set, NULL);

   elm_toolbar_item_selected_set(tabs.menu.item_home, true);
   elm_layout_content_set(tabs.layout, NULL, tabs.menu.content);

   elm_object_item_disabled_set(tabs.menu.item_image, true);
   elm_object_item_disabled_set(tabs.menu.item_sound, true);
   elm_object_item_disabled_set(tabs.menu.item_text, true);
   elm_object_item_disabled_set(tabs.menu.item_colorclass, true);

   evas_object_smart_callback_add(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, _property_attribute_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_RENAMED, _part_renamed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_UNSELECTED, _part_unselected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _project_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_SAVED, _editor_saved, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_OPENED, _project_opened, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CLOSED, _project_closed, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ADDED, _editor_part_added_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_DELETED, _editor_part_deleted_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_ADDED, _editor_part_item_added_cb, NULL);
   //evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_DELETED, _editor_part_item_deleted_cb, NULL);
   //evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_RESTACKED, _editor_part_restacked_cb, NULL);
   //evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_RESTACKED, _editor_part_item_restacked_cb, NULL);
   //evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_ADDED, _editor_state_added_cb, NULL);
   //evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_DELETED, _editor_state_deleted_cb, NULL);

   return tabs.layout;
}

void
tabs_menu_tab_open(Tabs_Menu view)
{
   assert(tabs.layout != NULL);

   _content_unset();


#define _TAB_HOME_CASE(HOME_TAB) \
   elm_toolbar_item_selected_set(tabs.menu.HOME_TAB, true); \
   elm_toolbar_item_selected_set(tabs.menu.item_home, true); \
   elm_layout_content_set(tabs.layout, NULL, tabs.menu.content);
   switch(view)
     {
        /* home case */
      case TAB_HOME_PROJECT_INFO:
         _TAB_HOME_CASE(tab_project_info);
         break;
      case TAB_HOME_OPEN_PROJECT:
         _TAB_HOME_CASE(tab_open_project);
         _tab_open_project_recents_update();
         break;
      case TAB_HOME_NEW_PROJECT:
         _TAB_HOME_CASE(tab_new_project);
         break;
      case TAB_HOME_IMPORT_EDJ:
         _TAB_HOME_CASE(tab_import_edj);
         break;
      case TAB_HOME_IMPORT_EDC:
         _TAB_HOME_CASE(tab_import_edc);
         break;

      /* editor cases */
      case TAB_IMAGE_EDITOR:
         if (ap.project)
           elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_image_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_image, true);
         break;
      case TAB_SOUND_EDITOR:
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_sound_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_sound, true);
         break;
      case TAB_COLORCLASS_EDITOR:
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_colorclass_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_colorclass, true);
         break;
      case TAB_STYLE_EDITOR:
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_text_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_text, true);
         break;
      case TAB_LAST:
         elm_toolbar_item_selected_set(tabs.menu.item_home, true);
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content);
      default:
         break;
     }
#undef _TAB_HOME_CASE
}

static void
_tab_close(void *data,
           Elm_Object_Item *it __UNUSED__,
           const char *emission __UNUSED__,
           const char *source __UNUSED__)
{
   Tabs_Item *item = (Tabs_Item *)data;
   tabs.items = eina_list_remove(tabs.items, item);
   _del_tab(item);
   if (!tabs.items) tabs_menu_tab_open(TAB_HOME_PROJECT_INFO);
}
void
tabs_tab_add(Group *group)
{
   Tabs_Item *item;
   Eina_Stringshare *msg;
   Change *change;

   assert(group != NULL);

   /* check group, maybe this group already opend */
   item = _find_tab(group);
   if (item)
     {
        elm_toolbar_item_selected_set(item->toolbar_item, true);
        return;
     }
   /* check the group history if tabs haven't tab with given group history MUST
    * be NULL */
   assert(group->history == NULL);

   gm_group_edit_object_load(ap.project, group, evas_object_evas_get(ap.win));
   group->history = history_add(group);

   item = mem_calloc(1, sizeof(Tabs_Item));
   item->group = group;
   item->content = workspace_add(tabs.layout, group);

   item->toolbar_item = elm_toolbar_item_append(tabs.toolbar, NULL, group->name,
                                               _content_set, (void *)item);
   elm_toolbar_item_selected_set(item->toolbar_item, true);
   elm_object_item_signal_callback_add(item->toolbar_item, "tab,close", "eflete", _tab_close, (void *)item);
   tabs.items = eina_list_append(tabs.items, item);

   msg = eina_stringshare_printf(_("open group \"%s\""), group->name);
   change = change_add(msg);
   history_change_add(group->history, change);
   eina_stringshare_del(msg);
}

void
tabs_current_tab_close(void)
{
   Tabs_Item *item;

   item = elm_object_item_data_get(tabs.selected);
   if (!item) return;
   tabs.items = eina_list_remove(tabs.items, item);
   _del_tab(item);
   if (!tabs.items) tabs_menu_tab_open(TAB_HOME_PROJECT_INFO);
}

void
tabs_clean(void)
{
   Tabs_Item *item;

   EINA_LIST_FREE(tabs.items, item)
     {
        _del_tab(item);
     }
   tabs.items = NULL;
}

Evas_Object *
tabs_current_workspace_get(void)
{
   return tabs.current_workspace;
}

Group *
tabs_current_group_get(void)
{
   return tabs.current_group;
}
