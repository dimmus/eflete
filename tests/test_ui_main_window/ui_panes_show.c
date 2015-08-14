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
 * @addtogroup ui_panes_show
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_panes_show
 * @{
 * <tr>
 * <td>ui_panes_show</td>
 * <td>ui_panes_show_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data structure.
 * @step 4 Create main window using ui_main_window_add().
 *
 * @procedure
 * @step 1 Call ui_panes_show.
 * @step 2 Check returned value.
 * </td>
 * <td></td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ui_panes_show_test_p)
{
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   app_init();

   ui_main_window_add();
   ret = ui_panes_show();
   ck_assert_msg(ret == EINA_TRUE, "Failed to show panes");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_show
 * @{
 * </TABLE>
 * @}
 * @}
 */
