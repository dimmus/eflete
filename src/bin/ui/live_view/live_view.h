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

#ifndef UI_LIVEVIEW_HEADER_H
#define UI_LIVEVIEW_HEADER_H

/**
 * @defgroup Live_View Live_View
 *
 * A Live_View is responsible for showing widgets.
 * After user manipulations with parts, states or programs and saving
 * into a file, this space will show the resulted widget
 * by using user's theme (already saved one).
 */

#include "widget_manager.h"
#include "project_manager.h"
#include "common_macro.h"
#include "live_widget.h"
#include <Evas.h>
#include <Elementary.h>

/**
 * @typedef Live_View
 *
 * Contain objects to show demo of widget (live view).
 */
struct _Live_View
{
   Evas_Object *live_view; /**< live_view object that contain widget for showing,
                             and spinner for scaling */
   Evas_Object *layout; /**< the layout of the whole live view widget */
   Evas_Object *panel; /**< panel for Live View properties */
   Evas_Object *object; /**< current widget for being showed in live view */
   Evas_Object *property; /**< Live View properties */
   Evas_Object *parent; /**< Live View block */
   Eina_Bool in_prog_edit; /**< Flag is this liveview in program editor */
};
typedef struct _Live_View Live_View;

/**
 * Add new live view object.
 *
 * This function will create "live view" object for showing widget.
 * It uses layout that contain spinner for scaling widget,
 * and also it contains groupspace (for showing widget example).
 *
 * @param parent The parent Evas_Object.
 * @param in_prog_edit Is this live_view for program_editor.
 * @return Pointer to live_view structure.
 *
 * @ingroup Live_View
 */
Live_View *
live_view_add(Evas_Object *parent, Eina_Bool in_prog_edit);

/**
 * Set and show given widget and apply the style to it for showing the view of
 * of this widget.
 *
 * This function will create widget/widgets for showing loaded group,
 * and create new theme for applying changed style to widget.
 * This function also will hide the scaling spinner.
 *
 * @param live live_view structure.
 * @param project project structure that contain path to swap file.
 * @param style style that contain current style name is being showed.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_widget_style_set(Live_View *live, Project *project, Style *style);

/**
 * Unset widget from live view and hide it aswell.
 *
 * This function will delete and unset all previously created widgets from live
 view and then hide live view and scaling spinner.
 *
 * @param live live_view structure.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_widget_style_unset(Live_View *live);

/**
 * Update changed and loaded project theme.
 *
 * This function will update the widget inside of the live view.
 * If previously loaded theme was changed it will update the widget aswell.
 *
 * @param live live_view structure.
 * @param project project structure that contain path to swap file.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_theme_update(Live_View *live, Project *project);

/**
 * Freeing the live_view structure.
 *
 * This function will free and delete the structure that contain all
 * information about live view.
 *
 * @param live live_view structure.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_free(Live_View *live);

/**
 * Add new part to live view properties list of parts.
 *
 * This function works only if part is SWALLOW, TEXTBLOCK or TEXT. All other
 * types of parts are being ignored.
 *
 * @param live live_view structure.
 * @param part new added part
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_part_add(Live_View *live, Part *part);

/**
 * Delete new part to live view properties list of parts.
 *
 * This function works only if part is SWALLOW, TEXTBLOCK or TEXT. All other
 * types of parts are being ignored.
 *
 * @param live live_view structure.
 * @param part part to be deleted
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_part_del(Live_View *live, Part *part);

/**
 * Rename part from list of parts in live view property.
 *
 * This function works only if part is SWALLOW, TEXTBLOCK or TEXT. All other
 * types of parts are being ignored.
 *
 * @param live live_view structure.
 * @param part part to be renamed
 * @param new_name new name of that part
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_part_rename(Live_View *live, Part *part, Eina_Stringshare *new_name);

/**
 * Restack part in list of parts in live view property above given part.
 *
 * This function works only if part is SWALLOW, TEXTBLOCK or TEXT. All other
 * types of parts are being ignored. It restack above nearest part
 * (related to given one) with same part type.
 *
 * @param live live_view structure.
 * @param part_move part to be restacked
 * @param part_above related part. part_move should be above this given part
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_part_restack_above(Live_View *live, Part *part_move, Part *part_above);

/**
 * Restack part in list of parts in live view property below given part.
 *
 * This function works only if part is SWALLOW, TEXTBLOCK or TEXT. All other
 * types of parts are being ignored. It restack above nearest part
 * (related to given one) with same part type.
 *
 * @param live live_view structure.
 * @param part_move part to be restacked
 * @param part_below related part. part_move should be below this given part
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_part_restack_below(Live_View *live, Part *part_move);

#endif /* UI_LIVEVIEW_HEADER_H */
