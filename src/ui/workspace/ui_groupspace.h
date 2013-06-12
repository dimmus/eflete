#ifndef UI_GROUPSPACE_HEADER_H
#define UI_GROUPSPACE_HEADER_H

#include "efl_tet.h"
#include <Evas.h>
#include <Elementary.h>
#include <Eina.h>
#include "widget_manager.h"

#define _ELLIP_STR "\xE2\x80\xA6"

/**
 * Add new groupspace object into workspace.
 * in: ap pointer to App_Data structure.
 * in: group Group structure wich contain group. This group will be painted.
 */
void
ui_groupspace_add (App_Data *ap, Group *group);

/**
 * Delete groupspace object. Before call this method need clear all part objects.
 * @param obj Groupspace object.
 */
void
ui_groupspace_del (Evas_Object *obj);

/**
 * Clear groupspace object. Its method delete all Evas_Objects for painted part's
 * @param group Group object, need to know what part objects must deleted.
 */
void
ui_groupspace_clear(Group *group);

#endif /* UI_GROUPSPACE_HEADER_H */
