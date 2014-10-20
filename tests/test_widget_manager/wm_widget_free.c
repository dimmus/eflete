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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_widget_free
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_widget_free
 * @{
 * <tr>
 * <td>wm_widget_free</td>
 * <td>wm_widget_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_free(widget).
 * @step 2 Check returned value.
 * </td>
 * <td>Widget *widget</td>
 * <td>EINA_TRUE returned.</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_free_test_p)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   Widget *widget = NULL;
   const char *widget_name = "radio";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   widget = wm_widget_add(widget_name, groups);
   ck_assert_msg(wm_widget_free(widget) == EINA_TRUE, "cannot delete Widget.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_free
 * @{
 * <tr>
 * <td>wm_widget_free</td>
 * <td>wm_widget_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_widget_free(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned.</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_free(NULL) == EINA_FALSE, "NULL parameter was deleted as Widget.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
