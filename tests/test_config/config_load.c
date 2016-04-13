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
 * @addtogroup config_load
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_load
 * @{
 * <tr>
 * <td>config_load</td>
 * <td>config_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create App_Data
 * @step 3 create Eflete window
 *
 * @procedure
 * @step 1 call config_load
 * </td>
 * <td>void</td>
 * <td>Config  will be loaded</td>
 * </tr>
 * @}
 */
EFLETE_TEST(config_load_test_p)
{
   Config *config;

   elm_init(0,0);
   app_init();
   config_load();

   config = config_get();
   ck_assert_msg(config != NULL, "Config not been loaded.");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup config_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
