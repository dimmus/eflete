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
 * @addtogroup ewe_entry_regex_set
 * @{
 * ewe_entry
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @procedure
 * @step 1 try to set regex to NULL
 * @step 2 Check returned value
 * </td>
 * <td>NULL, "", 0</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_n1)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_regex_set(NULL, "\\b(\\w+)\\s\\1\\b", 0) == EINA_FALSE, "Regexp should not be set to non exist object");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 try to set NULL string
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry, NULL, 0</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_n2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_set(obj, NULL, 0) == EINA_FALSE, "NULL should not be set");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 try to set extended regex string without EWE_REG_EXTENDED flag
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry, "\b(\w+)\s\1\b", 0</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_n3)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_set(obj, "\\b(\\w+)\\s\\1\\b", 0) == EINA_FALSE, "Extended regexp string should be set without proper flags");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_n4</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 try to set regex string with flags value is that bigger that allowed
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry, "[a-z]", 4</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_n4)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_set(obj, "[a-z]", 4) == EINA_FALSE, "Regexp string should not be set without proper flags");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_n5</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 try to set regex string with flags value that is smaller that allowed
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry, "[a-z]", -1</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_n5)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_set(obj, "[a-z]", -1) == EINA_FALSE, "Regexp string should be set without proper flags");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 try to set proper regex string
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry, "\b(\w+)\s\1\b", EWE_REG_EXTENDED</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_set(obj, "\\b(\\w+)\\s\\1\\b", EWE_REG_EXTENDED) == EINA_TRUE, "Regexp string should be set");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * <tr>
 * <td>ewe_entry_regex_set</td>
 * <td>ewe_entry_regex_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 try to set NULL string
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) entry, ""</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_set_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_set(obj, "", 0) == EINA_TRUE, "Empty string should be set");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
