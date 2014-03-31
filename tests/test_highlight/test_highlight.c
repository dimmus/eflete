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
* along with this program; If not, see .
*/

#include "test_highlight.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("highlight_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, highlight_add_test_p);
   tcase_add_test(tcase, highlight_add_test_n);
   tcase_add_test(tcase, highlight_bg_color_set_test_p);
   tcase_add_test(tcase, highlight_bg_color_set_test_n1);
   tcase_add_test(tcase, highlight_bg_color_set_test_n2);
   tcase_add_test(tcase, highlight_bg_color_set_test_n3);
   tcase_add_test(tcase, highlight_border_color_set_test_p);
   tcase_add_test(tcase, highlight_border_color_set_test_n1);
   tcase_add_test(tcase, highlight_border_color_set_test_n2);
   tcase_add_test(tcase, highlight_border_color_set_test_n3);
   tcase_add_test(tcase, highlight_handler_color_set_test_p);
   tcase_add_test(tcase, highlight_handler_color_set_test_n1);
   tcase_add_test(tcase, highlight_handler_color_set_test_n2);
   tcase_add_test(tcase, highlight_handler_color_set_test_n3);
 /*  tcase_add_test(tcase, highlight_handler_disable_set_test_p1); */
 /*  tcase_add_test(tcase, highlight_handler_disable_set_test_p2); */
 /*  tcase_add_test(tcase, highlight_handler_disable_set_test_n1); */
 /*  tcase_add_test(tcase, highlight_handler_disable_set_test_n2); */
   tcase_add_test(tcase, highlight_handler_mode_set_test_p);
   tcase_add_test(tcase, highlight_handler_mode_set_test_n1);
   tcase_add_test(tcase, highlight_handler_mode_set_test_n2);
 /*  tcase_add_test(tcase, highlight_object_follow_test_p1); */
 /*  tcase_add_test(tcase, highlight_object_follow_test_p2); */
 /*  tcase_add_test(tcase, highlight_object_follow_test_n1); */
 /*  tcase_add_test(tcase, highlight_object_follow_test_n2); */
 /*  tcase_add_test(tcase, highlight_object_follow_test_n3); */
 /*  tcase_add_test(tcase, highlight_object_follow_test_n4); */
 /*  tcase_add_test(tcase, highlight_object_unfollow_test_p1); */
 /*  tcase_add_test(tcase, highlight_object_unfollow_test_p2); */
 /*  tcase_add_test(tcase, highlight_object_unfollow_test_n1); */
 /*  tcase_add_test(tcase, highlight_object_unfollow_test_n2); */
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "highlight_test.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
