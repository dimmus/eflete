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
ui_groupspace_part_state_update(Part *part, Part_State *state,
                                Evas_Object *groupspace);

/**
 * Separate view parts on groupspace.
 *
 * @param groupspace A Evas_Object pointer to groupspace layout.
 * @param separate If it's param EINA_TRUE parts in groupspace will be
 * show in separated view, else parts displayed normaly.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_separate(Evas_Object *groupspace, Eina_Bool separate);


#endif /* UI_GROUPSPACE_HEADER_H */
