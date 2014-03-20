/* Edje Theme Editor
 * Copyright (C) 2013 Samsung Electronics.
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

#include "test_ui_signal_list.h"
#include "widget_manager.h"

/**
 * @addtogroup ui_signal_list_test
 * @{
 * @addtogroup ui_signal_list_data_set
 * @{
 * Signal List
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data. (style contain programs and signals)
 * @step 4 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals, (Style *) style</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_p1)
{
   Evas_Object *window, *gl_signals;
   Evas *e;
   Style *style;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_signals = ui_signal_list_add(window);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, style) == EINA_TRUE,
                 "Data not setted to the Signal List");

   wm_style_free(style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data. (style doesn't contain any programs and signals)
 * @step 4 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals, (Style *) style</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_p2)
{
   Evas_Object *window, *gl_signals;
   Evas *e;
   Style *style;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/notbase/test";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_signals = ui_signal_list_add(window);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, style) == EINA_TRUE,
                 "Data not setted to the Signal List");

   wm_style_free(style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_p3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data.
 * @step 4 created Signal List.
 * @step 5 Signal List filled with data already.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set with same style.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals, (Style *) style</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_p3)
{
   Evas_Object *window, *gl_signals;
   Evas *e;
   Style *style;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_signals = ui_signal_list_add(window);
   ui_signal_list_data_set(gl_signals, style);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, style) == EINA_TRUE,
                 "Data not setted to the Signal List");

   wm_style_free(style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_p4</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data.
 * @step 4 another Style. (style doesn't contain any programs or signals)
 * @step 5 created Signal List.
 * @step 6 Signal List filled with data already.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set with another style
 *         (that means, the style without any programs or signals).
 * @step 2 Check returned value.
 *
 * </td>
 * <td>(Evas_Object *) gl_signals, (Style *) style</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_p4)
{
   Evas_Object *window, *gl_signals;
   Evas *e;
   Style *style, *another_style;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   const char *another_style_name = "default";
   const char *another_full_style_name = "elm/radio/base/test";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   another_style = wm_style_add(another_style_name, another_full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   wm_style_data_load(another_style, e, edj);
   gl_signals = ui_signal_list_add(window);
   ui_signal_list_data_set(gl_signals, style);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, another_style) == EINA_TRUE,
                 "Data not resetted to the Signal List");

   wm_style_free(style);
   wm_style_free(another_style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data. (style contain programs and signals)
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set().
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Style *) style</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_n1)
{
   Evas_Object *window;
   Evas *e;
   Style *style;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);

   ck_assert_msg(ui_signal_list_data_set(NULL, style) == EINA_FALSE,
                 "Successfull set the data to NULL object.");

   wm_style_free(style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_n2)
{
   Evas_Object *window, *gl_signals;

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gl_signals = ui_signal_list_add(window);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, NULL) == EINA_FALSE,
                 "Successfull set the NULL data to the Signal List.");

   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(NULL, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_n3)
{
   elm_init(0,0);

   ck_assert_msg(ui_signal_list_data_set(NULL, NULL) == EINA_FALSE,
                 "Successfull set the NULL data to the NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 empty Style
 * @step 4 created Signal List
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set(gl_signals, group).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals, (Style *) style</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_n4)
{
   Evas_Object *window, *gl_signals;
   Style *style;

   elm_init(0,0);
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   style = wm_style_add(group_name, full_group_name, STYLE);
   gl_signals = ui_signal_list_add(window);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, style) == EINA_FALSE,
                 "Successfull set the not preloaded data to the Signal List.");

   wm_style_free(style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * <tr>
 * <td>ui_signal_list_data_set</td>
 * <td>ui_signal_list_data_set_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 * @step 3 Style filled with data.
 * @step 4 another empty Style.
 * @step 5 created Signal List.
 * @step 6 Signal List filled with data already.
 *
 * @procedure
 * @step 1 Call function ui_signal_list_data_set with another not preloaed group.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_signals, (Style *) style</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_signal_list_data_set_test_n5)
{
   Evas_Object *window, *gl_signals;
   Evas *e;
   Style *style, *another_style;
   const char *edj = "./edj_build/radio.edj";
   const char *group_name = "def";
   const char *full_group_name = "elm/radio/base/def";
   const char *another_group_name = "default";
   const char *another_full_group_name = "elm/radio/base/test";

   elm_init(0,0);

   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(group_name, full_group_name, STYLE);
   another_style = wm_style_add(another_group_name, another_full_group_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_signals = ui_signal_list_add(window);
   ui_signal_list_data_set(gl_signals, style);
   ck_assert_msg(ui_signal_list_data_set(gl_signals, another_style) == EINA_FALSE,
                 "Successfull set the not preloaded data to the Signal List.");

   wm_style_free(style);
   wm_style_free(another_style);
   evas_object_del(window);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list_data_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
