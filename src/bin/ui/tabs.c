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
#include "ui_workspace.h"
#include "tabs.h"
#include "new_history.h"
#include "signals.h"

struct _Tabs_Item {
   Group *group;
   Elm_Object_Item *toolbar_item;
   Evas_Object *content; //panes
   Evas_Object *workspace;
   Evas_Object *live_view;
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
   Evas_Object *current_live_view;
   Group *current_group;
   struct {
      Elm_Object_Item *item;
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
   } home;
};

typedef struct _Tabs Tabs;

Tabs tabs;

static void
_content_unset(void)
{
   Evas_Object *content;

   assert(tabs.layout != NULL);
   tabs.current_workspace = NULL;
   tabs.current_live_view = NULL;
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
   tabs.selected = toolbar_item;

   if (item)
     {
        _content_unset();
        elm_layout_content_set(tabs.layout, NULL, item->content);
        tabs.current_workspace = item->workspace;
        tabs.current_live_view = item->live_view;
        tabs.current_group = item->group;
        if (ap.project)
          ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, false);
        if (item->need_recalc)
          {
             workspace_edit_object_recalc(tabs.current_workspace);
             live_view_theme_update(tabs.current_live_view);
             item->need_recalc = false;
          }
     }
   else
     {
        tabs_tab_home_open(TAB_HOME_LAST);
        tabs.current_workspace = NULL;
        tabs.current_live_view = NULL;
        tabs.current_group = NULL;
        if (ap.project)
          ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
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
   free(item);
}

static void
_home_tab_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   evas_object_hide(elm_layout_content_unset(tabs.home.content, NULL));
   elm_layout_content_set(tabs.home.content, NULL, data);
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
_part_added(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *ei)
{
   Part_ *part = ei;

   assert(tabs.current_workspace != NULL);

   workspace_edit_object_part_add(tabs.current_workspace, part);
   live_view_part_add(tabs.current_live_view, part);
}

static void
_part_deleted(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Part_ *part = ei;

   assert(tabs.current_workspace != NULL);

   workspace_edit_object_part_del(tabs.current_workspace, part);
   live_view_part_del(tabs.current_live_view, part);
}

