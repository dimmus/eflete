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

#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup app_create
 * @{
 * eflete
 * <TABLE>
 * @}
 */

/**
 * @addtogroup app_create
 * @{
 * <tr>
 * <td>app_create</td>
 * <td>app_create_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Nothing.
 *
 * @procedure
 * @step 1 call app_create
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>Not NULL object</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (app_create_test_p1)
{
   ck_assert_msg(app_create() != NULL, "Cannot create empty App_Data");
}
END_TEST

/**
 * @addtogroup app_create
 * @{
 * <tr>
 * <td>app_create</td>
 * <td>app_create_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Nothing.
 *
 * @procedure
 * @step 1 call app_create
 * @step 2 check returned pointer
 * @step 3 call app_create
 * @step 4 check returned pointer
 * @step 5 check that app_create returns same application data.
 * </td>
 * <td>void</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (app_create_test_p2)
{
   App_Data *data1, *data2;

   data1 = app_create();
   ck_assert_msg(data1 != NULL, "Cannot create empty App_Data");
   data2 = app_create();
   ck_assert_msg(data2 != NULL, "Cannot create empty App_Data");
   ck_assert_msg(data1 == data2, "Created two different App_Data's for one application");
}
END_TEST

/**
 * @addtogroup app_create
 * @{
 * </TABLE>
 * @}
 * @}
 */
