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
#include "part_dialog.h"
#define GS_GROUP_KEY "gs_group_key"

/**
 * @addtogroup part_dialog_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 window with groupspace
 * @step 3 group with any group_name set as data to groupspace
 *
 * @procedure
 * @step 1 Call part_dialog_add(window, groupspace)
 * @step 2 Check returned pointer
 *
 * @passcondition Not NULL pointer returned
 * @}
 */
START_TEST (part_dialog_add_test_p)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *par, *grspace, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   grspace = ui_groupspace_add(par);
   Style style;
   style.style_name = eina_stringshare_add("test");
   evas_object_data_set(grspace, GS_GROUP_KEY, &style);
   res = part_dialog_add(par, grspace);
   if (!res) ck_abort_msg("NULL pointer returned");
   eina_stringshare_del(group.group_name);
   evas_object_del(par);
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call part_dialog_add(NULL, NULL)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (part_dialog_add_test_n1)
{
/*
   elm_init(0,0);
   Evas_Object *res;
   res = part_dialog_add(NULL, NULL);
   if (res) ck_abort();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 window with groupspace
 * @step 3 group with group_name==NULL set as data to groupspace
 *
 * @procedure
 * @step 1 Call part_dialog_add(window, groupspace)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (part_dialog_add_test_n2)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *par, *grspace, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   grspace = ui_groupspace_add(par);
   Group group;
   group.group_name = NULL;
   evas_object_data_set(grspace, GS_GROUP_KEY, &group);
   res = part_dialog_add(par, grspace);
   if (res) ck_abort_msg("Not NULL returned");
   evas_object_del(par);
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 window with groupspace
 *
 * @procedure
 * @step 1 Call part_dialog_add(window, groupspace)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (part_dialog_add_test_n3)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *par, *grspace, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   grspace = ui_groupspace_add(par);
   res = part_dialog_add(par, grspace);
   if (res) ck_abort_msg("Not NULL returned");
   evas_object_del(par);
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 window
 *
 * @procedure
 * @step 1 Call part_dialog_add(window, NULL)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (part_dialog_add_test_n4)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *par, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   res = part_dialog_add(par, NULL);
   if (res) ck_abort_msg("Not NULL returned");
   evas_object_del(par);
   app_shutdown();
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 window with groupspace
 * @step 3 group with any group_name set as data to groupspace
 *
 * @procedure
 * @step 1 Call part_dialog_add(NULL, groupspace)
 * @step 2 Check returned pointer
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (part_dialog_add_test_n5)
{
/*
   elm_init(0,0);
   app_init();
   Evas_Object *par, *grspace, *res;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   grspace = ui_groupspace_add(par);
   Group group;
   group.group_name = eina_stringshare_add("test");
   evas_object_data_set(grspace, GS_GROUP_KEY, &group);
   res = part_dialog_add(NULL, grspace);
   if (res) ck_abort_msg("Not NULL returned");
   eina_stringshare_del(group.group_name);
   evas_object_del(par);
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
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("part_dialog_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, part_dialog_add_test_p);
   tcase_add_test(tcase, part_dialog_add_test_n1);
   tcase_add_test(tcase, part_dialog_add_test_n2);
   tcase_add_test(tcase, part_dialog_add_test_n3);
   tcase_add_test(tcase, part_dialog_add_test_n4);
   tcase_add_test(tcase, part_dialog_add_test_n5);
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
   srunner_set_xml (runner, "test_part_dialog.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
