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

#include <check.h>
#include "colorclass_editor.h"
#include "ui_main_window.h"

/**
 * @addtogroup colorclass_viewer_add
 * @{
 * @objective Negative test case:
 *
 * @precondition
 * @1. initialized efl
 *
 * @procedure
 * @step 1 Call colorclass_viewer_add(NULL)
 * @step 2 Check returned pointer
 *
 * @passcondition: NULL returned
 * @}
 */
START_TEST (colorclass_viewer_add_test_n)
{
   elm_init(0,0);
   ck_assert_msg(colorclass_viewer_add(NULL) == NULL, "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorclass_viewer_add
 * @{
 * @objective Positive test case:
 *
 * @precondition
 * @1. initialized efl and app
 * @2. app->project is set to any valid pointer (colorclass_viewer itself do not
 * @uses project fields. it only makes a copy of pointer)
 *
 * @procedure
 * @step 1 Call colorclass_viewer_add(app)
 * @step 2 Check returned pointer
 *
 * @passcondition: Not NULL returned
 * @}
 */
START_TEST (colorclass_viewer_add_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   app->project = calloc(1, sizeof(Project));
   ui_main_window_add(app);
   ck_assert_msg(colorclass_viewer_add(app->project) != NULL, "failure: cannot"
      " create image editor window");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup test_suite
 * @{
 * @objective Creating above to the test case:
 *
 * @procedure
 * @step 1 Create suite
 * @step 2 Create test case
 * @step 3 Add unit tests to the test case
 * @step 4 Add test case to the suite
 *
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("test_colorclass_editor");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, colorclass_viewer_add_test_n);
   tcase_add_test(tcase, colorclass_viewer_add_test_p);
   suite_add_tcase(suite, tcase);
   return suite;
}

/**
 * @addtogroup main
 * @{
 * @objective : Run a Check Unit Test
 *
 * @procedure
 * @step 1 Create a suite
 * @step 2 Create a suite runner object of type SRunner
 * @step 3 Add report of Check tests to the xml format
 * @step 4 Run the suite, using the CK_VERBOSE flag
 * @step 5 Create int object for list of the failures
 *
 * @passcondition: Print a summary of the run unit tests.
 * @}
 */
int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml(runner, "test_colorclass_editor.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
