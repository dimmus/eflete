/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "utc_common.h"
#include "widget_manager.h"
#include "alloc.h"

/**
 * @defgroup test_widget_manager
 */

void wm_part_add_test_p(int);
void wm_part_add_test_n1(int);
void wm_part_add_test_n2(int);
void wm_part_add_test_n3(int);
void wm_part_add_test_n4(int);
void wm_part_del_test_p1(int);
void wm_part_del_test_p2(int);
void wm_part_del_test_n1(int);
void wm_part_del_test_n2(int);
void wm_part_del_test_n3(int);
void wm_style_add_test_p(int);
void wm_style_add_test_p1(int);
void wm_style_add_test_n1(int);
void wm_style_add_test_n2(int);
void wm_style_add_test_n3(int);
void wm_style_add_test_n4(int);
void wm_style_add_test_n5(int);
void wm_style_free_test_p1(int);
void wm_style_free_test_p2(int);
void wm_style_free_test_n(int);
void wm_program_signals_list_get_test_p1(int);
void wm_program_signals_list_get_test_p2(int);
void wm_program_signals_list_get_test_n1(int);
void wm_program_signals_list_get_test_n2(int);
void wm_program_signals_list_free_test_p(int);
void wm_program_signals_list_free_test_n(int);
void wm_class_add_test_p(int);
void wm_class_add_test_n1(int);
void wm_class_add_test_n2(int);
void wm_class_add_test_n3(int);
void wm_class_free_test_p(int);
void wm_class_free_test_n(int);
void wm_widget_add_test_p(int);
void wm_widget_add_test_n1(int);
void wm_widget_add_test_n2(int);
void wm_widget_add_test_n3(int);
void wm_widget_free_test_p(int);
void wm_widget_free_test_n(int);
void wm_widget_list_new_test_p(int);
void wm_widget_list_new_test_n(int);
void wm_widget_list_layouts_load_test_p(int);
void wm_widget_list_layouts_load_test_n(int);
void wm_widget_list_free_test_p(int);
void wm_widget_list_free_test_n(int);
void wm_style_object_find_test_p1(int);
void wm_style_object_find_test_p2(int);
void wm_style_object_find_test_n1(int);
void wm_style_object_find_test_n2(int);
void wm_style_object_find_test_n3(int);
void wm_part_type_get_test_p1(int);
void wm_part_type_get_test_p2(int);
void wm_part_type_get_test_n1(int);
void wm_part_type_get_test_n2(int);
void wm_part_current_state_set_test_p1(int);
void wm_part_current_state_set_test_p2(int);
void wm_part_current_state_set_test_p3(int);
void wm_part_current_state_set_test_n1(int);
void wm_part_current_state_set_test_n2(int);
void wm_part_current_state_set_test_n3(int);
void wm_widget_list_objects_load_test_p(int);
void wm_widget_list_objects_load_test_n1(int);
void wm_widget_list_objects_load_test_n2(int);
void wm_widget_list_objects_load_test_n3(int);
void wm_layouts_list_objects_load_test_p(int);
void wm_layouts_list_objects_load_test_n1(int);
void wm_layouts_list_objects_load_test_n2(int);
void wm_layouts_list_objects_load_test_n3(int);
void wm_style_copy_test_p(int);
void wm_style_copy_test_n1(int);
void wm_style_copy_test_n2(int);
