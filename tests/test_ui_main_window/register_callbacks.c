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
 * @addtogroup register_callbacks
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup register_callbacks
 * @{
 * <tr>
 * <td>register_callbacks</td>
 * <td>register_callbacks_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 * @step 6 create widget list.
 * @step 7 set data into widget list.
 * @step 8 set widget list into block data
 *
 * @procedure
 * @step 1 Call register_callbacks
 * @step 2 Check returned value
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (register_callbacks_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list;
   App_Data *app_data;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *edj_path = "./edj_build/register_callbacks.edj";

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   project = pm_open_project_edj("UTC", edj_path);
   widget_list = ui_widget_list_add(app_data->win);
   ui_widget_list_data_set(widget_list, project);
   ui_block_widget_list_set(app_data, widget_list);

   result = register_callbacks(app_data);
   ck_assert_msg(result == EINA_TRUE, "Could not register callbacks!");

   pm_project_close(project);
   evas_object_del(app_data->win);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup register_callbacks
 * @{
 * <tr>
 * <td>register_callbacks</td>
 * <td>register_callbacks_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize other required libraries
 * @step 4 create application data
 *
 * @procedure
 * @step 1 Call register_callbacks
 * @step 2 Check returned value
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (register_callbacks_test_n1)
{
   elm_init(0, 0);
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();

   result = register_callbacks(app_data);
   ck_assert_msg(result == EINA_FALSE, "Callbacks were registered!");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup register_callbacks
 * @{
 * <tr>
 * <td>register_callbacks</td>
 * <td>register_callbacks_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call register_callbacks
 * @step 2 Check returned value
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (register_callbacks_test_n2)
{
   elm_init(0, 0);
   Eina_Bool result = EINA_FALSE;

   result = register_callbacks(NULL);
   ck_assert_msg(result == EINA_FALSE, "Callbacks were registered!");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup register_callbacks
 * @{
 * </TABLE>
 * @}
 * @}
 */
