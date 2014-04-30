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

#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup main_window_get
 * @{
 * eflete
 * <TABLE>
 * @}
 */

/**
 * @addtogroup main_window_get
 * @{
 * <tr>
 * <td>main_window_get</td>
 * <td>main_window_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 * @step 4 create main window
 *
 * @procedure
 * @step 1 call main_window_get
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>Not NULL object</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (main_window_get_test_p)
{
   elm_init(0,0);
   app_init();
   App_Data *app = app_create();
   ui_main_window_add(app);

   ck_assert_msg(main_window_get() != NULL, "Main window not exist");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup main_window_get
 * @{
 * <tr>
 * <td>main_window_get</td>
 * <td>main_window_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 *
 * @procedure
 * @step 1 call main_window_get
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>NULL object</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (main_window_get_test_n1)
{
   elm_init(0,0);
   app_init();
   app_create();

   ck_assert_msg(main_window_get() == NULL, "Main window was returned");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup main_window_get
 * @{
 * <tr>
 * <td>main_window_get</td>
 * <td>main_window_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 call main_window_get
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>NULL object</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (main_window_get_test_n2)
{
   elm_init(0,0);

   ck_assert_msg(main_window_get() == NULL, "Main window was returned");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup main_window_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
