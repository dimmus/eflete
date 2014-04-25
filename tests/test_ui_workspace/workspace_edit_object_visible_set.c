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

#include "test_ui_workspace.h"

/**
 * @addtogroup ui_workspace_test
 * @{
 * @addtogroup workspace_edit_object_visible_set
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_visible_set
 * @{
 * <tr>
 * <td>workspace_edit_object_visible_set</td>
 * <td>workspace_edit_object_visible_set_test_p</td>
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
 * @step 1 Set visible exist part with workspace_edit_object_visible_set
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (Eina_Bool) EINA_TRUE</td>
 * <td>EINA_TRUE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_visible_set_test_p)
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
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_visible_set.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_visible_set.edj");

   res = workspace_edit_object_visible_set(workspace, "bg", EINA_TRUE);
   ck_assert_msg(res == EINA_TRUE, "Failed set visibily part object, loaded into workspace");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_visible_set
 * @{
 * <tr>
 * <td>workspace_edit_object_visible_set</td>
 * <td>workspace_edit_object_visible_set_test_n</td>
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
 * @step 1 Set visible non exist part with workspace_edit_object_visible_set
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "non_exist", (Eina_Bool) EINA_TRUE </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_visible_set_test_n)
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
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_visible_set.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_visible_set.edj");

   res = workspace_edit_object_visible_set(workspace, "non_exist", EINA_TRUE);
   ck_assert_msg(res == EINA_FALSE, "Set visibily for non exist part");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_visible_set
 * @{
 * <tr>
 * <td>workspace_edit_object_visible_set</td>
 * <td>workspace_edit_object_visible_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Set visible part without workspace object workspace_edit_object_visible_set
 * @step 2 check returned value from function
 * </td>
 * <td>NULL, (char *) "bg", (Eina_Bool) EINA_TRUE </td>
 * <td>EINA_FALSE returned from function</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_visible_set_test_n1)
{
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);

   res = workspace_edit_object_visible_set(NULL, "bg", EINA_TRUE);
   ck_assert_msg(res == EINA_FALSE, "Set visibily for part in NULL workspace object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_visible_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
