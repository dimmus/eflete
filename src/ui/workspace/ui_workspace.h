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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#ifndef UI_WORKSPACE_HEADER_H
#define UI_WORKSPACE_HEADER_H

/**
 * @defgroup Workspace Workspace
 * @ingroup EFL_TET
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
#include "widget_manager.h"
#include "ui_ruler.h"

/**
 * Create a new Workspace object to the parent.
 *
 * @param parent The parent object.
 *
 * @return The new object or NULL if it cannot be created.
 *
 * @ingroup Workspace
 */
Evas_Object *
workspace_add(Evas_Object *parent);

/**
 * Set background image for workspace. If image file corrupt or have invalid
 * image format or extenstion, then will be set default background image
 * (transparency style). Background image will be tiled automaticaly.
 *
 * @param obj The workspace object.
 * @param path The path to the image file load from.
 *
 * @return EINA_TRUE, on success or EINA_FALSE, on errors.
 *
 * @ingroup Workspace
 */
Eina_Bool
ws_background_image_set(Evas_Object *obj, const char *path);

/**
 * Get the groupspace object, which loaded into workspace.
 *
 * @param obj The workspace object.
 *
 * @return The groupspace object or NULL if it didn't exist'.
 *
 * @ingroup Workspace
 */
Evas_Object *
ws_groupspace_get(Evas_Object *obj);

/**
 * Set the editable object to the workspace.
 *
 * @param obj The workspace object.
 * @param group The group was be editing.
 * @param file Path to the file from been loaded the edje edit object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_set(Evas_Object *obj, Group *group, const char *file);

/**
 *
 */
void
workspace_edit_object_unset(Evas_Object *obj);

/**
 *
 */
Group *
workspace_edit_object_get(Evas_Object *obj);

/**
 *
 */
void
workspace_edit_object_recalc(Evas_Object *obj);

/**
 *
 */
Eina_Bool
workspace_edit_object_part_add(Evas_Object *obj, const char *part,
                               Edje_Part_Type type, const char *data);
/**
 *
 */
Eina_Bool
workspace_edit_object_part_del(Evas_Object *obj, const char *part);

/**
 * Stack part above above in workspace module. This function is a wrapper
 * on _edit_object_part_restack_above();
 *
 * @param obj The workspace object.
 * @param part The name of part, which need above above.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_part_above(Evas_Object *obj, const char *part);

/**
 * Stack part below below in workspace module. This function is a wrapper
 * on _edit_object_part_restack_below();
 *
 * @param obj The workspace object.
 * @param part The name of part, which need below below.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_part_below(Evas_Object *obj, const char *part);

/**
 *
 */
Eina_Bool
workspace_edit_object_part_state_set(Evas_Object *obj, Part *part);

Eina_Bool
workspace_edit_object_part_state_add(Evas_Object *obj, const char *part,
                                     const char *state, double value);

Eina_Bool
workspace_edit_object_part_state_del(Evas_Object *obj, const char *part,
                                     const char *state, double value);
/**
 * Set a visibility for part of current groupedit object in the workspace.
 *
 * @param obj The workspace object
 * @param part The name of groupedit part to be showed of hided
 * @param visible Use EINA_TRUE to show, EINA_FALSE to hide part.
 *
 * @return EINA_TRUE, if it's showed/hided, EINA_FALSE otherwise
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_visible_set(Evas_Object *obj,
                                  const char *part,
                                  Eina_Bool visible);

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
ws_zoom_factor_set(Evas_Object *obj, double factor);

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
ws_zoom_factor_get(Evas_Object *obj);

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
ws_legend_visible_set(Evas_Object *obj);

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


/*-------------DEPRECATED functions-----------------------*/

/**
 * Move highlight object on workspace.
 *
 * This function will resize the whole smart object that is highlight.
 * That means that all handlers will be both resized and moved.
 * (There will be calculations of size and position of every handler)
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with new functional of smart highlight object.
 */
Eina_Bool
ws_object_highlight_move(Evas_Object *obj);

/**
 * Move highlight object on workspace.
 *
 * This function will resize the highlight particulary.
 * That means that all handlers will be moved only.
 * (There will be calculations of position of every handler)
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with new functional of smart highlight object.
 */
Eina_Bool
ws_object_highlight_handler_move(Evas_Object *obj);

/**
 * Hide highlight object.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with new functional of smart highlight object.
 */
Eina_Bool
ws_object_highlight_hide(Evas_Object *obj);

/**
 * Delete highlight object's.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with new functional of smart highlight object.
 */
Eina_Bool
ws_object_highlight_del(Evas_Object *obj);

/**
 * Delete highlight object's.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with new functional of smart highlight object.
 */
Evas_Object *
ws_space_hl_get(Evas_Object *obj);

/**
 * Delete highlight object's.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with new functional of smart highlight object.
 */
Evas_Object *
ws_highlight_get(Evas_Object *obj);


/**
 * Get current separate state of workspace object.
 *
 * @param obj The workspace object.
 *
 * @return EINA_FALSE if workspace in normal mode or EINA_TRUE if it separated.
 *
 * @ingroup Workspace
 * @deprecated replace with functional of smart groupspace
 */
Eina_Bool
ws_separated_mode_get(Evas_Object *obj);

/**
 * Set separate mode for workspace object.
 *
 * @param obj The workspace object.
 * @param separated EINA_TRUE if workspace set to separate view, EINA_FALSE otherwise.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 * @deprecated replace with functional of smart groupspace
 */
Eina_Bool
ws_separated_mode_set(Evas_Object *obj, Eina_Bool separated);

/*-------------End of DEPRECATED functions------------------*/


#endif /* UI_WORKSPACE_HEADER_H */
