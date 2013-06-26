#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

#include "efl_tet.h"
#include "widget_manager.h"
#include <Elementary.h>
#include "widget_manager.h"

/**
 *
 */
Evas_Object *
ui_property_view_new(Evas_Object *parent);

/**
 *
 */
void
ui_property_group_view_set(Evas_Object *prop_view, Evas_Object *group_view);

/**
 *
 */
void
ui_property_part_view_set(Evas_Object *prop_view, Evas_Object *part_view);

/**
 *
 */
Evas_Object *
ui_property_group_view_get(Evas_Object *prop_view);

/**
 *
 */
Evas_Object *
ui_property_part_view_get(Evas_Object *prop_view);

/**
 *
 */
Evas_Object *
ui_prop_group_info_view_add(Evas_Object *prop_view, Group *group);

/**
 *
 */
void
ui_prop_group_info_view_update(Evas_Object *prop_view, Group *group);

/**
 *
 */
Evas_Object *
ui_prop_part_info_view_add(Evas_Object *prop_view, Part *part);

/**
 *
 */
Evas_Object *
ui_prop_part_info_state_view_add(Evas_Object *part_view, Part_State *state);

/**
 *
 */
void
ui_prop_part_info_state_set(Evas_Object *part_view, Evas_Object *state_view);

/**
 *
 */
Evas_Object *
ui_prop_part_info_state_view_get(Evas_Object *part_view);

#endif /* UI_PROPERTY_H */
