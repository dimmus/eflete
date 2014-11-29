/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_part_current_state_set
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * <tr>
 * <td>wm_part_current_state_set</td>
 * <td>wm_part_current_state_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 edj file contain requested part
 * @step 6 Create new Part
 *
 * @procedure
 * @step 1 Call function wm_part_current_state_set(part, "visible 0.0");.
 * @step 2 Check that function was end successfully
 * @step 3 Check Part's state
 * @step 4 Check Part's state value
 * </td>
 * <td>Part *part, "visible 0.0"</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_current_state_set_test_p1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/wm_part_current_state_set.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);

   ck_assert_msg(wm_part_current_state_set(part, "visible 0.0"), "Error!");
   ck_assert_str_eq(part->curr_state, "visible");
   ck_assert_msg(part->curr_state_value == 0.0, "Wrong current state value.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * <tr>
 * <td>wm_part_current_state_set</td>
 * <td>wm_part_current_state_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Create Part but don't add it to Style structure (without using wm_part_add).
 *
 * @procedure
 * @step 1 Call function wm_part_current_state_set(part, "visible 0.0");.
 * @step 2 Check Part's state
 * @step 3 Check Part's state value
 * </td>
 * <td>Part *part, "visible 0.0"</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_current_state_set_test_p2)
{
   elm_init(0,0);
   Part *part = NULL;

   part = (Part *)mem_malloc(sizeof(Part));
   part->name = eina_stringshare_add("elm.double.eflete");
   part->curr_state = eina_stringshare_add("default");
   ck_assert_msg(wm_part_current_state_set(part, "visible 0.0"), "Error!");
   ck_assert_str_eq(part->curr_state, "visible");
   ck_assert_msg(part->curr_state_value == 0.0, "Wrong current state value.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * <tr>
 * <td>wm_part_current_state_set</td>
 * <td>wm_part_current_state_set_test_p3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 edj file contain requested part
 * @step 6 Create new Part
 *
 * @procedure
 * @step 1 Call function wm_part_current_state_set(part, "visible fhtagn");.
 * @step 2 Check that function returned false.
 * @step 3 Check Part's state
 * @step 4 Check Part's state value
 * </td>
 * <td>Part *part, "notvisible fhtagn"</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_current_state_set_test_p3)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/wm_part_current_state_set.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);

   ck_assert_msg(wm_part_current_state_set(part, "notvisible fhtagn"), "Error. State was changed!");
   ck_assert_str_eq(part->curr_state, "notvisible");
   ck_assert_msg(part->curr_state_value == 0.0, "State was changed.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * <tr>
 * <td>wm_part_current_state_set</td>
 * <td>wm_part_current_state_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 edj file contain requested part
 * @step 6 Create new Part
 *
 * @procedure
 * @step 1 Call function wm_part_current_state_set(part, "visible fhtagn");.
 * @step 2 Check that function returned false.
 * @step 3 Check Part's state
 * @step 4 Check Part's state value
 * </td>
 * <td>Part *part, NULL</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_current_state_set_test_n1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/wm_part_current_state_set.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);

   ck_assert_msg(!wm_part_current_state_set(part, NULL), "Error!");
   ck_assert_str_ne(part->curr_state, "visible");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * <tr>
 * <td>wm_part_current_state_set</td>
 * <td>wm_part_current_state_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 edj file contain requested part
 * @step 6 Create new Part
 *
 * @procedure
 * @step 1 Call function wm_part_current_state_set(part, "");.
 * @step 2 Check that function returned false.
 * @step 3 Check Part's state
 * @step 4 Check Part's state value
 * </td>
 * <td>Part *part, NULL</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_current_state_set_test_n2)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/wm_part_current_state_set.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);

   ck_assert_msg(!wm_part_current_state_set(part, ""), "Error. State was changed!");
   ck_assert_str_ne(part->curr_state, "notvisible");
   ck_assert_msg(part->curr_state_value == 0.0, "State was changed.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * <tr>
 * <td>wm_part_current_state_set</td>
 * <td>wm_part_current_state_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_current_state_set(NULL, "visible 0.0");.
 * @step 2 Check that function was end successfully
 * </td>
 * <td>NULL, "visible 0.0"</td>
 * <td>false returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_current_state_set_test_n3)
{
   elm_init(0,0);

   ck_assert_msg(!wm_part_current_state_set(NULL, "visible 0.0"), "Error!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_current_state_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
