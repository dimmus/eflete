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
 * @addtogroup settings_test
 * @{
 * @addtogroup config_init
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_init
 * @{
 * <tr>
 * <td>config_init</td>
 * <td>config_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call config_init
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(config_init_test_p)
{
   elm_init(0,0);

   ck_assert_msg(config_init(), "Config not initialized.");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup config_init
 * @{
 * </TABLE>
 * @}
 * @}
 */
