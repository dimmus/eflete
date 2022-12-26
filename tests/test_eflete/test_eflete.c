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

#include "test_eflete.h"

static const Efl_Test_Case etc[] = {
  { "Alloc", mem_malloc_test },
  { "Calloc", mem_calloc_test },
  { NULL, NULL }
};

SUITE_INIT(elm)
{
   //ck_assert_int_eq(elm_init(0, 0), 1);
}

SUITE_SHUTDOWN(elm)
{
   //ck_assert_int_eq(elm_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Memory allocation", etc, SUITE_INIT_FN(elm), SUITE_SHUTDOWN_FN(elm));

   return (failed_count == 0) ? 0 : 255;
}

// Suite* test_suite (void) {
//    Suite *suite = suite_create("eflete_test");
//    TCase *tcase = tcase_create("TCase");

//    tcase_add_test(tcase, app_init_test_p);

//    tcase_add_test(tcase, app_shutdown_test_p1);
//    tcase_add_test(tcase, app_shutdown_test_p2);

//    suite_add_tcase(suite, tcase);
//    return suite;
// }

// int main(void) {
//    int number_failed;
//    Suite *suite = test_suite();
//    SRunner *runner = srunner_create(suite);
//    srunner_set_xml (runner, "test_eflete.xml");
//    srunner_run_all(runner, CK_VERBOSE);
//    number_failed = srunner_ntests_failed(runner);
//    srunner_free(runner);
//    return number_failed;
// }
