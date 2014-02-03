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
#include "ui_workspace.h"
/**
 * @addtogroup ws_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parent and layout
 * @step 2 Call ws_add()
 *
 * @passcondition: Workspace object returned from function
 * @}
 */
START_TEST (ws_add_test_p)
{
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ck_assert_msg(ws != NULL, "Workspace object not returned");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ws_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Create window parent(not layout)
 * @step 2 Call ws_add()
 * @param parent The parent object
 *
 * @passcondition: Null returned from function
 * @}
 */
START_TEST (ws_add_test_n1)
{
/*
   elm_init(0, 0);
   Evas_Object *parent;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   Workspace* ws = ws_add(parent);
   ck_assert_msg(ws == NULL, "NULL expected but not-NULL returned");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ws_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create parent
 * @step 2 Call ws_add()
 * @param parent The parent object
 *
 * @passcondition: NULL returned from function
 * @}
 */
START_TEST (ws_add_test_n2)
{
/*
   elm_init(0, 0);
   Workspace *ws = ws_add(NULL);
   ck_assert_msg(ws == NULL, "Not NULL returned\n");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ws_zoom_step_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parent and layout
 * @step 2 Call ws_zoom_step_set()
 * @step 3 Call ws_zoom_step_get()
 *
 * @passcondition: value match with eps=10e-6
 * @}
 */
START_TEST (ws_zoom_step_p)
{
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ws_zoom_step_set(ws, 50.4);
   double diff = 50.4 - ws_zoom_step_get(ws);
   if(diff < 0) diff = -diff;
   ck_assert_msg(diff < 10e-6, "Value missmatch");
   ws_zoom_step_set(ws, 4.2);
   diff = 4.2 - ws_zoom_step_get(ws);
   if(diff < 0) diff = -diff;
   ck_assert_msg(diff < 10e-6, "Value missmatch");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ws_zoom_step_set_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Call ws_zoom_step_set()
 * @param parent The parent object
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ws_zoom_step_set_n)
{
/*
   elm_init(0, 0);
   ws_zoom_step_set(NULL, 10);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ws_zoom_step_get_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Call ws_zoom_step_get()
 * @param parent The parent object
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ws_zoom_step_get_n)
{
/*
   elm_init(0, 0);
   ws_zoom_step_get(NULL);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ws_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ws
 * @step 2 Free ws
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ws_free_test_p)
{
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ws_free(ws);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ws_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Free ws
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ws_free_test_n)
{
/*
   elm_init(0, 0);
   ws_free(NULL);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ui_object_highlight_set_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Create ws
 * @step 2 Set highlight to non-existing object
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ui_object_highlight_set_n)
{
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ui_object_highlight_set(ws, NULL);
   ui_object_highlight_set(NULL, NULL);
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ui_object_highlight_move_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Try to move highlight
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ui_object_highlight_move_n)
{
/*
   elm_init(0, 0);
   ui_object_highlight_move(NULL);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ui_object_highlight_hide_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Try to hide highlight
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ui_object_highlight_hide_n)
{
/*
   elm_init(0, 0);
   ui_object_highlight_hide(NULL);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ui_ws_zoom_in_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ws
 * @step 2 Set zoom
 * @step 3 Zoom in
 *
 * @passcondition: value should be increased by 0.1
 * @}
 */
START_TEST (ui_ws_zoom_in_p1)
{
/*TODO: add correct groupspace loading*/
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ws_zoom_step_set(ws, 5);
   ck_assert_msg(ui_ws_zoom_in(ws) == EINA_TRUE, "EINA_FALSE returned");
   double diff = ws_zoom_step_get(ws) - 5.1;
   if(diff < 0) diff = -diff;
   ck_assert_msg(diff < 10e-6, "wrong value after zooming in");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ui_ws_zoom_in_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ws
 * @step 2 Set zoom to 10
 * @step 3 Zoom in
 *
 * @passcondition: value should be 10
 * @}
 */
START_TEST (ui_ws_zoom_in_p2)
{
/*TODO: add correct groupspace loading*/
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ws_zoom_step_set(ws, 10);
   ck_assert_msg(ui_ws_zoom_in(ws) == EINA_FALSE, "EINA_TRUE returned(EINA_FALSE expected)");
   double diff = 10.0 - ws_zoom_step_get(ws);
   if(diff < 0) diff = -diff;
   ck_assert_msg(diff < 10e-6, "Value out of range");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ui_ws_zoom_in_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Try to zoom in
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ui_ws_zoom_in_n)
{
/*
   elm_init(0, 0);
   ui_ws_zoom_in(NULL);
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup ui_ws_zoom_out_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ws
 * @step 2 Set zoom
 * @step 3 Zoom out
 *
 * @passcondition: value should be decreased by 0.1
 * @}
 */
START_TEST (ui_ws_zoom_out_p1)
{
/*TODO: add correct groupspace loading*/
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ws_zoom_step_set(ws, 5);
   ck_assert_msg(ui_ws_zoom_out(ws) == EINA_TRUE, "EINA_FALSE returned");
   double diff = 4.9 - ws_zoom_step_get(ws);
   if(diff < 0) diff = -diff;
   ck_assert_msg(diff < 10e-6, "wrong value after zooming in");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ui_ws_zoom_out_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ws
 * @step 2 Set zoom to 0.1
 * @step 3 Zoom out
 *
 * @passcondition: value should be 0.1
 * @}
 */
START_TEST (ui_ws_zoom_out_p2)
{
/*TODO: add correct groupspace loading*/
/*
   elm_init(0, 0);
   Evas_Object *parent, *layout;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(parent);
   Workspace* ws = ws_add(layout);
   ws_zoom_step_set(ws, 0.1);
   ck_assert_msg(ui_ws_zoom_out(ws) == EINA_FALSE, "EINA_TRUE returned(EINA_FALSE expected)");
   double diff = ws_zoom_step_get(ws) - 0.1;
   if(diff < 0) diff = -diff;
   ck_assert_msg(diff < 10e-6, "Value out of range");
   elm_shutdown();
   ws_free(ws);
*/
}
END_TEST

/**
 * @addtogroup ui_ws_zoom_out_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ws
 * @step 2 Try to zoom out
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ui_ws_zoom_out_n)
{
/*
   elm_init(0, 0);
   ui_ws_zoom_out(NULL);
   elm_shutdown();
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
   Suite *suite = suite_create("ui_workspace_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ws_add_test_p);
   tcase_add_test(tcase, ws_add_test_n1);
   tcase_add_test(tcase, ws_add_test_n2);
   tcase_add_test(tcase, ws_zoom_step_p);
   tcase_add_test(tcase, ws_zoom_step_get_n);
   tcase_add_test(tcase, ws_zoom_step_set_n);
   tcase_add_test(tcase, ws_free_test_p);
   tcase_add_test(tcase, ws_free_test_n);
   tcase_add_test(tcase, ui_object_highlight_set_n);
   tcase_add_test(tcase, ui_object_highlight_move_n);
   tcase_add_test(tcase, ui_object_highlight_hide_n);
   tcase_add_test(tcase, ui_ws_zoom_in_p1);
   tcase_add_test(tcase, ui_ws_zoom_in_p2);
   tcase_add_test(tcase, ui_ws_zoom_in_n);
   tcase_add_test(tcase, ui_ws_zoom_out_p1);
   tcase_add_test(tcase, ui_ws_zoom_out_p2);
   tcase_add_test(tcase, ui_ws_zoom_out_n);

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
   srunner_set_xml (runner, "test_uiworkspace.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
