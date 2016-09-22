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

#include "test_change.h"

/**
 * @addtogroup change_test
 * @{
 * @addtogroup change_free
 * @{
 * Change
 * <TABLE>
 * @}
 */

/**
 * @addtogroup change_free
 * @{
 * <tr>
 * <td>change_free</td>
 * <td>change_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 add change
 *
 * @procedure
 * @step 1 call change_free
 * </td>
 * <td>Change *change</td>
 * <td>Not aborted</td>
 * </tr>
 * @}
 */
EFLETE_TEST (change_free_test_p)
{
   Change *change;
   eina_init();
   change = change_add("test");

   change_free(change);

   eina_shutdown();
}
END_TEST

/**
 * @addtogroup change_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
