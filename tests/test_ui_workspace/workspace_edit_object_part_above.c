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
 * @addtogroup workspace_edit_object_part_above
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_above
 * @{
 * <tr>
 * <td>workspace_edit_object_part_above</td>
 * <td>workspace_edit_object_part_above_test_p</td>
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
 * @step 1 Above exist part. Here should returned EINA_TRUE.
 * @step 2 Again above part. Here should returned EINA_FALSE, because part already at top.
 * </td>
 * <td>(Evas_Object *)workspace, (char *)"radio"</td>
 * <td>All check's passed'</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_above_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");
   ret = workspace_edit_object_part_above(workspace, "radio");
   ck_assert_msg(ret == EINA_TRUE, "Failed above part");
   ret = workspace_edit_object_part_above(workspace, "radio");
   ck_assert_msg(ret == EINA_FALSE, "Already top part above again");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_above
 * @{
 * <tr>
 * <td>workspace_edit_object_part_above</td>
 * <td>workspace_edit_object_part_above_test_n</td>
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
 * @step 1 Above non exist part.
 * </td>
 * <td>(Evas_Object *)workspace, (char *)"non_exist</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_above_test_n)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");
   ret = workspace_edit_object_part_above(workspace, "non_exist");
   ck_assert_msg(ret == EINA_FALSE, "Non exist part is above");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_above
 * @{
 * <tr>
 * <td>workspace_edit_object_part_above</td>
 * <td>workspace_edit_object_part_above_test_n1</td>
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
 * @step 1 Above part with NULL pointer string name.
 * </td>
 * <td>(Evas_Object *)workspace, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_above_test_n1)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");
   ret = workspace_edit_object_part_above(workspace, NULL);
   ck_assert_msg(ret == EINA_FALSE, "NULL named part is above");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_above
 * @{
 * <tr>
 * <td>workspace_edit_object_part_above</td>
 * <td>workspace_edit_object_part_above_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Above part with NULL pointer workspace object.
 * </td>
 * <td>NULL, (char *)"bg"</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_above_test_n2)
{
   elm_init(0, 0);
   Eina_Bool ret = EINA_TRUE;

   ret = workspace_edit_object_part_above(NULL, "bg");
   ck_assert_msg(ret == EINA_FALSE, "Above part in NULL object workspace");
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup workspace_edit_object_part_above
 * @{
 * </TABLE>
 * @}
 * @}
 */
