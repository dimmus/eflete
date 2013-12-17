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
#include "ui_groupspace.h"
#include "ui_widget_list.h"

/**
 * @addtogroup groupspace_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Parent object
 * @step 2 Add groupspace.
 *
 * @passcondition: not NULL returned
 * @}
 */
START_TEST (groupspace_add_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *gs;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gs = ui_groupspace_add(par);
   if (!gs)
      ck_abort_msg("failure(groupspace_add): cannot return pointer to groupspace Evas_Object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: NULL should be returned
 * @}
 */
START_TEST (groupspace_add_test_n)
{
   elm_init(0,0);
   Evas_Object *res = ui_groupspace_add(NULL);
   if (res)
      ck_abort_msg("failure: not NULL pointer returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_set_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to workspace
 * @step 2 Add Project object
 * @step 3 Create Group object
 * @step 4 Call set() function
 * @step 5 Call update() function
 * @step 6 Call unset() function
 * @param part The name of a part.
 *
 * @passcondition: No crash occurs
 * @}
 */
START_TEST (groupspace_set_up_unset_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);
   ui_groupspace_update(ws->groupspace);
   ui_groupspace_unset(ws->groupspace);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_set_up_unset_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call functions with NULL parameter.
 *
 * @passcondition: No crash occurs
 * @}
 */
START_TEST (groupspace_set_up_unset_test_n)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(NULL, pro, group);
   ui_groupspace_set(ws, NULL, group);
   ui_groupspace_set(ws, pro, NULL);
   ui_groupspace_update(NULL);
   ui_groupspace_unset(NULL);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_part_update_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Parent object
 * @step 2 Create Part object
 * @step 3 Call function
 *
 * @passcondition: No crash occurs
 * @}
 */
START_TEST (part_state_update_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);
   Part *part = wm_part_add(obj, "bg");
   ui_groupspace_part_state_update(ws->groupspace, part);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_part_update_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Create Parent object
 * @step 2 Create Part object
 * @step 3 Call function with NULL params
 *
 * @passcondition: No crash occurs
 * @}
 */
START_TEST (part_state_update_test_n)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);
   Part *part = wm_part_add(obj, "bg");
   ui_groupspace_part_state_update(ws->groupspace, NULL);
   ui_groupspace_part_state_update(NULL, part);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_separate_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to workspace
 * @step 2 Call function.
 *
 * @passcondition: No crash occurs
 * @}
 */
START_TEST (separate_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   ui_groupspace_separate(ws);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_separate_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL
 *
 * @passcondition: No crash occurs
 * @}
 */
START_TEST (separate_test_n)
{
   elm_init(0,0);
   ui_groupspace_separate(NULL);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_group_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Prepare groupspace
 * @step 2 Call function.
 *
 * @passcondition: not NULL object should be returned from function
 * @}
 */
START_TEST (group_get_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);
   Group *gr = ui_groupspace_group_get(ws->groupspace);
   if (!gr)
      ck_abort_msg("failure: cannot return group structure pointer");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup group_get_test
 * @{
 * @objective negative test case:
 *
 * @procedure
 * @step 1 call function with NULL parameter.
 *
 * @passcondition: NULL should be returned from function
 * @}
 */
START_TEST (group_get_test_n)
{
   elm_init(0,0);
   Group *group = ui_groupspace_group_get(NULL);
   if (group)
      ck_abort_msg("failure: not NULL pointer returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_state_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to groupspace layout
 * @step 2 Create Part Object
 * @step 2 Call function.
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (part_state_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);

   Part *part = wm_part_add(obj, "bg");

   char *stname = "new_state";
   double stvalue = 0.0;
   if (!ui_groupspace_part_state_add(ws->groupspace, part, stname, stvalue))
      ck_abort_msg("failure: cannot add state for part");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_state_test
 * @{
 * @objective negative test case:
 *
 * @procedure
 * @step 1 call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (part_state_test_n)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);

   Part *part = wm_part_add(obj, "bg");
   if ((ui_groupspace_part_state_add(ws->groupspace, part, NULL, 0)) ||
       (ui_groupspace_part_state_add(ws->groupspace, NULL, "", 0)) ||
       (ui_groupspace_part_state_add(NULL, part, "", 0)))
   {
      ck_abort_msg("failure: returned EINA_TRUE for incorrect params set");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup box_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to groupspace layout
 * @step 2 Create Part Object
 * @step 3 Call function.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (box_get_test_p)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas_Object *obj = edje_edit_object_add(evas_object_evas_get(layout));
   edje_object_file_set(obj, "./edje_build/radio.edj", "elm/radio/base/def");
   const char *grname = "def";
   const char *full_grname = "elm/radio/base/def";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./ebje_build/radio.edj";
   path = "./ebje_build";
   Project *pro = pm_open_project_edj(name, path);
   ws->groupspace = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);

   ck_assert_msg(ui_groupspace_box_get(ws->groupspace), "NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup box_get_test
 * @{
 * @objective Negaitive test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter
 *
 * @passcondition: NULL object should returned from function
 * @}
 */
START_TEST (box_get_test_n)
{
   elm_init(0,0);
   ck_assert_msg(!ui_groupspace_box_get(NULL),"Not NULL returned");
   elm_shutdown();
}
END_TEST


/* @addtogroup test_suite
 * @{
 * @objective Creating above to the test case:
 *
 * @procedure
 * @step 1 Create suite
 * @step 2 Create test case
 * @step 3 Add unit tests to the test case
 * @step 4 Add test case to the suite.
 *
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("ui_groupspace_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, groupspace_add_test_p);
   tcase_add_test(tcase, groupspace_add_test_n);
   tcase_add_test(tcase, groupspace_set_up_unset_test_p);
   tcase_add_test(tcase, groupspace_set_up_unset_test_n);
   tcase_add_test(tcase, part_state_update_test_p);
   tcase_add_test(tcase, part_state_update_test_n);
   tcase_add_test(tcase, separate_test_p);
   tcase_add_test(tcase, separate_test_n);
   tcase_add_test(tcase, group_get_test_p);
   tcase_add_test(tcase, group_get_test_n);
   tcase_add_test(tcase, part_state_test_p);
   tcase_add_test(tcase, part_state_test_n);
   tcase_add_test(tcase, box_get_test_p);
   tcase_add_test(tcase, box_get_test_n);
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
 * @step 5 Create int object for list of the failures.
 *
 * @passcondition: Print a summary of the run unit tests.
 * @}
 */
int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_groupspace.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
