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

#include "test_modal_window.h"

/**
 * @addtogroup modal_window_test
 * @{
 * @addtogroup mw_about_add
 * @{
 * modal_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup mw_about_add
 * @{
 * <tr>
 * <td>mw_about_add</td>
 * <td>mw_about_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call mw_about_add(NULL, NULL)
 * @step 2 Check returned pointer
 * </td>
 * <td>NULL, NULL</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (mw_about_add_test_p)
{
   elm_init(0,0);
   App_Data *app;

   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(mw_about_add(NULL, NULL) != NULL, "cannot create new Modal Window");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_about_add
 * @{
 * <tr>
 * <td>mw_about_add</td>
 * <td>mw_about_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window NOT created
 *
 * @procedure
 * @step 1 Call mw_about_add(NULL, NULL)
 * @step 2 Check returned pointer
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (mw_about_add_test_n)
{
   elm_init(0,0);

   app_init();
   ck_assert_msg(mw_about_add(NULL, NULL) == NULL, "Not NULL pointer returned");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_about_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
