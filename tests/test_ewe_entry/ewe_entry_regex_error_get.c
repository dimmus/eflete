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
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * ewe_entry
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_n1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @procedure
 * @step 1 call ewe_entry_regex_error_get(NULL)
 * @step 2 Check returned value
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>EWE_REG_NOERROR and NULL as returned error sring</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_n1)
{
   ewe_init(0, 0);
   ck_assert_msg(ewe_entry_regex_error_get(NULL) == EWE_REG_NOERROR,
      "ewe_entry_regex_error_get() should return EWE_REG_NOERROR if obj is NULL, but it did not");
   ck_assert_msg(ewe_entry_regex_error_text_get(NULL) == NULL,
      "ewe_entry_regex_error_text_get() should return NULL if obj is NULL");
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_n2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_error_get(obj)
 * @step 2 Check returned value
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_NOERROR and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_n2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_NOERROR,
      "ewe_entry_regex_error_get() should return EWE_REG_NOERROR if regex is not set yet, but it did not");
   ck_assert_msg(ewe_entry_regex_error_text_get(obj) == NULL,
      "ewe_entry_regex_error_text_get() should return NULL if regex is not set yet");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[a-z]", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_NOERROR
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * @step 5 set entry text that matches to set regex
 * @step 6 call ewe_entry_regex_check(obj)
 * @step 7 Check if ewe_entry_regex_error_get() returns EWE_REG_NOERROR
 * @step 8 call ewe_entry_regex_error_text_get()
 * @step 9 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_NOERROR and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[a-z]", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_NOERROR,
      "ewe_entry_regex_error_get() should return EWE_REG_NOERROR if regex set successfully, but it did not");
   ewe_entry_entry_set(obj, "abc");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Success"),
      "ewe_entry_regex_error_text_get() should return \"Success\" on EWE_REG_NOERROR error message but it did not");
   ewe_entry_regex_check(obj);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_NOERROR,
      "ewe_entry_regex_error_get() should return EWE_REG_NOERROR if regex check is successfull, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Success"),
      "ewe_entry_regex_error_text_get() should return \"Success\" on EWE_REG_NOERROR error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_BADPAT
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_BADPAT and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_BADPAT,
      "ewe_entry_regex_error_get() should return EWE_REG_BADPAT, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid regular expression"),
      "ewe_entry_regex_error_text_get() should return \"Invalid regular expression\" on EWE_REG_BADPAT error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p3</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[[.ll.]]", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_ECOLLATE
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_ECOLLATE and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p3)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[[.ll.]]", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_ECOLLATE,
      "ewe_entry_regex_error_get() should return EWE_REG_BADPAT, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid collation character"),
      "ewe_entry_regex_error_text_get() should return \"Invalid collation character\" on EWE_REG_ECOLLATE error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p4</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[[:anum]]", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_ECTYPE
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_ECTYPE and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p4)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[[:anum:]]", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_ECTYPE,
      "ewe_entry_regex_error_get() should return EWE_REG_ECTYPE, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid character class name"),
      "ewe_entry_regex_error_text_get() should return \"Invalid character class name\" on EWE_REG_ECTYPE error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p5</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "\\", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_EESCAPE
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_EESCAPE and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p5)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "\\", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_EESCAPE,
      "ewe_entry_regex_error_get() should return EWE_REG_EESCAPE, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Trailing backslash"),
      "ewe_entry_regex_error_text_get() should return \"Trailing backslash\" on EWE_REG_EESCAPE error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p6</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "()\\1", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_ESUBREG
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_ESUBREG and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p6)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "()\\1", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_ESUBREG,
      "ewe_entry_regex_error_get() should return EWE_REG_ESUBREG, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid back reference"),
      "ewe_entry_regex_error_text_get() should return \"Invalid back reference\" on EWE_REG_ESUBREG error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p7</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[url", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_EBRACK
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_EBRACK and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p7)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[url", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_EBRACK,
      "ewe_entry_regex_error_get() should return EWE_REG_EBRACK, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Unmatched [ or [^"),
      "ewe_entry_regex_error_text_get() should return \"Unmatched [ or [^\" on EWE_REG_EBRACK error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p8</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, ")\\(", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_EPAREN
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_EPAREN and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p8)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, ")\\(", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_EPAREN,
      "ewe_entry_regex_error_get() should return EWE_REG_EPAREN, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Unmatched ( or \\("),
      "ewe_entry_regex_error_text_get() should return \"Unmatched ( or \\(\" on EWE_REG_EPAREN error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p9</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "}\\{", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_EBRACE
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_EBRACE and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p9)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "}\\{", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_EBRACE,
      "ewe_entry_regex_error_get() should return EWE_REG_EBRACE, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Unmatched \\{"),
      "ewe_entry_regex_error_text_get() should return \"Unmatched \\{\" on EWE_REG_EBRACE error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p10</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "10{1|}", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_BADBR
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_BADBR and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p10)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "10{1|}", EWE_REG_EXTENDED);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_BADBR,
      "ewe_entry_regex_error_get() should return EWE_REG_BADBR, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid content of \\{\\}"),
      "ewe_entry_regex_error_text_get() should return \"Invalid content of \\{\\}\" on EWE_REG_BADBR error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p11</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[\\-.]", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_ERANGE
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_ERANGE and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p11)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[\\-.]", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_ERANGE,
      "ewe_entry_regex_error_get() should return EWE_REG_ERANGE, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid range end"),
      "ewe_entry_regex_error_text_get() should return \"Invalid range end\" on EWE_REG_ERANGE error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p12</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "*+**?+*", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_BADRPT
 * @step 3 call ewe_entry_regex_error_text_get()
 * @step 4 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_BADRPT and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p12)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "*+**?+*", 0);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_BADRPT,
      "ewe_entry_regex_error_get() should return EWE_REG_BADRPT, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "Invalid preceding regular expression"),
      "ewe_entry_regex_error_text_get() should return \"Invalid preceding regular expression\" on EWE_REG_BADRPT error message but it did not");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_entry_regex_error_get
 * @{
 * <tr>
 * <td>ewe_entry_regex_error_get</td>
 * <td>ewe_entry_regex_error_get_p13</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create parent window
 * @step 3 call ewe_entry_add() with proper parameter
 * @procedure
 * @step 1 call ewe_entry_regex_set(obj, "[a-z]", 0)
 * @step 2 Check if ewe_entry_regex_error_get() returns EWE_REG_NOMATCH
 * @step 3 set text to the entry that does not match to the set regex
 * @step 4 call ewe_entry_regex_error_text_get()
 * @step 5 Check returned value
 * </td>
 * <td>(Evas_Object *) entry</td>
 * <td>EWE_REG_NOMATCH and corresponding error text</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_entry_regex_error_get_p13)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("entry", "Entry");
   Evas_Object *obj = ewe_entry_add(win);
   ewe_entry_regex_set(obj, "[a-z]", 0);
   ewe_entry_entry_set(obj, "123");
   ewe_entry_regex_check(obj);
   ck_assert_msg(ewe_entry_regex_error_get(obj) == EWE_REG_NOMATCH,
      "ewe_entry_regex_error_get() should return EWE_REG_NOMATCH, but it did not");
   ck_assert_msg(!strcmp(ewe_entry_regex_error_text_get(obj), "No match"),
      "ewe_entry_regex_error_text_get() should return \"No match\" on EWE_REG_NOMATCH error message but it did not");
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
