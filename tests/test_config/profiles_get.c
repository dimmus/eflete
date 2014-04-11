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
 * @addtogroup settings_test
 * @{
 * @addtogroup profiles_get
 * @{
 * Config
 * <TABLE>
 * @}
 */
/**
 * @addtogroup profiles_get
 * @{
 * <tr>
 * <td>profiles_get</td>
 * <td>profiles_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 *
 * @procedure
 * @step 1 call profiles_get
 * </td>
 * <td>void</td>
 * <td>not NULL pointer</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profiles_get_test_p)
{
   Eina_List *profiles;

   elm_init(0,0);
   config_init();
   config_load();

   profiles = profiles_get();
   ck_assert_msg(profiles != NULL, "Config not loaded.");

   EINA_LIST_STRINGSHARE_FREE(profiles);
   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profiles_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
