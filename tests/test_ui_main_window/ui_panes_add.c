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
 * @addtogroup ui_panes_add
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_panes_add
 * @{
 * <tr>
 * <td>ui_panes_add</td>
 * <td>ui_panes_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library
 * @step 2 Initialize requred libraries
 * @step 3 Config init
 * @step 4 Create application data structure
 * @step 5 Config load
 * @step 6 Create window, which setted into (App_Data)->win
 * @step 7 Create layout, which setted into (App_Data)->win_layout.
 *
 * @procedure
 * @step 1 Call ui_panes_add.
 * @step 2 Check returned value.
 * @step 3 Check (App_Data)->block.left_top
 * @step 4 Check (App_Data)->block.left_bottom
 * @step 5 Check (App_Data)->block.bottom_left
 * @step 6 Check (App_Data)->block.bottom_right
 * @step 7 Check (App_Data)->block.right_top
 * @step 8 Check (App_Data)->block.right_bottom
 * @step 9 Check (App_Data)->block.canvas
 * </td>
 * <td></td>
 * <td>All checks are passed</td>
 * </tr>
 * @}
 */

EFLETE_TEST(ui_panes_add_test_p)
{
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();

   ui_main_window_add();
   ret = ui_panes_add();
   ck_assert_msg(ret == EINA_TRUE, "Failed to create panes");
   ck_assert_msg(ap->block.left_top != NULL,
                 "Failed to create left_top block");
   ck_assert_msg(ap->block.left_bottom != NULL,
                 "Failed to create left_bottom block");
   ck_assert_msg(ap->block.bottom_right != NULL,
                 "Failed to create bottom_right block");
   ck_assert_msg(ap->block.right_top != NULL,
                 "Failed to create right_top block");
   ck_assert_msg(ap->block.right_bottom != NULL,
                 "Failed to create right_bottom block");
   ck_assert_msg(ap->block.canvas != NULL,
                 "Failed to create canvas block");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
