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
#include <Elementary.h>
#include "widget_manager.h"

/**
 * @addtogroup wm_part_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Part object
 * @step 2 Add object to function.
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (wm_part_free_test1)
{
   elm_init(0,0);
   Evas_Object *obj, *win;
   elm_theme_extension_add(NULL, "./test_widget_manager/data/check.edj");
   win = elm_win_add(NULL, "check", ELM_WIN_BASIC);
   obj = elm_check_add(win);
   const char *name = "defaul";
   Part *part = wm_part_add(obj, name);
   fail_unless(wm_part_free(part) == EINA_TRUE, "failure: cannot delete Part object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_part_free_test2)
{
   elm_init(0,0);
   fail_unless(wm_part_free(NULL) == EINA_FALSE, "failure: delete NULL Part object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to function
 * @param obj The edje edit object with a 'group' data
 * @param part The name of a part.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_part_add_test1)
{
   elm_init(0,0);
   Evas_Object *obj, *win;
   elm_theme_extension_add(NULL, "./test_widget_manager/data/check.edj");
   win = elm_win_add(NULL, "check", ELM_WIN_BASIC);
   obj = elm_check_add(win);
   const char *part = "defaul";
   Part* pr = wm_part_add(obj, part);
   if (!pr)
   {
      ck_abort_msg("failure: cannot add new Part object");
   }
   wm_part_free(pr);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Calling NULL parameters to functions.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_part_add_test2)
{
   elm_init(0,0);
   fail_unless(wm_part_add(NULL, NULL) == EINA_FALSE, "failure: cannot add new Part object object with NULL parameters");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to function
 * @param obj The edje edit object with a 'group' data
 * @param name The name of a program.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_group_add_test1)
{
   elm_init(0,0);
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group* group = wm_group_add(grname, full_grname);
   if (!group)
   {
      ck_abort_msg("failure: cannot creating new Group object");
   }
   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Calling NULL parameters to function.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_group_add_test2)
{
   elm_init(0,0);
   fail_unless(wm_group_add(NULL, NULL) == EINA_FALSE, "failure: cannot creating Group object with NULL parameters");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Group object
 * @step 2 Add object to function.
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (wm_group_free_test1)
{
   elm_init(0,0); 
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *gr = wm_group_add(grname, full_grname);
   fail_unless(wm_group_free(gr) == EINA_TRUE, "failure: cannot delete Group object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_group_free_test2)
{
   elm_init(0,0);
   fail_unless(wm_group_free(NULL) == EINA_FALSE, "failure: delete NULL Program object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object
 * @step 2 Create Group object
 * @step 2 Add object to function.
 *
 * @passcondition: list of signal should returned from function
 * @}
 */
