/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */
#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_marker_add
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_marker_add
 * @{
 * <tr>
 * <td>ewe_ruler_marker_add</td>
 * <td>ewe_ruler_marker_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize ewe library
 * @step 2 create a parent window
 * @step 3 create ruler
 *
 * @procedure
 * @step 1 create marker
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object * ruler, NULL</td>
 * <td>Not NULL object returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST(ewe_ruler_marker_add_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Evas_Object *ruler, *parent;
   Ewe_Ruler_Marker *result;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ruler = ewe_ruler_add(parent);

   result = ewe_ruler_marker_add(ruler, NULL);
   ck_assert_msg(result != NULL, "Marker not returned");

   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_marker_add
 * @{
 * <tr>
 * <td>ewe_ruler_marker_add</td>
 * <td>ewe_ruler_marker_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize ewe library
 * @step 2 don't create ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_add
 * @step 2 check returned value from function
 * </td>
 * <td> NULL</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_ruler_marker_add_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Ewe_Ruler_Marker *result;

   result = ewe_ruler_marker_add(NULL, NULL);
   ck_assert_msg(result == NULL, "Not NULL returned");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_marker_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
