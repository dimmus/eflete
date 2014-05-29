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

#include "test_open_file_dialog.h"

/**
 * @addtogroup open_file_dialog_test
 * @{
 * @addtogroup add_callbacks_wd
 * @{
 * open_file_dialog
 * <TABLE>
 * @}
 */

/**
 * @addtogroup add_callbacks_wd
 * @{
 * <tr>
 * <td>add_callbacks_wd</td>
 * <td>add_callbacks_wd_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 * @step 5 create widget list.
 *
 * @procedure
 * @step 1 Call add_callbacks_wd(widget_list, app_data)
 * @step 2 Check returned value
 * </td>
 * <td>Evas_Object *widget_list, App_Data *app_data</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (add_callbacks_wd_test_p1)
{
   elm_init(0, 0);
   Evas_Object *widget_list;
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);
   widget_list = ui_widget_list_add(app_data->win);

   result = add_callbacks_wd(widget_list, app_data);
   ck_assert_msg(result == EINA_TRUE, "Could not register callbacks!");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup add_callbacks_wd
 * @{
 * <tr>
 * <td>add_callbacks_wd</td>
 * <td>add_callbacks_wd_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 * @step 5 create widget list.
 *
 * @procedure
 * @step 1 Call add_callbacks_wd(widget_list, NULL)
 * @step 2 Check returned value
 * </td>
 * <td>Evas_Object *widget_list, NULL</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (add_callbacks_wd_test_p2)
{
   elm_init(0, 0);
   Evas_Object *widget_list;
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);
   widget_list = ui_widget_list_add(app_data->win);

   result = add_callbacks_wd(widget_list, NULL);
   ck_assert_msg(result == EINA_TRUE, "Could not register callbacks!");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup add_callbacks_wd
 * @{
 * <tr>
 * <td>add_callbacks_wd</td>
 * <td>add_callbacks_wd_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call add_callbacks_wd(NULL, NULL)
 * @step 2 Check returned value
 * </td>
 * <td>Evas_Object *widget_list, NULL</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (add_callbacks_wd_test_n)
{
   elm_init(0, 0);
   Eina_Bool result = EINA_FALSE;

   result = add_callbacks_wd(NULL, NULL);
   ck_assert_msg(result == EINA_FALSE, "Callbacks were registered!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup add_callbacks_wd
 * @{
 * </TABLE>
 * @}
 * @}
 */
