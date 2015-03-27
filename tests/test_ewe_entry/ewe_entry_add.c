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
#include "test_ewe_entry.h"

/**
 * @addtogroup ewe_entry_test
 * @{
 * @addtogroup ewe_entry_add
 * @{
 * ewe_entry
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_entry_add
 * @{
 * <tr>
 * <td>ewe_entry_add</td>
 * <td>ewe_entry_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @procedure
 * @step 1 call ewe_entry_add() with proper parameter
 * @step 2 check if entry created
 * </td>
 * <td>(Evas_Object *) win</td>
 * <td>Entry created successfully</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_add_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   ck_assert_msg(ewe_entry_add(win) != NULL, "Add failure");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_add
 * @{
 * <tr>
 * <td>ewe_entry_add</td>
 * <td>ewe_entry_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @procedure
 * @step 1 call ewe_entry_add() with NULL
 * @step 2 check if entry created
 * </td>
 * <td>NULL</td>
 * <td>ewe_entry_add() returned NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_add_test_n)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_add(NULL) == NULL, "Add failure on negative test");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
