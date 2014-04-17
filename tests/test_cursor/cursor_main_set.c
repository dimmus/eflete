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

#include "test_cursor.h"
#include "config.h"

/**
 * @addtogroup cursor_test
 * @{
 * @addtogroup cursor_main_set
 * @{
 * Cursor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup cursor_main_set
 * @{
 * <tr>
 * <td>cursor_main_set</td>
 * <td>cursor_main_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call cursor_main_set
 * </td>
 * <td>(Cursor_Type) CURSOR_ARROW</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_main_set_test_p1)
{
   elm_init(0,0);

   ck_assert_msg(cursor_main_set(CURSOR_ARROW), "Main cursor is not setted");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_main_set
 * @{
 * <tr>
 * <td>cursor_main_set</td>
 * <td>cursor_main_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call cursor_main_set
 * </td>
 * <td>(Cursor_Type) CURSOR_SYSTEM</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_main_set_test_p2)
{
   elm_init(0,0);

   ck_assert_msg(cursor_main_set(CURSOR_SYSTEM), "Main cursor is not setted");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_main_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
