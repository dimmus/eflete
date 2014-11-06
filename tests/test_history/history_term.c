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
 * @addtogroup history_term
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_term
 * @{
 * <tr>
 * <td>history_term</td>
 * <td>history_term_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize history module.
 *
 * @procedure
 * @step 1 Call history_term.
 * @step 2 Check returned value.
 * </td>
 * <td>(History *) history</td>
 * <td>EINA_TRUE value returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_term_test_p)
{
   History *history = NULL;
   Eina_Bool result = EINA_FALSE;
   elm_init(0, 0);
   history = history_init();

   result = history_term(history);
   ck_assert_msg(result, "Failed to terminate history module.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_term
 * @{
 * <tr>
 * <td>history_term</td>
 * <td>history_term_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call history_term.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE value returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_term_test_n)
{
   Eina_Bool result = EINA_TRUE;
   elm_init(0, 0);

   result = history_term(NULL);
   ck_assert_msg(!result, "Terminate history module with NULL pointer object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_term
 * @{
 * </TABLE>
 * @}
 * @}
 */
