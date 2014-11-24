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

#include "test_save_file_dialog.h"
#include "test_common.h"

/**
 * @addtogroup save_file_dialog_test
 * @{
 * @addtogroup save_as_edc_file
 * @{
 * save_file_dialog
 * <TABLE>
 * @}
 */

/**
 * @addtogroup save_as_edc_file
 * @{
 * <tr>
 * <td>save_as_edc_file</td>
 * <td>save_as_edc_file_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 *
 * @procedure
 * @step 1 Call function save_as_edc_file(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (save_as_edc_file_test_n1)
{
   elm_init(0, 0);
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);

   result = save_as_edc_file(app_data);
   ck_assert_msg(result == EINA_FALSE, "'Save as edc' dialog was created");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup save_as_edc_file
 * @{
 * <tr>
 * <td>save_as_edc_file</td>
 * <td>save_as_edc_file_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 open new project and create widget list
 *
 * @procedure
 * @step 1 Call function save_as_edc_file(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (save_as_edc_file_test_n2)
{
   elm_init(0, 0);
   setup("save_as_edc_file_test_n2");

   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();
   app_data->project = pm_project_open("./save_as_edc_file_test_n2/save_as_edc_file_test_n2.pro");

   result = save_as_edc_file(app_data);
   ck_assert_msg(result == EINA_FALSE, "'Save as edc' dialog was created");

   app_shutdown();
   teardown("save_as_edc_file_test_n2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup save_as_edc_file
 * @{
 * <tr>
 * <td>save_as_edc_file</td>
 * <td>save_as_edc_file_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call function save_as_edc_file(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (save_as_edc_file_test_n3)
{
   elm_init(0, 0);
   Eina_Bool result = EINA_FALSE;

   result = save_as_edc_file(NULL);
   ck_assert_msg(result == EINA_FALSE, "'Save as edc' dialog was created");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup save_as_edc_file
 * @{
 * </TABLE>
 * @}
 * @}
 */
