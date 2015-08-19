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
#include "group_manager.h"

struct _Tabs_Item {
   Group *group;
   Elm_Object_Item *item;
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

/*
static void
_content_unset(void)
{
   assert(tabs.layout != NULL);
   elm_layout_content_unset(tabs.layout, NULL);
}

static void
_content_set(Evas_Object *content)
{
   Evas_Object *content = (Evas_Object *)data;

   assert(tabs.layout != NULL);
   elm_layout_content_set(tabs.layout, NULL, content);
}
*/

Evas_Object *
tabs_add(void)
{
   assert (ap->win != NULL);

   tabs.layout = elm_layout_add(ap->win);
   elm_layout_theme_set(tabs.layout, "layout", "tabs", "default");
   tabs.toolbar = elm_toolbar_add(tabs.layout);
   elm_layout_content_set(tabs.layout, "elm.swallow.toolbar", tabs.toolbar);
   elm_object_style_set(tabs.toolbar, "tabs");
   elm_toolbar_shrink_mode_set(tabs.toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.toolbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(tabs.toolbar, 0.0);
   tabs.home.item = elm_toolbar_item_append(tabs.toolbar, "home", NULL, NULL, NULL);
   elm_toolbar_item_selected_set(tabs.home.item, true);

   TODO("add tabs with wizards!");

   return tabs.layout;
}

void
tabs_home_open(Tabs_View view __UNUSED__)
{
   assert(tabs.layout != NULL);
   assert(tabs.home.item != NULL);

   elm_toolbar_item_selected_set(tabs.home.item, true);
   TODO("select the tab with wizard accourdingly to view`");
}

