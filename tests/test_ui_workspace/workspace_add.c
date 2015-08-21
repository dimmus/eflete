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
 * @addtogroup workspace_add
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_add
 * @{
 * <tr>
 * <td>workspace_add</td>
 * <td>workspace_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extension theme from EFLETE_THEME file
 * @step 3 create parent window
 *
 * @procedure
 * @step 1 Call workspace_add
 * @step 2 Check returned pointer
 * </td>
 * <td>(Evas_Object *) parent</td>
 * <td>Returned pointer not NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_add_test_p)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent, NULL, NULL);
   ck_assert_msg(workspace != NULL, "Failed create workspace object.");
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
