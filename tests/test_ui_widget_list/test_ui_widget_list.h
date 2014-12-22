/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "ui_widget_list.h"
#include "utc_common.h"

/**
 * @defgroup test_ui_widget_list
 */

#define TEST_NAME "widgtet_list"

void ui_widget_list_add_test_p(int);
void ui_widget_list_add_test_n(int);

void ui_widget_list_data_set_test_p1(int);
void ui_widget_list_data_set_test_p2(int);
void ui_widget_list_data_set_test_p3(int);
void ui_widget_list_data_set_test_p4(int);
void ui_widget_list_data_set_test_n1(int);
void ui_widget_list_data_set_test_n2(int);

void ui_widget_list_class_data_reload_test_p1(int);
void ui_widget_list_class_data_reload_test_p2(int);
void ui_widget_list_class_data_reload_test_n1(int);
void ui_widget_list_class_data_reload_test_n2(int);
void ui_widget_list_class_data_reload_test_n3(int);

void ui_widget_list_layouts_reload_test_p(int);
void ui_widget_list_layouts_reload_test_n1(int);
void ui_widget_list_layouts_reload_test_n2(int);
void ui_widget_list_layouts_reload_test_n3(int);

void ui_widget_list_part_add_test_p(int);
void ui_widget_list_part_add_test_n1(int);
void ui_widget_list_part_add_test_n2(int);
void ui_widget_list_part_add_test_n3(int);
void ui_widget_list_part_add_test_n4(int);

void ui_widget_list_selected_part_above_test_p(int);
void ui_widget_list_selected_part_above_test_n1(int);
void ui_widget_list_selected_part_above_test_n2(int);
void ui_widget_list_selected_part_above_test_n3(int);
void ui_widget_list_selected_part_above_test_n4(int);

void ui_widget_list_selected_part_below_test_p(int);
void ui_widget_list_selected_part_below_test_n1(int);
void ui_widget_list_selected_part_below_test_n2(int);
void ui_widget_list_selected_part_below_test_n3(int);
void ui_widget_list_selected_part_below_test_n4(int);

void ui_widget_list_selected_part_del_test_p(int);
void ui_widget_list_selected_part_del_test_n1(int);
void ui_widget_list_selected_part_del_test_n2(int);
void ui_widget_list_selected_part_del_test_n3(int);

void ui_widget_list_selected_part_get_test_p(int);
void ui_widget_list_selected_part_get_test_n1(int);
void ui_widget_list_selected_part_get_test_n2(int);

void ui_widget_list_part_selected_set_test_p(int);
void ui_widget_list_part_selected_set_test_n1(int);
void ui_widget_list_part_selected_set_test_n2(int);
void ui_widget_list_part_selected_set_test_n3(int);

void ui_widget_list_title_set_test_p(int);
void ui_widget_list_title_set_test_n1(int);
void ui_widget_list_title_set_test_n2(int);

void ui_widget_list_selected_parts_get_test_p(int);
void ui_widget_list_selected_parts_get_test_n1(int);
void ui_widget_list_selected_parts_get_test_n2(int);

void ui_widget_list_tab_activate_test_p(int);
void ui_widget_list_tab_activate_test_n1(int);
void ui_widget_list_tab_activate_test_n2(int);
