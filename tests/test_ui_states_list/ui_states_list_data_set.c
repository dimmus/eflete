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


#include "test_ui_states_list.h"

/**
 * @addgroup test_ui_states_list
 * @{
 * @addtogroup ui_states_list_data_set
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 created States List
 * @step 6 Part which states will be shown and set.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(gl_states, style, part).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states, (Style *) style, (Part *) part</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_p)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);

   ck_assert_msg(ui_states_list_data_set(gl_states, style, part) == EINA_TRUE,
                 "Data not setted to the State List");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 created States List
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(gl_states, style, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states, (Style *) style, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_n1)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_states = ui_states_list_add(window);

   ck_assert_msg(ui_states_list_data_set(gl_states, style, NULL) == EINA_FALSE,
                 "Data was set to the State List");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 created States List
 * @step 6 Part which states will be shown and set.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(gl_states, NULL, part).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states, NULL, (Part *) part</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_n2)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);

   ck_assert_msg(ui_states_list_data_set(gl_states, NULL, part) == EINA_FALSE,
                 "Data was set to the State List");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 Part which states will be shown and set.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(NULL, style, part).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Style *) style, (Part *) part</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_n3)
{
   Evas_Object *window;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);

   ck_assert_msg(ui_states_list_data_set(NULL, style, part) == EINA_FALSE,
                 "Data was set to the State List");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(gl_states, style, part).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_n4)
{
   elm_init(0,0);

   ck_assert_msg(ui_states_list_data_set(NULL, NULL, NULL) == EINA_FALSE,
                 "Data was set to the State List");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_n5</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 Part which states will be shown and set.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(window, style, part).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) window, (Style *) style, (Part *) part</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_n5)
{
   Evas_Object *window;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE);
   wm_style_data_load(style, e, edj);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);

   ck_assert_msg(ui_states_list_data_set(window, style, part) == EINA_TRUE,
                 "Something is wrong with setting data.");
   ck_assert_msg(elm_genlist_items_count(window) == 0,
                 "Data was set into window.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_set
 * @{
 * <tr>
 * <td>ui_states_list_data_set</td>
 * <td>ui_states_list_data_set_test_n6</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Empty Style.
 * @step 5 created States List
 * @step 6 Part which states will be shown and set.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_set(gl_states, style, part).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states, (Style *) style, (Part *) part</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_set_test_n6)
{
   Evas_Object *window, *gl_states;
   Style *style;
   Part *part;
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   style = wm_style_add(style_name, full_style_name, STYLE);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);

   ck_assert_msg(ui_states_list_data_set(gl_states, style, part) == EINA_FALSE,
                 "Data was setted to the State List with empty Style");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup ui_states_list_data_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