static void
_project_changed(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   /* project may be changed in editors for example */
   if (tabs.current_live_view == NULL) return;
   live_view_theme_update(tabs.current_live_view);
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
        if (item->workspace == tabs.current_workspace)
          {
             workspace_edit_object_recalc(tabs.current_workspace);
             live_view_theme_update(tabs.current_live_view);
          }
        else
          item->need_recalc = true;
     }
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
//   elm_layout_content_set(tabs.layout, "elm.swallow.toolbar", tabs.toolbar_editors);
   elm_object_style_set(tabs.toolbar_editors, "tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar_editors, ELM_TOOLBAR_SHRINK_NONE);
   elm_toolbar_select_mode_set(tabs.toolbar_editors, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar_editors, 0.0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar_editors, 0.0, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar_editors, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar_editors);
   evas_object_show(tabs.toolbar_editors);
   /* addind two different toolbars */
   tabs.toolbar = elm_toolbar_add(tabs.layout);
//   elm_layout_content_set(tabs.layout, "elm.swallow.toolbar", tabs.toolbar);
   elm_object_style_set(tabs.toolbar, "tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.toolbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar);
   evas_object_show(tabs.toolbar);

   tabs.home.content = elm_layout_add(ap.win);
   elm_layout_theme_set(tabs.home.content, "layout", "tab_home", "default");
   tabs.home.tabs = elm_toolbar_add(tabs.home.content);
   elm_layout_content_set(tabs.home.content, "elm.swallow.toolbar", tabs.home.tabs);
   elm_toolbar_horizontal_set(tabs.home.tabs, false);
   elm_object_style_set(tabs.home.tabs, "tabs_vertical");
   elm_toolbar_shrink_mode_set(tabs.home.tabs, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.home.tabs, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(tabs.home.tabs, 0.0);

   tabs.home.content_open_project = _tab_open_project_add();
   tabs.home.content_new_project = _tab_new_project_add();
   tabs.home.content_import_edj = _tab_import_edj_add();

   tabs.home.tab_open_project =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Open project"), _home_tab_change, tabs.home.content_open_project);
   tabs.home.tab_new_project =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("New project"), _home_tab_change, tabs.home.content_new_project);
   tabs.home.tab_import_edj =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Import edj-file"), _home_tab_change, tabs.home.content_import_edj);
   tabs.home.tab_import_edc =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Import edc-file"), _home_tab_change, NULL);
   tabs.home.tab_project_info =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Project info"), _home_tab_change, NULL);

   tabs.home.item = elm_toolbar_item_append(tabs.toolbar_editors, "home", NULL,
                                            _content_set, NULL);
   tabs.home.item = elm_toolbar_item_append(tabs.toolbar_editors, "image2", NULL,
                                            _content_set, NULL);
   tabs.home.item = elm_toolbar_item_append(tabs.toolbar_editors, "sound2", NULL,
                                            _content_set, NULL);
   tabs.home.item = elm_toolbar_item_append(tabs.toolbar_editors, "text2", NULL,
                                            _content_set, NULL);

   elm_toolbar_item_selected_set(tabs.home.item, true);

   evas_object_smart_callback_add(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, _property_attribute_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_ADDED, _part_added, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_DELETED, _part_deleted, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _project_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_SAVED, _editor_saved, NULL);

   return tabs.layout;
}

void
tabs_tab_home_open(Tabs_View view)
{
   assert(tabs.layout != NULL);

   if (!elm_toolbar_item_selected_get(tabs.home.item))
     elm_toolbar_item_selected_set(tabs.home.item, true);

   _content_unset();
   elm_layout_content_set(tabs.layout, NULL, tabs.home.content);

   switch(view)
     {
      case TAB_HOME_PROJECT_INFO:
         elm_toolbar_item_selected_set(tabs.home.tab_project_info, true);
         break;
      case TAB_HOME_OPEN_PROJECT:
         elm_toolbar_item_selected_set(tabs.home.tab_open_project, true);
         _tab_open_project_recents_update();
         break;
      case TAB_HOME_NEW_PROJECT:
         elm_toolbar_item_selected_set(tabs.home.tab_new_project, true);
         break;
      case TAB_HOME_IMPORT_EDJ:
         elm_toolbar_item_selected_set(tabs.home.tab_import_edj, true);
         break;
      case TAB_HOME_IMPORT_EDC:
         elm_toolbar_item_selected_set(tabs.home.tab_import_edj, true);
         break;
      case TAB_HOME_LAST:
      default:
         break;
     }
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
}

void
tabs_tab_add(Group *group)
{
   Tabs_Item *item;

   assert(group != NULL);

   /* check group, maybe this group already opend */
   item = _find_tab(group);
   if (item)
     {
        elm_toolbar_item_selected_set(item->toolbar_item, true);
        return;
     }
   gm_group_edit_object_load(ap.project, group, evas_object_evas_get(ap.win));
   group->history = history_add(group);

   item = mem_calloc(1, sizeof(Tabs_Item));
   item->group = group;
   item->content = elm_panes_add(tabs.layout);
   elm_panes_horizontal_set(item->content, true);
   item->workspace = workspace_add(item->content, group);
   item->live_view = live_view_add(item->content, false, group);
   elm_object_part_content_set(item->content, "left", item->workspace);
   elm_object_part_content_set(item->content, "right", item->live_view);

   item->toolbar_item = elm_toolbar_item_append(tabs.toolbar, NULL, group->name,
                                               _content_set, (void *)item);
   elm_toolbar_item_selected_set(item->toolbar_item, true);
   elm_object_item_signal_callback_add(item->toolbar_item, "tab,close", "eflete", _tab_close, (void *)item);
   tabs.items = eina_list_append(tabs.items, item);
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
