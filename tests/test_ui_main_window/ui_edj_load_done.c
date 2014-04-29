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

#include "test_ui_main_window.h"

/**
 * @addtogroup ui_main_window_test
 * @{
 * @addtogroup ui_edj_load_done
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_edj_load_done
 * @{
 * <tr>
 * <td>ui_edj_load_done</td>
 * <td>ui_edj_load_done_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 *
 * @procedure
 * @step 1 Call function ui_edj_load_done(app_data, "./edj_build/ui_edj_load_done.edj").
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data, (const char *)path = "./edj_build/ui_edj_load_done.edj"</td>
 * <td>Not NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_edj_load_done_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list;
   App_Data *app_data;
   const char *path = "./edj_build/ui_edj_load_done.edj";

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);

   widget_list = ui_edj_load_done(app_data, path);
   ck_assert_msg(widget_list != NULL, "Could not load EDJ file");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * <tr>
 * <td>ui_edj_load_done</td>
 * <td>ui_edj_load_done_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 *
 * @procedure
 * @step 1 Call function ui_edj_load_done(app_data, "./edj_build/Makefile").
 * @step 2 Check returned value.
 * @step 3 Call function ui_edj_load_done(app_data, "./edj_build/radio2.png").
 * @step 4 Check returned value.
 * @step 5 Call function ui_edj_load_done(app_data, "./edj_build/find_me_I_am_exist.edj").
 * @step 6 Check returned value.
 * @step 7 Call function ui_edj_load_done(app_data, NULL).
 * @step 8 Check returned value.
 * @step 9 Call function ui_edj_load_done(NULL, NULL).
 * @step 10 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_edj_load_done_test_n)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list;
   App_Data *app_data;
   const char *path1 = "./edj_build/Makefile";
   const char *path2 = "./edj_build/radio2.png";
   const char *path3 = "./edj_build/find_me_I_am_exist.edj";

   app_init();
   app_data = app_create();
   ui_main_window_add(app_data);

   widget_list = ui_edj_load_done(app_data, path1);
   ck_assert_msg(widget_list == NULL, "Wrong file was loaded");
   widget_list = ui_edj_load_done(app_data, path2);
   ck_assert_msg(widget_list == NULL, "Wrong file was loaded");
   widget_list = ui_edj_load_done(app_data, path3);
   ck_assert_msg(widget_list == NULL, "Unexisted file was loaded");
   widget_list = ui_edj_load_done(app_data, NULL);
   ck_assert_msg(widget_list == NULL, "Something was loaded");
   widget_list = ui_edj_load_done(NULL, NULL);
   ck_assert_msg(widget_list == NULL, "Something was loaded");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * </TABLE>
 * @}
 * @}
 */
