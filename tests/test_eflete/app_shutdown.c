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

#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup app_shutdown
 * @{
 * eflete
 * <TABLE>
 * @}
*/

/**
 * @addtogroup app_shutdown
 * @{
 * <tr>
 * <td>app_shutdown</td>
 * <td>app_shutdown_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data.
 *
 * @procedure
 * @step 1 call app_shutdown
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (app_shutdown_test_p)
{
   elm_init(0,0);
   app_init();
   app_create();

   ck_assert_msg(app_shutdown() == EINA_TRUE, "Could not shutdown some of libraries");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup app_shutdown
 * @{
 * <tr>
 * <td>app_shutdown</td>
 * <td>app_shutdown_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 *
 * @procedure
 * @step 1 call app_shutdown
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (app_shutdown_test_n)
{
   elm_init(0,0);
   app_init();

   ck_assert_msg(app_shutdown() == EINA_FALSE, "Everything gone right");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup app_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */
