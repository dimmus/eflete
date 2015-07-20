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
 * @addtogroup ui_widget_list_title_set
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_title_set
 * @{
 * <tr>
 * <td>ui_widget_list_title_set</td>
 * <td>ui_widget_list_title_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 create widget list.
 *
 * @procedure
 * @step 1 Call ui_widget_list_title_set
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list, (const char *)"Opened EDJ file"</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_title_set_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list;
   Eina_Bool result = EINA_FALSE;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);

   result = ui_widget_list_title_set(widget_list, "Opened EDJ file");
   ck_assert_msg(result, "Can't set title into widget list");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_title_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
