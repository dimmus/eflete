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
 * </tr>
 * @}
 */
EFLETE_TEST(config_save_test_p)
{
   elm_init(0,0);
   app_init();
   config_load();

   ck_assert_msg(config_save(), "Config not saved.");

   app_shutdown();
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
