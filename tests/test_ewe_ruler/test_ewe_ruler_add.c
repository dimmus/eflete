/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_add
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_add
 * @{
 * <tr>
 * <td>ewe_ruler_add</td>
 * <td>ewe_ruler_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize ewe library
 * @step 2 create a parent window
 *
 * @procedure
 * @step 1 create a ruler object
 * @step 2 check returned value from function
 * </td>
 * <td>(Evas Object *) parent</td>
 * <td>Not NULL object returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EWE_TEST(ewe_ruler_add_test_p)
{
   app_init();
   Evas_Object *result, *parent;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   result = ewe_ruler_add(parent);
   ck_assert_msg(result != NULL, "Not layout returned\n");
   evas_object_del(parent);
   app_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_add
 * @{
 * <tr>
 * <td>ewe_ruler_add</td>
 * <td>ewe_ruler_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize ewe library
 *
 * @procedure
 * @step 1 call ewe_ruler_add
 * @step 2 check returned value from function
 * </td>
 * <td> NULL</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EWE_TEST(ewe_ruler_add_test_n)
{
   app_init();
   Evas_Object *result;
   result = ewe_ruler_add(NULL);
   ck_assert_msg(result == NULL, "Not NULL returned\n");
   app_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
