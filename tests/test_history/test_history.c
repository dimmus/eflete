/*
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

#include "test_history.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("test_history");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, history_init_test_p);
   tcase_add_test(tcase, history_term_test_p);
   tcase_add_test(tcase, history_term_test_n);

   tcase_add_test(tcase, history_clear_test_p);
   tcase_add_test(tcase, history_clear_test_n);

   tcase_add_test(tcase, history_module_add_test_p1);
   tcase_add_test(tcase, history_module_add_test_p2);
   tcase_add_test(tcase, history_module_add_test_n1);
   tcase_add_test(tcase, history_module_add_test_n2);

   tcase_add_test(tcase, history_module_del_test_p1);
   tcase_add_test(tcase, history_module_del_test_p2);
   tcase_add_test(tcase, history_module_del_test_n1);
   tcase_add_test(tcase, history_module_del_test_n2);
   tcase_add_test(tcase, history_module_del_test_n3);

   tcase_add_test(tcase, history_diff_add_test_p1);
   tcase_add_test(tcase, history_diff_add_test_p2);
   tcase_add_test(tcase, history_diff_add_test_p3);
   tcase_add_test(tcase, history_diff_add_test_p4);
   tcase_add_test(tcase, history_diff_add_test_p5);
   tcase_add_test(tcase, history_diff_add_test_p6);
   tcase_add_test(tcase, history_diff_add_test_p7);
   tcase_add_test(tcase, history_diff_add_test_p8);
   tcase_add_test(tcase, history_diff_add_test_p9);
   tcase_add_test(tcase, history_diff_add_test_n1);
   tcase_add_test(tcase, history_diff_add_test_n2);
   tcase_add_test(tcase, history_diff_add_test_n3);
   tcase_add_test(tcase, history_diff_add_test_n4);
   tcase_add_test(tcase, history_diff_add_test_n5);
   tcase_add_test(tcase, history_diff_add_test_n6);
   tcase_add_test(tcase, history_diff_add_test_n7);
   tcase_add_test(tcase, history_diff_add_test_n8);

   tcase_add_test(tcase, history_undo_test_p1);
   tcase_add_test(tcase, history_undo_test_p2);
   tcase_add_test(tcase, history_undo_test_p3);
   tcase_add_test(tcase, history_undo_test_p4);
   tcase_add_test(tcase, history_undo_test_p5);
   tcase_add_test(tcase, history_undo_test_p6);
   tcase_add_test(tcase, history_undo_test_p7);
   tcase_add_test(tcase, history_undo_test_p8);
   tcase_add_test(tcase, history_undo_test_p9);
   tcase_add_test(tcase, history_undo_test_p10);
   tcase_add_test(tcase, history_undo_test_p11);
   tcase_add_test(tcase, history_undo_test_p12);
   tcase_add_test(tcase, history_undo_test_p13);
   tcase_add_test(tcase, history_undo_test_p14);
   tcase_add_test(tcase, history_undo_test_p15);
   tcase_add_test(tcase, history_undo_test_n1);
   tcase_add_test(tcase, history_undo_test_n2);
   tcase_add_test(tcase, history_undo_test_n3);

   tcase_add_test(tcase, history_redo_test_p1);
   tcase_add_test(tcase, history_redo_test_p2);
   tcase_add_test(tcase, history_redo_test_p3);
   tcase_add_test(tcase, history_redo_test_p4);
   tcase_add_test(tcase, history_redo_test_p5);
   tcase_add_test(tcase, history_redo_test_p6);
   tcase_add_test(tcase, history_redo_test_p7);
   tcase_add_test(tcase, history_redo_test_p8);
   tcase_add_test(tcase, history_redo_test_p9);
   tcase_add_test(tcase, history_redo_test_p10);
   tcase_add_test(tcase, history_redo_test_p11);
   tcase_add_test(tcase, history_redo_test_p12);
   tcase_add_test(tcase, history_redo_test_n1);
   tcase_add_test(tcase, history_redo_test_n2);
   tcase_add_test(tcase, history_redo_test_n3);

   tcase_add_test(tcase, history_genlist_get_test_p1);
   tcase_add_test(tcase, history_genlist_get_test_p2);
   tcase_add_test(tcase, history_genlist_get_test_n1);
   tcase_add_test(tcase, history_genlist_get_test_n2);

   tcase_add_test(tcase, history_module_depth_set_test_p);
   tcase_add_test(tcase, history_module_depth_set_test_n1);
   tcase_add_test(tcase, history_module_depth_set_test_n2);

   tcase_add_test(tcase, history_module_depth_get_test_p1);
   tcase_add_test(tcase, history_module_depth_get_test_p2);
   tcase_add_test(tcase, history_module_depth_get_test_n1);
   tcase_add_test(tcase, history_module_depth_get_test_n2);

   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml(runner, "test_history.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
