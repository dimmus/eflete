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

#include "test_ui_main_window.h"

/**
 * @addtogroup ui_main_window_test
 * @{
 * @addtogroup ui_edj_load
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_edj_load
 * @{
 * <tr>
 * <td>ui_edj_load</td>
 * <td>ui_edj_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 *
 * @procedure
 * @step 1 Call function ui_edj_load(app_data, "./edj_build/ui_edj_load.edj").
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data, (const char *)path = "./edj_build/ui_edj_load.edj"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_edj_load_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool result;
   App_Data *app_data;
   const char *path = "./edj_build/ui_edj_load.edj";

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);

   result = ui_edj_load(app_data, path);
   ck_assert_msg(result == EINA_TRUE, "Could not load EDJ file");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load
 * @{
 * <tr>
 * <td>ui_edj_load</td>
 * <td>ui_edj_load_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 *
 * @procedure
 * @step 1 Call function ui_edj_load(app_data, "./edj_build/Makefile").
 * @step 2 Check returned value.
 * @step 3 Call function ui_edj_load(app_data, "./edj_build/radio2.png").
 * @step 4 Check returned value.
 * @step 5 Call function ui_edj_load(app_data, "./edj_build/find_me_I_am_exist.edj").
 * @step 6 Check returned value.
 * @step 7 Call function ui_edj_load(app_data, NULL).
 * @step 8 Check returned value.
 * @step 9 Call function ui_edj_load(NULL, NULL).
 * @step 10 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_edj_load_test_n)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Eina_Bool result;
   App_Data *app_data;
   const char *path1 = "./edj_build/Makefile";
   const char *path2 = "./edj_build/radio2.png";
   const char *path3 = "./edj_build/find_me_I_am_exist.edj";

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);

   result = ui_edj_load(app_data, path1);
   ck_assert_msg(result != EINA_TRUE, "Wrong file was loaded");
   result = ui_edj_load(app_data, path2);
   ck_assert_msg(result != EINA_TRUE, "Wrong file was loaded");
   result = ui_edj_load(app_data, path3);
   ck_assert_msg(result != EINA_TRUE, "Unexisted file was loaded");
   result = ui_edj_load(app_data, NULL);
   ck_assert_msg(result != EINA_TRUE, "Something was loaded");
   result = ui_edj_load(NULL, NULL);
   ck_assert_msg(result != EINA_TRUE, "Something was loaded");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
