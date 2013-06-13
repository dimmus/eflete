#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

#include "efl_tet.h"
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

/**
 *
 */
Evas_Object *
ui_prop_view_item_label_add(Evas_Object *prop, const char *name, char *label);

/**
 *
 */
Evas_Object *
ui_prop_view_item_one_edit_int_add(Evas_Object *prop, const char *name,
							int *data, const char *tooltip);

/**
 *
 */
Evas_Object *
ui_prop_view_item_two_edit_int_add(Evas_Object *prop, const char *name,
							int *data1, const char *tooltip1,
							int *data2, const char *tooltip2);

/**
 *
 */
Evas_Object *
ui_prop_view_item_four_edit_int_add(Evas_Object *prop, const char *name,
							int *data1, int *data2, int *data3, int *data4,
							const char *tooltip);

/**
 * data is Stringshare
 */
Evas_Object *
ui_prop_view_item_one_edit_string_add(Evas_Object *prop, const char *name,
							const char *data, const char *tooltip);

/**
 * data1, data2 is Stringshare
 */
Evas_Object *
ui_prop_view_item_two_edit_string_add(Evas_Object *prop, const char *name,
							const char *data1, const char *tooltip1,
							const char *data2, const char *tooltip2);

/**
 *
 */
Evas_Object *
ui_prop_view_item_bool_add(Evas_Object *prop, const char *name,
							Eina_Bool check, const char *tooltip);

/**
 *
 */
Evas_Object *
ui_prop_view_item_two_bool_add(Evas_Object *prop, const char *name,
							Eina_Bool check1, const char *tooltip1,
							Eina_Bool check2, const char *tooltip2);

/**
 *
 */
Evas_Object *
ui_prop_view_item_one_edit_double_add(Evas_Object *prop, const char *name,
							double *data, const char *tooltip);

/**
 *
 */
Evas_Object *
ui_prop_view_item_two_edit_double_add(Evas_Object *prop, const char *name,
							double *data1, const char *tooltip1,
							double *data2, const char *tooltip2);

/**
 *
 */
Evas_Object *
ui_prop_view_item_color_add(Evas_Object *prop, const char *name,
							int *r, int *g, int *b, int *a,
							const char *tooltip);

#endif /* UI_PROPERTY_H */
