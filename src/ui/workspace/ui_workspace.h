/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
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
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UI_WORKSPACE_HEADER_H
#define UI_WORKSPACE_HEADER_H

/**
 * @defgroup Workspace Workspace
 * @ingroup EFL_TET
 *
 * A Workspace module create and control workspace area. It's contain buttons
 * for zoom operation with workspace, button to split part of showed group.
 * In workspace providet highlight element for show current part.
 * Also Workspace contain two rulers vertical and horizontal with functional
 * like most graphics editors. A popup menu can show by click right button on
 * workspace.
 */

#include <Evas.h>
#include <Elementary.h>
#include "widget_manager.h"
#include "ui_ruler.h"

/**
 * @typedef  Workspace
 *
 * Contain helper objects for manipulate workspace.
 * A canvas object get handle to an Evas canvas.
 */
struct _Workspace
{
    Evas *canvas;
    Evas_Object *bg;
    double zoom_step;
    Evas_Object *ruler_hor;
    Evas_Object *ruler_ver;
    Evas_Object *button_zoom_out;
    Evas_Object *button_zoom_in;
    Evas_Object *button_separate;
    Evas_Object *scroller;
    Evas_Object *groupspace;
    struct {
        Evas_Object *legend;
        Eina_Bool visible;
    } legend;
    Eina_Bool separated;
    struct {
        Evas_Object *highlight;
        Part *part; /* highlighting object */
        Evas_Object *space_hl;
    } highlight;
};
typedef struct _Workspace Workspace;

/**
 * Set zoom step for workspace.
 *
 * @param step Step size.
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ws_zoom_step_set (Workspace *ws, double step);

/**
 * Get zoom step for workspace.
 *
 * @param ws Workspace structure.
 * @return integer value of step size.
 *
 * @ingroup Workspace
 */
double
ws_zoom_step_get (Workspace *ws);

/**
 * Add new workspace object to the given parent Elemntary (container) object
 *
 * @param parent The parent object.
 * @return pointer to an Workspace structure in wich store workspace elements.
 *
 * @ingroup Workspace
 */
Workspace *
ws_add (Evas_Object *parent);

/**
 *
 */
void
ws_free(Workspace *ws);

/**
 * Set highlight to selected part.
 *
 * @param ws Workspace structure.
 * @param part Evas_Object pointer to selected part object.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_set(Workspace *ws, Part *part);

/**
 * Move highlight object on workspace.
 *
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_move(Workspace *ws);

/**
 * Hide highlight object.
 *
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_hide(Workspace *ws);

/**
 * Delete highlight object's'.
 *
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_del(Workspace *ws);

/**
 * Zoom in loaded objects on workspace. With step 0.1.
 *
 * @param ws Workspace structure.
 * @return EINA_TRUE if zoom finished normal, or EINA_FALSE if zoom coefficient
 * more than 10 (1000%).
 *
 * @ingroup Workspace
 */
Eina_Bool
ui_ws_zoom_in(Workspace *ws);

/**
 * Zoom out loaded objects on workspace. With step 0.1.
 *
 * @param ws Workspace structure.
 * @return EINA_TRUE if zoom finished normal, or EINA_FALSE if zoom coefficient
 * less than 0.1 (10%).
 *
 * @ingroup Workspace
 */
Eina_Bool
ui_ws_zoom_out(Workspace *ws);

/**
 * Show or hide legend element on workspace.
 *
 * @param ws Workspace structure.
 * @param visible EINA_TRUE if need show legend, or EINA_FALSE need to hide
 * legend.
 *
 * @ingroup Workspace
 */
void
ui_ws_legend_visible_set(Workspace *ws, Eina_Bool visible);
#endif /* UI_WORKSPACE_HEADER_H */
