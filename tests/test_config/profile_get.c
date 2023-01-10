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
 * </tr>
 * @}
 */
EFL_START_TEST(profile_get_test_p2)
{
   // elm_init(0,0);
   // app_init();

   config_load();
   ck_assert_msg(profile_get() != NULL, "Profile not loaded.");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup profile_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void profile_get_test(TCase *tc)
{
   tcase_add_test(tc, profile_get_test_p2);
}