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

#include "test_ui_signal_list.h"

/**
 * @addtogroup ui_signal_list_test
 * @{
 * @addtogroup ui_signal_list_data_unset
 * @{
 * Signal List
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_signal_list_data_unset
 * @{
 * <tr>
 * <td>ui_signal_list_data_unset</td>
 * <td>ui_signal_list_data_unset_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data. (style contain programs and signals)
 * @step 4 created Signal List
 * @step 5 set the Style to the Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_unset.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals</td>
 * <td>Returned pointer must much with setted.</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_unset_test_p1)
{
   Evas_Object *window, *gl_signals;
   Evas *e;
   Style *style, *ret_style;
   const char *edj = "./edj_build/ui_signal_list_data_unset.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   gl_signals = ui_signal_list_add(window);
   ui_signal_list_data_set(gl_signals, style);
   ret_style = ui_signal_list_data_unset(gl_signals);
   ck_assert_msg(ret_style == style,
                 "Returned style do not matched with setted style.");

   wm_style_free(style);
   evas_object_del(window);
   elm_shutdown();

}
END_TEST

/**
 * @addtogroup ui_signal_list_data_unset
 * @{
 * <tr>
 * <td>ui_signal_list_data_unset</td>
 * <td>ui_signal_list_data_unset_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_unset
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_unset_test_p2)
{
   Evas_Object *window, *gl_signals;

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gl_signals = ui_signal_list_add(window);
   ck_assert_msg(ui_signal_list_data_unset(gl_signals) == NULL,
                 "Unsetted the Signal List data not NULL");

   evas_object_del(window);
   elm_shutdown();

}
END_TEST

/**
 * @addtogroup ui_signal_list_data_unset
 * @{
 * <tr>
 * <td>ui_signal_list_data_unset</td>
 * <td>ui_signal_list_data_unset_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_unset
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_unset_test_n)
{
   elm_init(0,0);

   ck_assert_msg(ui_signal_list_data_unset(NULL) == NULL,
                 "Unsetted the Signal List data not NULL");

   elm_shutdown();

}
END_TEST


/**
 * @addtogroup ui_signal_list_data_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
