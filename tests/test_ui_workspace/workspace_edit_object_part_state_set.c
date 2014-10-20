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

#include "test_ui_workspace.h"

/**
 * @addtogroup ui_workspace_test
 * @{
 * @addtogroup workspace_edit_object_part_state_set
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_part_state_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_set</td>
 * <td>workspace_edit_object_part_state_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 Create part object with many states. Non default state set.
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 select part state with workspace_edit_object_part_state_set
 * @step 2 check returned value from function
 * @step 3 check that current state name in edje edit object is equal to setted state
 * @step 4 check that current state value in edje edit object is equal to setted state
 * </td>
 * <td>(Evas_Object *) workspace, (Part *) part</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_set_test_p)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Part *part = NULL;
   Evas *e = NULL;
   const char *get_state_name = NULL;
   double get_state_value = -1;


   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/workspace_edit_object_part_state_set.edj",
                        style->full_group_name);
   part = wm_part_add(style, "elm.text");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_set.edj");
   part->curr_state = eina_stringshare_add("visible");
   part->curr_state_value = 0.0;

   res = workspace_edit_object_part_state_set(workspace, part);
   ck_assert_msg(res == EINA_TRUE, "Failed to set state for part loaded into workspace");
   get_state_name = edje_edit_part_selected_state_get(style->obj, part->name,
                                                      &get_state_value);
   ck_assert_msg(strcmp(get_state_name, "visible") == 0,
                 "Setted state name is not equal to name in edje edit object");
   ck_assert_msg(get_state_value == 0.0,
                 "Setted state value is not equal to value in edje edit object");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_set</td>
 * <td>workspace_edit_object_part_state_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create style object
 * @step 5 Create part object with many states. Non default state set.
 *
 * @procedure
 * @step 1 select part state with workspace_edit_object_part_state_set
 * @step 2 check returned value from function
 * </td>
 * <td>NULL, (Part *) part</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_set_test_n)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent;
   Style *style = NULL;
   Part *part = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/workspace_edit_object_part_state_set.edj",
                        style->full_group_name);
   part = wm_part_add(style, "elm.text");
   part->curr_state = eina_stringshare_add("visible");
   part->curr_state_value = 0.0;

   res = workspace_edit_object_part_state_set(NULL, part);
   ck_assert_msg(res == EINA_FALSE, "Set state for part with NULL object workspace");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_set</td>
 * <td>workspace_edit_object_part_state_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 set edit object into workspace
 *
 * @procedure
 * @step 1 select part state with workspace_edit_object_part_state_set
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, NULL</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_set_test_n1)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_set.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_set.edj");

   res = workspace_edit_object_part_state_set(workspace, NULL);
   ck_assert_msg(res == EINA_FALSE, "Set state for NULL pointer of part");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_set</td>
 * <td>workspace_edit_object_part_state_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 Create part object with many states. Non exist state as selected set.
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 select part state with workspace_edit_object_part_state_set
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (Part *) part</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_set_test_n2)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Part *part = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/workspace_edit_object_part_state_set.edj",
                        style->full_group_name);
   part = wm_part_add(style, "elm.text");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_set.edj");
   part->curr_state = eina_stringshare_add("non_exist");
   part->curr_state_value = 2.0;

   res = workspace_edit_object_part_state_set(workspace, part);
   ck_assert_msg(res == EINA_FALSE, "Set non exist state for part");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
