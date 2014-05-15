/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "test_part_dialog.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("part_dialog_test");
   TCase *tcase = tcase_create("TCase");

/**   tcase_add_test(tcase, part_dialog_add_test_p); TODO: separate functional tests*/
   tcase_add_test(tcase, part_dialog_add_test_n1);
   tcase_add_test(tcase, part_dialog_add_test_n2);
   tcase_add_test(tcase, part_dialog_add_test_n3);
   tcase_add_test(tcase, part_dialog_add_test_n4);

   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_part_dialog.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
