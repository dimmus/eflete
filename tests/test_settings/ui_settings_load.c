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
 * @addtogroup ui_settings_load
 * @{
 * Settings
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_settings_load
 * @{
 * <tr>
 * <td>ui_settings_load</td>
 * <td>ui_settings_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create App_Data
 * @step 3 create Eflete window
 *
 * @procedure
 * @step 1 call ui_settings_load
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_settings_load_test_p)
{
   elm_init(0,0);
   App_Data *app = app_create();
   app_init();
   ui_main_window_add(app);

   ck_assert_msg(ui_settings_load(), "UI setting not been loaded.");

   app_free(app);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_settings_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
