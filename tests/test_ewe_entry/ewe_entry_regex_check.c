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
 * @addtogroup ewe_entry_regex_check
 * @{
 * ewe_entry
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_entry_regex_check
 * @{
 * <tr>
 * <td>ewe_entry_regex_check</td>
 * <td>ewe_entry_regex_set_check_n1</td>
 * <td>
 * @precondition
 * @step 1 Init ewe
 * @step 2 Create parent window
 * @step 3 Call ewe_entry_add() with proper parameter and set entry text
 * @procedure
 * @step 1 Call ewe_entry_regex_check() without previoustly set regex.
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_check_test_n1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_check(obj) == EINA_FALSE, "ewe_entry_regex_check() should return EINA_FALSE if regex is not set");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_check
 * @{
 * <tr>
 * <td>ewe_entry_regex_check</td>
 * <td>ewe_entry_regex_check_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @procedure
 * @step 1 Call ewe_entry_regex_check(NULL)
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry = NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_check_test_n2)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_regex_check(NULL) == EINA_FALSE, "ewe_entry_regex_check(NULL) should return EINA_FALSE");
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_check
 * @{
 * <tr>
 * <td>ewe_entry_regex_check</td>
 * <td>ewe_entry_regex_check_test_p</td>
 * <td>
 * @precondition
 * @step 1 Init ewe
 * @step 2 Create parent window
 * @step 3 Call ewe_entry_add() with proper parameter
 * @step 4 Set entry text that does not match to future regex
 * @step 5 Set regex
 * @procedure
 * @step 1 Call ewe_entry_regex_check()
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_check_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_entry_set(obj, "123");
   ewe_entry_regex_set(obj, "[a-z]", 0);
   ck_assert_msg(ewe_entry_regex_check(obj) == EINA_FALSE, "Entry string does not match to set regex, but ewe_entry_regex_check() returned EINA_TRUE");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_check
 * @{
 * <tr>
 * <td>ewe_entry_regex_check</td>
 * <td>ewe_entry_regex_check_test_p</td>
 * <td>
 * @precondition
 * @step 1 Init ewe
 * @step 2 Sreate parent window
 * @step 3 Sall ewe_entry_add() with proper parameter
 * @step 4 Set entry text that does not match to future regex
 * @step 5 Set regex
 * @procedure
 * @step 1 Call ewe_entry_regex_check()
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_check_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_entry_set(obj, "abc");
   ewe_entry_regex_set(obj, "[a-z]", 0);
   ck_assert_msg(ewe_entry_regex_check(obj) == EINA_TRUE, "Entry string matches to set regex, but ewe_entry_regex_check() returned EINA_FALSE");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_check
 * @{
 * <tr>
 * <td>ewe_entry_regex_check</td>
 * <td>ewe_entry_regex_check_test_p</td>
 * <td>
 * @precondition
 * @step 1 Init ewe
 * @step 2 Sreate parent window
 * @step 3 Sall ewe_entry_add() with proper parameter
 * @step 4 Set entry text that match to future regex
 * @step 5 Set regex
 * @procedure
 * @step 1 Call ewe_entry_regex_check()
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_check_test_p3)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_entry_set(obj, "ABC");
   ewe_entry_regex_set(obj, "[a-z]", EWE_REG_ICASE);
   ck_assert_msg(ewe_entry_regex_check(obj) == EINA_TRUE, "Entry string matches to set regex, but ewe_entry_regex_check() returned EINA_FALSE");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_check
 * @{
 * </TABLE>
 * @}
 * @}
 */
