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
 * @addtogroup workspace_edit_object_get
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_edit_object_get
 * @{
 * <tr>
 * <td>workspace_edit_object_get</td>
 * <td>workspace_edit_object_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 Mmap edj file.
 * @step 5 create workspace object
 * @step 6 create style object
 * @step 7 load style data from edje file and store pointer in variable
 * @step 8 set edit object into workspace
 *
 * @procedure
 * @step 1 get style with workspace_edit_object_get
 * @step 2 compare returned pointer with pointer, which stored at step 7 of precondition
 * </td>
 * <td>(Evas_Object *) workspace</td>
 * <td>Returned and stored pointers are equals</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_get_test_p)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Style *style = NULL, *style_ret = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent, NULL, NULL);
   e = evas_object_evas_get(parent);
   mmap_file = eina_file_open("./edj_build/workspace_edit_object_get.edj", EINA_FALSE);
   style = wm_style_add("test", "elm/radio/base/test", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   //workspace_edit_object_set(workspace, style, "./edj_build/workspace_edit_object_get.edj");
   //style_ret = workspace_edit_object_get(workspace);
   ck_assert_msg(style == style_ret, "Setted and getted style objects are not equals");

   wm_style_free(style);
   eina_file_close(mmap_file);
   //workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_get
 * @{
 * <tr>
 * <td>workspace_edit_object_get</td>
 * <td>workspace_edit_object_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 create workspace object
 *
 * @procedure
 * @step 1 get style with workspace_edit_object_get
 * @step 2 returned pointer is NULL
 * </td>
 * <td>(Evas_Object *) workspace</td>
 * <td>NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_edit_object_get_test_p1)
{
   elm_init(0, 0);
   app_init();
   Evas_Object *parent, *workspace;
   Style *style_ret = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   workspace = workspace_add(parent, NULL, NULL);
   //style_ret = workspace_edit_object_get(workspace);
   ck_assert_msg(style_ret == NULL, "Get edit object from workspace, "
                                "whithout prevision set");

   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_edit_object_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
