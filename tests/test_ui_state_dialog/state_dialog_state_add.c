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

#include "test_ui_state_dialog.h"

/**
 * @addtogroup ui_state_dialog_test
 * @{
 * @addtogroup state_dialog_state_add
 * @{
 * ui_state_dialog
 * <TABLE>
 * @}
 */

/**
 * @addtogroup state_dialog_state_add
 * @{
 * <tr>
 * <td>state_dialog_state_add</td>
 * <td>state_dialog_state_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 * @step 5 open edj project
 * @step 6 get a canvas form win
 * @step 7 add and load data of style to the project structure
 * @step 8 get the firs part from style
 * @step 9 add state list to app data
 * @step 10 set state list with data from edj project
 * @step 11 set block state list with data from state_list
 *
 * @procedure
 * @step 1 Call function state_dialog_state_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data</td>
 * <td>Evas_Object *popup</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (state_dialog_state_add_test_p)
{
   elm_init(0, 0);
   App_Data *app_data;
   Evas *canvas;
   Evas_Object *popup, *state_list;
   Style *style = NULL;
   Part *part = NULL;
   const char *name, *path;

   name = "UTC";
   path = "./edj_build/state_dialog_state_add.edj";
   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);
   app_data->project = pm_open_project_edj(name, path);
   canvas = evas_object_evas_get(app_data->win);
   style = wm_style_add("radio", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, canvas, path);
   part = EINA_INLIST_CONTAINER_GET(style->parts, Part);
   state_list = ui_states_list_add(app_data->win);
   ui_states_list_data_set(state_list, style, part);
   ui_block_state_list_set(app_data, state_list);

   popup = state_dialog_state_add(app_data);
   ck_assert_msg(popup != NULL, "Failed to create state dialog");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup state_dialog_state_add
 * @{
 * <tr>
 * <td>state_dialog_state_add</td>
 * <td>state_dialog_state_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 *
 * @procedure
 * @step 1 Call function state_dialog_state_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (state_dialog_state_add_test_n1)
{
   elm_init(0, 0);
   App_Data *app_data;
   Evas_Object *popup;

   app_init();
   app_data = app_create();

   popup = state_dialog_state_add(app_data);
   ck_assert_msg(popup == NULL, "It create popup with 'state dialog' without creating main window");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup state_dialog_state_add
 * @{
 * <tr>
 * <td>state_dialog_state_add</td>
 * <td>state_dialog_state_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call function state_dialog_state_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (state_dialog_state_add_test_n2)
{
   elm_init(0, 0);
   Evas_Object *popup;

   popup = state_dialog_state_add(NULL);
   ck_assert_msg(popup == NULL, "It create popup with 'state dialog' with NULL data");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup state_dialog_state_add
 * @{
 * <tr>
 * <td>state_dialog_state_add</td>
 * <td>state_dialog_state_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 *
 * @procedure
 * @step 1 Call function state_dialog_state_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (state_dialog_state_add_test_n3)
{
   elm_init(0, 0);
   App_Data *app_data;
   Evas_Object *popup;

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);

   popup = state_dialog_state_add(app_data);
   ck_assert_msg(popup == NULL, " Failed to create state dialog");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup state_dialog_state_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
