/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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

#include "test_ewe_entry.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("ewe_init_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ewe_entry_add_test_p);
   tcase_add_test(tcase, ewe_entry_add_test_n);
   tcase_add_test(tcase, ewe_entry_regex_set_test_n1);
   tcase_add_test(tcase, ewe_entry_regex_set_test_n2);
   tcase_add_test(tcase, ewe_entry_regex_set_test_n3);
   tcase_add_test(tcase, ewe_entry_regex_set_test_n4);
   tcase_add_test(tcase, ewe_entry_regex_set_test_n5);
   tcase_add_test(tcase, ewe_entry_regex_set_test_p1);
   tcase_add_test(tcase, ewe_entry_regex_set_test_p2);
   tcase_add_test(tcase, ewe_entry_regex_unset_test_n1);
   tcase_add_test(tcase, ewe_entry_regex_unset_test_n2);
   tcase_add_test(tcase, ewe_entry_regex_unset_test_p);
   tcase_add_test(tcase, ewe_entry_regex_check_test_n1);
   tcase_add_test(tcase, ewe_entry_regex_check_test_n2);
   tcase_add_test(tcase, ewe_entry_regex_check_test_p1);
   tcase_add_test(tcase, ewe_entry_regex_check_test_p2);
   tcase_add_test(tcase, ewe_entry_regex_check_test_p3);
   tcase_add_test(tcase, ewe_entry_regex_glow_n1);
   tcase_add_test(tcase, ewe_entry_regex_glow_n2);
   tcase_add_test(tcase, ewe_entry_regex_glow_p1);
   tcase_add_test(tcase, ewe_entry_regex_glow_p2);
   tcase_add_test(tcase, ewe_entry_regex_error_get_n1);
   tcase_add_test(tcase, ewe_entry_regex_error_get_n2);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p1);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p2);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p3);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p4);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p5);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p6);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p7);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p8);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p9);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p10);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p11);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p12);
   tcase_add_test(tcase, ewe_entry_regex_error_get_p13);
   tcase_add_test(tcase, ewe_entry_regex_autocheck_n1);
   tcase_add_test(tcase, ewe_entry_regex_autocheck_n2);
   tcase_add_test(tcase, ewe_entry_regex_autocheck_p1);
   tcase_add_test(tcase, ewe_entry_regex_autocheck_p2);
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_ewe_entry.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
