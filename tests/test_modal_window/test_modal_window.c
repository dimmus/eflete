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
#include "modal_window.h"
#include "ui_main_window.h"

/**
 * @addtogroup mw_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call mw_add(NULL) (parameter is deprecated)
 * @step 2 Check returned pointer
 *
 * @passcondition Not NULL pointer returned
 * @}
 */
START_TEST (mw_add_test_p)
{
/*
   elm_init(0,0);
   Evas_Object *res;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   res = mw_add(NULL);
   ck_assert_msg(res != NULL, "cannot create new Modal Window");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window NOT created
 *
 * @procedure
 * @step 1 Call mw_add(NULL) (parameter is deprecated)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (mw_add_test_n)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *res;
   res = mw_add(NULL);
   ck_assert_msg(res == NULL, "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_about_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call mw_about_add(NULL) (parameter is deprecated)
 * @step 2 Check returned pointer
 *
 * @passcondition Not NULL pointer returned
 * @}
 */
START_TEST (mw_about_add_test_p)
{
/*
   elm_init(0,0);
   Evas_Object *res;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   res = mw_about_add(NULL);
   ck_assert_msg(res != NULL, "cannot create new About Window");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_about_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window NOT created
 *
 * @procedure
 * @step 1 Call mw_about_add(NULL) (parameter is deprecated)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (mw_about_add_test_n)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *res;
   res = mw_about_add(NULL);
   ck_assert_msg(res == NULL, "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_title_set
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_title_set(modal_window, "text")
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (mw_title_set_test_p)
{
/*
   elm_init(0,0);
   Evas_Object *mw;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL);
   ck_assert_msg(mw_title_set(mw, "test text"), "Can't set title");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_title_set
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call mw_title_set(NULL, "text")
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (mw_title_set_test_n1)
{
/*
   elm_init(0,0);
   app_init();
   ck_assert_msg(mw_title_set(NULL, "new_modal_window") == false, "Trying to set"
                  " title to NULL object");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_title_set
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_title_set(modal_window, NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (mw_title_set_test_n2)
{
/*
   elm_init(0,0);
   Evas_Object *mw;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL);
   ck_assert_msg(mw_title_set(mw, NULL) == false, "Trying to set NULL title");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_info_text_set(modal_window, "some text")
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (mw_info_text_set_test_p)
{
/*
   elm_init(0,0);
   Evas_Object *mw;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL);
   ck_assert_msg(mw_info_text_set(mw, "new_text_set"), "Can't set info text");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call mw_info_text_set(NULL, "some text")
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (mw_info_text_set_test_n1)
{
/*
   elm_init(0,0);
   app_init();
   ck_assert_msg(mw_info_text_set(NULL, "new_text_set") == false,
                  "Trying to set info text to NULL object");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_info_text_set(modal_window, NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (mw_info_text_set_test_n2)
{
/*
   elm_init(0,0);
   Evas_Object *mw;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL);
   ck_assert_msg(mw_info_text_set(mw, NULL) == false,
                  "Trying to set NULL info text");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_icon_set
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 * @step 4 icon created
 *
 * @procedure
 * @step 1 Call mw_icon_set(modal_window, icon)
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (mw_icon_set_test_p)
{
/*
   elm_init(0,0);
   Evas_Object *mw;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL);
   Evas_Object *ic = elm_icon_add(app->win);
   ck_assert_msg(mw_icon_set(mw, ic), "Can't set icon");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_icon_set
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_icon_set(modal_window, NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (mw_icon_set_test_n1)
{
/*
   elm_init(0,0);
   Evas_Object *mw;
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL);
   ck_assert_msg(mw_icon_set(mw, NULL) == false, "Can't set icon");
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup mw_icon_set
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 icon created
 *
 * @procedure
 * @step 1 Call mw_icon_set(NULL, icon)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (mw_icon_set_test_n2)
{
/*
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   Evas_Object *ic = elm_icon_add(app->win);
   ck_assert_msg(mw_icon_set(NULL, ic) == false, "Can't set icon");
   app_shutdown();
   elm_shutdown();
*/
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
   Suite *suite = suite_create("modal_window_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, mw_add_test_p);
   tcase_add_test(tcase, mw_add_test_n);
   tcase_add_test(tcase, mw_about_add_test_p);
   tcase_add_test(tcase, mw_about_add_test_n);
   tcase_add_test(tcase, mw_title_set_test_n1);
   tcase_add_test(tcase, mw_title_set_test_p);
   tcase_add_test(tcase, mw_title_set_test_n2);
   tcase_add_test(tcase, mw_info_text_set_test_p);
   tcase_add_test(tcase, mw_info_text_set_test_n1);
   tcase_add_test(tcase, mw_info_text_set_test_n2);
   tcase_add_test(tcase, mw_icon_set_test_p);
   tcase_add_test(tcase, mw_icon_set_test_n1);
   tcase_add_test(tcase, mw_icon_set_test_n2);
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
   srunner_set_xml (runner, "test_modal_window.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
