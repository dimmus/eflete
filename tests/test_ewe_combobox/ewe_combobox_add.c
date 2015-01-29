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
#include "test_ewe_combobox.h"

/**
* @addtogroup ewe_combobox_test
* @{
* @addtogroup ewe_combobox_add
* @{
* ewe_combobox
* <TABLE>
* @}
*/

/**
* @addtogroup ewe_combobox_add
* @{
* <tr>
* <td>ewe_combobox_add</td>
* <td>ewe_comobox_add_test_p</td>
* <td>
* @precondition
* @step 1 init ewe
* @step 2 create window
* @procedure
* @step 1 call ewe_combobox_add() with proper parameter
* @step 2 check if combobox created
* </td>
* <td>(Evas_Object *) win</td>
* <td>Not NULL pointer returned</td>
* <td>_REAL_RESULT_</td>
* <td>_PASSED_</td>
* </tr>
* @}
*/
EFLETE_TEST(ewe_combobox_add_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("combobox", "Combobox");
   Evas_Object *obj = ewe_combobox_add(win);
   ck_assert_msg(obj != NULL, "Add failure");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
* @addtogroup ewe_combobox_add
* @{
* <tr>
* <td>ewe_combobox_add</td>
* <td>ewe_combobox_add_test_n</td>
* <td>
* @precondition
* @step 1 initialize ewe library
* @procedure
* @step 1 Call combobox_add
* @step 2 Check returned pointeri
* </td>
* <td>NULL(The parent object)</td>
* <td>Returned  pointer is NULL</td>
* <td>_REAL_RESULT_</td>
* <td>_PASSED_</td>
* </tr>
* @}
*/

EFLETE_TEST(ewe_combobox_add_test_n)
{
   ewe_init(0, 0);
   Evas_Object *obj = ewe_combobox_add(NULL);
   ck_assert_msg(obj == NULL, "Add failure on negative test");
   ewe_shutdown();
}
END_TEST

/**
* @addtogroup ewe_combobox_add
* @{
* </TABLE>
* @}
* @}
*/
