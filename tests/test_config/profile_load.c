/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
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
 * @step 2 initialize application data
 *
 * @procedure
 * @step 1 call profile_load
 * </td>
 * <td>(const char *)"default"</td>
 * <td>Profile will be loaded</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_load_test_p1)
{
   Profile *profile;

   elm_init(0,0);
   app_init();

   profile_load("default");
   profile = profile_get();
   ck_assert_msg(profile != NULL, "Profile not been loaded.");

   config_shutdown(app_data_get());
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
 * @step 2 initialize application data
 *
 * @procedure
 * @step 1 call profile_load
 * </td>
 * <td>(const char *)"not_valid_name"</td>
 * <td>Profile will be created</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_load_test_p2)
{
   Profile *profile;

   elm_init(0,0);
   app_init();

   profile_load("not_valid_name");
   profile = profile_get();
   ck_assert_msg(profile != NULL, "Profile not been loaded.");

   config_shutdown(app_data_get());
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
 * @step 2 initialize application data
 *
 * @procedure
 * @step 1 call profile_load
 * </td>
 * <td>NULL</td>
 * <td>NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_load_test_n)
{
   Profile *profile;

   elm_init(0,0);
   app_init();

   profile_load(NULL);
   profile = profile_get();
   ck_assert_msg(profile == NULL, "Profile is loaded.");

   config_shutdown(app_data_get());
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
