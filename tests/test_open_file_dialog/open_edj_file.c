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
 * @addtogroup open_edj_file
 * @{
 * open_file_dialog
 * <TABLE>
 * @}
 */

/**
 * @addtogroup open_edj_file
 * @{
 * <tr>
 * <td>open_edj_file</td>
 * <td>open_edj_file_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 *
 * @procedure
 * @step 1 Call function open_edj_file(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (open_edj_file_test_n1)
{
   elm_init(0, 0);
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_create();

   result = open_edj_file(app_data);
   ck_assert_msg(result == EINA_FALSE, "'Open edc' dialog was created");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup open_edj_file
 * @{
 * <tr>
 * <td>open_edj_file</td>
 * <td>open_edj_file_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call function open_edj_file(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (open_edj_file_test_n2)
{
   elm_init(0, 0);
   Eina_Bool result = EINA_FALSE;

   result = open_edj_file(NULL);
   ck_assert_msg(result == EINA_FALSE, "'Open edc' dialog was created");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup open_edj_file
 * @{
 * </TABLE>
 * @}
 * @}
 */
