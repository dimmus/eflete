/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
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
* along with this program; If not, see see www.gnu.org/licenses/gpl-2.0.htm.
*/

#include "test_settings.h"

/**
 * @addtogroup settings_test
 * @{
 * @addtogroup ui_element_settings_init
 * @{
 * Settings
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_element_settings_init
 * @{
 * <tr>
 * <td>ui_element_settings_init</td>
 * <td>ui_element_settings_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call ui_element_settings_init
 * </td>
 * <td>void</td>
 * <td>Not NULL pointer</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_element_settings_init_test_p)
{
   elm_init(0,0);

   UI_Elements_Settings *set = ui_element_settings_init();
   ck_assert_msg(set != NULL, "Return NULL pointer.");

   if (set) free(set);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_element_settings_init
 * @{
 * </TABLE>
 * @}
 * @}
 */
