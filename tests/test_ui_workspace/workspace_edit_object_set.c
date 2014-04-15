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
 * @addtogroup workspace_edit_object_set
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_set
 * @{
 * <tr>
 * <td>workspace_edit_object_set</td>
 * <td>workspace_edit_object_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 *
 * @procedure
 * @step 1 call workspace_edit_object_set
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *workspace, Style *style, char *</td>
 * <td>Returned EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_set_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   res = workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");
   ck_assert_msg(res == EINA_TRUE, "Fail add edit object into workspace");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_set
 * @{
 * <tr>
 * <td>workspace_edit_object_set</td>
 * <td>workspace_edit_object_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 *
 * @procedure
 * @step 1 call workspace_edit_object_set with invalid file path.
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *workspace, Style *style, char *</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_set_test_n)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   res = workspace_edit_object_set(workspace, style,
                                   "./edj_build/invalid.edj");
   ck_assert_msg(res == EINA_FALSE, "Add edit object with invalid file path");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_set
 * @{
 * <tr>
 * <td>workspace_edit_object_set</td>
 * <td>workspace_edit_object_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create style object
 * @step 4 load style data from edje file
 *
 * @procedure
 * @step 1 call workspace_edit_object_set with invalid workspace param.
 * @step 2 check returned value
 * </td>
 * <td>NULL, Style *style, char *</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_set_test_n1)
{
   elm_init(0, 0);
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   res = workspace_edit_object_set(NULL, style, "./edj_build/radio.edj");
   ck_assert_msg(res == EINA_FALSE, "Add edit object to workspace NULL object");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_set
 * @{
 * <tr>
 * <td>workspace_edit_object_set</td>
 * <td>workspace_edit_object_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 *
 * @procedure
 * @step 1 call workspace_edit_object_set with invalid style param.
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *workspace, NULL, char *</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_set_test_n2)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   res = workspace_edit_object_set(workspace, NULL, "./edj_build/radio.edj");
   ck_assert_msg(res == EINA_FALSE, "Add edit object to workspace without style");
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_set
 * @{
 * <tr>
 * <td>workspace_edit_object_set</td>
 * <td>workspace_edit_object_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 *
 * @procedure
 * @step 1 call workspace_edit_object_set with NULL file path.
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *workspace, Style *style, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_set_test_n3)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   res = workspace_edit_object_set(workspace, style, NULL);
   ck_assert_msg(res == EINA_FALSE, "Add edit object with NULL file path");
   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
