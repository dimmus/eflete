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
 * @addtogroup ewe_ruler_step_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_step_set
 * @{
 * <tr>
 * <td>ewe_ruler_step_set</td>
 * <td>ewe_ruler_step_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 ewe_ruler added
 *
 * @procedure
 * @step 1 call ewe_ruler_step_set with step "100"
 * @step 2 check returned value
 * @step 3 compare setted and getted values
 * </td>
 * <td>Evas_Object *ruler, unsigned int step</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFL_START_TEST(ewe_ruler_step_set_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   unsigned int step;
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_step_set(ruler, NULL, 100) == EINA_TRUE,
                 "Cannot set step for the ruler");
   step = ewe_ruler_step_get(ruler, NULL);
   ck_assert_msg(step == 100,
                 "Getted and setted value are not equal");

   evas_object_del(win);
   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_step_set
 * @{
 * <tr>
 * <td>ewe_ruler_step_set</td>
 * <td>ewe_ruler_step_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_step_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, unsigned int step</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_step_set_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();

   ck_assert_msg(ewe_ruler_step_set(NULL, NULL, 100) == EINA_FALSE, "Setted step to the NULL ruler object");

   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_step_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
