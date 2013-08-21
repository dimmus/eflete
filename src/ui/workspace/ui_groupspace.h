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
   FIXME add comment!
  */
#define _ELLIP_STR "\xE2\x80\xA6"

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

#endif /* UI_GROUPSPACE_HEADER_H */
