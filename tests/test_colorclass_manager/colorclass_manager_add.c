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

#include "test_colorclass_manager.h"
#include "test_common.h"

/**
 * @addtogroup colorclass_manager_test
 * @{
 * @addtogroup colorclass_manager_add
 * @{
 * Color Class editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup colorclass_manager_add
 * @{
 * <tr>
 * <td>colorclass_manager_add</td>
 * <td>colorclass_manager_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 create and open Project
 * @step 4 create main window
 *
 * @procedure
 * @step 1 call colorclass_manager_add
 * @step 2 check returned pointer
 * </td>
 * <td>(Project *)project</td>
 * <td>Evas_Object returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (colorclass_manager_add_test_p)
{
   elm_init(0,0);
   setup("colorclass_manager_add_test_p");

   App_Data *app;
   Evas_Object *colorclass;

   app_init();
   app = app_data_get();
   app->project = pm_project_open("./colorclass_manager_add_test_p/colorclass_manager_add_test_p.pro");

   ui_main_window_add(app);
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->mmap_file);

   colorclass  = colorclass_manager_add(app->project);
   ck_assert_msg(colorclass != NULL, "Unable to create image editor window");

   evas_object_del(colorclass);
   pm_project_close(app->project);
   app->project = NULL;
   app_shutdown();
   teardown("./colorclass_manager_add_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorclass_manager_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
