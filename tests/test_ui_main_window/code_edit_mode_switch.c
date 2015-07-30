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
 * @addtogroup code_edit_mode_switch
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup code_edit_mode_switch
 * @{
 * <tr>
 * <td>code_edit_mode_switch</td>
 * <td>code_edit_mode_switch_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 *
 * @procedure
 * @step 1 Call code_edit_mode_switch(ap, true)
 * @step 2 Check returned value
 * @step 3 Call code_edit_mode_switch(ap, false)
 * @step 4 Check returned value
 * </td>
 * <td>App_Data *app_data, Eina_Bool is_on</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (code_edit_mode_switch_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);

   result = code_edit_mode_switch(app_data, true);
   ck_assert_msg(result == EINA_TRUE, "Could not switch Code Edit mode ON!");
   result = code_edit_mode_switch(app_data, false);
   ck_assert_msg(result == EINA_TRUE, "Could not switch Code Edit mode OFF!");

   evas_object_del(app_data->win);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup code_edit_mode_switch
 * @{
 * </TABLE>
 * @}
 * @}
 */
