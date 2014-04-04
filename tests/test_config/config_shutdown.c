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
 * @addtogroup config_shutdown
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_shutdown
 * @{
 * <tr>
 * <td>config_shutdown</td>
 * <td>config_shutdown_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 *
 * @procedure
 * @step 1 call config_shutdown
 * </td>
 * <td>void</td>
 * <td>config is NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(config_shutdown_test_p)
{
   elm_init(0,0);
   config_init();

   config_shutdown();
   ck_assert_msg(config_get() == NULL, "Config not initialized.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup config_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */
