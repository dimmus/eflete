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
 * @addtogroup change_add
 * @{
 * Change
 * <TABLE>
 * @}
 */

/**
 * @addtogroup change_add
 * @{
 * <tr>
 * <td>change_add</td>
 * <td>change_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 *
 * @procedure
 * @step 1 call change_add
 * </td>
 * <td>"test"</td>
 * <td>not NULL returned</td>
 * </tr>
 * @}
 */
EFL_START_TEST (change_add_test_p)
{
   // eina_init();

   ck_assert(change_add("test") != NULL);
   ck_assert(change_add(NULL) != NULL);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup change_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

void change_add_test(TCase *tc)
{
   tcase_add_test(tc, change_add_test_p);
}
