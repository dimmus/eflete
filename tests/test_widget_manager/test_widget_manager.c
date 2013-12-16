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
#include <Elementary.h>
#include "widget_manager.h"
#include "common_macro.h"

/**
 * @addtogroup wm_part_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Edje Edit object of the tested existing group
 * @step 4 Part structure created with rectangle object.
 *
 * @procedure
 * @step 1 Call function wm_part_free(part).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (wm_part_free_test_p1)
{
   elm_init(0,0);
   Evas_Object *obj = NULL, *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   const char *name = "elm.swallow.content";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./edj_build/radio.edj", "elm/radio/base/def");
   part = wm_part_add(obj, name);
   part->obj = evas_object_rectangle_add(e);
   ck_assert_msg(wm_part_free(part) == true, "cannot delete Part object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Edje Edit object of the tested existing group
 * @step 4 Part structure created with empty object.
 *
 * @procedure
 * @step 1 Call function wm_part_free(part).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (wm_part_free_test_p2)
{
   elm_init(0,0);
   Evas_Object *obj = NULL, *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   const char *name = "elm.swallow.content";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./edj_build/radio.edj", "elm/radio/base/def");
   part = wm_part_add(obj, name);
   ck_assert_msg(wm_part_free(part) == true, "cannot delete Part object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_free_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_free(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (wm_part_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_part_free(NULL) == false, "NULL Part object was deleted");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Edje Edit object of the tested existing group
 * @step 4 edj file contain requested part
 *
 * @procedure
 * @step 1 Call function wm_part_add(edje_edit_object, "elm.swallow.content").
 * @step 2 Check returned Part
 * @step 3 Check Part's type.
 *
 * @passcondition not NULL pointer returned
 * @}
 */
