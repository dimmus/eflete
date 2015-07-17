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
 * @addtogroup wm_class_add
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_class_add
 * @{
 * <tr>
 * <td>wm_class_add</td>
 * <td>wm_class_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_class_add(style_name, list_of_groups, NULL).
 * @step 2 Check returned Class.
 * @step 3 Check that Class contains Style structure list.
 * </td>
 * <td>char* class_name = "notbase", Eina_List *groups, NULL</td>
 * <td>All check's are passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_class_add_test_p)
{
   elm_init(0, 0);
   Eina_List *groups = NULL;
   Style *style = NULL;
   Class *class = NULL;
   const char* style_name = "test";
   const char* class_name = "notbase";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   class = wm_class_add(class_name, groups, NULL);
   ck_assert_msg(class != NULL, "Class is not created.");
   style = EINA_INLIST_CONTAINER_GET(class->styles->next, Style);
   ck_assert_str_eq(style->name, style_name);

   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_class_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
