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
 * @addtogroup new_theme_create
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup new_theme_create
 * @{
 * <tr>
 * <td>new_theme_create</td>
 * <td>new_theme_create_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 * @step 4 Create main window.
 *
 * @procedure
 * @step 1 Call new_theme_create(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(new_theme_create_test_p1)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);

   ret = new_theme_create(app_data);
   ck_assert_msg(ret == EINA_TRUE, "Failed to create new theme and open project");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup new_theme_create
 * @{
 * <tr>
 * <td>new_theme_create</td>
 * <td>new_theme_create_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 *
 * @procedure
 * @step 1 Call new_theme_create(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST(new_theme_create_test_p2)
{
   App_Data *app_data = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app_data = app_create();

   ret = new_theme_create(app_data);
   ck_assert_msg(ret == EINA_TRUE, "Failed to create new theme and open project");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup new_theme_create
 * @{
 * <tr>
 * <td>new_theme_create</td>
 * <td>new_theme_create_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call new_theme_create(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST(new_theme_create_test_n)
{
   elm_init(0, 0);
   Eina_Bool ret = EINA_FALSE;

   ret = new_theme_create(NULL);
   ck_assert_msg(ret == EINA_FALSE, "New theme was opened");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup new_theme_create
 * @{
 * </TABLE>
 * @}
 * @}
 */
