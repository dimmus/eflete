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
 * @addtogroup history_init
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_init
 * @{
 * <tr>
 * <td>history_init</td>
 * <td>history_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call history_init.
 * @step 2 Check returned pointer.
 * </td>
 * <td>void</td>
 * <td>Not NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_init_test_p)
{
   History *history = NULL;
   elm_init(0, 0);

   history = history_init();
   ck_assert_msg(history != NULL, "Failed to initialize history module.");

   history_term(history);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup history_init
 * @{
 * </TABLE>
 * @}
 * @}
 */
