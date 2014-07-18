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
 * @step 4 Config load
 * @step 5 Create application data structure
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
 * <td>(App_Data *) app_data</td>
 * <td>All checks are passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST(ui_panes_add_test_p)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   config_init();
   config_load();
   app_data = app_create();

   app_data->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);
   app_data->win_layout = elm_layout_add(app_data->win);

   ret = ui_panes_add(app_data);
   ck_assert_msg(ret == EINA_TRUE, "Failed to create panes");
   ck_assert_msg(app_data->block.left_top != NULL,
                 "Failed to create left_top block");
   ck_assert_msg(app_data->block.left_bottom != NULL,
                 "Failed to create left_bottom block");
   ck_assert_msg(app_data->block.bottom_left != NULL,
                 "Failed to create bottom_left block");
   ck_assert_msg(app_data->block.bottom_right != NULL,
                 "Failed to create bottom_right block");
   ck_assert_msg(app_data->block.right_top != NULL,
                 "Failed to create right_top block");
   ck_assert_msg(app_data->block.right_bottom != NULL,
                 "Failed to create right_bottom block");
   ck_assert_msg(app_data->block.canvas != NULL,
                 "Failed to create canvas block");

   config_shutdown();
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * <tr>
 * <td>ui_panes_add</td>
 * <td>ui_panes_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 *
 * @procedure
 * @step 1 Call ui_panes_add.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_panes_add_test_n1)
{
   Eina_Bool ret = EINA_TRUE;

   elm_init(0, 0);
   app_init();

   ret = ui_panes_add(NULL);
   ck_assert_msg(ret == EINA_FALSE, "Create panes without application data");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * <tr>
 * <td>ui_panes_add</td>
 * <td>ui_panes_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 *
 * @procedure
 * @step 1 Call ui_panes_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *)app_data</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_panes_add_test_n2)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_TRUE;

   elm_init(0, 0);
   app_init();
   app_data = app_create();

   ret = ui_panes_add(app_data);
   ck_assert_msg(ret == EINA_FALSE, "Create panes without main window create");

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
