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

#include "test_highlight.h"

/**
 * @addtogroup highlight_test
 * @{
 * @addtogroup highlight_handler_mode_set
 * @{
 * highlight
 * <TABLE>
 * @}
 */

/**
 * @addtogroup highlight_handler_mode_set
 * @{
 * <tr>
 * <td>highlight_handler_mode_set</td>
 * <td>highlight_handler_mode_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 *
 * @procedure
 * @step 1 Call function highlight_handler_mode_set(highlight, HIGHLIGHT_STATIC_HANDLERS).
 * @step 2 Check returned value.
 * @step 3 Call function highlight_handler_mode_set(highlight, HIGHLIGHT_DYNAMIC_HANDLERS).
 * @step 4 Check returned value.
 * </td>
 * <td>Evas_Object *highlight, Highlight_Mode mode = HIGHLIGHT_STATIC_HANDLERS</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_handler_mode_set_test_p)
{
   Evas_Object *parent, *highlight;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);

   ck_assert_msg(highlight_handler_mode_set(highlight, HIGHLIGHT_STATIC_HANDLERS), "Failed to change highlight's mode.");
   ck_assert_msg(highlight_handler_mode_set(highlight, HIGHLIGHT_DYNAMIC_HANDLERS), "Failed to change highlight's mode.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_handler_mode_set
 * @{
 * <tr>
 * <td>highlight_handler_mode_set</td>
 * <td>highlight_handler_mode_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 another Evas_Object created (Not highlight).
 *
 * @procedure
 * @step 1 Call function highlight_handler_mode_set(bg, HIGHLIGHT_STATIC_HANDLERS).
 * @step 2 Check returned value.
 * </td>
 * <td>Evas_Object *bg, Highlight_Mode mode = HIGHLIGHT_STATIC_HANDLERS</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_handler_mode_set_test_n1)
{
   Evas_Object *parent, *bg;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   bg = elm_bg_add(parent);

   ck_assert_msg(!highlight_handler_mode_set(bg, HIGHLIGHT_STATIC_HANDLERS), "Changing mode for not highlight object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_handler_mode_set
 * @{
 * <tr>
 * <td>highlight_handler_mode_set</td>
 * <td>highlight_handler_mode_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 another Evas_Object created (Not highlight).
 *
 * @procedure
 * @step 1 Call function highlight_handler_mode_set(NULL, HIGHLIGHT_STATIC_HANDLERS).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, Highlight_Mode mode = HIGHLIGHT_STATIC_HANDLERS</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_handler_mode_set_test_n2)
{
   elm_init(0, 0);

   ck_assert_msg(!highlight_handler_mode_set(NULL, HIGHLIGHT_STATIC_HANDLERS), "Changing mode for NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_handler_mode_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
