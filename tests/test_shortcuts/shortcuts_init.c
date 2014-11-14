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
 * @addtogroup shortcuts_init
 * @{
 * Shortcuts
 * <TABLE>
 * @}
 */

/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_init
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_p1)
{
   elm_init(0,0);

   ck_assert_msg(shortcuts_init(), "Shortcuts not initialized.");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_init
 * @step 2 call shortcuts_shutdown
 * @step 3 call shortcuts_init
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_p2)
{
   elm_init(0,0);

   ck_assert_msg(shortcuts_init(), "Shortcuts not initialized at first time.");
   shortcuts_shutdown();
   ck_assert_msg(shortcuts_init(), "Shortcuts not initialized at second time.");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_init
 * @{
 * <tr>
 * <td>shortcuts_init</td>
 * <td>shortcuts_init_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call shortcuts_init
 * @step 2 call shortcuts_init
 * </td>
 * <td>void</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_init_test_n)
{
   elm_init(0,0);

   ck_assert_msg(shortcuts_init(), "Shortcuts not initialized at first time.");
   ck_assert_msg(!shortcuts_init(), "Shortcuts initialized after it was already initialized.");

   shortcuts_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_init
 * @{
 * </TABLE>
 * @}
 * @}
 */

