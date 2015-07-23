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

#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup win_layout_get
 * @{
 * eflete
 * <TABLE>
 * @}
 */

/**
 * @addtogroup win_layout_get
 * @{
 * <tr>
 * <td>win_layout_get</td>
 * <td>win_layout_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 * @step 4 create main window
 *
 * @procedure
 * @step 1 call win_layout_get
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>Not NULL object</td>
 * </tr>
 * @}
 */
EFLETE_TEST (win_layout_get_test_p)
{
   elm_init(0,0);
   app_init();
   App_Data *app = app_data_get();
   ui_main_window_add(app);

   ck_assert_msg(win_layout_get() != NULL, "failure: win_layout not exist");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup win_layout_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
