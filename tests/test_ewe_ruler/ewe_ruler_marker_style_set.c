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
 * @addtogroup ewe_ruler_marker_style_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_marker_style_set
 * @{
 * <tr>
 * <td>ewe_ruler_marker_style_set</td>
 * <td>ewe_ruler_marker_style_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add marker to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_style_set(ruler, marker, "pointer")
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that value is setted
 * @step 4 call ewe_ruler_marker_style_set(ruler, marker, "default")
 * @step 5 check that returned value is EINA_TRUE
 * @step 6 check that value is setted
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Marker* marker</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_ruler_marker_style_set_test_p)
{
   logger_init();
   app_init();
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Marker *marker = ewe_ruler_marker_add(ruler, NULL);

   ck_assert_msg(ewe_ruler_marker_style_set(ruler, marker, "pointer") == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_str_eq(ewe_ruler_marker_style_get(ruler, marker), "pointer");
   ck_assert_msg(ewe_ruler_marker_style_set(ruler, marker, "default") == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_str_eq(ewe_ruler_marker_style_get(ruler, marker), "default");

   evas_object_del(win);
   app_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_marker_style_set
 * @{
 * <tr>
 * <td>ewe_ruler_stle_set</td>
 * <td>ewe_ruler_marker_style_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_style_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_ruler_marker_style_set_test_n)
{
   logger_init();
   app_init();

   ck_assert_msg(ewe_ruler_marker_style_set(NULL, NULL, "pointer") == EINA_FALSE, "Setted style for NULL ruler object");

   app_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_ruler_marker_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
