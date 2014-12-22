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
 * @addtogroup ui_states_list_selected_state_del
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_selected_state_del
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_del</td>
 * <td>ui_states_list_selected_state_del_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Mmap edj file.
 * @step 5 Style filled with data.
 * @step 6 created States List
 * @step 7 Part which states will be shown and set (contain four states).
 * @step 8 Part was set into states list.
 * @step 9 Select third state in states list.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_del(gl_states).
 * @step 2 Check returned value.
 * @step 3 Check new number of states.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_del_test_p)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   Elm_Object_Item *eoi;
   const char *edj = "./edj_build/radio.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   int count_before, count_after;
   Eina_File *mmap_file = NULL;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(window);
   mmap_file = eina_file_open(edj, EINA_FALSE);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts->next->next, Part);
   ui_states_list_data_set(gl_states, style, part);
   eoi = elm_genlist_first_item_get(gl_states);
   eoi = elm_genlist_item_next_get(eoi);
   eoi = elm_genlist_item_next_get(eoi);
   elm_genlist_item_selected_set(eoi, EINA_TRUE);

   count_before = elm_genlist_items_count(gl_states);
   ck_assert_msg(ui_states_list_selected_state_del(gl_states) == EINA_TRUE,
                 ".");
   count_after = elm_genlist_items_count(gl_states);
   ck_assert_msg(count_before - 1 == count_after,
                 "New state was deleted incorrectly.");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_del
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_del</td>
 * <td>ui_states_list_selected_state_del_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_del(window).
 * @step 2 Check returned value.
 * @step 3 Check new number of states.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_del_test_n1)
{
   Evas_Object *window;

   elm_init(0,0);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(ui_states_list_selected_state_del(window) == EINA_FALSE,
                 "Deleting state from not state list was successfull.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_del
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_del</td>
 * <td>ui_states_list_selected_state_del_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_del(window).
 * @step 2 Check returned value.
 * @step 3 Check new number of states.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_del_test_n2)
{
   elm_init(0,0);

   ck_assert_msg(ui_states_list_selected_state_del(NULL) == EINA_FALSE,
                 "Deleting state from not state list was successfull.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
