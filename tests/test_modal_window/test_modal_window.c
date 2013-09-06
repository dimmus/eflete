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
#include "modal_window.h"

/**
 * @addtogroup mw_add
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object parent as Elementary Window
 * @step 2 Add object to function
 *
 * @passcondition: new Modal Window should return
 * @}
 */
START_TEST (mw_add_test1)
{
   elm_init(0,0);
   Evas_Object *par, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   res = mw_add(par);
   if(!res)
   {
      ck_abort_msg("failure: cannot create new Modal Window");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_add
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to function NULL object
 *
 * @passcondition: NULL should return
 * @}
 */
START_TEST (mw_add_test2)
{
   Evas_Object *par, *res;
   par = NULL;
   res = mw_add(par);
   if(res)
   {
      ck_abort_msg("failure: cannot create new Modal Window with NULL parent");
   }
}
END_TEST

/**
 * @addtogroup mw_title_set
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create modal window
 * @step 2 Add object to function
 *
 * @passcondition: should work witout errors
 * @}
 */
START_TEST (mw_title_set_test1)
{
   elm_init(0,0);
   Evas_Object *par, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   res = mw_add(par);
   mw_title_set(res, "new_modal_window");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_title_set
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to function NULL object
 *
 * @passcondition: NULL should return
 * @}
 */
START_TEST (mw_title_set_test2)
{
   Evas_Object *par, *res;
   par = NULL;
   res = mw_add(par);
   mw_title_set(res, "null");
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create modal window
 * @step 2 Add object to function
 *
 * @passcondition: should work witout errors
 * @}
 */
START_TEST (mw_info_text_set_test1)
{
   elm_init(0,0);
   Evas_Object *par, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   res = mw_add(par);
   mw_info_text_set(res, "new_text_set");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to function NULL object
 *
 * @passcondition: NULL should return
 * @}
 */
START_TEST (mw_info_text_set_test2)
{
   Evas_Object *par, *res;
   par = NULL;
   res = mw_add(par);
   mw_info_text_set(res, NULL);
}
END_TEST

/**
 * @addtogroup mw_icon_set
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create modal window
 * @step 2 Add object to function
 *
 * @passcondition: should work witout errors
 * @}
 */
START_TEST (mw_icon_set_test1)
{
   elm_init(0,0);
   Evas_Object *par, *res, *win;
   win = elm_win_add(NULL, "win", ELM_WIN_BASIC);
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   res = mw_add(par);
   Evas_Object *ic = elm_icon_add(win);
   elm_image_file_set(ic, "./test_modal_window/data/1.jpg", NULL);
   mw_icon_set(res, ic);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_icon_set
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to function NULL object
 *
 * @passcondition: NULL should return
 * @}
 */
START_TEST (mw_icon_set_test2)
{
   Evas_Object *par, *res, *win;
   win = elm_win_add(NULL, "win", ELM_WIN_BASIC);
   par = NULL;
   res = mw_add(par);
   Evas_Object *ic = elm_icon_add(win);
   elm_image_file_set(ic, NULL, NULL);
   mw_icon_set(res, ic);
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
   Suite *suite = suite_create("modal_window_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, mw_add_test1);
   tcase_add_test(tcase, mw_add_test2);
   tcase_add_test(tcase, mw_title_set_test1);
   tcase_add_test(tcase, mw_title_set_test2);
   tcase_add_test(tcase, mw_info_text_set_test1);
   tcase_add_test(tcase, mw_info_text_set_test2);
   tcase_add_test(tcase, mw_icon_set_test1);
   tcase_add_test(tcase, mw_icon_set_test2);
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
   srunner_set_xml (runner, "test_modal_window.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
