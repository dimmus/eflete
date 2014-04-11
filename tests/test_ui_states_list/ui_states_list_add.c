/* Edje Theme Editor
* Copyright (C) 2014 Samsung Electronics.
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


#include "test_ui_states_list.h"

/**
 * @addgroup test_ui_states_list
 * @{
 * @addtogroup ui_states_list_add
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_add
 * @{
 * <tr>
 * <td>ui_states_list_add</td>
 * <td>ui_states_list_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 call ui_states_list_add
 * @step 2 check returned pointer
 * </td>
 * <td>(Evas_Object *) window</td>
 * <td>Returned pointer not NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_add_test_p)
{
   Evas_Object *window, *states;

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   states = ui_states_list_add(window);
   ck_assert_msg(states != NULL,
                 "States List not created.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_add
 * @{
 * <tr>
 * <td>ui_states_list_add</td>
 * <td>ui_states_list_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call ui_states_list_add
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>Returned pointer is NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_add_test_n)
{
   elm_init(0,0);

   ck_assert_msg(ui_states_list_add(NULL) == NULL,
                 "States List created with parent NULL.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
