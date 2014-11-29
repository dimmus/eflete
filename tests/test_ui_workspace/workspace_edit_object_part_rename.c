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
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * <tr>
 * <td>workspace_edit_object_part_rename</td>
 * <td>workspace_edit_object_part_rename_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a workspace
 * @step 4 create a edje edit object
 * @step 5 load some group from file to edje edit object
 * @step 6 set to the workspace editable object
 *
 * @procedure
 * @step 1 Rename part "radio" into "NEW_radio"
 * </td>
 * <td>(Evas_Object *) workspace, (const char *) old_name = "radio", (const char *) new_name = "NEW_radio"</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_rename_test_p1)
{
   Evas_Object *parent, *workspace, *edje_edit;
   Style *style = calloc(1, sizeof(Style));

   elm_init(0, 0);
   app_init();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/workspace_edit_object_part_rename.edj", "elm/radio/base/def");
   style->obj = edje_edit;
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_rename.edj");

   ck_assert_msg(workspace_edit_object_part_rename(workspace, "radio", "NEW_radio"),
                 "Renaming was unsuccesfull");

   evas_object_del(parent);

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * <tr>
 * <td>workspace_edit_object_part_rename</td>
 * <td>workspace_edit_object_part_rename_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a workspace
 * @step 4 create a edje edit object
 * @step 5 load some group from file to edje edit object
 * @step 6 set to the workspace editable object
 *
 * @procedure
 * @step 1 Check that there are no parts with name "NEW_radio"
 * @step 2 Rename part "radio" into "NEW_radio"
 * </td>
 * <td>(Evas_Object *) workspace, (const char *) old_name = "radio", (const char *) new_name = "NEW_radio"</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_rename_test_p2)
{
   Evas_Object *parent, *workspace, *edje_edit;
   Style *style = calloc(1, sizeof(Style));

   elm_init(0, 0);
   app_init();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/workspace_edit_object_part_rename.edj", "elm/radio/base/def");
   style->obj = edje_edit;
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_rename.edj");

   ck_assert_msg(workspace_edit_object_part_rename(workspace, "radio", "NEW_radio"),
                 "Renaming was unsuccesfull");
   ck_assert_msg(workspace_edit_object_part_rename(workspace, "NEW_radio", "radio"),
                 "Renaming back was unsuccesfull");


   evas_object_del(parent);

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * <tr>
 * <td>workspace_edit_object_part_rename</td>
 * <td>workspace_edit_object_part_rename_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a workspace
 * @step 4 create a edje edit object
 * @step 5 load some group from file to edje edit object
 * @step 6 set to the workspace editable object
 *
 * @procedure
 * @step 1 Rename unexisted part "qqqqq" into "NEW_radio"
 * </td>
 * <td>(Evas_Object *) workspace, (const char *) old_name = "qqqqq", (const char *) new_name = "NEW_radio"</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_rename_test_n1)
{
   Evas_Object *parent, *workspace, *edje_edit;
   Style *style = calloc(1, sizeof(Style));

   elm_init(0, 0);
   app_init();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/workspace_edit_object_part_rename.edj", "elm/radio/base/def");
   style->obj = edje_edit;
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_rename.edj");

   ck_assert_msg(!workspace_edit_object_part_rename(workspace, "qqqqq", "NEW_radio"),
                 "Renaming was succesfull with unexisted part");


   evas_object_del(parent);

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * <tr>
 * <td>workspace_edit_object_part_rename</td>
 * <td>workspace_edit_object_part_rename_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a workspace
 * @step 4 create a edje edit object
 * @step 5 load some group from file to edje edit object
 * @step 6 set to the workspace editable object
 *
 * @procedure
 * @step 1 Rename part "radio" into "bg" (part called "bg" already exists)
 * </td>
 * <td>(Evas_Object *) workspace, (const char *) old_name = "radio", (const char *) new_name = "bg"</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_rename_test_n2)
{
   Evas_Object *parent, *workspace, *edje_edit;
   Style *style = calloc(1, sizeof(Style));

   elm_init(0, 0);
   app_init();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/workspace_edit_object_part_rename.edj", "elm/radio/base/def");
   style->obj = edje_edit;
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_rename.edj");

   ck_assert_msg(!workspace_edit_object_part_rename(workspace, "radio", "bg"),
                 "Renaming part into name that is already exist was successfull");


   evas_object_del(parent);

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * <tr>
 * <td>workspace_edit_object_part_rename</td>
 * <td>workspace_edit_object_part_rename_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a workspace
 * @step 4 create a edje edit object
 * @step 5 load some group from file to edje edit object
 * @step 6 set to the workspace editable object
 *
 * @procedure
 * @step 1 Rename part "radio" into its current name.
 * </td>
 * <td>(Evas_Object *) workspace, (const char *) old_name = "radio", (const char *) new_name = "radio"</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_rename_test_n3)
{
   Evas_Object *parent, *workspace, *edje_edit;
   Style *style = calloc(1, sizeof(Style));

   elm_init(0, 0);
   app_init();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/workspace_edit_object_part_rename.edj", "elm/radio/base/def");
   style->obj = edje_edit;
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_rename.edj");

   ck_assert_msg(!workspace_edit_object_part_rename(workspace, "radio", "radio"),
                 "Renaming part from radio to radio... So useless act!");


   evas_object_del(parent);

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * <tr>
 * <td>workspace_edit_object_part_rename</td>
 * <td>workspace_edit_object_part_rename_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a workspace
 * @step 4 create a edje edit object
 * @step 5 load some group from file to edje edit object
 * @step 6 set to the workspace editable object
 *
 * @procedure
 * @step 1 Call function workspace_edit_object_part_rename with third parameter as NULL
 * @step 2 Call function workspace_edit_object_part_rename with second parameter as NULL
 * @step 3 Call function workspace_edit_object_part_rename with first parameter as NULL
 * @step 4 Call function workspace_edit_object_part_rename with second parameter as ""
 * @step 5 Call function workspace_edit_object_part_rename with third parameter as ""
 * </td>
 * <td>NULL, NULL, NULL</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(workspace_edit_object_part_rename_test_n4)
{
   Evas_Object *parent, *workspace, *edje_edit;
   Style *style = calloc(1, sizeof(Style));

   elm_init(0, 0);
   app_init();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/workspace_edit_object_part_rename.edj", "elm/radio/base/def");
   style->obj = edje_edit;
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_rename.edj");

   ck_assert_msg(!workspace_edit_object_part_rename(workspace, "radio", NULL),
                 "Renaming part into name with NULL was successfull");
   ck_assert_msg(!workspace_edit_object_part_rename(workspace, NULL, "NEW_radio"),
                 "Renaming part with name NULL was successfull");
   ck_assert_msg(!workspace_edit_object_part_rename(NULL, "radio", "NEW_radio"),
                 "Renaming part in NULL workspace was successful");
   ck_assert_msg(!workspace_edit_object_part_rename(workspace, "", "NEW_radio"),
                 "Renaming part with wrong name \"\" was successful");
   ck_assert_msg(!workspace_edit_object_part_rename(workspace, "radio", ""),
                 "Renaming part into wrong name \"\" was successful");

   evas_object_del(parent);

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_rename
 * @{
 * </TABLE>
 * @}
 * @}
 */
