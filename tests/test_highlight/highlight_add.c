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
 * @addtogroup highlight_add
 * @{
 * highlight
 * <TABLE>
 * @}
 */

/**
 * @addtogroup highlight_add
 * @{
 * <tr>
 * <td>highlight_add</td>
 * <td>highlight_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 *
 * @procedure
 * @step 1 Call function highlight_add(parent).
 * @step 2 Check returned value.
 * </td>
 * <td>Evas_Object *parent</td>
 * <td>Evas_Object returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_add_test_p)
{
   Evas_Object *parent, *highlight;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   highlight = highlight_add(parent);
   ck_assert_msg(highlight != NULL, "Failed to create highlight object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_add
 * @{
 * <tr>
 * <td>highlight_add</td>
 * <td>highlight_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function highlight_add(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>Evas_Object returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_add_test_n)
{
   Evas_Object *highlight;

   elm_init(0, 0);

   highlight = highlight_add(NULL);
   ck_assert_msg(highlight == NULL, "Highlight object was created without parent Evas_Object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
