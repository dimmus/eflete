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

#include "test_ui_workspace.h"

/**
 * @addgroup ui_workspace_test
 * @{
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 *
 * @procedure
 * @step 1 create part state with workspace_edit_object_part_state_add
 * @step 2 check returned value from function
 * @step 3 check new state name and value are exist in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (char *) "test", (double) 1.0</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_p)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_edit_object_part_state_add(workspace, "bg", "test", 1.0);
   ck_assert_msg(res == EINA_TRUE, "Failed add state for part, loaded into workspace");
   res = edje_edit_state_exist(style->obj, "bg", "test", 1.0);
   ck_assert_msg(res == EINA_TRUE, "New state doesn't exist in edje edit object");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 *
 * @procedure
 * @step 1 Add new state with workspace_edit_object_part_state_add,
 *         with name that exist, but with different value. (default 0.5)
 * @step 2 check returned value from function
 * @step 3 check new state name and value are exist in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (char *) "default", (double) 0.5</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_p1)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_edit_object_part_state_add(workspace, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_TRUE, "Failed add state for part, loaded into workspace");
   res = edje_edit_state_exist(style->obj, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_TRUE, "New state doesn't exist in edje edit object");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 *
 * @procedure
 * @step 1 Add new state with workspace_edit_object_part_state_add, for non exist part
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "non_exist", (char *) "test", (double) 1.0</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_n)
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
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_edit_object_part_state_add(workspace, "non_exist", "test", 1.0);
   ck_assert_msg(res == EINA_FALSE, "Add state for non exist part.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 *
 * @procedure
 * @step 1 Add already exist state with workspace_edit_object_part_state_add
 *         for part.
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (char *) "default", (double) 0.0</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_n1)
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
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_edit_object_part_state_add(workspace, "bg", "default", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Add already exist state for part.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Add new state with function workspace_edit_object_part_state_add.
 * @step 2 check returned value from function
 * </td>
 * <td>NULL, (char *) "bg", (char *) "new", (double) 0.0</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_n2)
{
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);

   res = workspace_edit_object_part_state_add(NULL, "bg", "new", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Add state with workspace objet NULL");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 *
 * @procedure
 * @step 1 Add new state with state name NULL. Using function
 * workspace_edit_object_part_state_add.
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", NULL, (double) 0.0</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_n3)
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
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_edit_object_part_state_add(workspace, "bg", NULL, 0.0);
   ck_assert_msg(res == EINA_FALSE, "Add state without name");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 *
 * @procedure
 * @step 1 Add new state with part name NULL. Using function
 * workspace_edit_object_part_state_add.
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, NULL, (char *) "new", (double) 0.0</td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_n4)
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
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_edit_object_part_state_add(workspace, NULL, "new", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Add state to part without name");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
