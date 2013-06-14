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

#define _ELLIP_STR "\xE2\x80\xA6"

/**
 * Add new groupspace object into workspace.
 *
 * @param project pointer to Project structure.
 * @param ws pointer to Workspace structure.
 * @param group Group structure wich contain group. This group will be painted.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_add(Project *project, Workspace *ws, Group *group);

/**
 * Delete groupspace object. Before call this method need clear all part objects.
 *
 * @param obj Groupspace object.
 *
 * @ingroup Groupsp
 ace
 */
void
ui_groupspace_del (Evas_Object *obj);

/**
 * Clear groupspace object. Its method delete all Evas_Objects for painted part's
 *
 * @param group Group object, need to know what part objects must deleted.
 *
 * @ingroup Groupspace
 */
void
ui_groupspace_clear(Group *group);

#endif /* UI_GROUPSPACE_HEADER_H */
