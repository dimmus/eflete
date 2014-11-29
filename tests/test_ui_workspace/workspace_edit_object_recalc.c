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
 * @addtogroup workspace_edit_object_recalc
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_recalc
 * @{
 * <tr>
 * <td>workspace_edit_object_recalc</td>
 * <td>workspace_edit_object_recalc_test_p</td>
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
 * @step 1 call workspace_edit_object_recalc
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace</td>
 * <td>Returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_recalc_test_p)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_recalc.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_recalc.edj");
   ret = workspace_edit_object_recalc(workspace);
   ck_assert_msg(ret == EINA_TRUE, "Fail recalc edit object in workspace");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_recalc
 * @{
 * <tr>
 * <td>workspace_edit_object_recalc</td>
 * <td>workspace_edit_object_recalc_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 *
 * @procedure
 * @step 1 call workspace_edit_object_recalc
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_recalc_test_n)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_FALSE;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   ret = workspace_edit_object_recalc(workspace);
   ck_assert_msg(ret == EINA_FALSE, "Recalc edit object in workspace, "
                                    "without prevision set edit object");

   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_recalc
 * @{
 * <tr>
 * <td>workspace_edit_object_recalc</td>
 * <td>workspace_edit_object_recalc_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call workspace_edit_object_recalc with NULL param
 * @step 2 check returned value
 * </td>
 * <td>NULL</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_recalc_test_n1)
{
   elm_init(0, 0);
   Eina_Bool ret = EINA_TRUE;

   ret = workspace_edit_object_recalc(NULL);
   ck_assert_msg(ret == EINA_FALSE, "Recalc edit object in NULL object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_recalc
 * @{
 * </TABLE>
 * @}
 * @}
 */
