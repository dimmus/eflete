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

#include "test_history.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_clear
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_clear
 * @{
 * <tr>
 * <td>history_clear</td>
 * <td>history_clear_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize history module.
 *
 * @procedure
 * @step 1 Call history_clear.
 * @step 2 Check returned value.
 * </td>
 * <td>(History *) history</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_clear_test_p)
{
   History *history = NULL;
   Eina_Bool result = EINA_FALSE;
   elm_init(0, 0);
   history = history_init();

   result = history_clear(history);
   ck_assert_msg(result, "Failed to clear history.");

   history_term(history);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_clear
 * @{
 * </TABLE>
 * @}
 * @}
 */
