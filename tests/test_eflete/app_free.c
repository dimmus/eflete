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
 * @addtogroup app_free
 * @{
 * eflete
 * <TABLE>
 * @}
 */

/**
 * @addtogroup app_free
 * @{
 * <tr>
 * <td>app_free</td>
 * <td>app_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 create application data.
 *
 * @procedure
 * @step 1 call app_free
 * @step 2 check returned pointer
 * </td>
 * <td>App_Data *app</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (app_free_test_p)
{
   App_Data *ap = app_create();

   ck_assert_msg(app_free(ap) == EINA_TRUE, "Could not free application data");
}
END_TEST

/**
 * @addtogroup app_free
 * @{
 * <tr>
 * <td>app_free</td>
 * <td>app_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 Nothing.
 *
 * @procedure
 * @step 1 call app_free
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (app_free_test_n)
{
   ck_assert_msg(app_free(NULL) == EINA_FALSE, "Something was freed");
}
END_TEST


/**
 * @addtogroup app_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
