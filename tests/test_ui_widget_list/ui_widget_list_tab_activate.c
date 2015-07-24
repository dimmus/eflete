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


#include "test_ui_widget_list.h"

/**
 * @addtogroup ui_widget_list_test
 * @{
 * @addtogroup ui_widget_list_tab_activate
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_tab_activate
 * @{
 * <tr>
 * <td>ui_widget_list_tab_activate</td>
 * <td>ui_widget_list_tab_activate_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create widget list
 *
 * @procedure
 * @step 1 Call ui_widget_list_tab_activate(widget_list, 0);
 * @step 2 Check returned pointer
 * @step 3 Call ui_widget_list_tab_activate(widget_list, 1);
 * @step 4 Check returned pointer
 * </td>
 * <td>(Evas_Object *) parent</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_tab_activate_test_p)
{
   elm_init(0, 0);
   Evas_Object *parent, *widget_list;
   Eina_Bool res = EINA_FALSE;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);

   res = ui_widget_list_tab_activate(widget_list, 0);
   ck_assert_msg(res == EINA_TRUE, "Failed to activate first Tab of widget list");
   res = ui_widget_list_tab_activate(widget_list, 1);
   ck_assert_msg(res == EINA_TRUE, "Failed to activate second Tab of widget list");
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_tab_activate
 * @{
 * <tr>
 * <td>ui_widget_list_tab_activate</td>
 * <td>ui_widget_list_tab_activate_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create widget list
 *
 * @procedure
 * @step 1 Call ui_widget_list_tab_activate(widget_list, 3);
 * @step 2 Check returned pointer
 * </td>
 * <td>(Evas_Object *) parent</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_tab_activate_test_n1)
{
   elm_init(0, 0);
   Evas_Object *parent, *widget_list;
   Eina_Bool res = EINA_FALSE;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);

   res = ui_widget_list_tab_activate(widget_list, 3);
   ck_assert_msg(res == EINA_FALSE, "The activated tab does not exist");
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_tab_activate
 * @{
 * </TABLE>
 * @}
 * @}
 */
