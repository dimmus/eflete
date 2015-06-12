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

#include "test_widget_manager.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("widget_manager_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, wm_part_add_test_p);
   tcase_add_test(tcase, wm_part_add_test_n1);
   tcase_add_test(tcase, wm_part_add_test_n2);
   tcase_add_test(tcase, wm_part_add_test_n3);
   tcase_add_test(tcase, wm_part_add_test_n4);
   tcase_add_test(tcase, wm_part_del_test_p1);
   tcase_add_test(tcase, wm_part_del_test_p2);
   tcase_add_test(tcase, wm_part_del_test_n1);
   tcase_add_test(tcase, wm_part_del_test_n2);
   tcase_add_test(tcase, wm_part_del_test_n3);
   tcase_add_test(tcase, wm_program_signals_list_get_test_p1);
   tcase_add_test(tcase, wm_program_signals_list_get_test_p2);
   tcase_add_test(tcase, wm_program_signals_list_get_test_n1);
   tcase_add_test(tcase, wm_program_signals_list_get_test_n2);
   tcase_add_test(tcase, wm_style_add_test_p);
   tcase_add_test(tcase, wm_style_add_test_p1);
   tcase_add_test(tcase, wm_style_add_test_n1);
   tcase_add_test(tcase, wm_style_add_test_n2);
   tcase_add_test(tcase, wm_style_add_test_n3);
   tcase_add_test(tcase, wm_style_add_test_n4);
   tcase_add_test(tcase, wm_style_add_test_n5);
   tcase_add_test(tcase, wm_style_free_test_p1);
   tcase_add_test(tcase, wm_style_free_test_p2);
   tcase_add_test(tcase, wm_style_free_test_n);
   tcase_add_test(tcase, wm_program_signals_list_free_test_p);
   tcase_add_test(tcase, wm_program_signals_list_free_test_n);
   tcase_add_test(tcase, wm_class_add_test_p);
   tcase_add_test(tcase, wm_class_add_test_n1);
   tcase_add_test(tcase, wm_class_add_test_n2);
   tcase_add_test(tcase, wm_class_add_test_n3);
   tcase_add_test(tcase, wm_class_free_test_p);
   tcase_add_test(tcase, wm_class_free_test_n);
   tcase_add_test(tcase, wm_widget_add_test_p);
   tcase_add_test(tcase, wm_widget_add_test_n1);
   tcase_add_test(tcase, wm_widget_add_test_n2);
   tcase_add_test(tcase, wm_widget_add_test_n3);
   tcase_add_test(tcase, wm_widget_free_test_p);
   tcase_add_test(tcase, wm_widget_free_test_n);
   tcase_add_test(tcase, wm_widget_list_free_test_p);
   tcase_add_test(tcase, wm_widget_list_free_test_n);
   tcase_add_test(tcase, wm_widget_list_new_test_p);
   tcase_add_test(tcase, wm_widget_list_new_test_n);
   tcase_add_test(tcase, wm_layouts_list_new_test_p);
   tcase_add_test(tcase, wm_layouts_list_new_test_n);
   tcase_add_test(tcase, wm_style_object_find_test_p1);
   tcase_add_test(tcase, wm_style_object_find_test_p2);
   tcase_add_test(tcase, wm_style_object_find_test_n1);
   tcase_add_test(tcase, wm_style_object_find_test_n2);
   tcase_add_test(tcase, wm_style_object_find_test_n3);
   tcase_add_test(tcase, wm_part_type_get_test_p1);
   tcase_add_test(tcase, wm_part_type_get_test_p2);
   tcase_add_test(tcase, wm_part_type_get_test_n1);
   tcase_add_test(tcase, wm_part_type_get_test_n2);
   tcase_add_test(tcase, wm_part_current_state_set_test_p1);
   tcase_add_test(tcase, wm_part_current_state_set_test_p2);
   tcase_add_test(tcase, wm_part_current_state_set_test_p3);
   tcase_add_test(tcase, wm_part_current_state_set_test_n1);
   tcase_add_test(tcase, wm_part_current_state_set_test_n2);
   tcase_add_test(tcase, wm_part_current_state_set_test_n3);
   tcase_add_test(tcase, wm_widget_list_objects_load_test_p);
   tcase_add_test(tcase, wm_widget_list_objects_load_test_n1);
   tcase_add_test(tcase, wm_widget_list_objects_load_test_n2);
   tcase_add_test(tcase, wm_widget_list_objects_load_test_n3);
   tcase_add_test(tcase, wm_layouts_list_objects_load_test_p);
   tcase_add_test(tcase, wm_layouts_list_objects_load_test_n1);
   tcase_add_test(tcase, wm_layouts_list_objects_load_test_n2);
   tcase_add_test(tcase, wm_layouts_list_objects_load_test_n3);
   tcase_add_test(tcase, wm_styles_build_alias_test_p1);
   tcase_add_test(tcase, wm_styles_build_alias_test_p2);
   tcase_add_test(tcase, wm_styles_build_alias_test_n);

   tcase_add_test(tcase, wm_style_copy_test_p);
   tcase_add_test(tcase, wm_style_copy_test_n1);
   tcase_add_test(tcase, wm_style_copy_test_n2);

   tcase_add_test(tcase, wm_part_by_name_find_test_p1);
   tcase_add_test(tcase, wm_part_by_name_find_test_n1);
   tcase_add_test(tcase, wm_part_by_name_find_test_n2);
   tcase_add_test(tcase, wm_part_by_name_find_test_n3);
   tcase_add_test(tcase, wm_part_by_name_find_test_n4);

   tcase_add_test(tcase, wm_style_data_reload_test_p);
   tcase_add_test(tcase, wm_style_data_reload_test_n1);
   tcase_add_test(tcase, wm_style_data_reload_test_n2);
   tcase_add_test(tcase, wm_style_data_reload_test_n3);

   suite_add_tcase(suite, tcase);

   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_widget_manager.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
