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
 * @addtogroup ui_workspace_test
 * @{
 * @addtogroup workspace_highlight_set
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_highlight_set
 * @{
 * <tr>
 * <td>workspace_highlight_set</td>
 * <td>workspace_highlight_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 create part object with exist part loaded.
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_highlight_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Part *) part</td>
 * <td>Returned EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_highlight_set_test_p)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Part *part = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj",
                        style->full_group_name);
   part = wm_part_add(style, "bg");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_highlight_set(workspace, part);
   ck_assert_msg(res == EINA_TRUE, "Failed set highlight for workspace");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_highlight_set
 * @{
 * <tr>
 * <td>workspace_highlight_set</td>
 * <td>workspace_highlight_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 create part object with non exist part name.
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_highlight_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Part *) part</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_highlight_set_test_n)
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
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj",
                        style->full_group_name);
   part = wm_part_add(style, "b_g");
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_highlight_set(workspace, part);
   ck_assert_msg(res == EINA_FALSE, "Set highlight for workspace to non exist part");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_highlight_set
 * @{
 * <tr>
 * <td>workspace_highlight_set</td>
 * <td>workspace_highlight_set_test_n1</td>
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
 * @step 1 call workspace_highlight_set with NULL part param
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_highlight_set_test_n1)
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
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj",
                        style->full_group_name);
   workspace_edit_object_set(workspace, style, "./edj_build/radio.edj");

   res = workspace_highlight_set(workspace, NULL);
   ck_assert_msg(res == EINA_FALSE, "Set highlight for workspace to non exist part");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_highlight_set
 * @{
 * <tr>
 * <td>workspace_highlight_set</td>
 * <td>workspace_highlight_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 create part object with exist part name.
 *
 * @procedure
 * @step 1 call workspace_highlight_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Part *) part</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_highlight_set_test_n2)
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
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj",
                        style->full_group_name);
   part = wm_part_add(style, "bg");

   res = workspace_highlight_set(workspace, part);
   ck_assert_msg(res == EINA_FALSE, "Set highlight for workspace without "
                                    "groupedit object");

   wm_style_free(style);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_highlight_set
 * @{
 * <tr>
 * <td>workspace_highlight_set</td>
 * <td>workspace_highlight_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create style object
 * @step 4 create part object with exist part loaded.
 *
 * @procedure
 * @step 1 call workspace_highlight_set with workspace NULL param
 * @step 2 check returned value
 * </td>
 * <td>NULL, (Part *) part</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_highlight_set_test_n3)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent;
   Style *style = NULL;
   Part *part = NULL;
   Evas *e = NULL;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj",
                        style->full_group_name);
   part = wm_part_add(style, "bg");

   res = workspace_highlight_set(NULL, part);
   ck_assert_msg(res == EINA_FALSE, "Set highlight for NULL workspace object");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_highlight_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
