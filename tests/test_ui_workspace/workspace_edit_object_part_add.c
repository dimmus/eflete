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
 * @addtogroup workspace_edit_object_part_add
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_add</td>
 * <td>workspace_edit_object_part_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Add new part with type EDJE_PART_TYPE_RECTANGLE
 * @step 2 check returned value
 * @step 3 check existent currently created part in edj file.
 * </td>
 * <td>(Evas_Object *) workspace, (char *)"new_part", EDJE_PART_TYPE_RECTANGLE, NULL</td>
 * <td>All check's passed'</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_add_test_p)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_add.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_add.edj");
   ret = workspace_edit_object_part_add(workspace, "new_part",
                                        EDJE_PART_TYPE_RECTANGLE, NULL);
   ck_assert_msg(ret == EINA_TRUE, "Failed add new part into edit object ");
   ck_assert_msg(edje_edit_part_exist(style->obj, "new_part") == EINA_TRUE,
                 "New part didn't exist in edit object");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_add</td>
 * <td>workspace_edit_object_part_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Add new part with type EDJE_PART_TYPE_IMAGE and image name data
 * @step 2 check returned value
 * @step 3 check existent currently created part in edj file.
 * </td>
 * <td>(Evas_Object *) workspace, (char *)"new_part", EDJE_PART_TYPE_IMAGE, (char *)"radio_base.png"</td>
 * <td>All check's passed'</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_add_test_p1)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_FALSE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_add.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_add.edj");
   ret = workspace_edit_object_part_add(workspace, "new_part",
                                        EDJE_PART_TYPE_IMAGE, "radio_base.png");
   ck_assert_msg(ret == EINA_TRUE, "Failed add new part into edit object ");
   ck_assert_msg(edje_edit_part_exist(style->obj, "new_part") == EINA_TRUE,
                 "New part didn't exist in edit object");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_add</td>
 * <td>workspace_edit_object_part_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Add new part with name already existent part
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, (char *)"bg", EDJE_PART_TYPE_RECTANGLE, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_add_test_n)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_add.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_add.edj");
   ret = workspace_edit_object_part_add(workspace, "bg",
                                        EDJE_PART_TYPE_RECTANGLE, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Add new part into edit object, "
                                    "where part with curent name already exist");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_add</td>
 * <td>workspace_edit_object_part_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Add new part to NULL pointer of workspace object.
 * @step 2 check returned value
 * </td>
 * <td>NULL, (char *)"new_part", EDJE_PART_TYPE_RECTANGLE, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_add_test_n1)
{
   elm_init(0, 0);
   Eina_Bool ret = EINA_TRUE;

   ret = workspace_edit_object_part_add(NULL, "new_part",
                                        EDJE_PART_TYPE_RECTANGLE, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Add new part for NULL object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_add</td>
 * <td>workspace_edit_object_part_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Add new part with NULL pointer string name part
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace, NULL, EDJE_PART_TYPE_RECTANGLE, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_add_test_n2)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_add.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_add.edj");
   ret = workspace_edit_object_part_add(workspace, NULL,
                                        EDJE_PART_TYPE_RECTANGLE, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Add new part into without name");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * <tr>
 * <td>workspace_edit_object_part_add</td>
 * <td>workspace_edit_object_part_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 * @step 5 create style object
 * @step 6 load style data from edje file
 * @step 7 set edit object into workspace
 *
 * @procedure
 * @step 1 Add new part with invalid part type
 * @step 2 check returned value
 * @step 3 check existent currently created part in edj file.
 * </td>
 * <td>(Evas_Object *) workspace, (char *)"new_part", 105, NULL</td>
 * <td>Returned EINA_FALSE in both check's'</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_part_add_test_n3)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Eina_Bool ret = EINA_TRUE;
   Style *style = NULL;
   Evas *e = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/workspace_edit_object_part_add.edj");
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_part_add.edj");
   ret = workspace_edit_object_part_add(workspace, "new_part", 105, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Add new part into with invalid type");
   ck_assert_msg(edje_edit_part_exist(style->obj, "new_part") == EINA_FALSE,
                 "New part created with invalid type");

   wm_style_free(style);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_part_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
