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
   App_Data *ap = app_data_get();
   double res = -1;
   Style *style = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   ui_main_window_add(ap);
   mmap_file = eina_file_open("./edj_build/workspace_zoom_factor_get.edj", EINA_FALSE);
   e = evas_object_evas_get(ap->workspace);
   style = wm_style_add("test", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   workspace_edit_object_set(ap->workspace, style, "./edj_build/workspace_zoom_factor_get.edj");
   workspace_zoom_factor_set(ap->workspace, 1.5);
   res = workspace_zoom_factor_get(ap->workspace);
   ck_assert_msg(res == 1.5, "Failed get zoom factor");

   wm_style_free(style);
   eina_file_close(mmap_file);
   app_shutdown();
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
