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
#include "ui_signal_list.h"
#include "common_macro.h"

/**
 * @addtogroup ui_signal_list_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 created widget Window
 *
 * @procedure
 * @step 1 Call function ui_signal_list_add(window).
 * @step 2 Check returned value.
 *
 * @passcondition not NULL pointer returned
 * @}
 */
START_TEST (ui_signal_list_add_test_p)
{
   elm_init(0,0);
   Evas_Object *win;
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ck_assert_msg(ui_signal_list_add(win) != NULL, "Signal List can't be created.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function ui_signal_list_add(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (ui_signal_list_add_test_n)
{
   elm_init(0,0);
   ck_assert_msg(ui_signal_list_add(NULL) == NULL, "Signal List created with parent NULL.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Group filled with data. (group contain programs and signals)
 * @step 4 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, group).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_p1)
{
   elm_init(0,0);
   Evas_Object *win = NULL, *gl_signals = NULL;
   Evas *e = NULL;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   gl_signals = ui_signal_list_add(win);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, group) == true, "Signal List couldn't set data.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Group filled with data. (group doesn't contain any programs and signals)
 * @step 4 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, group).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_p2)
{
   elm_init(0,0);
   Evas_Object *win = NULL, *gl_signals = NULL;
   Evas *e = NULL;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/notbase/test";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   gl_signals = ui_signal_list_add(win);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, group) == true, "Signal List couldn't set data.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Group filled with data.
 * @step 4 created Signal List.
 * @step 5 Signal List filled with data already.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, group) with same group.
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_p3)
{
   elm_init(0,0);
   Evas_Object *win = NULL, *gl_signals = NULL;
   Evas *e = NULL;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   gl_signals = ui_signal_list_add(win);
   ui_signal_list_data_set(gl_signals, group);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, group) == true, "Signal List couldn't set data.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Group filled with data.
 * @step 4 another Group. (group doesn't contain any programs or signals)
 * @step 5 created Signal List.
 * @step 6 Signal List filled with data already.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, another_group) with another group (that is without any programs or signals).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_p4)
{
   elm_init(0,0);
   Evas_Object *win = NULL, *gl_signals = NULL;
   Evas *e = NULL;
   Group *group = NULL, *another_group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";
   const char *another_group_name = "default";
   const char *another_full_group_name = "elm/radio/base/test";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   another_group = wm_group_add(another_group_name, another_full_group_name);
   wm_group_data_load(another_group, e, edj);
   gl_signals = ui_signal_list_add(win);
   ui_signal_list_data_set(gl_signals, group);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, another_group) == true, "Signal List couldn't set data.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Group filled with data. (group contain programs and signals)
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(NULL, group).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_n1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);

   ck_assert_msg(ui_signal_list_data_set(NULL, group) == false, "Signal List set data successfull.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_n2)
{
   elm_init(0,0);
   Evas_Object *win = NULL, *gl_signals = NULL;
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gl_signals = ui_signal_list_add(win);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, NULL) == false, "Signal List set data successfull.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(NULL, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(ui_signal_list_data_set(NULL, NULL) == false, "Signal List set data successfull.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 empty Group
 * @step 4 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, group).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_n4)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Group *group = NULL;
   Evas_Object *gl_signals = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   gl_signals = ui_signal_list_add(win);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, group) == false, "Signal List set data successfull.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Group filled with data.
 * @step 4 another empty Group.
 * @step 5 created Signal List.
 * @step 6 Signal List filled with data already.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, another_group) with another empty group.
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_signal_list_data_set_test_n5)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Group *group = NULL, *another_group = NULL;
   Evas_Object *gl_signals = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";
   const char *another_group_name = "default";
   const char *another_full_group_name = "elm/radio/base/test";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   another_group = wm_group_add(another_group_name, another_full_group_name);
   gl_signals = ui_signal_list_add(win);
   ui_signal_list_data_set(gl_signals, group);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, another_group) == false, "Signal List set data successfull.");

   wm_group_free(group);
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
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("ui_signal_list_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_signal_list_add_test_p);
   tcase_add_test(tcase, ui_signal_list_add_test_n);
   tcase_add_test(tcase, ui_signal_list_data_set_test_p1);
   tcase_add_test(tcase, ui_signal_list_data_set_test_p2);
   tcase_add_test(tcase, ui_signal_list_data_set_test_p3);
   tcase_add_test(tcase, ui_signal_list_data_set_test_p4);
   tcase_add_test(tcase, ui_signal_list_data_set_test_n1);
   tcase_add_test(tcase, ui_signal_list_data_set_test_n2);
   tcase_add_test(tcase, ui_signal_list_data_set_test_n3);
   tcase_add_test(tcase, ui_signal_list_data_set_test_n4);
   tcase_add_test(tcase, ui_signal_list_data_set_test_n5);
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
   srunner_set_xml (runner, "test_uisignal_list.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
