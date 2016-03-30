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

#ifndef GROUPVIEW_H
#define GROUPVIEW_H
/**
 * @defgroup Groupview Groupview
 * @ingroup Workspace
 *
 * The Groupview it a graphical view of editable object. Groupview has a
 * container and two handlers. Handlers match of rel1 and rel2 point of Edje
 * object, it means top-left corner (rel1) and bottom-right corner (rel2).
 * Handler's moving will cause groupview size change'. Moving the handler
 * groupview size will be changed.
 *
 * The Groupview has its style. The style comprises of three edje group:
 * @li "eflete/groupview/base/default";
 * @li "eflete/groupview/handler_TL/default";
 * @li "eflete/groupview/handler_BR/default".
 *
 * Primitives of the editable object are drawn in the groupview container. Each
 * primitive has the border, this border show the Edje part geometry, it means
 * that Edje uses this geometry for the object calculation. Real size and
 * position of primitive will be differ from calculating part geometry.
 * Object area is calculating together with editable object primitives. Object
 * area is a region which set in the parameters of rel1 and rel2 blocks
 * in the edc-file. It's' parameters are 'relative' and 'to' (to_x, to_y).
 *
 */

#include "eflete.h"
#include "highlight.h"

struct _Geom{
   int x, y, w, h;
};

/**
 * emited when part is selected in groupview.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Groupview
 */
#define SIGNAL_GROUPVIEW_CLICKED "SIGNAL_GROUPVIEW_CLICKED"
#define SIGNAL_GROUPVIEW_HL_PART_DRAG_START "SIGNAL_GROUPVIEW_HL_PART_DRAG_START"
#define SIGNAL_GROUPVIEW_HL_PART_CHANGED "SIGNAL_GROUPVIEW_HL_PART_CHANGED"
#define SIGNAL_GROUPVIEW_HL_PART_DRAG_STOP "SIGNAL_GROUPVIEW_HL_PART_DRAG_STOP"

/**
 * Geometry of groupview part primitive.
 *
 * @ingroup Groupview
 */
typedef struct _Geom Groupview_Geom;

struct _Groupview_HL_Event {
   Part *part;
   Evas_Coord x;
   Evas_Coord y;
   Evas_Coord w;
   Evas_Coord h;
   Handler_Type hl_type;
};

typedef struct _Groupview_HL_Event Groupview_HL_Event;

/**
 * Add new groupview object to parent object.
 * To delete groupview object use evas_object_del.
 *
 * @param parent The parent object.
 * @param group Group to be shown.
 *
 * @return The groupview object.
 *
 * @ingroup Groupview
 */
Evas_Object *
groupview_add(Evas_Object *parent, Group *group);

/******************************************************************************/
/******************************************************************************/

/**
 * Recalculate all parts geometry for groupview.
 *
 * @param obj The groupview object.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_recalc_all(Evas_Object *obj);

/**
 * Update the parts attributes, without recalculation.
 *
 * @paramaram obj The groupview object.
 *
 * @ingroup Groupview
 */
void
groupview_soft_update(Evas_Object *obj);

/**
 * Update the parts attributes, and recalculate them.
 *
 * @paramaram obj The groupview object.
 *
 * @ingroup Groupview
 */
void
groupview_hard_update(Evas_Object *obj);

/**
 * Get the selected part geomentry.
 *
 * @param obj The groupview object,
 *
 * @return The Groupview_Geom, or NULL if any part is notselected
 *
 * @ingroup Groupview
 */
const Groupview_Geom *
groupview_part_selected_geom_get(Evas_Object *obj);

/**
 * Add to groupview a new part. The function add part to edje edit object and
 * add groupview part.
 *
 * @param obj The groupview object,
 * @param part Pointer to Part sturcture,
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_part_add(Evas_Object *obj, Part *part);

/**
 * Delete the part from groupview and from editable object.
 *
 * @param obj The groupview object,
 * @param part Pointer to part which will be deleted.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_part_del(Evas_Object *obj, Part *part);

/**
 * Get the object area geometry.
 *
 * @param obj The groupview object.
 *
 * @return The groupview part object, with object area geometry.
 *
 * @ingroup Groupview
 */
const Groupview_Geom *
groupview_part_selected_object_area_geom_get(Evas_Object *obj);
/**
 * Set the object area visibility.
 *
 * @param obj The groupview object,
 * @param visible The object area visibility.
 *
 * @ingroup Groupview
 */
void
groupview_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible);

/**
 * Get the object area visibility, if object area is not set, EINA_FALSE will be
 * returned.
 *
 * @param obj The groupview object.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_part_object_area_visible_get(Evas_Object *obj);

/**
 * Set the groupview's separate mode.
 *
 * @param obj The groupview object,
 * @param separated ON/OFF (true/false) the separate mode,
 *
 * @return EINA_TRUE when success, EINA_FALSE otherwise.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_parts_separated(Evas_Object *obj, Eina_Bool separated);

/**
 * Check if separate mode is activated.
 *
 * @param obj The groupview object.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_parts_separated_is(Evas_Object *obj);

/**
 * Select the part in groupview.
 *
 * @param obj The groupview object,
 * @param part The name of the part which has to be selected.
 *
 * @ingroup Groupview
 */
void
groupview_part_select(Evas_Object *obj, const char *part);

/**
 * Set a visibility for groupview part.
 *
 * @param obj The groupview object
 * @param part The Part object to be showed of hided
 * @param visible Use EINA_TRUE to show, EINA_FALSE to hide part.
 *
 * @ingroup Groupview
 */
void
groupview_part_visible_set(Evas_Object *obj, Part *part);

/**
 * Restack part below another part in groupview module.
 *
 * @param obj The groupview object.
 * @param part The name of part, which need above above.
 * @param below Name of part for which will be moved 'part'.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupview
 */
Eina_Bool
groupview_part_restack(Evas_Object *obj, const char *part, const char *below);

/**
 * Set zoom factor for view zoommed style in groupview object.
 *
 * @param obj The groupview object.
 * @param factor A factor for scale. Where value 1.0 = 100% scale (not zoom
 * object). Minimum is 0.01, maximum is 20. (from 1% to 2000% zoom).
 *
 * @ingroup Groupview
 */
void
groupview_zoom_factor_set(Evas_Object *obj, double factor);

/**
 * Get the zoom factor.
 *
 * @param obj The groupview object.
 *
 * @return the zoom factor value
 *
 * @ingroup Groupview
 */
double
groupview_zoom_factor_get(Evas_Object *obj);

/**
 * Change selection state for given item.
 *
 * @param obj The groupview object.
 * @param item_name The name of item, which selection should be changed.
 * @param selected If EINA_TRUE - item will be selected, otherwise unselected.
 *
 * @ingroup Groupview
 */
void
groupview_part_item_selected_set(Evas_Object *obj, Eina_Stringshare *item_name, Eina_Bool selected);

/**
 * Protrusion values it's distinction of object geomentry, what calculated by
 * smart object and real drawed geometry. The real drawed object geomentry
 * needed for correct zoom.
 *
 * @param obj The groupview object.
 *
 * @return The protrusion values.
 *
 * @ingroup Groupview
 */
Groupview_Geom *
groupview_protrusion_get(Evas_Object *obj);

#endif /* GROUPVIEW_H */
