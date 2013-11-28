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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include <check.h>
#include "image_editor.h"
#include "ui_main_window.h"

/**
 * @addto group image_editor_window_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 project loaded
 * @step 4 widget_list_objects loaded from file
 *
 * @procedure
 * @step 1 Call image_editor_window_add(app->project, SINGLE)
 * @step 2 Check returned pointer
 *
 * @passcondition Not NULL returned
 */
START_TEST (image_editor_window_add_test_p1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");
   wm_widget_list_objects_load(app->project->widgets,
                        evas_object_evas_get(app->win), app->project->swapfile);
   ck_assert_msg(image_editor_window_add(app->project, SINGLE) != NULL,
                 "failure: cannot create image editor window");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addto group image_editor_window_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 project loaded
 * @step 4 widget_list_objects loaded from file
 *
 * @procedure
 * @step 1 Call image_editor_window_add(app->project, MULTIPLE)
 * @step 2 Check returned pointer
 *
 * @passcondition Not NULL returned
 */
START_TEST (image_editor_window_add_test_p2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");
   wm_widget_list_objects_load(app->project->widgets,
                        evas_object_evas_get(app->win), app->project->swapfile);
   ck_assert_msg(image_editor_window_add(app->project, MULTIPLE) != NULL,
                 "failure: cannot create image editor window");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_window_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call image_editor_window_add(NULL, SINGLE)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (image_editor_window_add_test_n1)
{
   elm_init(0,0);
   ck_assert_msg(image_editor_window_add(NULL, SINGLE) == NULL,
                 "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_window_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call image_editor_window_add(NULL, MULTIPLE)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (image_editor_window_add_test_n2)
{
   elm_init(0,0);
   ck_assert_msg(image_editor_window_add(NULL, MULTIPLE) == NULL,
                 "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup test_suite
 * @{
 * @objective Creating above to the test case
 *
 * @procedure
 * @step 1 Create suite
 * @step 2 Create test case
 * @step 3 Add unit tests to the test case
 * @step 4 Add test case to the suite
 *
 * @passcondition Return a Suite object
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("image_editor_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, image_editor_window_add_test_p1);
   tcase_add_test(tcase, image_editor_window_add_test_p2);
   tcase_add_test(tcase, image_editor_window_add_test_n1);
   tcase_add_test(tcase, image_editor_window_add_test_n2);
   suite_add_tcase(suite, tcase);
   return suite;
}

/**
 * @addtogroup main
 * @{
 * @objective Run a Check Unit Test
 *
 * @procedure
 * @step 1 Create a suite
 * @step 2 Create a suite runner object of type SRunner
 * @step 3 Add report of Check tests to the xml format
 * @step 4 Run the suite, using the CK_VERBOSE flag
 * @step 5 Create int object for list of the failures
 *
 * @passcondition Print a summary of the run unit tests.
 * @}
 */
int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml(runner, "test_image_editor.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
