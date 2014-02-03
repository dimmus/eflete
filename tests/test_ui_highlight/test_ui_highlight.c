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
#include "highlight.h"
#include "ui_main_window.h"

/**
 * @addtogroup hl_highlight_add
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL arguments
 *
 * @passcondition: NULL returned
 * @}
 */
START_TEST (hl_highlight_add_test_n1)
{
   ck_assert_msg(/*hl_highlight_add(NULL) == NULL*/ false, "Not NULL returned");
}
END_TEST

/**
 * @addtogroup hl_highlight_add
 * @{
 * @objective Posirive test case:
 *
 * @procedure
 * @step 1 Create app  App_Data *app
 * @step 2 Create main window
 * @step 3 Add highlight
 *
 * @passcondition: Return Evas_Object
 * @}
 */
START_TEST (hl_highlight_add_test_n2)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(app != NULL,"App_Data is NULL");
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   fail_unless(/*hl_highlight_add(app->ws->groupspace) != NULL*/ false, "failure: cannot add highlight");
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup hl_highlight_object_set
 * @note for future API!
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL arguments
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (hl_highlight_object_set_test_n1)
{
   /* TODO: add a hl_highlight_object_set() function call, after it appears in the API*/
}
END_TEST


/**
 * @addtogroup  hl_highlight_object_set
 * @note for future API!
 * @{
 * @objective Posirive test case:
 *
 * @procedure
 * @step 1 Create app  App_Data *app
 * @step 2 Create main window
 *
 * @passcondition: Return Evas_Object
 * @}
 */
START_TEST ( hl_highlight_object_set_test_n2)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(app != NULL,"App_Data is NULL");
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   fail_unless(/*hl_highlight_add(app->ws->groupspace) != NULL*/ false, "failure: cannot add highlight");
   /* TODO: add a hl_highlight_object_set() function call, after it appears in the API*/
   elm_shutdown();
   app_free(app);
}
END_TEST


/**
 * @addto group hl_highlight_handler_color_set
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL arguments
 *
 * @passcondition: test passed
 */
START_TEST (hl_highlight_handler_color_set_test_n1)
{
  /*hl_highlight_handler_color_set(NULL, 255, 255, 255, 255);*/
}
END_TEST

/**
 * @addto group hl_highlight_handler_color_set
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create app  App_Data *app
 * @step 2 Create main window
 * @step 3 Add highlight
 * @step 4 Call hl_highlight_handler_color_set
 *
 * @passcondition: test passed
 */
START_TEST (hl_highlight_handler_color_set_test_n2)
{
/*
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(app != NULL,"App_Data is NULL");
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
//   app->ws->highlight.highlight = hl_highlight_add(app->ws->groupspace);
   fail_unless(app->ws->highlight.highlight != NULL, "failure: cannot add highlight");
//   hl_highlight_handler_color_set(app->ws->highlight.highlight, 255, 255, 255, 255);
   elm_shutdown();
   app_free(app);
*/
}
END_TEST

/**
 * @addto group hl_highlight_border_color_set
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL arguments
 *
 * @passcondition: test passed
 */
START_TEST (hl_highlight_border_color_set_test_n1)
{
//  hl_highlight_border_color_set(NULL, 255, 255, 255, 255);
}
END_TEST

/**
 * @addto group hl_highlight_border_color_set
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create app  App_Data *app
 * @step 2 Create main window
 * @step 3 Add highlight
 * @step 4 Call hl_highlight_border_color_set
 *
 * @passcondition: test passed
 */
START_TEST (hl_highlight_border_color_set_test_n2)
{
/*
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(app != NULL,"App_Data is NULL");
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
//   app->ws->highlight.highlight = hl_highlight_add(app->ws->groupspace);
   fail_unless(app->ws->highlight.highlight != NULL, "failure: cannot add highlight");
//   hl_highlight_border_color_set(app->ws->highlight.highlight, 255, 255, 255, 255);
   elm_shutdown();
   app_free(app);
*/
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
   Suite *suite = suite_create("test_highlight");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, hl_highlight_add_test_n1);
   tcase_add_test(tcase, hl_highlight_add_test_n2);
   tcase_add_test(tcase, hl_highlight_object_set_test_n1);
   tcase_add_test(tcase, hl_highlight_object_set_test_n2);
   tcase_add_test(tcase, hl_highlight_handler_color_set_test_n1);
   tcase_add_test(tcase, hl_highlight_handler_color_set_test_n2);
   tcase_add_test(tcase, hl_highlight_border_color_set_test_n1);
   tcase_add_test(tcase, hl_highlight_border_color_set_test_n2);
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
   srunner_set_xml (runner, "test_highlight.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
