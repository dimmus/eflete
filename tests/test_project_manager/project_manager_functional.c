/**
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "test_project_manager.h"

/**
 * @addtogroup project_manager
 * @{
 * @addtogroup project_manager_functional
 * @{
 * Project manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup Project_manager_functional
 * @{
 * <tr>
 * <td>project_manager</td>
 * <td>project mnager functional</td>
 * <td>
 * @precondition
 * @step 1 NONE
 *
 * @procedure
 * </td>
 * <td>NO args</td>
 * <td>All good and nice</td>
 * </tr>
 * @}
 */
EFL_START_TEST(project_manager_functional)
{
}
EFL_END_TEST

/**
 * @addtogroup logger_init
 * @{
 * </TABLE>
 * @}
 * @}
 *
 */

void project_manager_functional_test(TCase *tc)
{
   tcase_add_test(tc, project_manager_functional);
}