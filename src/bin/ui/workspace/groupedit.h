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

#ifndef GROUPEDIT_H
#define GROUPEDIT_H
/**
 * @defgroup Groupedit Groupedit
 * @ingroup Workspace
 *
 * The Groupedit it a graphical view of editable object. Groupedit has a
 * container and two handlers. Handlers match of rel1 and rel2 point of Edje
 * object, it means top-left corner (rel1) and bottom-right corner (rel2).
 * Handler's moving will cause groupedit size change'. Moving the handler
 * groupedit size will be changed.
 *
 * The Groupedit has its style. The style comprises of three edje group:
 * @li "eflete/groupedit/base/default";
 * @li "eflete/groupedit/handler_TL/default";
 * @li "eflete/groupedit/handler_BR/default".
 *
 * Primitives of the editable object are drawn in the groupedit container. Each
 * primitive has the border, this border show the Edje part geometry, it means
 * that Edje uses this geometry for the object calculation. Real size and
 * position of primitive will be differ from calculating part geometry.
 * Object area is calculating together with editable object primitives. Object
 * area is a region which set in the parameters of rel1 and rel2 blocks
 * in the edc-file. It's' parameters are 'relative' and 'to' (to_x, to_y).
 *
 */

#include "group_manager.h"

struct _Geom{
   int x, y, w, h;
};

/**
 * emited when part is selected in groupedit.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Groupedit
 */
#define SIGNAL_GROUPEDIT_PART_SELECTED "SIGNAL_GROUPEDIT_PART_SELECTED"
/**
 * emited when part is unselected in groupedit.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Groupedit
 */
#define SIGNAL_GROUPEDIT_PART_UNSELECTED "SIGNAL_GROUPEDIT_PART_UNSELECTED"

/**
 * Geometry of groupedit part primitive.
 *
 * @ingroup Groupedit
 */
typedef struct _Geom Groupedit_Geom;

/**
 * Add new groupedit object to parent object.
 * To delete groupedit object use evas_object_del.
 *
 * @param parent The parent object.
 * @param group Group to be shown.
 *
 * @return The groupedit object.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_add(Evas_Object *parent, Group *group);

TODO("aspect for groupedit")
//Eina_Bool
//ws_groupedit_aspect_set(Evas_Object *obj, double x, double y);

/******************************************************************************/
/******************************************************************************/

/**
 * Recalculate all parts geometry for groupedit.
 *
 * @param obj The groupedit object.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_recalc_all(Evas_Object *obj);

/**
 * Update the parts attributes, without recalculation.
 *
 * @paramaram obj The groupedit object.
 *
 * @ingroup Groupedit
 */
void
groupedit_soft_update(Evas_Object *obj);

/**
 * Update the parts attributes, and recalculate them.
 *
 * @paramaram obj The groupedit object.
 *
 * @ingroup Groupedit
 */
void
groupedit_hard_update(Evas_Object *obj);

/**
 * Get the part object, which geometry corresponds to Edje part geometry from
 * editable object.
 *
 * @param obj The groupedit object,
 * @param part The part name, whose object need to get.
 *
 * @return The groupedit part object, with geometry like Edje part.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_edit_object_part_draw_get(Evas_Object *obj, const char *part);

/**
 * Add to groupedit a new part. The function add part to edje edit object and
 * add groupedit part.
 *
 * @param obj The groupedit object,
 * @param part Pointer to Part_ sturcture,
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_add(Evas_Object *obj, Part_ *part);

/**
 * Delete the part from groupedit and from editable object.
 *
 * @param obj The groupedit object,
 * @param part Pointer to part which will be deleted.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_del(Evas_Object *obj, Part_ *part);

/**
 * Set the current state value to the part.
 *
 * @param obj The groupedit object,
 * @param part Pointer to Part_ structure
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_state_set(Evas_Object *obj, Part_ *part);

/**
 * Get the object with the object area geometry.
 *
 * @param obj The groupedit object.
 *
 * @return The groupedit part object, with object area geometry.
 *
 * @ingroup Groupedit
 */
Evas_Object *
groupedit_part_object_area_get(Evas_Object *obj);

/**
 * Set the object area visibility.
 *
 * @param obj The groupedit object,
 * @param visible The object area visibility.
 *
 * @ingroup Groupedit
 */
void
groupedit_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Get the object area visibility, if object area is not set, EINA_FALSE will be
 * returned.
 *
 * @param obj The groupedit object.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_part_object_area_visible_get(Evas_Object *obj);

/**
 * Set the groupedit's separate mode.
 *
 * @param obj The groupedit object,
 * @param separated ON/OFF (true/false) the separate mode,
 *
 * @return EINA_TRUE when success, EINA_FALSE otherwise.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_parts_separated(Evas_Object *obj,
                                      Eina_Bool separated);

/**
 * Check if separate mode is activated.
 *
 * @param obj The groupedit object.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_parts_separated_is(Evas_Object *obj);

/**
 * Select the part in groupedit.
 *
 * @param obj The groupedit object,
 * @param part The name of the part which has to be selected.
 *
 * @ingroup Groupedit
 */
void
groupedit_edit_object_part_select(Evas_Object *obj, const char *part);

/**
 * Set a visibility for groupedit part.
 *
 * @param obj The groupedit object
 * @param part The Part object to be showed of hided
 * @param visible Use EINA_TRUE to show, EINA_FALSE to hide part.
 *
 * @ingroup Groupedit
 */
void
groupedit_part_visible_set(Evas_Object *obj, Part_ *part);

/**
 * Restack part below another part in groupedit module.
 *
 * @param obj The groupedit object.
 * @param part The name of part, which need above above.
 * @param below Name of part for which will be moved 'part'.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_edit_object_part_restack(Evas_Object *obj,
                                   const char *part,
                                   const char *below);

/**
 * Set zoom factor for view zoommed style in groupedit object.
 *
 * @param obj The groupedit object.
 * @param factor A factor for scale. Where value 1.0 = 100% scale (not zoom
 * object). Minimum is 0.01, maximum is 20. (from 1% to 2000% zoom).
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
groupedit_zoom_factor_set(Evas_Object *obj, double factor);


/**
 * Change selection state for given item.
 *
 * @param obj The groupedit object.
 * @param item_name The name of item, which selection should be changed.
 * @param selected If EINA_TRUE - item will be selected, otherwise unselected.
 *
 * @ingroup Groupedit
 */
void
groupedit_edit_object_part_item_selected_set(Evas_Object *obj,
                                             Eina_Stringshare *item_name,
                                             Eina_Bool selected);

/**
 * Protrusion values it's distinction of object geomentry, what calculated by
 * smart object and real drawed geometry. The real drawed object geomentry
 * needed for correct zoom.
 *
 * @param obj The groupedit object.
 *
 * @return The protrusion values.
 *
 * @ingroup Groupedit
 */
const Groupedit_Geom *
groupedit_protrusion_get(Evas_Object *obj);

#endif /* GROUPEDIT_H */
