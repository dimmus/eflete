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
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_p</td>
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
 * @step 1 Delete exist state with workspace_edit_object_part_state_del
 * @step 2 check returned value from function
 * @step 3 check deleted state name and value in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "radio", (char *) "visible", (double) 0.0 </td>
 * <td>All check's passed'</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_p)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");

   res = workspace_edit_object_part_state_del(workspace, "radio", "visible", 0.0);
   ck_assert_msg(res == EINA_TRUE, "Failed delete state from part, loaded into workspace");
   res = edje_edit_state_exist(style->obj, "bg", "visible", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Deleted state still exist in edje edit object");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 set edit object into workspace
 * @step 7 Add new state with workspace_edit_object_part_state_add,
 *         with name that exist, but with different value. (default 0.5)
 *
 * @procedure
 * @step 1 Delete state, which added at step 7 of precondition using
 *         workspace_edit_object_part_state_del
 * @step 2 check returned value from function
 * @step 3 check deleted state name and value in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "radio", (char *) "default", (double) 0.5 </td>
 * <td>All check's passed'</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_p1)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_part_state_add(workspace, "bg", "default", 0.5);

   res = workspace_edit_object_part_state_del(workspace, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_TRUE, "Failed delete state from part, loaded into workspace");
   res = edje_edit_state_exist(style->obj, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_FALSE, "Delete state still exist in edje edit object");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_n</td>
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
 * @step 1 Delete state with workspace_edit_object_part_state_del from non exist
 *         part.
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "non_exist", (char *) "test", (double) 1.0 </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_n)
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
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");

   res = workspace_edit_object_part_state_del(workspace, "non_exist", "test", 1.0);
   ck_assert_msg(res == EINA_FALSE, "Delete state for non exist part.");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_n1</td>
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
 * @step 1 Delete non exist state from part with workspace_edit_object_part_state_del
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (char *) "non_exist", (double) 0.0 </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_n1)
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
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");

   res = workspace_edit_object_part_state_del(workspace, "bg", "nonexist", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Delete non exist state for part.");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Delete state with NULL pointer to workspace, using
 *         workspace_edit_object_part_state_del
 * @step 2 check returned value from function
 * </td>
 * <td>NULL, (char *) "bg", (char *) "default", (double) 0.0 </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_n2)
{
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);

   res = workspace_edit_object_part_state_del(NULL, "bg", "default", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Delete state with workspace objet NULL");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_n3</td>
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
 * @step 1 Delete state with state name NULL pointer string. Using function
 *         workspace_edit_object_part_state_del.
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", NULL, (double) 0.0 </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_n3)
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
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");

   res = workspace_edit_object_part_state_del(workspace, "bg", NULL, 0.0);
   ck_assert_msg(res == EINA_FALSE, "Delete state without name");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_n4</td>
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
 * @step 1 delete state with part name NULL. Using function
 *         workspace_edit_object_part_state_del.
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, NULL, (char *) "default", (double) 0.0 </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_n4)
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
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_state_del.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");

   res = workspace_edit_object_part_state_add(workspace, NULL, "default", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Delete state from part without name");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
