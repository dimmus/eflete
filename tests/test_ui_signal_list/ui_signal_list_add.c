/* Edje Theme Editor
 * Copyright (C) 2013 Samsung Electronics.
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

#include "test_ui_signal_list.h"

/**
 * @addtogroup ui_state_list_test
 * @{
 * @addtogroup ui_state_list_add
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_signal_list_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 * @step 2 created widget Window
 *
 * @procedure
 * @step 1 Call function ui_signal_list_add(window).
 * @step 2 Check returned value.
 *
 * @passcondition not NULL pointer returned
 * @}
 */
EFLETE_TEST(ui_signal_list_add_test_p)
{
   elm_init(0,0);
   Evas_Object *win;
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ck_assert_msg(ui_signal_list_add(win) != NULL, "Signal List can't be created.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function ui_signal_list_add(NULL).
 * @step 2 Check returned value.
 *
 * @passcondition NULL pointer returned
 * @}
 */
EFLETE_TEST(ui_signal_list_add_test_n)
{
   elm_init(0,0);
   ck_assert_msg(ui_signal_list_add(NULL) == NULL, "Signal List created with parent NULL.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_state_list_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
