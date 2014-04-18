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


#include "test_ui_widget_list.h"

/**
 * @addtogroup test_ui_widget_list
 * @{
 * @addtogroup ui_widget_list_add
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_add
 * @{
 * <tr>
 * <td>ui_widget_list_add</td>
 * <td>ui_widget_list_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 Call ui_widget_list_add
 * @step 2 Check returned pointer
 * </td>
 * <td>(Evas_Object *) parent</td>
 * <td>Returned pointer not NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_add_test_p)
{
   elm_init(0, 0);
   Evas_Object *parent, *widget_list;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);
   ck_assert_msg(widget_list != NULL, "Failed create widget list object.");
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_add
 * @{
 * <tr>
 * <td>ui_widget_list_add</td>
 * <td>ui_widget_list_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call ui_widget_list_add
 * @step 2 Check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_add_test_n)
{
   elm_init(0, 0);
   Evas_Object *widget_list;
   widget_list = ui_widget_list_add(NULL);
   ck_assert_msg(widget_list == NULL, "Create widget list object without parent.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
