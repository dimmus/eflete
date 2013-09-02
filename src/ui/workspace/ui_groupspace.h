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

#ifndef UI_GROUPSPACE_HEADER_H
#define UI_GROUPSPACE_HEADER_H

/**
 * @defgroup Groupspace Groupspace
 * @ingroup Workspace
 *
 * A Grouspace submodule responsible for draw widgets. Groupspace control
 * sizes and positions all part of selected group. Groupspace "translate"
 * part from Edje to Evas layers. Such an approach provide more opportunities
 * for futher manipulations with part objects.
 */

#include "efl_tet.h"
#include <Evas.h>
#include <Elementary.h>
#include <Eina.h>
#include "widget_manager.h"
#include "add_part_dialog.h"


/**
 * Add new groupspace object into workspace.
 *
 * @param parent The parent Evas_Object .
 * @return Pointer to groupspace Evas_Object.
 *
 * @ingroup Groupspace
 */
Evas_Object *
ui_groupspace_add(Evas_Object *parent);

/**
 * Set group data to groupspace object.
 *
 * @param ws pointer to Workspace structure.
 * @param project pointer to Project structure.
 * @param group Group structure wich contain group. This group will be painted.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_set(Workspace *ws, Project *project, Group *group);

/**
 * Unset group data from groupspace object.
 *
 * @param obj Groupspace object.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_unset (Evas_Object *obj);

/**
 * Update view parts on groupspace. Need to draw/redraw states of parts.
 *
 * @param obj Groupspace object.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_update (Evas_Object *obj);

/**
 * Update view parts on groupspace with new state.
 *
 * @param part A Part object, wich selected in widgetlist.
 * @param state A Part_State object, wich contain selected state for part.
 * @param groupspace Groupspace object.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_part_state_update(Evas_Object *groupspace,
                                Part *part);
                                //const char *state);
                                //double value);

/**
 * Separate view parts on groupspace.
 *
 * @param ws A workspace pointer to struct.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_separate(Workspace *ws);

/**
 * Get current group, that loaded into groupspace.
 *
 * @param groupspace A Evas_Object pointer to groupspace layout.
 * @return Group struct pointer, if group currently loaded and NULL if no one
 * group loaded.
 *
 * @ingroup Groupspace
 */
Group *
ui_groupspace_group_get(Evas_Object *groupspace);

/**
 * Get box object into groupspace.
 *
 * @param groupspace A Evas_Object pointer to groupspace layout.
 * @return Evas_Object pointer, if box currently created, else NULL.
 *
 * @ingroup Groupspace
 */
Evas_Object *
ui_groupspace_box_get(Evas_Object *groupspace);

/**
 *	Add state for current selected part.
 *
 * @param groupspace A Evas_Object pointer to groupspace layout.
 * @param part A Part object, wich selected in widgetlist.
 * @param state_name Name of the new state.
 * @param state_value Value of the new state.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Groupspace
 */
Eina_Bool
ui_groupspace_part_state_add(Evas_Object *groupspace, Part *part,
                             char *state_name, double state_value);

/**
 *	Get part relative geometry. Needed for highlight relative space.
 *
 * @param group A Group struct pointer.
 * @param part A Part object, wich selected in widgetlist.
 * @param x Pointer to an integer in which to store the X coordinate of the space.
 * @param y Pointer to an integer in which to store the Y coordinate of the space.
 * @param w Pointer to an integer in which to store the width of the space.
 * @param h Pointer to an integer in which to store the height of the space.
 *
 * @ingroup Groupspace
 */
void
ui_groupsapce_part_space_geometry_get(Group *group, Part *part,
                               int *x, int *y, int *w, int *h);

#endif /* UI_GROUPSPACE_HEADER_H */
