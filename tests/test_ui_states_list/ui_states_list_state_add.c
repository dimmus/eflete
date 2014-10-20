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

#include "test_ui_states_list.h"

/**
 * @addtogroup ui_states_list_test
 * @{
 * @addtogroup ui_states_list_state_add
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_state_add
 * @{
 * <tr>
 * <td>ui_states_list_state_add</td>
 * <td>ui_states_list_state_add_test_p</td>
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
 * @step 1 Get number of states in states list.
 * @step 2 Call function ui_states_list_state_add(gl_states, "new_state").
 * @step 3 Check new number of states in states list.
 * </td>
 * <td>(Evas_Object *) gl_states, (const char *) state = "new_state"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_state_add_test_p)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/ui_states_list_state_add.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   const char *state = "new_state";
   int count_before, count_after;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);
   ui_states_list_data_set(gl_states, style, part);

   count_before = elm_genlist_items_count(gl_states);
   ck_assert_msg(ui_states_list_state_add(gl_states, state) == EINA_TRUE,
                 "Can't add new state.");
   count_after = elm_genlist_items_count(gl_states);
   ck_assert_msg(count_before + 1 == count_after,
                 "New state was added incorrectly.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_state_add
 * @{
 * <tr>
 * <td>ui_states_list_state_add</td>
 * <td>ui_states_list_state_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 *
 * @procedure
 * @step 1 Get number of states in not states list.
 * @step 2 Call function ui_states_list_state_add(gl_states, "new_state").
 * @step 3 Check that number of items in not states list wasn't changed.
 * @step 4 Check that there is no items at all.
 * </td>
 * <td>(Evas_Object *) gl_states, (const char *) state = "new_state"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_state_add_test_n1)
{
   Evas_Object *window;
   const char *state = "new_state";
   int count_before, count_after;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   count_before = elm_genlist_items_count(window);
   ck_assert_msg(ui_states_list_state_add(window, state) == EINA_TRUE,
                 "Can't add new state.");
   count_after = elm_genlist_items_count(window);
   ck_assert_msg(count_before == count_after,
                 "New state was added incorrectly.");
   ck_assert_msg(count_after == 0,
                 "There is some items in window.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_state_add
 * @{
 * <tr>
 * <td>ui_states_list_state_add</td>
 * <td>ui_states_list_state_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_states_list_state_add(gl_states, "new_state").
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (const char *) state = "new_state"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_state_add_test_n2)
{
   const char *state = "new_state";

   elm_init(0,0);

   ck_assert_msg(ui_states_list_state_add(NULL, state) == EINA_FALSE,
                 "New state added successfully.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_state_add
 * @{
 * <tr>
 * <td>ui_states_list_state_add</td>
 * <td>ui_states_list_state_add_test_n3</td>
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
 * @step 1 Get number of states in states list.
 * @step 2 Call function ui_states_list_state_add(gl_states, NULL).
 * @step 3 Check that number of states wasn't changed.
 * </td>
 * <td>(Evas_Object *) gl_states, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_state_add_test_n3)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/ui_states_list_state_add.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   int count_before, count_after;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, edj);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);
   ui_states_list_data_set(gl_states, style, part);

   count_before = elm_genlist_items_count(gl_states);
   ck_assert_msg(ui_states_list_state_add(gl_states, NULL) == EINA_FALSE,
                 "New state added successfully.");
   count_after = elm_genlist_items_count(gl_states);
   ck_assert_msg(count_before == count_after,
                 "New state was added.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup ui_states_list_state_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
