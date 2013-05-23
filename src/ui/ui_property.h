#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

#include "efl_tet.h"
#include "pm.h"
#include <Elementary.h>

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
 * 1 0
 */
Evas_Object *
ui_prop_part_info_view_state_add(Evas_Object *prop, Part_State *state);

/**
 *
 */
Evas_Object *
ui_prop_view_item_label_add(Evas_Object *prop, const char *name, char *label);

/**
 *
 */
Evas_Object *
ui_prop_view_item_two_edit_int_add(Evas_Object *prop, const char *name,
									int *data1, const char *tooltip1,
									int *data2, const char *tooltip2);

/**
 * data is Stringshare
 */
Evas_Object *
ui_prop_view_item_one_edit_string_add(Evas_Object *prop, const char *name,
									const char *data, const char *tooltip);

/**
 *
 */
Evas_Object *
ui_prop_view_item_toggle_add(Evas_Object *prop, const char *name,
									Eina_Bool check, const char *tooltip);

#endif /* UI_PROPERTY_H */
