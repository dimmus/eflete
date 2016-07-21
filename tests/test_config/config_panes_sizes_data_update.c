/**
 * Edje Theme Editor
 * Copyright (C) 2013 Samsung Electronics.
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
 * @addtogroup config_panes_sizes_data_update
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_panes_sizes_data_update
 * @{
 * <tr>
 * <td>config_panes_sizes_data_update</td>
 * <td>config_panes_sizes_data_update_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 *
 * @procedure
 * @step 1 Call config_panes_sizes_data_update(NULL)
 * @step 2 Check returned value
 * </td>
 * <td></td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (config_panes_sizes_data_update_test_p)
{
   elm_init(0, 0);
   Eina_Bool result = EINA_FALSE;

   app_init();
   config_load();

   result = config_panes_sizes_data_update();
   ck_assert_msg(result == EINA_TRUE, "Could not update Panes sizes data");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup config_panes_sizes_data_update
 * @{
 * </TABLE>
 * @}
 * @}
 */
