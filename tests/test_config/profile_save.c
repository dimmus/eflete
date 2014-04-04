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
 * @addtogroup profile_save
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup profile_save
 * @{
 * <tr>
 * <td>profile_save</td>
 * <td>profile_save_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 *
 * @procedure
 * @step 1 call profile_save
 * </td>
 * <td>(const char *)"default"</td>
 * <td>Profile will be loaded</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_save_test_p)
{

   elm_init(0,0);
   config_init();
   config_load();

   ck_assert_msg(profile_save("default"), "Profile not saved.");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profile_save
 * @{
 * <tr>
 * <td>profile_save</td>
 * <td>profile_save_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 *
 * @procedure
 * @step 1 call profile_save
 * </td>
 * <td>NULL</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_save_test_n)
{
   elm_init(0,0);
   config_init();

   ck_assert_msg(!profile_save(NULL), "Profile is saved.");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profile_save
 * @{
 * </TABLE>
 * @}
 * @}
 */
