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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_widget_list_new
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_widget_list_new
 * @{
 * <tr>
 * <td>wm_widget_list_new</td>
 * <td>wm_widget_list_new_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 1 Path to edj file.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_new("./edj_build/wm_widget_list_new.edj").
 * @step 2 Check returned widget list.
 * @step 3 Check widget list's content.
 * </td>
 * <td>char *file = "./edj_build/wm_widget_list_new.edj"</td>
 * <td>All Checks was successfuly passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_list_new_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_widget_list_new.edj";
   const char *widget_name = "radio";
   Eina_Inlist *widget_list = NULL;
   Widget *widget = NULL;

   widget_list = wm_widget_list_new(file);
   ck_assert_msg(widget_list != NULL, "Widget List is not created.");
   widget = EINA_INLIST_CONTAINER_GET(widget_list, Widget);
   ck_assert_str_eq(widget->name, widget_name);

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_new
 * @{
 * <tr>
 * <td>wm_widget_list_new</td>
 * <td>wm_widget_list_new_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_widget_list_new(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_list_new_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_list_new(NULL) == NULL, "new widget list was created");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_new
 * @{
 * </TABLE>
 * @}
 * @}
 */
