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
 * @addtogroup wm_widget_add
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_widget_add
 * @{
 * <tr>
 * <td>wm_widget_add</td>
 * <td>wm_widget_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_add("radio", list_of_groups).
 * @step 2 Check returned Widget.
 * @step 3 Check Widget's content.
 * </td>
 * <td>char *widget_name = "radio"; Eina_List *groups;</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_add_test_p)
{
   elm_init(0,0);
   Eina_List *groups = NULL;
   Widget *widget = NULL;
   Class *class = NULL;
   const char *widget_name = "radio";
   const char *class_name = "base";

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   widget = wm_widget_add(widget_name, groups);
   ck_assert_msg(widget != NULL, "Widget is not created.");
   class = EINA_INLIST_CONTAINER_GET(widget->classes, Class);
   ck_assert_str_eq(class->name, class_name);

   wm_widget_free(widget);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add
 * @{
 * <tr>
 * <td>wm_widget_add</td>
 * <td>wm_widget_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_add(NULL, list_of_groups).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, Eina_List *groups;</td>
 * <td>NULL object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_add_test_n1)
{
   elm_init(0,0);
   Eina_List *groups = NULL;

   groups = eina_list_append(groups, "elm/radio/base/test");
   groups = eina_list_append(groups, "elm/radio/notbase/test");
   ck_assert_msg(wm_widget_add(NULL, groups) == NULL, "created new widget with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add
 * @{
 * <tr>
 * <td>wm_widget_add</td>
 * <td>wm_widget_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 List of groups.
 *
 * @procedure
 * @step 1 Call function wm_widget_add("radio", NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>char *widget_name = "radio", NULL</td>
 * <td>NULL object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_add_test_n2)
{
   elm_init(0,0);
   const char *widget_name = "radio";
   ck_assert_msg(wm_widget_add(widget_name, NULL) == NULL, "created new widget with NULL parameter");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add
 * @{
 * <tr>
 * <td>wm_widget_add</td>
 * <td>wm_widget_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_widget_add(NULL, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_add_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_widget_add(NULL, NULL) == NULL, "created new widget with NULL parameters");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
