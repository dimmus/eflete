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

#ifndef UI_WORKSPACE_H
#define UI_WORKSPACE_H

/**
 * @defgroup Workspace Workspace
 * @ingroup EFLETE
 *
 * A Workspace module create and control workspace area. It's contain control
 * for zoom operation with workspace, button to switch separate mode view.
 * Workspace provide highlight element for show and edit current part.
 * Also Workspace contain two rulers vertical and horizontal with functional
 * like most graphics editors. A popup menu can show by mouse click right
 * button on workspace object.
 */

#include <Evas.h>
#include <Elementary.h>
#include <Ewe.h>
#include "group_manager.h"

/**
 * Create a new Workspace object to the parent.
 *
 * @param parent The parent object.
 * @param group The group to be editing.
 *
 * @return The new object or NULL if it cannot be created.
 *
 * @ingroup Workspace
 */
Evas_Object *
workspace_add(Evas_Object *parent, Group *group);

/**
 * Update part info in part list
 *
 * @param obj The workspace object.
 * @param part Part to be updated.
 *
 * @ingroup Workspace
 */
void
workspace_group_navigator_update_part(Evas_Object *obj, Part_ *part);

/**
 * Force recal for editable object from the workspace.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_recalc(Evas_Object *obj);

/**
 * Update the parts attributes, without recalculation.
 *
 * @paramaram obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_groupview_soft_update(Evas_Object *obj);

/**
 * Add new part to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of added part
 *
 * @ingroup Workspace
 */
void
workspace_part_add(Evas_Object *obj, Eina_Stringshare *part_name);

/**
 * Add new part item to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param item_name name of added part_item
 *
 * @ingroup Workspace
 */
void
workspace_part_item_add(Evas_Object *obj,
                        Eina_Stringshare *part_name,
                        Eina_Stringshare *item_name);

/**
 * Add new part state to group_edit and group_navigator.
 * Part should be already added to edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param state_name name of added part_state
 *
 * @ingroup Workspace
 */
void
workspace_part_state_add(Evas_Object *obj,
                         Eina_Stringshare *part_name,
                         Eina_Stringshare *state_name);

/**
 * Add new part item to group_edit and group_navigator.
 * Part should be already deleted from edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param item_name name of deleted part_item
 *
 * @ingroup Workspace
 */
void
workspace_part_item_del(Evas_Object *obj,
                        Eina_Stringshare *part_name,
                        Eina_Stringshare *item_name);

/**
 * Add new part state to group_edit and group_navigator.
 * Part should be already deleted from edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of part
 * @param state_name name of deleted part_state
 *
 * @ingroup Workspace
 */
void
workspace_part_state_del(Evas_Object *obj,
                         Eina_Stringshare *part_name,
                         Eina_Stringshare *state_name);

/**
 * Del part from group_edit and group_navigator.
 * Part should be already deleted edit_object.
 *
 * @param obj The workspace object.
 * @param part_name name of deleted part
 *
 * @ingroup Workspace
 */
void
workspace_part_del(Evas_Object *obj, Eina_Stringshare *part_name);

/**
 *
 */
void
workspace_part_restack(Evas_Object *obj,
                       Eina_Stringshare *part_name,
                       Eina_Stringshare *relative_part_name);

/**
 *
 */
void
workspace_part_item_restack(Evas_Object *obj,
                            Eina_Stringshare *part_name,
                            Eina_Stringshare *part_item_name,
                            Eina_Stringshare *relative_part_item_name);

/**
 * Set zoom factor for view zoommed style in groupspace.
 *
 * @param obj The Workspace object.
 * @param factor A factor for scale. Where value 1.0 = 100% scale (not zoom
 * object). Minimum is 0.01, maximum is 20. (from 1% to 2000% zoom).
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_zoom_factor_set(Evas_Object *obj, double factor);

/**
 * Get zoom factor from workspace.
 *
 * @param The workspace object.
 *
 * @return double value of zoom factor, or '0' if fail.
 *
 * @ingroup Workspace
 */
double
workspace_zoom_factor_get(Evas_Object *obj);

/**
 * Switch beetwen show or hide mode of legend in workspace object.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_legend_visible_set(Evas_Object *obj);

/**
 * Set highlight to selected part.
 *
 * @param obj The workspace object.
 * @param part Evas_Object pointer to selected part object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_set(Evas_Object *obj, Part *part);

/**
 * Unset highlight from previously selected part.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_unset(Evas_Object *obj);

/**
 * Turn on/off the separatr mode
 *
 * @param obj The workspace object,
 * @param separated ON/OFF (true/false) the separate mode,
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_separate_mode_set(Evas_Object *obj, Eina_Bool separate);

/**
 * Check if separate mode is activated.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_separate_mode_get(Evas_Object *obj);

/**
 * Turn on/off the middle border of highlight (align) visibility.
 *
 * @param obj The workspace object,
 * @param separated ON/OFF (true/false) the middle border (align) visibility,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_align_visible_set(Evas_Object *obj, Eina_Bool flag);

/**
 * Returns if middle border of highlight (align) is visible
 *
 * @param obj The workspace object,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_highlight_align_visible_get(Evas_Object *obj);

/**
 * Turn on/off the object area visibility.
 *
 * @param obj The workspace object,
 * @param separated ON/OFF (true/false) the object area visibility,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_object_area_visible_set(Evas_Object *obj, Eina_Bool flag);

/**
 * Returns if the object area is visible
 *
 * @param obj The workspace object,
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_object_area_visible_get(Evas_Object *obj);

#endif /* UI_WORKSPACE_H */
