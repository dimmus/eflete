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
 * @addtogroup workspace_edit_object_part_below
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_part_below
 * @{
 * <tr>
 * <td>workspace_edit_object_part_below</td>
 * <td>workspace_edit_object_part_below_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Below exist part.
 * @step 2 Again below part.
 * </td>
 * <td>(Evas_Object *) workspace, (char *)"events"</td>
 * <td>All check's passed'</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_below_test_p)
{
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_below.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_below.edj");

   ret = workspace_edit_object_part_below(workspace, "events");
   ck_assert_msg(ret == EINA_TRUE, "Failed below part");
   ret = workspace_edit_object_part_below(workspace, "events");
   ck_assert_msg(ret == EINA_FALSE, "Part already at bottom was below again");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_below
 * @{
 * <tr>
 * <td>workspace_edit_object_part_below</td>
 * <td>workspace_edit_object_part_below_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Below non exist part.
 * </td>
 * <td>(Evas_Object *) workspace, (char *)"non_exist"</td>
 * <td>Function return EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_below_test_n)
{
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_below.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_below.edj");

   ret = workspace_edit_object_part_below(workspace, "non_exist");
   ck_assert_msg(ret == EINA_FALSE, "Below non exist part");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_below
 * @{
 * <tr>
 * <td>workspace_edit_object_part_below</td>
 * <td>workspace_edit_object_part_below_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Below part with NULL strin name.
 * </td>
 * <td>(Evas_Object *) workspace, NULL</td>
 * <td>Function return EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_below_test_n1)
{
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_below.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_below.edj");

   ret = workspace_edit_object_part_below(workspace, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Below NULL named part");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_below
 * @{
 * <tr>
 * <td>workspace_edit_object_part_below</td>
 * <td>workspace_edit_object_part_below_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Below part with NULL workspace object.
 * </td>
 * <td>NULL, (char *)"events"</td>
 * <td>Function return EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_below_test_n2)
{
   Eina_Bool ret = EINA_TRUE;

   elm_init(0, 0);

   ret = workspace_edit_object_part_below(NULL, "bg");
   ck_assert_msg(ret == EINA_FALSE, "Below part in NULL object workspace");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_below
 * @{
 * </TABLE>
 * @}
 * @}
 */
