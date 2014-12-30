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
 * @addtogroup workspace_zoom_factor_get
 * @{
 * ui_workspace
 * <TABLE>
 * @}
 */

/**
 * @addtogroup workspace_zoom_factor_get
 * @{
 * <tr>
 * <td>workspace_zoom_factor_get</td>
 * <td>workspace_zoom_factor_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extenstion theme from EFLETE_THEME file
 * @step 3 create parent window
 * @step 4 Mmap edj file.
 * @step 5 create workspace object
 * @step 6 create style
 * @step 7 load data into created style from edj file
 * @step 8 set loaded object into workspace
 * @step 9 Set zoom factor equal 1.5
 *
 * @procedure
 * @step 1 call workspace_zoom_factor_get
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) workspace</td>
 * <td>Returned (double) 1.5</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_zoom_factor_get_test_p)
{
   elm_init(0, 0);
   app_init();
   double res = -1;
   Evas_Object *parent, *workspace;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open("./edj_build/workspace_zoom_factor_get.edj", EINA_FALSE);
   workspace = workspace_add(parent);
   e = evas_object_evas_get(parent);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(workspace, style, "./edj_build/workspace_zoom_factor_get.edj");
   workspace_zoom_factor_set(workspace, 1.5);
   res = workspace_zoom_factor_get(workspace);
   ck_assert_msg(res == 1.5, "Failed get zoom factor");

   wm_style_free(style);
   eina_file_close(mmap_file);
   workspace_edit_object_unset(workspace);
   evas_object_del(workspace);
   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_zoom_factor_get
 * @{
 * <tr>
 * <td>workspace_zoom_factor_get</td>
 * <td>workspace_zoom_factor_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call workspace_zoom_factor_get with NULL pointer workspace object
 * @step 2 check returned value
 * </td>
 * <td>NULL</td>
 * <td>Returned 0</td>
 * </tr>
 * @}
 */
EFLETE_TEST (workspace_zoom_factor_get_test_n1)
{
   elm_init(0, 0);
   double res = -1;
   res = workspace_zoom_factor_get(NULL);
   ck_assert_msg(res == 0, "Get zoom factor from NULL object");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup workspace_zoom_factor_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
