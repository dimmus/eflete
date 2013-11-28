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
#include "notify.h"
#include "ui_main_window.h"

/**
 * @addtogroup notify
 * @{
 */

/**
 * @addtogroup noti_error_show
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_error_show(win_layout_get(), "text")
 * @step 2 Check returned value
 * @step 3 Call noti_error_show(win_layout_get(), NULL)
 * @step 4 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (noti_error_show_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_error_show(win_layout_get(), "text"),
                 "can't show notify with text");
   ck_assert_msg(noti_error_show(win_layout_get(), NULL),
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_error_show
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call noti_error_show(NULL, "text")
 * @step 2 Check returned value
 * @step 3 Call noti_error_show(NULL, NULL)
 * @step 4 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (noti_error_show_test_n)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_error_show(NULL, "text") == false,
                 "can't show notify with text");
   ck_assert_msg(noti_error_show(NULL, NULL) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_warning_show
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_warning_show(win_layout_get(), "text")
 * @step 2 Check returned value
 * @step 3 Call noti_warning_show(win_layout_get(), NULL)
 * @step 4 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (noti_warning_show_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_warning_show(win_layout_get(), "text"),
                 "can't show notify with text");
   ck_assert_msg(noti_warning_show(win_layout_get(), NULL),
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_warning_show
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call noti_warning_show(NULL, "text")
 * @step 2 Check returned value
 * @step 3 Call noti_warning_show(NULL, NULL)
 * @step 4 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (noti_warning_show_test_n)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_warning_show(NULL, "text") == false,
                 "can't show notify with text");
   ck_assert_msg(noti_warning_show(NULL, NULL) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info_show
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_info_show(win_layout_get(), "text", 1)
 * @step 2 Check returned value
 * @step 3 Call noti_info_show(win_layout_get(), NULL, 1)
 * @step 4 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (noti_info_show_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_info_show(win_layout_get(), "text", 1),
                 "can't show notify with text");
   ck_assert_msg(noti_info_show(win_layout_get(), NULL, 1),
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info_show
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call noti_info_show(NULL, "text", 1)
 * @step 2 Check returned value
 * @step 3 Call noti_info_show(NULL, NULL, 1)
 * @step 4 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (noti_info_show_test_n1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_info_show(NULL, "text", 1) == false,
                 "can't show notify with text");
   ck_assert_msg(noti_info_show(NULL, NULL, 1) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info_show
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_info_show(win_layout_get(), "text", -1)
 * @step 2 Check returned value
 * @step 3 Call noti_info_show(win_layout_get(), NULL, -1)
 * @step 4 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (noti_info_show_test_n2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_info_show(win_layout_get(), "text", -1) == false,
                 "can't show notify with text");
   ck_assert_msg(noti_info_show(win_layout_get(), NULL, -1) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST


/**
 * @}
 */

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
   Suite *suite = suite_create("notify_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, noti_error_show_test_p);
   tcase_add_test(tcase, noti_error_show_test_n);
   tcase_add_test(tcase, noti_warning_show_test_p);
   tcase_add_test(tcase, noti_warning_show_test_n);
   tcase_add_test(tcase, noti_info_show_test_p);
   tcase_add_test(tcase, noti_info_show_test_n1);
   tcase_add_test(tcase, noti_info_show_test_n2);
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
   srunner_set_xml (runner, "test_notify.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
