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
 * @addtogroup ui_states_list_data_unset
 * @{
 * ui_states_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_states_list_data_unset
 * @{
 * <tr>
 * <td>ui_states_list_data_unset</td>
 * <td>ui_states_list_data_unset_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 Style filled with data.
 * @step 5 created States List
 * @step 6 Part which states will be shown and set.
 * @step 7 States list is already filled with part data.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_unset(gl_states).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_unset_test_p1)
{
   Evas_Object *window, *gl_states;
   Evas *e;
   Style *style;
   Part *part;
   const char *edj = "./edj_build/ui_states_list_data_unset.edj";
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

   ck_assert_msg(ui_states_list_data_unset(gl_states) == EINA_TRUE,
                 "Data cant be unset from State List");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_unset
 * @{
 * <tr>
 * <td>ui_states_list_data_unset</td>
 * <td>ui_states_list_data_unset_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 * @step 2 add theme extension "eflete theme".
 * @step 3 created Window.
 * @step 4 created States List
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_unset(gl_states).
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) gl_states</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_unset_test_p2)
{
   Evas_Object *window, *gl_states;

   elm_init(0,0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   window = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gl_states = ui_states_list_add(window);

   ck_assert_msg(ui_states_list_data_unset(gl_states) == EINA_TRUE,
                 "Data cant be unset from empty State List");

   elm_theme_extension_del(NULL, EFLETE_THEME);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_unset
 * @{
 * <tr>
 * <td>ui_states_list_data_unset</td>
 * <td>ui_states_list_data_unset_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm.
 *
 * @procedure
 * @step 1 Call function ui_states_list_data_unset(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_states_list_data_unset_test_n)
{
   elm_init(0,0);

   ck_assert_msg(ui_states_list_data_unset(NULL) == EINA_FALSE,
                 "Data was unset from NULL");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_states_list_data_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
