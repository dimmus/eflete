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
 * @addtogroup ui_states_list_selected_state_get
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_selected_state_get
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_get</td>
 * <td>ui_states_list_selected_state_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Mmap edj file.
 * @step 5 Style filled with data.
 * @step 6 created States List
 * @step 7 Part which states will be shown and set.
 * @step 8 Part was set into states list.
 * @step 9 Select third element of states list.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_get(gl_states).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>"disabled 0.00" returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_get_test_p1)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/ui_states_list_selected_state_get.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   Eina_File *mmap_file = NULL;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(edj, EINA_FALSE);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts->next->next, Part);
   ui_states_list_data_set(gl_states, style, part);
   ui_states_list_state_add(gl_states, "new state 0.00");

   ck_assert_str_eq(ui_states_list_selected_state_get(gl_states), "new state 0.00");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_get
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_get</td>
 * <td>ui_states_list_selected_state_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Mmap edj file.
 * @step 4 Style filled with data.
 * @step 5 created States List
 * @step 6 Part which states will be shown and set.
 * @step 7 Part was set into states list.
 * @step 8 Select first element of states list.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_get(gl_states).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>"default 0.00" returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_get_test_p2)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   Elm_Object_Item *eoi;
   const char *edj = "./edj_build/ui_states_list_selected_state_get.edj";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";
   Eina_File *mmap_file = NULL;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(edj, EINA_FALSE);
   e = evas_object_evas_get(window);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   gl_states = ui_states_list_add(window);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);
   ui_states_list_data_set(gl_states, style, part);

   eoi = elm_genlist_first_item_get(gl_states);
   elm_genlist_item_selected_set(eoi, EINA_TRUE);
   ck_assert_str_eq(ui_states_list_selected_state_get(gl_states), "default 0.00");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_get
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_get</td>
 * <td>ui_states_list_selected_state_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 created Window.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_get(window).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) window</td>
 * <td>NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_get_test_n1)
{
   Evas_Object *window;

   elm_init(0,0);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(ui_states_list_selected_state_get(window) == NULL,
                 "Got selected state from window!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_get
 * @{
 * <tr>
 * <td>ui_states_list_selected_state_get</td>
 * <td>ui_states_list_selected_state_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_states_list_selected_state_get(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_selected_state_get_test_n2)
{
   elm_init(0,0);

   ck_assert_msg(ui_states_list_selected_state_get(NULL) == NULL,
                 "Got selected state from Nothing!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_selected_state_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
