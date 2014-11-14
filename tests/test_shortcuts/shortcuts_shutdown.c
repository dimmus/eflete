/*
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

#include "test_shortcuts.h"

/**
 * @addtogroup shortcuts_test
 * @{
 * @addtogroup shortcuts_shutdown
 * @{
 * Shortcuts
 * <TABLE>
 * @}
 */

/**
 * @addtogroup shortcuts_shutdown
 * @{
 * <tr>
 * <td>shortcuts_shutdown</td>
 * <td>shortcuts_shutdown_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_init
 * @step 2 call shortcuts_shutdown
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_shutdown_test_p1)
{
   elm_init(0,0);

   shortcuts_init();
   ck_assert_msg(shortcuts_shutdown(), "Can't shutdown shortcuts.");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_shutdown
 * @{
 * <tr>
 * <td>shortcuts_shutdown</td>
 * <td>shortcuts_shutdown_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_init
 * @step 2 call shortcuts_shutdown
 * @step 3 call shortcuts_init
 * @step 4 call shortcuts_shutdown
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_shutdown_test_p2)
{
   elm_init(0,0);

   shortcuts_init();
   ck_assert_msg(shortcuts_shutdown(), "Can't shutdown shortcuts.");
   shortcuts_init();
   ck_assert_msg(shortcuts_shutdown(), "Can't shutdown shortcuts after second initialization.");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_shutdown
 * @{
 * <tr>
 * <td>shortcuts_shutdown</td>
 * <td>shortcuts_shutdown_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_shutdown
 * </td>
 * <td>void</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_shutdown_test_n1)
{
   elm_init(0,0);

   ck_assert_msg(!shortcuts_shutdown(), "Shutdown not initialized shortcuts was successfull but should not!");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_shutdown
 * @{
 * <tr>
 * <td>shortcuts_shutdown</td>
 * <td>shortcuts_shutdown_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_shutdown
 * </td>
 * <td>void</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_shutdown_test_n2)
{
   elm_init(0,0);

   shortcuts_init();
   ck_assert_msg(shortcuts_shutdown(), "Can't shutdown shortcuts.");
   ck_assert_msg(!shortcuts_shutdown(), "Shutdown not initialized shortcuts was successfull but should not!");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */

