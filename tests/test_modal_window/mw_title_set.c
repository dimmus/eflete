/* Edje Theme Editor
* Copyright (C) 2014 Samsung Electronics.
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

#include "test_modal_window.h"

/**
 * @addtogroup modal_window_test
 * @{
 * @addtogroup mw_title_set
 * @{
 * modal_window
 * <TABLE>
 * @}
 */


/**
 * @addtogroup mw_title_set
 * @{
 * <tr>
 * <td>mw_title_set</td>
 * <td>mw_title_set_p</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_title_set(modal_window, "test text")
 * @step 2 Check returned value
 * </td>
 * <td>Evas_Object *modal_window, const char *text = "test text"</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (mw_title_set_p)
{
   Evas_Object *mw;

   elm_init(0,0);
   app_init();
   ui_main_window_add();
   mw = mw_add(NULL, NULL, NULL);

   ck_assert_msg(mw_title_set(mw, "test text"), "Can't set title");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_title_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
