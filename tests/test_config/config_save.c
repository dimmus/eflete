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

#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_save
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_save
 * @{
 * <tr>
 * <td>config_save</td>
 * <td>config_save_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 init config
 * @step 3 load config
 * @step 4 create App_Data
 * @step 5 create Eflete window
 *
 * @procedure
 * @step 1 call config_save
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(config_save_test_p)
{
   App_Data *ap;

   elm_init(0,0);
   ap = app_create();
   app_init();
   config_init();
   config_load();
   ui_main_window_add(ap);

   ck_assert_msg(config_save(ap), "Config not saved.");

   config_shutdown();
   app_free(ap);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup config_save
 * @{
 * </TABLE>
 * @}
 * @}
 */
