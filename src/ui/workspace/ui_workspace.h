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

#ifndef UI_WORKSPACEH
#define UI_WORKSPACEH

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
 * @param style The style to be editing.
 * @param file Path to the file from been loaded the edje edit object.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_set(Evas_Object *obj, Style *style, const char *file);

/**
 * Unset the editable object from the workspace.
 *
 * @param obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_edit_object_unset(Evas_Object *obj);

/**
 * Get the editable object from the workspace.
 *
 * @param obj The workspace object.
 *
 * @return A @Style object, which stored in workspace as editable, or NULL if
 * no one object is setted.
 *
 * @ingroup Workspace
 */
Style *
workspace_edit_object_get(Evas_Object *obj);

/**
 * Force recal for editable object from the workspace.
 *
 * @param obj The workspace object.
 *
 * @ingroup Workspace
 */
void
workspace_edit_object_recalc(Evas_Object *obj);

/**
 * Add new part into style, which currently loaded into workspace.
 *
 * @param obj The workspace object.
 * @param part The name for new part in style.
 * @param type One of @Edje_Part_Type enum types of edje parts.
 * @param data If new part type is @EDJE_PART_TYPE_IMAGE in param 'data' stored
 * image name, which will be set as normal for created part.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
 */
Eina_Bool
workspace_edit_object_part_add(Evas_Object *obj, const char *part,
                               Edje_Part_Type type, const char *data);
/**
 * Delete part from style, which currently loaded into workspace.
 *
 * @param obj The workspace object.
 * @param part The name for deleting part.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Workspace
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

#endif /* UI_WORKSPACE_H */
