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
 * @addtogroup wm_class_free
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_class_free
 * @{
 * <tr>
 * <td>wm_class_free</td>
 * <td>wm_class_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Class structure.
 *
 * @procedure
 * @step 1 Call function wm_style_free(style).
 * @step 2 Check returned value.
 * </td>
 * <td>Class *class</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_class_free_test_p)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   Class *class= NULL;
   const char* class_name = "test";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   class = wm_class_add(class_name, groups, NULL);

   ck_assert_msg(wm_class_free(class) == EINA_TRUE, "cannot delete Class structure.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_class_free
 * @{
 * <tr>
 * <td>wm_class_free</td>
 * <td>wm_class_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_class_free(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_class_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_class_free(NULL) == EINA_FALSE, "NULL Class structure was deleted.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_class_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
