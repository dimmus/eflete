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
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_p1</td>
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
 * @step 1 Restack exist part below another exist part.
 * </td>
 * <td>(Evas_Object *)workspace, (const char *)"elm.swallow.content", (const char*)"elm.text", (Eina_Bool) EINA_FALSE</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_p1)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_restack.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_restack.edj");

   ret = workspace_edit_object_part_restack(workspace, "elm.swallow.content", "elm.text", EINA_FALSE);
   ck_assert_msg(ret == EINA_TRUE, "Failed below part \"elm.swallow.content\" relative to \"elm.text\"");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_p2</td>
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
 * @step 1 Restack exist part below another exist part.
 * </td>
 * <td>(Evas_Object *)workspace, (const char *)"elm.swallow.content", (const char*)"bg", (Eina_Bool) EINA_TRUE</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_p2)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_restack.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_restack.edj");

   ret = workspace_edit_object_part_restack(workspace, "elm.swallow.content", "bg", EINA_TRUE);
   ck_assert_msg(ret == EINA_TRUE, "Failed below part \"elm.swallow.content\" relative to \"bg\"");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_n1</td>
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
 * @step 1 Restack non exist part above exist part.
 * </td>
 * <td>(Evas_Object *)workspace, (char *)"non_exist,  (const char*)"bg", (Eina_Bool) EINA_TRUE</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_n1)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");

   ret = workspace_edit_object_part_restack(workspace, "non_exist", "bg", EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Non exist part is reordered");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_n2</td>
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
 * @step 1 Restack exist part above non exist part.
 * </td>
 * <td>(Evas_Object *)workspace, (const char*)"bg", (const char *)"non_exist", (Eina_Bool) EINA_TRUE</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_n2)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");

   ret = workspace_edit_object_part_restack(workspace, "bg", "non_exist", EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Part is reordered relative to non exist part");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_n3</td>
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
 * @step 1 Restack part with NULL pointer string name.
 * </td>
 * <td>(Evas_Object *)workspace, NULL, (const char *)"bg", EINA_FALSE</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_n3)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");

   ret = workspace_edit_object_part_restack(workspace, NULL, "bg", EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "NULL named part is reordered");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_n4</td>
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
 * @step 1 Restack part relative to part with NULL pointer string name.
 * </td>
 * <td>(Evas_Object *)workspace, (const char *)"bg", NULL, EINA_FALSE</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_n4)
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
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_above.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_above.edj");

   ret = workspace_edit_object_part_restack(workspace, "bg", NULL, EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Part reordered relative to NULL named part");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * <tr>
 * <td>workspace_edit_object_part_restack</td>
 * <td>workspace_edit_object_part_restack_test_n5</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Restack part with NULL pointer workspace object.
 * </td>
 * <td>NULL, (const char*)"bg", (const char *)"elm.text", (Eina_Bool) EINA_TRUE</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_restack_test_n5)
{
   elm_init(0, 0);
   Eina_Bool ret = EINA_TRUE;

   ret = workspace_edit_object_part_restack(NULL, "bg", "elm.text", EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Reorder part in NULL object workspace");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup workspace_edit_object_part_restack
 * @{
 * </TABLE>
 * @}
 * @}
 */
