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

#ifndef UI_WIDGET_LIST_H
#define UI_WIDGET_LIST_H

/**
 * @defgroup WidgetList WidgetList
 * @ingroup Block
 *
 * This widget inherit from the Naviframe and Genlist.
 *
 * This widget emits the following signals:
 * @li "wl,group,select": the user double clicked on the group name;
 * @li "wl,group,back": the user clicked on the "back" button;
 * @li "wl,part,select": the user selected on the part name;
 * @li "wl,part,back": the user clicked on the "back" button;
 * @li "wl,part,add" : the user clicked on the "plus" button;
 * @li "wl,part,del" : the user clicked on the "minus" button;
 * @li "wl,part,above" : the user clicked on the "above" button;
 * @li "wl,part,below" : the user clicked on the "below" button;
 * @li "wl,part,show" : the user clicked on the "eye" in item;
 * @li "wl,part,hide": the user clicked on the "eye" in item.
 *
 * A Widget List used for view a list of widgets styles
 */

#include "main_window.h"
#include "property.h"

/**
 * Add a new 'widget list' object.
 *
 * @param parent The parent object
 * @return a new 'widget list' widget handle or NULL, on errors
 *
 * @ingroup WidgetList
 */
Evas_Object *
ui_widget_list_add(Evas_Object *parent);

/**
 * Set a title for 'widget list' object to main frame, main frame - it a list
 * of widgets (consist widgets names).
 *
 * @param object A 'widget list' object
 * @param title The title string
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_title_set(Evas_Object *object, const char *title);

/**
 * Set to a 'widget list' object a data of widgets (@see widget manager).
 *
 * @param object A 'widget list' object
 * @param project A opened project
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_data_set(Evas_Object *object, Project *project);

/**
 * Add to end new part item in list.
 *
 * @param object A 'widget list' object
 * @param style The struct @Style of current loaded group from theme,
 * in this style part will be added
 * @param name The name of new part with will be added
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_part_add(Evas_Object *object, Style *style, const char *name);

/**
 * Delete the selected item (part) from list.
 *
 * @param object A 'widget list' object
 * @param style The struct @Style of current loaded group from theme,
 * in this style part will be deleted
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_selected_part_del(Evas_Object *object, Style *style);

/**
 * Move above selected item of widgetlist.
 *
 * @param object The container object, which contain genlist of widgetlist.
 * @param style The struct @Style of current loaded group from theme.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_selected_part_above(Evas_Object *object, Style *style);

/**
 * Move below selected item of widgetlist.
 *
 * @param object The container object, which contain genlist of widgetlist.
 * @param style The struct @Style of current loaded group from theme.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_selected_part_below(Evas_Object *object, Style *style);

/**
 * Get the selected @Part in the widget list.
 *
 * @param object A 'widget list' object
 *
 * @return The selected Part, or NULL if none is selected.
 *
 * @ingroup WidgetList
 */
Part *
ui_widget_list_selected_part_get(Evas_Object *object);

/**
 * Get a list of selected @Part's in the widget list.
 *
 * @param object A 'widget list' object
 *
 * @return The list of selected Parts, or NULL if none are selected.
 *
 * @ingroup WidgetList
 */
Eina_List *
ui_widget_list_selected_parts_get(Evas_Object *object);

/**
 * Select or Unselect in a given widget list Part if is exist.
 * The list must be freed by the caller when done with eina_list_free().
 *
 * @param object A 'widget list' object
 * @param name The name of new part with will be selcted
 * @param selected Defines if given part will be selected or unselected.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_part_selected_set(Evas_Object *object,
                                 const char *part,
                                 Eina_Bool select);

/**
 * Reload content class genlist.
 *
 * @param gl_styles A 'style list' object. (genlist container)
 * @param classes A Eina_Inlist pointer for loaded classes in project
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_class_data_reload(Evas_Object *gl_styles, Eina_Inlist *classes);

/**
 * Reload layouts into widget list.
 *
 * @param obj A widget list object.
 * @param project A pointer of project structure.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_layouts_reload(Evas_Object *obj, Project *project);

/**
 * Genlist item of part view will have update with relevant data of part structure.
 *
 * @param object A widget list object.
 * @param part The name of new part wich item should be updated.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_part_update(Evas_Object *object, const char *part);

/**
 * Reload all items, that represent parts in widget list.
 *
 * @param object A widget list object.
 * @param style The struct @Style of current loaded group from theme.
 *
 * @ingroup WidgetList
 */
void
ui_widget_list_style_parts_reload(Evas_Object *object, Style *style);

/**
 * Activates n-th tab of Widget list block
 *
 * @param object A widget list object.
 * @param tab_index index of item to be activated
 * @return EINA_TRUE on success, otherwise EINA_FALSE
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_tab_activate(Evas_Object *object, unsigned int tab_index);

/**
 * Reload items for a container part (TABLE or BOX). This function create new
 * subitems for part items.
 *
 * @param obj A widget list object.
 * @param part A Part object with items, that need reload
 * @param addition Eina_True if item was added, EINA_FALSE - was deleted
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_part_items_refresh(Evas_Object *obj, Part *part, Eina_Bool additon);

#endif /* UI_WIDGET_LIST_H */
