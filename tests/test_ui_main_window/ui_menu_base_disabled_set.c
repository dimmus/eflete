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
 * @addtogroup ui_menu_base_disabled_set
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_menu_base_disabled_set
 * @{
 * <tr>
 * <td>ui_menu_base_disabled_set</td>
 * <td>ui_menu_base_disabled_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 initialize application with app_init() function
 * @step 3 Create application data structure.
 * @step 4 Create window, which was set into (App_Data)->win
 * @step 5 Create layout, which was set into (App_Data)->win_layout
 * @step 6 Add new menu_hash
 *
 * @procedure
 * @step 1 Call function ui_menu_base_disabled_set.
 * @step 2 Check returned value.
 * </td>
 * <td>Eina_Hash *menu_hash, Eina_Bool flag = EINA_TRUE</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_base_disabled_set_test_p)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app_data = app_create();
   app_data->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);
   app_data->win_layout = elm_layout_add(app_data->win);
   ui_menu_add(app_data);

   ret = ui_menu_base_disabled_set(app_data->menu_hash, EINA_TRUE);
   ck_assert_msg(ret == EINA_TRUE, "Failed to disable menu");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_base_disabled_set
 * @{
 * <tr>
 * <td>ui_menu_base_disabled_set</td>
 * <td>ui_menu_base_disabled_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 initialize application with app_init() function
 *
 * @procedure
 * @step 1 Call function ui_menu_base_disabled_set.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, Eina_Bool flag = EINA_TRUE</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_base_disabled_set_test_n)
{
   elm_init(0, 0);
   Eina_Bool ret = EINA_FALSE;

   ret = ui_menu_base_disabled_set(NULL, EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Failed to disable menu");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_base_disabled_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
