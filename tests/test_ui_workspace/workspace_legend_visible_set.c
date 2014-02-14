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
 * @addtogroup workspace_legend_visible_set
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_legend_visible_set
 * @{
 * <tr>
 * <td>workspace_legend_visible_set</td>
 * <td>workspace_legend_visible_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 *
 * @procedure
 * @step 1 call workspace_legend_visible_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace</td>
 * <td>Returned EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_legend_visible_set_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   res = workspace_legend_visible_set(workspace);
   ck_assert_msg(res == EINA_TRUE, "Failed change visibility legend for workspace");
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_legend_visible_set
 * @{
 * <tr>
 * <td>workspace_legend_visible_set</td>
 * <td>workspace_legend_visible_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call workspace_legend_visible_set with NULL pointer workspace object
 * @step 2 check returned value
 * </td>
 * <td>NULL</td>
 * <td>Returned EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_legend_visible_set_test_n)
{
   elm_init(0, 0);
   Eina_Bool res = EINA_TRUE;
   res = workspace_legend_visible_set(NULL);
   ck_assert_msg(res == EINA_FALSE, "Change visibility legend for NULL object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_legend_visible_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
