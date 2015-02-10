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
 * @addtogroup wm_layouts_list_new
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_layouts_list_new
 * @{
 * <tr>
 * <td>wm_layouts_list_new</td>
 * <td>wm_layouts_list_new_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 1 Path to edj file.
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_new("./edj_build/wm_layouts_list_new.edj").
 * @step 2 Check returned layouts list.
 * @step 3 Check layouts list's content.
 * </td>
 * <td>char *file = "./edj_build/wm_layouts_list_new.edj"</td>
 * <td>All Checks was successfuly passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_layouts_list_new_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_layouts_list_new.edj";
   const char *layout_name = "layout/table";
   Eina_Inlist *layouts_list = NULL;
   Style *layout = NULL;

   layouts_list = wm_layouts_list_new(file);
   printf("\nLayouts list %p\n\n", layouts_list);
   ck_assert_msg(layouts_list != NULL, "Layouts doesn't loaded from file");
   layout = EINA_INLIST_CONTAINER_GET(layouts_list, Style);
   printf("\nStyle %s\n\n", layout->full_group_name);
   ck_assert_str_eq(layout->full_group_name, layout_name);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_layouts_list_new
 * @{
 * <tr>
 * <td>wm_layouts_list_new</td>
 * <td>wm_layouts_list_new_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_new(NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_layouts_list_new_test_n)
{
   elm_init(0,0);
   ck_assert_msg(wm_layouts_list_new(NULL) == NULL, "Layouts loaded from NULL pointer file name");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_layouts_list_new
 * @{
 * </TABLE>
 * @}
 * @}
 */
