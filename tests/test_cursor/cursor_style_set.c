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
 * @addtogroup cursor_style_set
 * @{
 * Cursor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup cursor_style_set
 * @{
 * <tr>
 * <td>cursor_style_set</td>
 * <td>cursor_style_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 * @step 4 set the cursor "default" theme
 * @step 5 create a new Evas_Object.
 *
 * @procedure
 * @step 1 call cursor_style_set
 * </td>
 * <td>(Evas_Object *)obj, CURSOR_FLUER</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_style_set_test_p)
{
   Evas_Object *obj;

   elm_init(0,0);
   config_init();
   config_load();
   obj = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   cursor_theme_set("default");

   ck_assert_msg(cursor_style_set(obj, CURSOR_FLEUR),
                 "Can not set the cursor to given object");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_style_set
 * @{
 * <tr>
 * <td>cursor_style_set</td>
 * <td>cursor_style_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 * @step 4 set the cursor "default" theme
 * @step 5 create a new Evas_Object.
 *
 * @procedure
 * @step 1 call cursor_style_set
 * </td>
 * <td>(Evas_Object *)obj, (Cursor_Type)-10000</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_style_set_test_n1)
{
   Evas_Object *obj;

   elm_init(0,0);
   config_init();
   config_load();
   obj = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   cursor_theme_set("default");

   ck_assert_msg(!cursor_style_set(obj, (Cursor_Type)-10000),
                 "Set the not valid cursor to giver object");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_style_set
 * @{
 * <tr>
 * <td>cursor_style_set</td>
 * <td>cursor_style_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize config
 * @step 3 load config
 * @step 4 set the cursor "default" theme
 *
 * @procedure
 * @step 1 call cursor_style_set
 * </td>
 * <td>NULL, (Cursor_Type)10</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(cursor_style_set_test_n2)
{
   elm_init(0,0);
   config_init();
   config_load();
   cursor_theme_set("default");

   ck_assert_msg(!cursor_style_set(NULL, CURSOR_ARROW),
                 "Set the cursor to not valid object");

   config_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup cursor_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
