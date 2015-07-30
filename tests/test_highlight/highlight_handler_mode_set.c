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
 * </TABLE>
 * @}
 * @}
 */
