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

#include "test_highlight.h"

/**
 * @addtogroup highlight_test
 * @{
 * @addtogroup highlight_border_color_set
 * @{
 * highlight
 * <TABLE>
 * @}
 */

/**
 * @addtogroup highlight_border_color_set
 * @{
 * <tr>
 * <td>highlight_border_color_set</td>
 * <td>highlight_border_color_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 *
 * @procedure
 * @step 1 Call function highlight_border_color_set(highlight, 100, 200, 255, 255).
 * @step 2 Check returned value.
 * </td>
 * <td>Evas_Object *highlight, int r = 100, int g = 200, int b = 255, int a = 255</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_border_color_set_test_p)
{
   Evas_Object *parent, *highlight;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);

   ck_assert_msg(highlight_border_color_set(highlight, 100, 200, 255, 255), "Failed to set up color to border.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_border_color_set
 * @{
 * <tr>
 * <td>highlight_border_color_set</td>
 * <td>highlight_border_color_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 *
 * @procedure
 * @step 1 Call function highlight_border_color_set(highlight, 600, -3200, 182, 5000).
 * @step 2 Check returned value.
 * </td>
 * <td>Evas_Object *highlight, int r = 600, int g = -3200, int b = 182, int a = 5000</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_border_color_set_test_n1)
{
   Evas_Object *parent, *highlight;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);

   ck_assert_msg(!highlight_border_color_set(highlight, 600, -3200, 182, 5000), "Set up wrong RGBA color.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_border_color_set
 * @{
 * <tr>
 * <td>highlight_border_color_set</td>
 * <td>highlight_border_color_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 another Evas_Object created (Not highlight).
 *
 * @procedure
 * @step 1 Call function highlight_border_color_set(bg, 100, 200, 255, 255).
 * @step 2 Check returned value.
 * </td>
 * <td>Evas_Object *bg, int r = 100, int g = 200, int b = 255, int a = 255</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_border_color_set_test_n2)
{
   Evas_Object *parent, *bg;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   bg = elm_bg_add(parent);

   ck_assert_msg(!highlight_border_color_set(bg, 100, 200, 255, 255), "Set up border color of not highlight object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_border_color_set
 * @{
 * <tr>
 * <td>highlight_border_color_set</td>
 * <td>highlight_border_color_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function highlight_border_color_set(NULL, 100, 200, 255, 255).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, int r = 100, int g = 200, int b = 255, int a = 255</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_border_color_set_test_n3)
{
   elm_init(0, 0);

   ck_assert_msg(!highlight_border_color_set(NULL, 100, 200, 255, 255), "Set up border color of not highlight object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_border_color_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
