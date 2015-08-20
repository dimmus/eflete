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

#include "tabs.h"

struct _Tabs_Item {
   Group *group;
   Elm_Object_Item *toolbar_item;
   Evas_Object *content; //panes
   Evas_Object *workspace;
   Evas_Object *live_view;
};

typedef struct _Tabs_Item Tabs_Item;

struct _Tabs {
   Evas_Object *layout;
   Evas_Object *toolbar;
   Eina_List *items;
   struct {
      Elm_Object_Item *item;
      Evas_Object *content;
      Evas_Object *tabs;
   } home;
};

typedef struct _Tabs Tabs;

Tabs tabs;

static void
_content_unset(void)
{
   Evas_Object *content;

   assert(tabs.layout != NULL);
   content = elm_layout_content_unset(tabs.layout, NULL);
   evas_object_hide(content);
}

static void
_content_set(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Tabs_Item *item = (Tabs_Item *)data;

   assert(tabs.layout != NULL);

   _content_unset();
   if (item)
     elm_layout_content_set(tabs.layout, NULL, item->content);
   else
     tabs_tab_home_open(TABS_LAST);
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

Evas_Object *
tabs_add(void)
{
   assert (ap.win != NULL);

   tabs.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tabs.layout, "layout", "tabs", "default");
   tabs.toolbar = elm_toolbar_add(tabs.layout);
   elm_layout_content_set(tabs.layout, "elm.swallow.toolbar", tabs.toolbar);
   elm_object_style_set(tabs.toolbar, "tabs");
   elm_toolbar_shrink_mode_set(tabs.toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.toolbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(tabs.toolbar, 0.0);
   tabs.home.item = elm_toolbar_item_append(tabs.toolbar, "home", NULL,
                                            _content_set, NULL);
   elm_toolbar_item_selected_set(tabs.home.item, true);

   TODO("add tabs with wizards!");

   return tabs.layout;
}

void
tabs_tab_home_open(Tabs_View view __UNUSED__)
{
   assert(tabs.layout != NULL);

   if (!elm_toolbar_item_selected_get(tabs.home.item))
     elm_toolbar_item_selected_set(tabs.home.item, true);

   _content_unset();
   elm_layout_content_set(tabs.layout, NULL, tabs.home.content);

   TODO("select the tab with wizard accourdingly to view");
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

   item = mem_calloc(1, sizeof(Tabs_Item));
   item->group = group;
   item->content = elm_panes_add(tabs.layout);
   elm_panes_horizontal_set(item->content, true);
   item->workspace = workspace_add(item->content);
   //item->live_view = live_view_add(item->content, false);
   elm_object_part_content_set(item->content, "left", item->workspace);
   //elm_object_part_content_set(item->content, "right", item->live_view);

   item->toolbar_item = elm_toolbar_item_append(tabs.toolbar, NULL, group->name,
                                               _content_set, (void *)item);
   elm_toolbar_item_selected_set(item->toolbar_item, true);
   tabs.items = eina_list_append(tabs.items, item);
}
