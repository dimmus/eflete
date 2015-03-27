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
 * @addtogroup ewe_entry_regex_unset
 * @{
 * ewe_entry
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_entry_regex_unset
 * @{
 * <tr>
 * <td>ewe_entry_regex_unset</td>
 * <td>ewe_entry_regex_set_untest_n1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 calls ewe_entry_regex_unset()
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_unset_test_n1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_unset(obj) == EINA_FALSE, "Calling ewe_entry_regex_unset() without previoustly regex should return NULL");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_unset
 * @{
 * <tr>
 * <td>ewe_entry_regex_unset</td>
 * <td>ewe_entry_regex_unset_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @procedure
 * @step 1 call ewe_entry_regex_unset() with NULL param.
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry = NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_unset_test_n2)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_regex_unset(NULL) == EINA_FALSE, "Calling ewe_entry_regex_unset(NULL) should return NULL");
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_unset
 * @{
 * <tr>
 * <td>ewe_entry_regex_unset</td>
 * <td>ewe_entry_regex_unset_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @step 4 set proper regex
 * @procedure
 * @step 1 call ewe_entry_regex_unset()
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_unset_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "\\b(\\w+)\\s\\1\\b", 0);
   ck_assert_msg(ewe_entry_regex_unset(obj) == EINA_TRUE, "ewe_entry_regex_unset() should return EINA_TRUE");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
