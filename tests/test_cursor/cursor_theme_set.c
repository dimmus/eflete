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
 * @addtogroup cursor_theme_set
 * @{
 * Cursor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup cursor_theme_set
 * @{
 * <tr>
 * <td>cursor_theme_set</td>
 * <td>cursor_theme_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 *
 * @procedure
 * @step 1 call cursor_theme_set
 * </td>
 * <td>(const char *) "default"</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_theme_set_test_p)
{
   elm_init(0,0);
   config_init();
   config_load();

   ck_assert_msg(cursor_theme_set("default"), "Cursors themes not setted");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_theme_set
 * @{
 * <tr>
 * <td>cursor_theme_set</td>
 * <td>cursor_theme_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 *
 * @procedure
 * @step 1 call cursor_theme_set
 * </td>
 * <td>(const char *) "not_valid_name"</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_theme_set_test_n1)
{
   elm_init(0,0);
   config_init();
   config_load();

   ck_assert_msg(!cursor_theme_set("not_valid_name"),
                 "Not valid theme name is setted");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_theme_set
 * @{
 * <tr>
 * <td>cursor_theme_set</td>
 * <td>cursor_theme_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 *
 * @procedure
 * @step 1 call cursor_theme_set
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_theme_set_test_n2)
{
   elm_init(0,0);
   config_init();
   config_load();

   ck_assert_msg(!cursor_theme_set(NULL),
                 "NULL theme is setted");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_theme_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
