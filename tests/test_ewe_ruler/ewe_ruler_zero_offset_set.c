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
 * @addtogroup ewe_ruler_zero_offset_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_zero_offset_set
 * @{
 * <tr>
 * <td>ewe_ruler_zero_offset_set</td>
 * <td>ewe_ruler_zero_offset_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 ewe_ruler added
 *
 * @procedure
 * @step 1 call ewe_ruler_zero_offset_set
 * @step 2 check returned value
 * @step 3 compare setted and getted values
 * </td>
 * <td>Evas_Object *ruler, int pos</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST(ewe_ruler_zero_offset_set_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   int zero_offset;
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_zero_offset_set(ruler, NULL, 100) == EINA_TRUE,
                 "Cannot set zero pointer of the ruler");
   zero_offset = ewe_ruler_zero_offset_get(ruler, NULL);
   ck_assert_msg(zero_offset == 100,
                 "Getted and setted value are not equal");

   evas_object_del(win);
   app_shutdown();
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_ruler_zero_offset_set
 * @{
 * <tr>
 * <td>ewe_ruler_zero_offset_set</td>
 * <td>ewe_ruler_zero_offset_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_zero_offset_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, int pos</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_ruler_zero_offset_set_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();

   ck_assert_msg(ewe_ruler_zero_offset_set(NULL, NULL, 100) == EINA_FALSE,
                 "Setted zero position of the NULL ruler object");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_zero_offset_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
