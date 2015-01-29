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
 * @addtogroup ewe_entry_regex_autocheck
 * @{
 * ewe_entry
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_entry_regex_autocheck
 * @{
 * <tr>
 * <td>ewe_entry_regex_autocheck</td>
 * <td>ewe_entry_regex_autocheck_n1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @procedure
 * @step 1 call ewe_entry_regex_autocheck_set()
 * @step 2 check if it returned EINA_FALSE
 * </td>
 * <td>(Evas_Object *) entry = NULL, Eina_Bool</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_autocheck_n1)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_regex_autocheck_set(NULL, EINA_TRUE) == EINA_FALSE, "EINA_FALSE is expected");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_autocheck
 * @{
 * <tr>
 * <td>ewe_entry_regex_autocheck</td>
 * <td>ewe_entry_regex_autocheck_n2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @procedure
 * @step 1 call ewe_entry_regex_autocheck_get()
 * @step 2 check if it returned EINA_FALSE
 * </td>
 * <td>(Evas_Object *) entry = NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_autocheck_n2)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_regex_autocheck_get(NULL) == EINA_FALSE, "EINA_FALSE is expected");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_autocheck
 * @{
 * <tr>
 * <td>ewe_entry_regex_autocheck</td>
 * <td>ewe_entry_regex_autocheck_p1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 check if default value is EINA_FALSE
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>Default visibility vale fot label should be EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_autocheck_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_autocheck_get(obj) == EINA_FALSE, "Default visibility value is not correct");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_autocheck
 * @{
 * <tr>
 * <td>ewe_entry_regex_autocheck</td>
 * <td>ewe_entry_regex_autocheck_p2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_autocheck_set(entry, EINA_TRUE)
 * @step 2 call ewe_entry_regex_autocheck_get(entry)
 * @step 3 check if value is changed
 * </td>
 * <td>(Evas_Object *) entry, EINA_TRUE</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_autocheck_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_autocheck_set(obj, EINA_TRUE) == EINA_TRUE, "ewe_entry_regex_autocheck_set() should return EINA_TRUE, but it did not");
   ck_assert_msg(ewe_entry_regex_autocheck_get(obj) == EINA_TRUE, "ewe_entry_regex_autocheck_get() should return EINA_TRUE, but it did not");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_autocheck
 * @{
 * </TABLE>
 * @}
 * @}
 */
