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

#include "test_change.h"

static const Efl_Test_Case etc[] = {
  { "Add", change_add_test },
  { "Free", change_free_test },
  { "Diff_Add", change_diff_add_test },
  { "Diff_Merge_Add", change_diff_merge_add_test },
  { NULL, NULL }
};

SUITE_INIT(eina)
{
   ck_assert_int_eq(eina_init(), 1);
}

SUITE_SHUTDOWN(eina)
{
   ck_assert_int_eq(eina_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Change", etc, SUITE_INIT_FN(eina), SUITE_SHUTDOWN_FN(eina));

   return (failed_count == 0) ? 0 : 255;
}

// Suite* test_suite (void) {
//    Suite *suite = suite_create("change_test");
//    TCase *tcase = tcase_create("TCase");

//    tcase_add_test(tcase, change_add_test_p);
//    tcase_add_test(tcase, change_free_test_p);
//    tcase_add_test(tcase, change_diff_add_test_p);
//    tcase_add_test(tcase, change_diff_merge_add_test_p);

//    suite_add_tcase(suite, tcase);
//    return suite;
// }

// int main(void) {
//    int number_failed;
//    Suite *suite = test_suite();
//    SRunner *runner = srunner_create(suite);
//    srunner_set_xml (runner, "test_change.xml");
//    srunner_run_all(runner, CK_VERBOSE);
//    number_failed = srunner_ntests_failed(runner);
//    srunner_free(runner);
//    return number_failed;
// }
