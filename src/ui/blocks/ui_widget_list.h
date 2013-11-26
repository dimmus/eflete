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

#ifndef UI_WIDGET_LIST_H
#define UI_WIDGET_LIST_H

/**
 * @defgroup WidgetList WidgetList
 * @ingroup Block
 *
 * This widget inherit from the Naviframe and Genlist.
 *
 * This widget emits the following signals:
 * @li "wl,group,select": the user double clicked on the group name
 * @li "wl,group,back": the user clicked on the "back" button
 * @li "wl,part,select": the user selected on the part name
 * @li "wl,part,back": the user clicked on the "back" button
 *
 * A Widget List used for view a list of widgets styles
 */

#include "ui_main_window.h"
#include "ui_property.h"
#include "ui_states_list.h"
#include "ui_groupspace.h"

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
 * @ingroup WidgetList
 */
void
ui_widget_list_title_set(Evas_Object *object, const char *title);

/**
 * Set to a 'widget list' object a data of widgets (@see widget manager).
 *
 * @param object A 'widget list' object
 * @param project A opened project
 * @return EINA_TRUE - it all ok, EINA_FALSE - samsing wrong
 *
 * @ingroup WidgetList
 */
Eina_Bool
ui_widget_list_data_set(Evas_Object *object, Project *project);

/**
 * Reolad content style genlist.
 *
 * @param gl_styles A 'style list' object. (genlist container)
 * @param styles A Eina_Inlist pointer for loaded styles in project
 *
 * @ingroup WidgetList
 */
void
ui_widget_list_style_data_reload(Evas_Object *gl_styles, Eina_Inlist *styles);

#endif /* UI_WIDGET_LIST_H */
