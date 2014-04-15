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

#include "test_program_editor.h"

/**
 * @addtogroup program_editor_test
 * @{
 * @addtogroup program_editor_window_add
 * @{
 * program_editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup program_editor_window_add
 * @{
 * <tr>
 * <td>program_editor_window_add</td>
 * <td>program_editor_window_add_test_p</td>
 * <td>
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 Style filled with parts
 *
 * @procedure
 * @step 1 Call program_editor_window_add(Style)
 * @step 2 Check returned pointer
 * </td>
 * <td>Style *style</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (program_editor_window_add_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   const char *full_style_name = "elm/radio/base/test";
   Evas_Object *manager, *win;
   App_Data *app;
   Eina_Inlist *widget_list = NULL;
   Evas *e = NULL;
   Style *style;

   app_init();
   app = app_create();
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->win = win;
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   style = wm_style_object_find(widget_list, full_style_name);

   manager = program_editor_window_add(style);
   ck_assert_msg(manager != NULL, "cannot create new Program Editor");

   wm_widget_list_free(widget_list);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup program_editor_window
 * @{
 * <tr>
 * <td>program_editor_window_add</td>
 * <td>program_editor_window_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 Empty new Style
 *
 * @procedure
 * @step 1 Call program_editor_window_add(Style)
 * @step 2 Check returned pointer
 * </td>
 * <td>Style *style</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (program_editor_window_add_test_n1)
{
   elm_init(0,0);
   const char *full_style_name = "elm/radio/base/test";
   const char *style_name = "test";
   Evas_Object *manager, *win;
   App_Data *app;
   Style *style;

   app_init();
   app = app_create();
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->win = win;
   style = wm_style_add(style_name, full_style_name, STYLE);

   manager = program_editor_window_add(style);
   ck_assert_msg(manager == NULL, "Program Editor was successfully created");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup program_editor_window
 * @{
 * <tr>
 * <td>program_editor_window</td>
 * <td>program_editor_window_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call program_editor_window_add(NULL)
 * @step 2 Check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (program_editor_window_add_test_n2)
{
   elm_init(0,0);
   Evas_Object *manager, *win;
   App_Data *app;

   app_init();
   app = app_create();
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->win = win;

   manager = program_editor_window_add(NULL);
   ck_assert_msg(manager == NULL, "Program Editor was successfully created");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup program_editor_window
 * @{
 * </TABLE>
 * @}
 * @}
 */
