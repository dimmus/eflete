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
 * @addtogroup profile_get
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup profile_get
 * @{
 * <tr>
 * <td>profile_get</td>
 * <td>profile_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 initialize config
 *
 * @procedure
 * @step 1 call profile_get
 * </td>
 * <td>void</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_get_test_p1)
{
   elm_init(0,0);
   app_init();
   App_Data *app = app_data_get();
   config_init(app);

   ck_assert_msg(profile_get() == NULL, "Profile not NULL");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profile_get
 * @{
 * <tr>
 * <td>profile_get</td>
 * <td>profile_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 load config
 *
 * @procedure
 * @step 1 call profile_get
 * </td>
 * <td>void</td>
 * <td>not NULL pointer</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(profile_get_test_p2)
{
   elm_init(0,0);
   app_init();
   App_Data *app = app_data_get();
   config_load(app);

   ck_assert_msg(profile_get() != NULL, "Profile not loaded.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup profile_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
