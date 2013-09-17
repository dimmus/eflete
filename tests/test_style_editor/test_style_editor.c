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

#include <check.h>
#include "style_editor.h"
#include "ui_main_window.h"

/**
 * @addtogroup style_editor_window_add
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function for with NULL argument
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (style_editor_window_add_test_n1)
{
   elm_init(0,0);
   style_editor_window_add(NULL, NULL);
   elm_shutdown();
}
END_TEST

/**
 * @addto group style_editor_window_add
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Add style editor window using style_editor_window_add
 *
 * @passcondition: return Evas_Object
 */
START_TEST (style_editor_window_add_test_n2)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create"
      " window");
   ui_edc_load_done(app, "first", EDC_PATH
      "naviframe.edc","","","");
   ck_assert_msg(style_editor_window_add(app->win, app->project) != NULL,
                                   "Window not added");
   elm_shutdown();
   app_shutdown();
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
   Suite *suite = suite_create("test_style_editor");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, style_editor_window_add_test_n1);
   tcase_add_test(tcase, style_editor_window_add_test_n2);
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
   srunner_set_xml(runner, "test_style_editor.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
