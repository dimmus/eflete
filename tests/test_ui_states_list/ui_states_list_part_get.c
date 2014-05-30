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

#include "test_ui_states_list.h"

/**
 * @addtogroup ui_states_list_test
 * @{
 * @addtogroup ui_states_list_part_get
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_part_get
 * @{
 * <tr>
 * <td>ui_states_list_part_get</td>
 * <td>ui_states_list_part_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 created States List
 * @step 6 Part which states will be shown and set.
 * @step 7 Part was set into states list.
 *
 * @procedure
 * @step 1 Call function ui_states_list_part_get(gl_states).
 * @step 2 Check returned value.
 * @step 3 Check Part's name.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>Part structure</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_part_get_test_p1)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part, *states_list_part;
   const char *edj = "./edj_build/ui_states_list_part_get.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);
   ui_states_list_data_set(gl_states, style, part);

   states_list_part = ui_states_list_part_get(gl_states);
   ck_assert_msg(states_list_part != NULL,
                 "Part wasn't set.");
   ck_assert_str_eq(part->name, "bg");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_part_get
 * @{
 * <tr>
 * <td>ui_states_list_part_get</td>
 * <td>ui_states_list_part_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 created States List (Part wasn't set)
 *
 * @procedure
 * @step 1 Call function ui_states_list_part_get(gl_states).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_part_get_test_p2)
{
   Evas_Object *window, *gl_states;
   Part *states_list_part;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gl_states = ui_states_list_add(window);

   states_list_part = ui_states_list_part_get(gl_states);
   ck_assert_msg(states_list_part == NULL,
                 "Part was set.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_part_get
 * @{
 * <tr>
 * <td>ui_states_list_part_get</td>
 * <td>ui_states_list_part_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 *
 * @procedure
 * @step 1 Call function ui_states_list_part_get(window).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) window</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_part_get_test_n1)
{
   Evas_Object *window;
   Part *states_list_part;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   states_list_part = ui_states_list_part_get(window);
   ck_assert_msg(states_list_part == NULL,
                 "Wrong object contain part.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_part_get
 * @{
 * <tr>
 * <td>ui_states_list_part_get</td>
 * <td>ui_states_list_part_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_states_list_part_get(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) window</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_part_get_test_n2)
{
   Part *states_list_part;

   elm_init(0,0);

   states_list_part = ui_states_list_part_get(NULL);
   ck_assert_msg(states_list_part == NULL,
                 "Wrong object contain part.");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup ui_states_list_part_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
