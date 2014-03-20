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

#include "test_ui_signal_list.h"

/**
 * @addtogroup ui_state_list_test
 * @{
 * @addtogroup ui_state_list_data_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

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
EFLETE_TEST(ui_signal_list_data_set_test_p1)
{
/*
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
*/
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
EFLETE_TEST(ui_signal_list_data_set_test_p2)
{
/*
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
*/
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
EFLETE_TEST(ui_signal_list_data_set_test_p3)
{
/*
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
*/
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
EFLETE_TEST(ui_signal_list_data_set_test_p4)
{
/*
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
*/
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
EFLETE_TEST(ui_signal_list_data_set_test_n1)
{
/*
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
*/
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
EFLETE_TEST(ui_signal_list_data_set_test_n2)
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
EFLETE_TEST(ui_signal_list_data_set_test_n3)
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
EFLETE_TEST(ui_signal_list_data_set_test_n4)
{
/*
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
*/
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
EFLETE_TEST(ui_signal_list_data_set_test_n5)
{
/*
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
*/
}
END_TEST

/**
 * @addtogroup ui_state_list_data_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