START_TEST (wm_part_add_test_p)
{
   elm_init(0,0);
   Evas_Object *obj = NULL, *win = NULL;
   Part *part = NULL;
   Evas *e = NULL;
   const char *name = "elm.swallow.content";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./edj_build/radio.edj", "elm/radio/base/def");
   part = wm_part_add(obj, name);
   ck_assert_msg(part != NULL, "Part object wasn't created.");
   ck_assert_msg(part->type == EDJE_PART_TYPE_SWALLOW, "Part type is wrong.");

   wm_part_free(part);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Edje Edit object of the tested group
 * @step 4 edj file doesn't contain requested part
 *
 * @procedure
 * @step 1 Call function wm_part_add(edje_edit_object, "eflete.part").
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_part_add_test_n1)
{
   elm_init(0,0);
   Evas_Object *obj = NULL, *win = NULL;
   Evas *e = NULL;
   const char *name = "eflete.part";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./edj_build/radio.edj", "elm/radio/base/def");
   ck_assert_msg(wm_part_add(obj, name) == NULL, "Unexisting Part was successfully added");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Edje Edit object of the tested group
 *
 * @procedure
 * @step 1 Call function wm_part_add(edje_edit_object, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_part_add_test_n2)
{
   elm_init(0,0);
   Evas_Object *obj = NULL, *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./edj_build/radio.edj", "elm/radio/base/def");
   ck_assert_msg(wm_part_add(obj, NULL) == NULL, "Part was added successfuly without part's name");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_add(NULL, "elm.swallow.content").
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_part_add_test_n3)
{
   elm_init(0,0);
   const char *part = "elm.swallow.content";
   ck_assert_msg(wm_part_add(NULL, part) == NULL, "Part was added successfuly without edje edit object.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_add(NULL, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (wm_part_add_test_n4)
{
   elm_init(0,0);
   ck_assert_msg(wm_part_add(NULL, NULL) == NULL, "Part was added successfuly with NULL parametres");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_group_add("testclass", "eflete/testclass/testgroup").
 * @step 2 Check returned Group.
 * @step 3 Check Group's full name.
 *
 * @passcondition not NULL pointer returned and full_group_name is set correctly.
 * @}
 */
START_TEST (wm_group_add_test_p)
{
   elm_init(0,0);
   const char *group_name = "testclass";
   const char *full_group_name = "eflete/testclass/testgroup";
   Group *group = NULL;

   group = wm_group_add(group_name, full_group_name);
   ck_assert_msg(group != NULL, "cannot create new Group object");
   ck_assert_str_eq(group->full_group_name, full_group_name);

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_group_add(NULL, "eflete/testclass/testgroup").
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_add_test_n1)
{
   elm_init(0,0);
   const char *full_group_name = "eflete/testclass/testgroup";
   ck_assert_msg(wm_group_add(NULL, full_group_name) == NULL, "Group object was created with NULL parameter.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_group_add("testclass", NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_add_test_n2)
{
   elm_init(0,0);
   const char *group_name = "testclass";
   ck_assert_msg(wm_group_add(group_name, NULL) == NULL, "Group object was created with NULL parameter.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_group_add(NULL, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_add_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_group_add(NULL, NULL) == NULL, "Group object was created with NULL parameters.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Group structure.
 *
 * @procedure
 * @step 1 Call function wm_group_free(group).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (wm_group_free_test_p)
{
   elm_init(0,0);
   Group *group = NULL;
   const char *group_name = "defaul";
   const char *full_group_name = "elm/check/base/defaul";
   group = wm_group_add(group_name, full_group_name);
   ck_assert_msg(wm_group_free(group) == true, "cannot delete Group object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_free_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_group_free(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (wm_group_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_group_free(NULL) == false, "Deleting NULL as Group object.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Group object, filled with data and containing Edje Edit object.
 * @step 4 Tested group contain few programs with different signals
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(group).
 * @step 2 Check returned Signal List.
 * @step 3 Check Signal List's information.
 *
 * @passcondition Eina_List of signals returned
 * @}
 */
START_TEST (wm_program_signals_list_get_test_p1)
{
   elm_init(0,0);
   Evas_Object *win;
   Evas *e;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";
   Eina_List *sig_list;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   sig_list = wm_program_signals_list_get(group);
   ck_assert_msg(sig_list != NULL, "Signals list isn't exist.");
   ck_assert_str_eq((char *)sig_list->next->data, "elm,state,radio,on");

   wm_program_signals_list_free(sig_list);
   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Empty Group object.
 * @step 4 Tested group doesn't contain any programs and signals
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(group).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_program_signals_list_get_test_p2)
{
   elm_init(0,0);
   Evas_Object *win;
   Evas *e;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/notbase/test";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   ck_assert_msg(wm_program_signals_list_get(group) == NULL, "Group returned data that should don't exist.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Empty Group object.
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(group).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_program_signals_list_get_test_n1)
{
   elm_init(0,0);
   Group *group = NULL;
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/test";

   group = wm_group_add(group_name, full_group_name);
   ck_assert_msg(wm_program_signals_list_get(group) == NULL, "Group returned data that should don't exist.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_program_signals_list_get_test_n2)
{
   elm_init(0,0);
   ck_assert_msg(wm_program_signals_list_get(NULL) == NULL, "Data returned from null pointer group.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Group object, filled with data and containing Edje Edit object.
 * @step 4 Tested group contain programs and signals
 * @step 5 Signal List.
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_free(signal_list).
 * @step 2 Check returned value.
 *
 * @passcondition true returned.
 * @}
 */
START_TEST (wm_program_signals_list_free_test_p)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Group *group = NULL;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";
   Eina_List *sig_list = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   group = wm_group_add(group_name, full_group_name);
   wm_group_data_load(group, e, edj);
   sig_list = wm_program_signals_list_get(group);
   ck_assert_msg(wm_program_signals_list_free(sig_list) == true, "Cannot free signal list.");

   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_free_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_free(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned.
 * @}
 */
START_TEST (wm_program_signals_list_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_program_signals_list_free(NULL) == false, "NULL signal list was deleted.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_style_add(style_name, list_of_groups).
 * @step 2 Check returned Style.
 * @step 3 Check that Style contains Group structure list.
 *
 * @passcondition not NULL pointer returned and Style contain right information.
 * @}
 */
START_TEST (wm_style_add_test_p)
{
   elm_init(0, 0);
   Eina_List *groups = NULL;
   Style *style = NULL;
   Group *group = NULL;
   const char* style_name = "test";
   const char* group_name = "notbase";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   style = wm_style_add(style_name, groups);
   ck_assert_msg(style != NULL, "Style is not created.");
   group = EINA_INLIST_CONTAINER_GET(style->groups->next, Group);
   ck_assert_str_eq(group->group_name, group_name);

   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_style_add(NULL, list_of_groups).
 * @step 2 Check returned value.
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (wm_style_add_test_n1)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   ck_assert_msg(wm_style_add(NULL, groups) == NULL, "Style structure was created.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_add("test", NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (wm_style_add_test_n2)
{
   elm_init(0,0);
   const char* style_name = "test";
   ck_assert_msg(wm_style_add(style_name, NULL) == NULL, "Style structure was created.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_add(NULL, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (wm_style_add_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_style_add(NULL, NULL) == NULL, "Style structure was created.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Style structure.
 *
 * @procedure
 * @step 1 Call function wm_style_free(style).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (wm_style_free_test_p)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   Style *style = NULL;
   const char* style_name = "test";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   style = wm_style_add(style_name, groups);

   ck_assert_msg(wm_style_free(style) == true, "cannot delete Style structure.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_free(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (wm_style_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_style_free(NULL) == false, "NULL Style structure was deleted.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_add("radio", list_of_groups).
 * @step 2 Check returned Widget.
 * @step 3 Check Widget's content.
 *
 * @passcondition not NULL pointer returned and Widget contain right information.
 * @}
 */
START_TEST (wm_widget_add_test_p)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   Widget *widget = NULL;
   Style *style = NULL;
   const char *widget_name = "radio";
   const char *style_name = "test";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   widget = wm_widget_add(widget_name, groups);
   ck_assert_msg(widget != NULL, "Widget is not created.");
   style = EINA_INLIST_CONTAINER_GET(widget->styles, Style);
   ck_assert_str_eq(style->style_name, style_name);

   wm_widget_free(widget);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_add(NULL, list_of_groups).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_widget_add_test_n1)
{
   elm_init(0,0);
   Eina_List *groups = NULL;

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   ck_assert_msg(wm_widget_add(NULL, groups) == NULL, "created new widget with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_add("radio", NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_widget_add_test_n2)
{
   elm_init(0,0);
   const char *widget_name = "radio";
   ck_assert_msg(wm_widget_add(widget_name, NULL) == NULL, "created new widget with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_widget_add(NULL, NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_widget_add_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_add(NULL, NULL) == NULL, "created new widget with NULL parameters");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_free(widget).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (wm_widget_free_test_p)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   Widget *widget = NULL;
   const char *widget_name = "radio";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   widget = wm_widget_add(widget_name, groups);
   ck_assert_msg(wm_widget_free(widget) == true, "cannot delete Widget.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_free_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_free(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (wm_widget_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_free(NULL) == false, "NULL parameter was deleted as Widget.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_new_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 1 Path to edj file.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_new("./edj_build/radio.edj").
 * @step 2 Check returned widget list.
 * @step 3 Check widget list's content.
 *
 * @passcondition not NULL pointer returned and List contain right information.
 * @}
 */
START_TEST (wm_widget_list_new_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   const char *widget_name = "radio";
   Eina_Inlist *widget_list = NULL;
   Widget *widget = NULL;

   widget_list = wm_widget_list_new(file);
   ck_assert_msg(widget_list != NULL, "Widget List is not created.");
   widget = EINA_INLIST_CONTAINER_GET(widget_list, Widget);
   ck_assert_str_eq(widget->widget_name, widget_name);

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_new_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_widget_list_new(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_widget_list_new_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_list_new(NULL) == NULL, "new widget list was created");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_free_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Widget list loaded from a file.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_free(widget_list).
 * @step 2 Check returned value.
 *
 * @passcondition true returned
 * @}
 */
START_TEST (wm_widget_list_free_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Eina_Inlist *widget_list = NULL;

   widget_list = wm_widget_list_new(file);
   ck_assert_msg(wm_widget_list_free(widget_list) == true, "widget list wasn't deleted.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_free_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_widget_list_free(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition false returned
 * @}
 */
START_TEST (wm_widget_list_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_list_free(NULL) == false, "NULL parameter was deleted as widget list.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Widget list filled with data (Widgets, Styles, Groups etc).
 * @step 4 Group existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_group_object_find(widget_list, "elm/radio/base/test").
 * @step 2 Check returned object.
 *
 * @passcondition not NULL pointer returned
 * @}
 */
START_TEST (wm_group_object_find_test_p1)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   const char *full_group_name = "elm/radio/base/test";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   ck_assert_msg(wm_group_object_find(widget_list, full_group_name) != NULL, "Group wasn't found");

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 widget list filled with data (widgets, styles, groups etc).
 * @step 4 Group not existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_group_object_find(widget_list, "elm/widget/base/test").
 * @step 2 Check returned object.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_object_find_test_p2)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   const char *full_group_name = "elm/widget/base/test";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   ck_assert_msg(wm_group_object_find(widget_list, full_group_name) == NULL, "Unexisted Group was found");

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Group existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_group_object_find(NULL, "elm/widget/base/test").
 * @step 2 Check returned object.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_object_find_test_n1)
{
   elm_init(0,0);
   const char *full_group_name = "elm/widget/base/test";
   ck_assert_msg(wm_group_object_find(NULL, full_group_name) == NULL, "Group was found from NULL list");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Widget list filled with data (Widgets, Styles, Groups etc).
 *
 * @procedure
 * @step 1 Call function wm_group_object_find(widget_list, NULL).
 * @step 2 Check returned object.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_object_find_test_n2)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   ck_assert_msg(wm_group_object_find(widget_list, NULL) == NULL, "NULL was found");

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_group_object_find_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_group_object_find(NULL, NULL).
 * @step 2 Check returned object.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_group_object_find_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_group_object_find(NULL, NULL) == NULL, "Group was found");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Edje Part Type for TEXT prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(EDJE_PART_TYPE_TEXT).
 * @step 2 Check returned object.
 *
 * @passcondition "TEXT" string returned
 * @}
 */
START_TEST (wm_part_type_get_test_p1)
{
   elm_init(0,0);
   Edje_Part_Type type = EDJE_PART_TYPE_TEXT;
   ck_assert_str_eq(wm_part_type_get(type), "TEXT");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Edje Part Type for NONE prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(EDJE_PART_TYPE_NONE).
 * @step 2 Check returned object.
 *
 * @passcondition "NONE" string returned
 * @}
 */
START_TEST (wm_part_type_get_test_p2)
{
   elm_init(0,0);
   Edje_Part_Type type = EDJE_PART_TYPE_NONE;
   ck_assert_str_eq(wm_part_type_get(type), "NONE");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Unexisting Edje Part Type prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(55).
 * @step 2 Check returned object.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_part_type_get_test_n1)
{
   elm_init(0,0);
   Edje_Part_Type type = 55;
   ck_assert_msg(!wm_part_type_get(type), "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 Unexisting Edje Part Type prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(-13).
 * @step 2 Check returned object.
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (wm_part_type_get_test_n2)
{
   elm_init(0,0);
   Edje_Part_Type type = -13;
   ck_assert_msg(!wm_part_type_get(type), "Not NULL returned");
   elm_shutdown();
}
END_TEST

/* @addtogroup test_suite
 * @{
 * @objective Creating above to the test case
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
   tcase_add_test(tcase, wm_part_add_test_p);
   tcase_add_test(tcase, wm_part_add_test_n1);
   tcase_add_test(tcase, wm_part_add_test_n2);
   tcase_add_test(tcase, wm_part_add_test_n3);
   tcase_add_test(tcase, wm_part_add_test_n4);
   tcase_add_test(tcase, wm_part_free_test_p1);
   tcase_add_test(tcase, wm_part_free_test_p2);
   tcase_add_test(tcase, wm_part_free_test_n);
   tcase_add_test(tcase, wm_group_add_test_p);
   tcase_add_test(tcase, wm_group_add_test_n1);
   tcase_add_test(tcase, wm_group_add_test_n2);
   tcase_add_test(tcase, wm_group_add_test_n3);
   tcase_add_test(tcase, wm_group_free_test_p);
   tcase_add_test(tcase, wm_group_free_test_n);
   tcase_add_test(tcase, wm_program_signals_list_get_test_p1);
   tcase_add_test(tcase, wm_program_signals_list_get_test_p2);
   tcase_add_test(tcase, wm_program_signals_list_get_test_n1);
   tcase_add_test(tcase, wm_program_signals_list_get_test_n2);
   tcase_add_test(tcase, wm_program_signals_list_free_test_p);
   tcase_add_test(tcase, wm_program_signals_list_free_test_n);
   tcase_add_test(tcase, wm_style_add_test_p);
   tcase_add_test(tcase, wm_style_add_test_n1);
   tcase_add_test(tcase, wm_style_add_test_n2);
   tcase_add_test(tcase, wm_style_add_test_n3);
   tcase_add_test(tcase, wm_style_free_test_p);
   tcase_add_test(tcase, wm_style_free_test_n);
   tcase_add_test(tcase, wm_widget_add_test_p);
   tcase_add_test(tcase, wm_widget_add_test_n1);
   tcase_add_test(tcase, wm_widget_add_test_n2);
   tcase_add_test(tcase, wm_widget_add_test_n3);
   tcase_add_test(tcase, wm_widget_free_test_p);
   tcase_add_test(tcase, wm_widget_free_test_n);
   tcase_add_test(tcase, wm_widget_list_new_test_p);
   tcase_add_test(tcase, wm_widget_list_new_test_n);
   tcase_add_test(tcase, wm_widget_list_free_test_p);
   tcase_add_test(tcase, wm_widget_list_free_test_n);
   tcase_add_test(tcase, wm_group_object_find_test_p1);
   tcase_add_test(tcase, wm_group_object_find_test_p2);
   tcase_add_test(tcase, wm_group_object_find_test_n1);
   tcase_add_test(tcase, wm_group_object_find_test_n2);
   tcase_add_test(tcase, wm_group_object_find_test_n3);
   tcase_add_test(tcase, wm_part_type_get_test_p1);
   tcase_add_test(tcase, wm_part_type_get_test_p2);
   tcase_add_test(tcase, wm_part_type_get_test_n1);
   tcase_add_test(tcase, wm_part_type_get_test_n2);
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
 * @passcondition Print a summary of the run unit tests.
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
