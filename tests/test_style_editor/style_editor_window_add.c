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

#include "test_style_editor.h"
#include "test_common.h"

/**
 * @addtogroup style_editor_test
 * @{
 * @addtogroup style_editor_window_add
 * @{
 * style_editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup style_editor_window_add
 * @{
 * <tr>
 * <td>style_editor_window_add</td>
 * <td>style_editor_window_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm and app
 * @step 2 main window created
 * @step 3 project opened
 *
 * @procedure
 * @step 1 Call style_editor_window_add()
 * @step 2 Check  returned pointer
 * </td>
 * <td>Project *project</td>
 * <td>returned pointer is not NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_p)
{
   App_Data *app = NULL;

   elm_init(0,0);
   setup("style_editor_window_add_test_p");

   app_init();
   app = app_data_get();
   app->project = pm_project_open("./style_editor_window_add_test_p/style_editor_window_add_test_p.pro");

   ui_main_window_add(app);
   ck_assert_msg(style_editor_window_add(app->project) != NULL,
                 "Unable to create style editor");

   elm_shutdown();
   teardown("./style_editor_window_add_test_p");
   app_shutdown();
}
END_TEST

/**
 * @addtogroup style_editor_window_add
 * @{
 * <tr>
 * <td>style_editor_window_add</td>
 * <td>style_editor_window_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call style_editor_window_add() with NULL param
 * @step 2 Check  returned pointer
 * </td>
 * <td>NULL</td>
 * <td>returned pointer is NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_n1)
{
   elm_init(0,0);
   ck_assert_msg(style_editor_window_add(NULL) == NULL,
                 "Style editor created without app data");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup style_editor_window_add
 * @{
 * <tr>
 * <td>style_editor_window_add</td>
 * <td>style_editor_window_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm and app
 * @step 2 main window  NOT created
 * @step 3 project opened
 *
 * @procedure
 * @step 1 Call style_editor_window_add()
 * @step 2 Check  returned pointer
 * </td>
 * <td>Project *project</td>
 * <td>returned pointer is NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_n2)
{
   App_Data *app = NULL;

   elm_init(0,0);
   setup("style_editor_window_add_test_n2");

   app_init();
   app = app_data_get();
   app->project = pm_project_open("./style_editor_window_add_test_n2/style_editor_window_add_test_n2.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->mmap_file);

   ck_assert_msg(style_editor_window_add(app->project) == NULL,
                 "Style_edtor added to non-existing window");

   elm_shutdown();
   teardown("./style_editor_window_add_test_n2");
   app_shutdown();
}
END_TEST

/**
 * @addtogroup style_editor_window_add
 * @{
 * <tr>
 * <td>style_editor_window_add</td>
 * <td>style_editor_window_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm and app
 * @step 2 main window  created
 * @step 3 project NOT opened
 *
 * @procedure
 * @step 1 Call style_editor_window_add()
 * @step 2 Check  returned pointer
 * </td>
 * <td>Project *project</td>
 * <td>returned pointer is NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_n3)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   ck_assert_msg(style_editor_window_add(app->project) == NULL,
                 "Style_edtor added without open project");
   elm_shutdown();
   app_shutdown();
}
END_TEST

/**
 * @addtogroup style_editor_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