START_TEST (wm_program_signals_list_get_test1)
{
   elm_init(0,0);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_widget_manager/data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   if (!wm_program_signals_list_get(group))
   {
      ck_abort_msg("failure: cannot create list of signals");
   }
   evas_object_del(obj);
   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_program_signals_list_get_test2)
{
   elm_init(0,0);
   const char *grname = "";
   const char *full_grname = "";
   Group *group = wm_group_add(grname, full_grname);
   fail_unless(wm_program_signals_list_get(group) == EINA_FALSE, "failure: return list of signals of NULL Group object");
   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Eina_List list object
 * @step 2 Add list object to function.
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (wm_program_signals_list_free_test1)
{
   elm_init(0,0);
   Eina_List *list = calloc(1, sizeof(Eina_List));
   if (wm_program_signals_list_free(list) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot delete list of signals");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_program_signals_list_free_test2)
{
   elm_init(0,0);
   Eina_List *list = NULL;
   fail_unless(wm_program_signals_list_free(list) == EINA_FALSE, "failure: cannot delete list of signals with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Group object
 * @step 2 Create Eina_List object of groups
 * @step 2 Add parameters to function
 * @param style The name of a style
 * @param list The list of groups in a current style.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_style_add_test1)
{
   elm_init(0, 0);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_widget_manager/data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   Eina_List *list = wm_program_signals_list_get(group);
   const char *style = "defaul";
   Style* st = wm_style_add(style, list);
   if (!st)
   {
      ck_abort_msg("failure: cannot create new style");
   }
   wm_group_free(group);
   wm_style_free(st);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_style_add_test2)
{
   elm_init(0,0);
   fail_unless(wm_style_add(NULL, NULL) == EINA_FALSE, "failure: cannot create new style with NULL parameters");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Style object
 * @step 2 Add object to function
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (wm_style_free_test1)
{
   elm_init(0,0);
   Style *style = calloc(1,sizeof(Style));
   if (wm_style_free(style) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot delete Style object");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_style_free_test2)
{
   elm_init(0,0);
   fail_unless(wm_style_free(NULL) == EINA_FALSE, "failure: cannot delete Style object with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create a parameters
 * @step 2 Create Group object
 * @step 3 Create Eina_List object
 * @step 4 Add Eina_List object to wm_widget_add() function
 * @param widget A name of a widget
 * @param groups A list of 'group' styles in a current widget
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_widget_add_test1)
{
   elm_init(0,0);
   const char *widget = "check";
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *gr = wm_group_add(grname, full_grname);
   Eina_List *groups = wm_program_signals_list_get(gr);
   Widget* wd = wm_widget_add(widget, groups);
   if (!wd)
   {
      ck_abort_msg("failure: cannot create new Widget object");
   }
   wm_group_free(gr);
   wm_widget_free(wd);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_widget_add_test2)
{
   elm_init(0,0);
   fail_unless(wm_widget_add(NULL, NULL) == EINA_FALSE, "failure: cannot create new widget with NULL parameters");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Widget object
 * @step 2 Add object to function
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (wm_widget_free_test1)
{
   elm_init(0,0);
   const char *wid = "check";
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *gr = wm_group_add(grname, full_grname);
   Eina_List *groups = wm_program_signals_list_get(gr);
   Widget *widget = wm_widget_add(wid, groups);
   if (wm_widget_free(widget) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot delete Widget object");
   }
   wm_group_free(gr);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_widget_free_test2)
{
   elm_init(0,0);
   fail_unless(wm_widget_free(NULL) == EINA_FALSE, "failure: cannot delete Widget object with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_new_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameter
 * @step 2 Add parameter to function
 * @param file A edj-file to load data of widget styles.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_widget_list_new_test1)
{
   elm_init(0,0);
   const char *file = "./test_widget_manager/data/check.edj";
   if (wm_widget_list_new(file) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot create list of Widget object");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_new_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_widget_list_new_test2)
{
   elm_init(0,0);
   fail_unless(wm_widget_list_new(NULL) == EINA_FALSE, "failure: cannot create new widget list with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Eina_Inlist object
 * @step 2 Add object to function.
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (wm_widget_list_free_test1)
{
   elm_init(0,0);
   const char *file = "./test_widget_manager/data/check.edj";
   Eina_Inlist *wl = wm_widget_list_new(file);
   if (wm_widget_list_free(wl) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot delete list of widgets");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_widget_list_free_test2)
{
   elm_init(0,0);
   fail_unless(wm_widget_list_free(NULL) == EINA_FALSE, "failure: cannot delete list of widgets with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameter to function
 * @param w_list The Eina_Inlist list that contain widgets structures
 * @param gr_name  The string full name of group.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_group_object_find_test1)
{
   elm_init(0,0);
   const char *file = "./test_widget_manager/data/check.edj";
   Eina_Inlist *w_list =  wm_widget_list_new(file);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_widget_manager/data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   if (wm_group_object_find(w_list, full_grname) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot return Group object");
   }
   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_group_object_find_test2)
{
   elm_init(0,0);
   if (wm_group_object_find(NULL, NULL))
   {
      ck_abort_msg("failure: cannot return Group object with NULL parameter");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Edje_Part_type object
 * @step 2 Add object to function
 * @param type The part type id.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (wm_part_type_get_test1)
{
   elm_init(0,0);
   Edje_Part_Type type = 2;
   ck_assert_str_eq(wm_part_type_get(type), "TEXT");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (wm_part_type_get_test2)
{
   elm_init(0,0);
   Edje_Part_Type type = EINA_FALSE;
   const char *res = "NONE";
   if (wm_part_type_get(type) != res)
   {
      ck_abort_msg("failure: cannot return name type of part with NULL parameter");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Create uncorect Edje_Part_Type object
 * @step 2 Add object to function
 *
 * @passcondition: NULL should returned from function
 * @}
 */
START_TEST (wm_part_type_get_test3)
{
   elm_init(0,0);
   Edje_Part_Type type = 55;
   ck_assert_msg(!wm_part_type_get(type),"Not NULL returned");
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
   Suite *suite = suite_create("widget_manager_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, wm_part_add_test1);
   tcase_add_test(tcase, wm_part_add_test2);
   tcase_add_test(tcase, wm_part_free_test1);
   tcase_add_test(tcase, wm_part_free_test2);
   tcase_add_test(tcase, wm_group_add_test1);
   tcase_add_test(tcase, wm_group_add_test2);
   tcase_add_test(tcase, wm_group_free_test1);
   tcase_add_test(tcase, wm_group_free_test2);
   tcase_add_test(tcase, wm_program_signals_list_get_test1);
   tcase_add_test(tcase, wm_program_signals_list_get_test2);
   tcase_add_test(tcase, wm_program_signals_list_free_test1);
   tcase_add_test(tcase, wm_program_signals_list_free_test2);
   tcase_add_test(tcase, wm_style_add_test1);
   tcase_add_test(tcase, wm_style_add_test2);
   tcase_add_test(tcase, wm_style_free_test1);
   tcase_add_test(tcase, wm_style_free_test2);
   tcase_add_test(tcase, wm_widget_add_test1);
   tcase_add_test(tcase, wm_widget_add_test2);
   tcase_add_test(tcase, wm_widget_free_test1);
   tcase_add_test(tcase, wm_widget_free_test2);
   tcase_add_test(tcase, wm_widget_list_new_test1);
   tcase_add_test(tcase, wm_widget_list_new_test2);
   tcase_add_test(tcase, wm_widget_list_free_test1);
   tcase_add_test(tcase, wm_widget_list_free_test2);
   tcase_add_test(tcase, wm_group_object_find_test1);
   tcase_add_test(tcase, wm_group_object_find_test2);
   tcase_add_test(tcase, wm_part_type_get_test1);
   tcase_add_test(tcase, wm_part_type_get_test2);
   tcase_add_test(tcase, wm_part_type_get_test3);
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
   srunner_set_xml (runner, "test_wmanager.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
