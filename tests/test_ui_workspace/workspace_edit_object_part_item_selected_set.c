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
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_selected_set</td>
 * <td>workspace_edit_object_part_item_selected_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 * @step 6 highlight part "table"
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_selected_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"first", (Eina_Bool) EINA_TRUE </td>
 * <td>Returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_selected_set_test_p1)
{
   Eina_Bool res = EINA_FALSE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name;
   const char *file = "./edj_build/workspace_edit_object_part_item_selected_set.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("first");
   part_name = eina_stringshare_add("table");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);
   part = wm_part_by_name_find(style, part_name);
   workspace_highlight_set(workspace, part);

   res = workspace_edit_object_part_item_selected_set(workspace, item, EINA_TRUE);
   ck_assert_msg(res == EINA_TRUE, "Failed change selection state of the part item");

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
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_selected_set</td>
 * <td>workspace_edit_object_part_item_selected_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 * @step 6 highlight part "table"
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_selected_set with wrong item name.
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"wrong_name", (Eina_Bool) EINA_TRUE </td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_selected_set_test_n1)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item, *part_name;
   const char *file = "./edj_build/workspace_edit_object_part_item_selected_set.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("unexist");
   part_name = eina_stringshare_add("table");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);
   part = wm_part_by_name_find(style, part_name);
   workspace_highlight_set(workspace, part);

   res = workspace_edit_object_part_item_selected_set(workspace, item, EINA_TRUE);
   ck_assert_msg(res == EINA_FALSE, "Change selection state of the unexist part item");

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
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_selected_set</td>
 * <td>workspace_edit_object_part_item_selected_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 * @step 6 highlight part "table"
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_selected_set without name
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, NULL, (Eina_Bool) EINA_TRUE </td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_selected_set_test_n2)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *part_name;
   const char *file = "./edj_build/workspace_edit_object_part_item_selected_set.edj";

   elm_init(0, 0);
   app_init();
   part_name = eina_stringshare_add("table");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);
   part = wm_part_by_name_find(style, part_name);
   workspace_highlight_set(workspace, part);

   res = workspace_edit_object_part_item_selected_set(workspace, NULL, EINA_TRUE);
   ck_assert_msg(res == EINA_FALSE, "Change selection state of the NULL part item");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(part_name);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_selected_set</td>
 * <td>workspace_edit_object_part_item_selected_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_selected_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, (Eina_Stringshare *)"first", (Eina_Bool) EINA_TRUE </td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_selected_set_test_n3)
{
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   app_init();

   Eina_Stringshare *item = eina_stringshare_add("first");
   res = workspace_edit_object_part_item_selected_set(NULL, item, EINA_TRUE);
   ck_assert_msg(res == EINA_FALSE, "Change selection state of the part item without workspace");

   eina_stringshare_del(item);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * <tr>
 * <td>workspace_edit_object_part_item_selected_set</td>
 * <td>workspace_edit_object_part_item_selected_set_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create workspace object
 * @step 4 create style object
 * @step 5 set edit object into workspace
 *
 * @procedure
 * @step 1 call workspace_edit_object_part_item_selected_set without highlight part
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (Eina_Stringshare *)"first", (Eina_Bool) EINA_TRUE </td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_item_selected_set_test_n4)
{
   Eina_Bool res = EINA_TRUE;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Stringshare *item;
   const char *file = "./edj_build/workspace_edit_object_part_item_selected_set.edj";

   elm_init(0, 0);
   app_init();
   item = eina_stringshare_add("first");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, file);

   res = workspace_edit_object_part_item_selected_set(workspace, item, EINA_TRUE);
   ck_assert_msg(res == EINA_FALSE, "Change selection state of the part item without part selection");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   eina_file_close(mmap_file);
   eina_stringshare_del(item);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup workspace_edit_object_part_item_selected_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
