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

#include "test_ui_workspace.h"

/**
 * @addtogroup ui_workspace_test
 * @{
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 Mmap edj file.
 * @step 5 create workspace object
 * @step 6 create style object
 *
 * @procedure
 * @step 1 create part state with workspace_edit_object_part_state_add
 * @step 2 check returned value from function
 * @step 3 check new state name and value are exist in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (char *) "test", (double) 1.0</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_p)
{
   Eina_Bool res = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   elm_init(0, 0);
   app_init();
   ui_main_window_add();
   mmap_file = eina_file_open("./edj_build/workspace_edit_object_part_state_add.edj", EINA_FALSE);
   e = evas_object_evas_get(ap.workspace);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(ap.workspace, style, "./edj_build/workspace_edit_object_part_state_add.edj");
   history_genlist_get(ap.history, ap.workspace);
   history_module_add(style->obj);

   res = workspace_edit_object_part_state_add(ap.workspace, "bg", "test", 1.0);
   ck_assert_msg(res == EINA_TRUE, "Failed add state for part, loaded into workspace");

   wm_style_free(style);
   eina_file_close(mmap_file);
   //workspace_edit_object_unset(ap.workspace);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_add</td>
 * <td>workspace_edit_object_part_state_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 Mmap edj file.
 * @step 5 create workspace object
 * @step 6 create style object
 *
 * @procedure
 * @step 1 Add new state with workspace_edit_object_part_state_add,
 *         with name that exist, but with different value. (default 0.5)
 * @step 2 check returned value from function
 * @step 3 check new state name and value are exist in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "bg", (char *) "default", (double) 0.5</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_add_test_p1)
{
   Eina_Bool res = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   elm_init(0, 0);
   app_init();
   ui_main_window_add();
   mmap_file = eina_file_open("./edj_build/workspace_edit_object_part_state_add.edj", EINA_FALSE);
   e = evas_object_evas_get(ap.workspace);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(ap.workspace, style, "./edj_build/workspace_edit_object_part_state_add.edj");
   history_genlist_get(ap.history, ap.workspace);
   history_module_add(style->obj);

   res = workspace_edit_object_part_state_add(ap.workspace, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_TRUE, "Failed add state for part, loaded into workspace");

   wm_style_free(style);
   eina_file_close(mmap_file);
   //workspace_edit_object_unset(ap.workspace);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
