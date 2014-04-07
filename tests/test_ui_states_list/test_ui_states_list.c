/* Edje Theme Editor
* Copyright (C) 2014 Samsung Electronics.
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

#include "test_ui_states_list.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("ui_states_list_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_states_list_add_test_p);
   tcase_add_test(tcase, ui_states_list_add_test_n);
   tcase_add_test(tcase, ui_states_list_data_set_test_p);
   tcase_add_test(tcase, ui_states_list_data_set_test_n1);
   tcase_add_test(tcase, ui_states_list_data_set_test_n2);
   tcase_add_test(tcase, ui_states_list_data_set_test_n3);
   tcase_add_test(tcase, ui_states_list_data_set_test_n4);
   tcase_add_test(tcase, ui_states_list_data_set_test_n5);
 /*  tcase_add_test(tcase, ui_states_list_data_set_test_n6); */
 /*  tcase_add_test(tcase, ui_states_list_part_get_test_p1); */
 /*  tcase_add_test(tcase, ui_states_list_part_get_test_p2); */
 /*  tcase_add_test(tcase, ui_states_list_part_get_test_n1); */
 /*  tcase_add_test(tcase, ui_states_list_part_get_test_n2); */
 /*  tcase_add_test(tcase, ui_states_list_state_add_test_p); */
 /*  tcase_add_test(tcase, ui_states_list_state_add_test_n1); */
 /*  tcase_add_test(tcase, ui_states_list_state_add_test_n2); */
 /*  tcase_add_test(tcase, ui_states_list_state_add_test_n3); */
 /*  tcase_add_test(tcase, ui_states_list_selected_state_get_test_p); */
 /*  tcase_add_test(tcase, ui_states_list_selected_state_get_test_n1); */
 /*  tcase_add_test(tcase, ui_states_list_selected_state_get_test_n2); */
 /*  tcase_add_test(tcase, ui_states_list_selected_states_get_test_p); */
 /*  tcase_add_test(tcase, ui_states_list_selected_states_get_test_n1); */
 /*  tcase_add_test(tcase, ui_states_list_selected_states_get_test_n2); */
  /*  tcase_add_test(tcase, ui_states_list_selected_states_del_test_p); */
  /*  tcase_add_test(tcase, ui_states_list_selected_states_del_test_n1); */
  /*  tcase_add_test(tcase, ui_states_list_selected_states_del_test_n2); */
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_ui_states_list.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
