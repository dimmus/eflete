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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; If not, see www.gnu.org/licenses/lgpl.html.
*/
#include "test_ewe_statusbar.h"

/**
 * @addtogroup ewe_statusbar_test
 * @{
 * @addtogroup ewe_statusbar_add
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_add
 * @{
 * <tr>
 * <td>ewe_statusbar_add</td>
 * <td>ewe_statusbar_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) win</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_add_test_p)
{
   Evas_Object *win = NULL;
   Evas_Object *ret = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ret = ewe_statusbar_add(win);
   ck_assert_msg(ret != NULL, "Failed to create statusbar widget");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_add
 * @{
 * <tr>
 * <td>ewe_statusbar_add</td>
 * <td>ewe_statusbar_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_add
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>Returned  pointer is NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_add_test_n)
{
   Evas_Object *ret = NULL;

   ewe_init(0, 0);

   ret = ewe_statusbar_add(NULL);
   ck_assert_msg(ret == NULL, "Statusbar widget was added without parent object");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
