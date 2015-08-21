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
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * ui_workspace
 * <TABLE>
 * @}
*/

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 Mmap edj file.
 * @step 5 create workspace object
 * @step 6 create style object
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Delete exist state with workspace_edit_object_part_state_del
 * @step 2 check returned value from function
 * @step 3 check deleted state name and value in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "radio", (char *) "visible", (double) 0.0 </td>
 * <td>All check's passed'</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_p)
{
   Eina_Bool res = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   elm_init(0, 0);
   app_init();
   ui_main_window_add();
   mmap_file = eina_file_open("./edj_build/workspace_edit_object_part_state_del.edj", EINA_FALSE);
   e = evas_object_evas_get(ap.workspace);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(ap.workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");
   history_genlist_get(ap.history, ap.workspace);
   history_module_add(style->obj);

   res = edje_edit_state_exist(style->obj, "radio", "visible", 0.0);
   ck_assert_msg(res != EINA_FALSE, "State not exist in edje edit object");
   res = workspace_edit_object_part_state_del(ap.workspace, "radio", "visible", 0.0);
   ck_assert_msg(res == EINA_TRUE, "Failed delete state from part, loaded into workspace");
   res = edje_edit_state_exist(style->obj, "radio", "visible", 0.0);
   ck_assert_msg(res == EINA_FALSE, "Deleted state still exist in edje edit object");

   wm_style_free(style);
   eina_file_close(mmap_file);
   //workspace_edit_object_unset(ap.workspace);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_state_del</td>
 * <td>workspace_edit_object_part_state_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 Mmap edj file.
 * @step 5 create workspace object
 * @step 6 create style object
 * @step 7 set edit object into workspace
 * @step 8 Add new state with workspace_edit_object_part_state_add,
 *         with name that exist, but with different value. (default 0.5)
 *
 * @procedure
 * @step 1 Delete state, which added at step 7 of precondition using
 *         workspace_edit_object_part_state_del
 * @step 2 check returned value from function
 * @step 3 check deleted state name and value in edje edit object.
 * </td>
 * <td>(Evas_Object *) workspace, (char *) "radio", (char *) "default", (double) 0.5 </td>
 * <td>All check's passed'</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_state_del_test_p1)
{
   Eina_Bool res = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   elm_init(0, 0);
   app_init();
   ui_main_window_add();
   mmap_file = eina_file_open("./edj_build/workspace_edit_object_part_state_del.edj", EINA_FALSE);
   e = evas_object_evas_get(ap.workspace);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(ap.workspace, style, "./edj_build/workspace_edit_object_part_state_del.edj");
   history_genlist_get(ap.history, ap.workspace);
   history_module_add(style->obj);
   workspace_edit_object_part_state_add(ap.workspace, "bg", "default", 0.5);

   res = workspace_edit_object_part_state_del(ap.workspace, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_TRUE, "Failed delete state from part, loaded into workspace");
   res = edje_edit_state_exist(style->obj, "bg", "default", 0.5);
   ck_assert_msg(res == EINA_FALSE, "Delete state still exist in edje edit object");

   wm_style_free(style);
   eina_file_close(mmap_file);
   //workspace_edit_object_unset(ap.workspace);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_state_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
