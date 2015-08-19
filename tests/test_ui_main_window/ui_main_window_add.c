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

#include "test_ui_main_window.h"

/**
 * @addtogroup ui_main_window_test
 * @{
 * @addtogroup ui_main_window_add
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_main_window_add
 * @{
 * <tr>
 * <td>ui_main_window_add</td>
 * <td>ui_main_window_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 *
 * @procedure
 * @step 1 Call ui_main_window.
 * @step 2 Check returned value.
 * @step 3 Check (App_Data)->win on NULL
 * @step 4 Check (App_Data)->win_layout on NULL
 * @step 5 Check (App_Data)->main_menu on NULL
 * @step 6 Check (App_Data)->workspace on NULL
 * @step 7 Check (App_Data)->live_view on NULL
 * @step 8 Check (App_Data)->color_sel on NULL
 * </td>
 * <td></td>
 * <td>All checks are passed</td>
 * </tr>
 * @}
 */

EFLETE_TEST(ui_main_window_add_test_p)
{
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();

   ret = ui_main_window_add();
   ck_assert_msg(ret == EINA_TRUE, "Failed to create main window");
   ck_assert_msg(ap.win != NULL, "Failed to create window object");
   ck_assert_msg(ap.win_layout != NULL, "Failed to create window layout");
   ck_assert_msg(ap.menu != NULL, "Failed to create window menu");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_main_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
