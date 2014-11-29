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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_program_signals_list_free
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_program_signals_list_free
 * @{
 * <tr>
 * <td>wm_program_signals_list_free</td>
 * <td>wm_program_signals_list_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas from created win.
 * @step 3 Style object, filled with data and containing Edje Edit object.
 * @step 4 Tested group contain programs and signals
 * @step 5 Signal List.
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_free(signals_list).
 * @step 2 Check returned value.
 * </td>
 * <td>Eina_List *sig_list</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_program_signals_list_free_test_p)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL;
   const char *edj = "./edj_build/wm_program_signals_list_free.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   Eina_List *sig_list = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   sig_list = wm_program_signals_list_get(style);
   ck_assert_msg(wm_program_signals_list_free(sig_list) == EINA_TRUE, "Cannot free signal list.");

   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signals_list_free
 * @{
 * <tr>
 * <td>wm_program_signals_list_free</td>
 * <td>wm_program_signals_list_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_free(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_program_signals_list_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_program_signals_list_free(NULL) == EINA_FALSE, "NULL signal list was deleted.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signals_list_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
