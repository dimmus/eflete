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
 * @addtogroup ui_panes_left_panes_min_size_toggle
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_panes_left_panes_min_size_toggle
 * @{
 * <tr>
 * <td>ui_panes_left_panes_min_size_toggle</td>
 * <td>ui_panes_left_panes_min_size_toggle_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 *
 * @procedure
 * @step 1 Call ui_panes_left_panes_min_size_toggle(ap, true)
 * @step 2 Check returned value
 * @step 3 Call ui_panes_left_panes_min_size_toggle(ap, false)
 * @step 4 Check returned value
 * </td>
 * <td>App_Data *app_data, Eina_Bool is_on</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_panes_left_panes_min_size_toggle_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app_data;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);

   result = ui_panes_left_panes_min_size_toggle(app_data, true);
   ck_assert_msg(result == EINA_TRUE, "Could not switch Panes min sizes ON!");
   result = ui_panes_left_panes_min_size_toggle(app_data, false);
   ck_assert_msg(result == EINA_TRUE, "Could not switch Panes min sizes OFF!");

   pm_project_close(project);
   evas_object_del(app_data->win);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_left_panes_min_size_toggle
 * @{
 * <tr>
 * <td>ui_panes_left_panes_min_size_toggle</td>
 * <td>ui_panes_left_panes_min_size_toggle_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 *
 * @procedure
 * @step 1 Call ui_panes_left_panes_min_size_toggle(NULL, true)
 * @step 2 Check returned value
 * @step 3 Call ui_panes_left_panes_min_size_toggle(NULL, false)
 * @step 4 Check returned value
 * </td>
 * <td>App_Data *app_data, Eina_Bool is_on</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_panes_left_panes_min_size_toggle_test_n)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app_data;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();

   result = ui_panes_left_panes_min_size_toggle(app_data, true);
   ck_assert_msg(result == EINA_FALSE, "Should not switch Panes min sizes ON!");
   result = ui_panes_left_panes_min_size_toggle(app_data, false);
   ck_assert_msg(result == EINA_FALSE, "Should not switch Panes min sizes OFF!");

   pm_project_close(project);
   evas_object_del(app_data->win);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_left_panes_min_size_toggle
 * @{
 * </TABLE>
 * @}
 * @}
 */
