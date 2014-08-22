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

#include "test_about_window.h"

/**
 * @addtogroup about_window_test
 * @{
 * @addtogroup about_window_add
 * @{
 * modal_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup about_window_add
 * @{
 * <tr>
 * <td>about_window_add</td>
 * <td>about_window_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call about_window_add()
 * @step 2 Check returned pointer
 * </td>
 * <td>void</td>
 * <td>Evas_Object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (about_window_add_test_p)
{
   elm_init(0,0);
   App_Data *app;
   Evas_Object *about;

   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   about = about_window_add();
   ck_assert_msg(about != NULL, "cannot create new About Window");

   evas_object_del(about);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup about_window_add
 * @{
 * <tr>
 * <td>about_window_add</td>
 * <td>about_window_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call about_window_add()
 * @step 2 Check returned pointer
 * </td>
 * <td>void</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (about_window_add_test_n)
{
   elm_init(0,0);
   ck_assert_msg(about_window_add() == NULL, "about_window_add() should return NULL if App_Data is not initialized, but it returned non-NULL value");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup about_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
