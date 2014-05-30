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
 * @addtogroup wm_program_signal_list_get
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_program_signal_list_get
 * @{
 * <tr>
 * <td>wm_program_signal_list_get</td>
 * <td>wm_program_signals_list_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Style object, filled with data and containing Edje Edit object.
 * @step 4 Tested group contain few programs with different signals
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(style).
 * @step 2 Check returned Signal List.
 * @step 3 Check Signal List's information.
 * </td>
 * <td>Style *style</td>
 * <td>All check's passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_program_signals_list_get_test_p1)
{
   elm_init(0,0);
   Evas_Object *win;
   Evas *e;
   Style *style = NULL;
   const char *edj = "./edj_build/wm_program_signals_list_get.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   Eina_List *sig_list;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   sig_list = wm_program_signals_list_get(style);
   ck_assert_msg(sig_list != NULL, "Signals list isn't exist.");
   ck_assert_str_eq((char *)sig_list->next->data, "elm,state,radio,on");

   wm_program_signals_list_free(sig_list);
   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get
 * @{
 * <tr>
 * <td>wm_program_signal_list_get</td>
 * <td>wm_program_signals_list_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Empty Style object.
 * @step 4 Tested group doesn't contain any programs and signals
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(style).
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_program_signals_list_get_test_p2)
{
   elm_init(0,0);
   Evas_Object *win;
   Evas *e;
   Style *style = NULL;
   const char *edj = "./edj_build/wm_program_signals_list_get.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/notbase/test";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   ck_assert_msg(wm_program_signals_list_get(style) == NULL, "Style returned data that should don't exist.");

   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get
 * @{
 * <tr>
 * <td>wm_program_signal_list_get</td>
 * <td>wm_program_signals_list_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Empty Style object.
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(style).
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_program_signals_list_get_test_n1)
{
   elm_init(0,0);
   Style *style = NULL;
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/test";

   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   ck_assert_msg(wm_program_signals_list_get(style) == NULL, "Style returned data that should don't exist.");

   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signal_list_get
 * @{
 * <tr>
 * <td>wm_program_signal_list_get</td>
 * <td>wm_program_signals_list_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_program_signals_list_get(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_program_signals_list_get_test_n2)
{
   elm_init(0,0);
   ck_assert_msg(wm_program_signals_list_get(NULL) == NULL, "Data returned from null pointer group.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_program_signals_list_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
