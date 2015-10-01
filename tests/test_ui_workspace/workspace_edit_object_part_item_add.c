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
 * @addtogroup workspace_edit_object_part_item_add
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_item_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_add</td>
 * <td>workspace_edit_object_part_item_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_add
 * @step 2 check returned value
 * @step 3 get list of items for part "table" and search item with name "second"
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"table", (Eina_Stringshare *)"second", (Eina_Stringshare *)"elm/radio/base/default"  </td>
 * <td>All checks are passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_add_test_p1)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name, *source, *data;
   Eina_List *items, *l;
   const char *file = "./edj_build/workspace_edit_object_part_item_add.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("positive_1");
   part_name = eina_stringshare_add("table");
   source = eina_stringshare_add("elm/radio/base/def");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent, NULL);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_add(workspace, part_name, item, source);
   ck_assert_msg(res == EINA_TRUE, "Failed add new item into part");
   res = EINA_FALSE;
   items = edje_edit_part_items_list_get(style->obj, part_name);
   EINA_LIST_FOREACH(items, l, data)
     {
        if (data == item)
          {
             res = EINA_TRUE;
             break;
          }
     }
   ck_assert_msg(res == EINA_TRUE, "Added item do not exist in edje edit object");

   edje_edit_string_list_free(items);
   wm_style_free(style);
   //workspace_edit_object_unset(workspace);
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
 * @addtogroup workspace_edit_object_part_item_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_add</td>
 * <td>workspace_edit_object_part_item_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_add with wrong part name
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"wrong_name", (Eina_Stringshare *)"second", (Eina_Stringshare *)"elm/radio/base/default"  </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_add_test_n1)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name, *source;
   const char *file = "./edj_build/workspace_edit_object_part_item_add.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("negative_1");
   part_name = eina_stringshare_add("wrong_name");
   source = eina_stringshare_add("elm/radio/base/def");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent, NULL);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_add(workspace, part_name, item, source);
   ck_assert_msg(res == EINA_FALSE, "Added new item into unexists part");

   wm_style_free(style);
   //workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   eina_stringshare_del(part_name);
   eina_stringshare_del(source);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_add</td>
 * <td>workspace_edit_object_part_item_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_add with item name, that
 *         aready present
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"table", (Eina_Stringshare *)"first", (Eina_Stringshare *)"elm/radio/base/default"  </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_add_test_n2)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name, *source;
   const char *file = "./edj_build/workspace_edit_object_part_item_add.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("first");
   part_name = eina_stringshare_add("table");
   source = eina_stringshare_add("elm/radio/base/def");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent, NULL);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_add(workspace, part_name, item, source);
   ck_assert_msg(res == EINA_FALSE, "Added new item with name, that already exist");

   wm_style_free(style);
   //workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   eina_stringshare_del(part_name);
   eina_stringshare_del(source);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_add</td>
 * <td>workspace_edit_object_part_item_add_test_n7</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_add with wrong source name
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"table", (Eina_Stringshare *)"negative_7", (Eina_Stringshare *)"wrong_source"  </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_add_test_n7)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name, *source;
   const char *file = "./edj_build/workspace_edit_object_part_item_add.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("negative_7");
   part_name = eina_stringshare_add("table");
   source = eina_stringshare_add("wrong_source");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent, NULL);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_add(workspace, part_name, item, source);
   ck_assert_msg(res == EINA_FALSE, "Added new item with wrong source");

   wm_style_free(style);
   //workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   eina_stringshare_del(part_name);
   eina_stringshare_del(source);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
