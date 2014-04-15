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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_style_free
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_style_free
 * @{
 * <tr>
 * <td>wm_style_free</td>
 * <td>wm_style_free_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 Fill Style with parts.
 *
 * @procedure
 * @step 1 Call function wm_style_free(style).
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_free_test_p1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL;
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj", "elm/radio/base/def");
   wm_style_data_load(style, e, "./edj_build/radio.edj");
   ck_assert_msg(wm_style_free(style) == EINA_TRUE, "cannot delete Style object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free
 * @{
 * <tr>
 * <td>wm_style_free</td>
 * <td>wm_style_free_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Empty Style structure.
 *
 * @procedure
 * @step 1 Call function wm_style_free(style).
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_free_test_p2)
{
   elm_init(0,0);
   Style *style = NULL;
   const char *style_name = "defaul";
   const char *full_style_name = "elm/check/base/defaul";
   style = wm_style_add(style_name, full_style_name, STYLE);
   ck_assert_msg(wm_style_free(style) == EINA_TRUE, "cannot delete Group object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free
 * @{
 * <tr>
 * <td>wm_style_free</td>
 * <td>wm_style_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_free(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_style_free(NULL) == EINA_FALSE, "Deleting NULL as Group object.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
