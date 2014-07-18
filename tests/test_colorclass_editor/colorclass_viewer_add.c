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

#include "test_colorclass_editor.h"

/**
 * @addtogroup colorclass_editor_test
 * @{
 * @addtogroup colorclass_viewer_add
 * @{
 * Color Class
 * <TABLE>
 * @}
 */

/**
 * @addtogroup colorclass_viewer_add
 * @{
 * <tr>
 * <td>colorclass_viewer_add</td>
 * <td>colorclass_viewer_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call colorclass_viewer_add
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (colorclass_viewer_add_test_n)
{
   elm_init(0,0);

   ck_assert_msg(colorclass_viewer_add(NULL) == NULL, "Not NULL returned");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorclass_viewer_add
 * @{
 * <tr>
 * <td>colorclass_viewer_add</td>
 * <td>colorclass_viewer_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 create empty Project
 * @step 4 create main window
 *
 * @procedure
 * @step 1 call colorclass_viewer_add
 * @step 2 check returned pointer
 * </td>
 * <td>(Project *)project</td>
 * <td>Evas_Object returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (colorclass_viewer_add_test_p)
{
   elm_init(0,0);
   App_Data *app;
   Evas_Object *colorclass;

   app_init();
   app = app_create();
   app->project = calloc(1, sizeof(Project));
   ui_main_window_add(app);

   colorclass  = colorclass_viewer_add(app->project);
   ck_assert_msg(colorclass != NULL, "failure: cannot create image editor window");

   evas_object_del(colorclass);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorclass_viewer_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
