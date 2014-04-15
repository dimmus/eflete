/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup profile_load
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup profile_load
 * @{
 * <tr>
 * <td>profile_load</td>
 * <td>profile_load_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 *
 * @procedure
 * @step 1 call profile_load
 * </td>
 * <td>(const char *)"default"</td>
 * <td>Profile will be loaded</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_load_test_p1)
{
   Profile *profile;

   elm_init(0,0);
   config_init();

   profile_load("default");
   profile = profile_get();
   ck_assert_msg(profile != NULL, "Profile not been loaded.");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profile_load
 * @{
 * <tr>
 * <td>profile_load</td>
 * <td>profile_load_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 *
 * @procedure
 * @step 1 call profile_load
 * </td>
 * <td>(const char *)"not_valid_name"</td>
 * <td>Profile will be created</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_load_test_p2)
{
   Profile *profile;

   elm_init(0,0);
   config_init();

   profile_load("not_valid_name");
   profile = profile_get();
   ck_assert_msg(profile != NULL, "Profile not been loaded.");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profile_load
 * @{
 * <tr>
 * <td>profile_load</td>
 * <td>profile_load_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 *
 * @procedure
 * @step 1 call profile_load
 * </td>
 * <td>NULL</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_load_test_n)
{
   Profile *profile;

   elm_init(0,0);
   config_init();

   profile_load(NULL);
   profile = profile_get();
   ck_assert_msg(profile == NULL, "Profile is loaded.");

   config_shutdown();
   elm_shutdown();
}
END_TEST



/**
 * @addtogroup profile_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
