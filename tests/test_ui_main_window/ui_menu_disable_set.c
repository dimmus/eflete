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
 * @addtogroup ui_menu_disable_set
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_menu_disable_set
 * @{
 * <tr>
 * <td>ui_menu_disable_set</td>
 * <td>ui_menu_disable_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 initialize application with app_init() function
 * @step 3 Create application data structure.
 * @step 4 Create window, which was set into (App_Data)->win
 * @step 5 Create layout, which was set into (App_Data)->win_layout
 * @step 6 Add new menu
 *
 * @procedure
 * @step 1 Call function ui_menu_disable_set.
 * @step 2 Check returned value.
 * </td>
 * <td>Menu *menu, MENU_FILE_SAVE, Eina_Bool flag = EINA_TRUE</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_disable_set_test_p)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app_data = app_data_get();
   app_data->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);
   app_data->win_layout = elm_layout_add(app_data->win);
   Menu *menu = ui_menu_add(app_data);

   ret = ui_menu_disable_set(menu, MENU_FILE_SAVE, EINA_TRUE);
   ck_assert_msg(ret == EINA_TRUE, "Failed to disable Save menu item");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_disable_set
 * @{
 * <tr>
 * <td>ui_menu_disable_set</td>
 * <td>ui_menu_disable_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 initialize application with app_init() function
 * @step 3 Create application data structure.
 * @step 4 Create window, which was set into (App_Data)->win
 * @step 5 Create layout, which was set into (App_Data)->win_layout
 * @step 6 Add new menu
 *
 * @procedure
 * @step 1 Call function ui_menu_disable_set.
 * @step 2 Check returned value.
 * </td>
 * <td>Menu *menu, MENU_COUNT + 1, Eina_Bool flag = EINA_TRUE</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_disable_set_test_n1)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app_data = app_data_get();
   app_data->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);
   app_data->win_layout = elm_layout_add(app_data->win);
   Menu *menu = ui_menu_add(app_data);

   ret = ui_menu_disable_set(menu, MENU_ITEMS_COUNT + 1, EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Unexisted menu item was disabled");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_disable_set
 * @{
 * <tr>
 * <td>ui_menu_disable_set</td>
 * <td>ui_menu_disable_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 initialize application with app_init() function
 * @step 3 Create application data structure.
 * @step 4 Create window, which was set into (App_Data)->win
 * @step 5 Create layout, which was set into (App_Data)->win_layout
 *
 * @procedure
 * @step 1 Call function ui_menu_disable_set.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, MENU_FILE_SAVE, Eina_Bool flag = EINA_TRUE</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_disable_set_test_n2)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app_data = app_data_get();
   app_data->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);
   app_data->win_layout = elm_layout_add(app_data->win);

   ret = ui_menu_disable_set(NULL, MENU_FILE_SAVE, EINA_TRUE);
   ck_assert_msg(ret == EINA_FALSE, "Menu item was disabled");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_disable_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
