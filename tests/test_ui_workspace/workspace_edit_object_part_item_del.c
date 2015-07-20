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
 * @addtogroup workspace_edit_object_part_item_del
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_item_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_del</td>
 * <td>workspace_edit_object_part_item_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_del
 * @step 2 check returned value
 * @step 3 get list of items for part "table", list should not contain deleted.
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"table", (Eina_Stringshare *)"second" </td>
 * <td>All checks are passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_del_test_p1)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name;
   Eina_List *items, *l = NULL;
   const char *file = "./edj_build/workspace_edit_object_part_item_del.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("second");
   part_name = eina_stringshare_add("table");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_del(workspace, part_name, item);
   ck_assert_msg(res == EINA_TRUE, "Failed delete item from part");
   items = edje_edit_part_items_list_get(style->obj, part_name);
   l = eina_list_data_find_list(items, item);
   ck_assert_msg(l == NULL, "Item still exist in edje object");

   edje_edit_string_list_free(items);
   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   eina_stringshare_del(part_name);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_del</td>
 * <td>workspace_edit_object_part_item_del_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_del with wrong part name
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"wrong_name", (Eina_Stringshare *)"first" </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_del_test_n1)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name;
   const char *file = "./edj_build/workspace_edit_object_part_item_del.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("first");
   part_name = eina_stringshare_add("wrong_name");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_del(workspace, part_name, item);
   ck_assert_msg(res == EINA_FALSE, "Deleted item from unexists part");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   eina_stringshare_del(part_name);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_del
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_del</td>
 * <td>workspace_edit_object_part_item_del_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_del with wrong item name.
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"table", (Eina_Stringshare *)"wrong_name"</td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_del_test_n2)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name;
   const char *file = "./edj_build/workspace_edit_object_part_item_del.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("wrong_name");
   part_name = eina_stringshare_add("table");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_del(workspace, part_name, item);
   ck_assert_msg(res == EINA_FALSE, "Delete item, that not presented in part");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   eina_stringshare_del(part_name);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
