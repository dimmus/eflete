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

#include "test_colorselector.h"

/**
 * @addtogroup colorselector_test
 * @{
 * @addtogroup colorselector_add
 * @{
 * colorselector
 * <TABLE>
 * @}
 */

/**
 * @addtogroup colorselector_add
 * @{
 * <tr>
 * <td>colorselector_add</td>
 * <td>colorselector_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call colorselector_add()
 * @step 2 Check returned pointer
 * </td>
 * <td>void</td>
 * <td>Evas_Object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (colorselector_add_test_p)
{
   elm_init(0,0);

   app_init();
   ui_main_window_add();
   ck_assert_msg(colorselector_add(ap.win) != NULL, "cannot create the Colorselector");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorselector_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
