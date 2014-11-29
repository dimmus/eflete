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

#include "test_animator.h"

/**
 * @addtogroup animator_test
 * @{
 * @addtogroup animator_window_add
 * @{
 * animator
 * <TABLE>
 * @}
 */

/**
 * @addtogroup animator_window_add
 * @{
 * <tr>
 * <td>animator_window_add</td>
 * <td>animator_window_add_test_p</td>
 * <td>
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 Style filled with parts
 *
 * @procedure
 * @step 1 Call animator_window_add(Style)
 * @step 2 Check returned pointer
 * </td>
 * <td>Style *style</td>
 * <td>Not NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (animator_window_add_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/animator_window_add_test.edj";
   const char *full_style_name = "elm/radio/base/test";
   Evas_Object *manager, *win;
   App_Data *app;
   Eina_Inlist *widget_list = NULL;
   Evas *e = NULL;
   Style *style;

   app_init();
   app = app_data_get();
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->win = win;
   e = evas_object_evas_get(win);
   widget_list = wm_widgets_list_new(file);
   wm_widgets_list_objects_load(widget_list, e, file);
   style = wm_style_object_find(widget_list, full_style_name);

   manager = animator_window_add(style);
   ck_assert_msg(manager != NULL, "cannot create new Animator");

   wm_widgets_list_free(widget_list);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup animator_window_add
 * @{
 * <tr>
 * <td>animator_window_add</td>
 * <td>animator_window_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 Empty new Style
 *
 * @procedure
 * @step 1 Call animator_window_add(Style)
 * @step 2 Check returned pointer
 * </td>
 * <td>Style *style</td>
 * <td>NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (animator_window_add_test_n1)
{
   elm_init(0,0);
   const char *full_style_name = "elm/radio/base/test";
   const char *style_name = "test";
   Evas_Object *manager, *win;
   App_Data *app;
   Style *style;

   app_init();
   app = app_data_get();
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->win = win;
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);

   manager = animator_window_add(style);
   ck_assert_msg(manager == NULL, "Animator was successfully created");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup animator_window_add
 * @{
 * <tr>
 * <td>animator_window_add</td>
 * <td>animator_window_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call animator_window_add(NULL)
 * @step 2 Check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (animator_window_add_test_n2)
{
   elm_init(0,0);
   Evas_Object *manager, *win;
   App_Data *app;

   app_init();
   app = app_data_get();
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->win = win;

   manager = animator_window_add(NULL);
   ck_assert_msg(manager == NULL, "Animator was successfully created");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup animator_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
