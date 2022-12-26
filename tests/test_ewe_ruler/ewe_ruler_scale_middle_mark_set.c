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
 * @addtogroup ewe_ruler_scale_middle_mark_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_scale_middle_mark_set
 * @{
 * <tr>
 * <td>ewe_ruler_scale_middle_mark_set</td>
 * <td>ewe_ruler_scale_middle_mark_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add scale to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_scale_middle_mark_set(ruler, scale, EINA_TRUE)
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that value is setted
 * @step 4 call ewe_ruler_scale_middle_mark_set(ruler, scale, EINA_FALSE)
 * @step 5 check that returned value is EINA_TRUE
 * @step 6 check that value is setted
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Scale* scale</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_scale_middle_mark_set_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Scale *scale = ewe_ruler_scale_add(ruler, NULL);

   ck_assert_msg(ewe_ruler_scale_middle_mark_set(ruler, scale, EINA_TRUE) == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_msg(ewe_ruler_scale_middle_mark_get(ruler, scale) == EINA_TRUE, "Value not setted");
   ck_assert_msg(ewe_ruler_scale_middle_mark_set(ruler, scale, EINA_FALSE) == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_msg(ewe_ruler_scale_middle_mark_get(ruler, scale) == EINA_FALSE, "Value not setted");

   evas_object_del(win);
   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_scale_middle_mark_set
 * @{
 * <tr>
 * <td>ewe_ruler_scale_middle_mark_set</td>
 * <td>ewe_ruler_scale_middle_mark_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_scale_middle_mark_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_scale_middle_mark_set_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();

   ck_assert_msg(ewe_ruler_scale_middle_mark_set(NULL, NULL, EINA_FALSE) == EINA_FALSE, "Setted visibility from NULL ruler object");

   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_scale_middle_mark_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
