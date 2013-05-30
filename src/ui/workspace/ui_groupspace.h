#ifndef UI_GROUPSPACE_HEADER_H
#define UI_GROUPSPACE_HEADER_H

#include "efl_tet.h"
#include <Evas.h>
#include <Elementary.h>
#include <Eina.h>
#include "widget_manager.h"
/**
 * Add new groupspace object into workspace.
 * in: ap pointer to App_Data structure.
 * in: group Group structure wich contain group. This group will be painted.
 */
void
ui_groupspace_add (App_Data *ap, Group *group);

/**
 * Delete groupspace object.
 * in: obj Groupspace object.
 */
void
ui_groupspace_del (Evas_Object *obj);
#endif /* UI_GROUPSPACE_HEADER_H */
