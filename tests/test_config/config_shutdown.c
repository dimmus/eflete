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
 *
 * @procedure
 * @step 1 call config_shutdown
 * </td>
 * <td>void</td>
 * <td>config is NULL</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_shutdown_test_p)
{
   // elm_init(0,0);
   // app_init();

   config_shutdown();
   ck_assert_msg(config_get() == NULL, "Config not initialized.");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_shutdown_test(TCase *tc)
{
   tcase_add_test(tc, config_shutdown_test_p);
}
