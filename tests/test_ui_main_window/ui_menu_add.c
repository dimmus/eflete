/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "test_ui_main_window.h"

/**
 * @addtogroup ui_main_window_test
 * @{
 * @addtogroup ui_menu_add
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_menu_add
 * @{
 * <tr>
 * <td>ui_menu_add</td>
 * <td>ui_menu_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 * @step 4 Create window, which was set into (App_Data)->win
 * @step 5 Create layout, which was set into (App_Data)->win_layout
 *
 * @procedure
 * @step 1 Call ui_menu_add.
 * @step 2 Check returned value.
 * @step 3 Check (App_Data)->menu_hash.
 * </td>
 * <td>(App_Data *) app_data</td>
 * <td>All checks are passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST(ui_menu_add_test_p)
{
   App_Data *app_data = NULL;
   Evas_Object *ret = NULL;

   elm_init(0, 0);
   app_init();
   app_data = app_create();

   app_data->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);
   app_data->win_layout = elm_layout_add(app_data->win);

   ret = ui_menu_add(app_data);
   ck_assert_msg(ret != NULL, "Failed to create menu");
   ck_assert_msg(app_data->menu_hash != NULL, "Failed to create hash of menu items");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_add
 * @{
 * <tr>
 * <td>ui_menu_add</td>
 * <td>ui_menu_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 *
 * @procedure
 * @step 1 Call ui_menu_add.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_add_test_n1)
{
   Evas_Object *ret = NULL;

   elm_init(0, 0);
   app_init();

   ret = ui_menu_add(NULL);
   ck_assert_msg(ret == NULL, "Create menu without application data");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_add
 * @{
 * <tr>
 * <td>ui_menu_add</td>
 * <td>ui_menu_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 *
 * @procedure
 * @step 1 Call ui_menu_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *)app_data</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_menu_add_test_n2)
{
   App_Data *app_data = NULL;
   Evas_Object *ret = NULL;

   elm_init(0, 0);
   app_init();
   app_data = app_create();

   ret = ui_menu_add(app_data);
   ck_assert_msg(ret == NULL, "Create menu without main window create");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
