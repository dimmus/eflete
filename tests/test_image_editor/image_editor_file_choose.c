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

#include "test_image_editor.h"
#include "test_common.h"

/**
 * @addtogroup image_editor_test
 * @{
 * @addtogroup image_editor_file_choose
 * @{
 * image editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup image_editor_file_choose
 * @{
 * <tr>
 * <td>image_editor_file_choose</td>
 * <td>image_editor_file_choose_test_p</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 call wm_widgets_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>image_editor, "radio.png"</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_p)
{
   elm_init(0,0);
   setup("image_editor_file_choose_test_p");

   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./image_editor_file_choose_test_p/image_editor_file_choose_test_p.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->mmap_file);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(images, "radio.png") == EINA_TRUE,
                 "failure: cannot select file");

   evas_object_del(images);
   app_shutdown();
   teardown("./image_editor_file_choose_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_file_choose
 * @{
 * <tr>
 * <td>image_editor_file_choose</td>
 * <td>image_editor_file_choose_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 call wm_widgets_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>image_editor, "not_correct_image_name"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_n1)
{
   elm_init(0,0);
   setup("image_editor_file_choose_test_n1");

   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./image_editor_file_choose_test_n1/image_editor_file_choose_test_n1.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->mmap_file);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(images, "not_correct_image_name") == EINA_FALSE,
                 "Selected image that not exist");

   evas_object_del(images);
   app_shutdown();
   teardown("./image_editor_file_choose_test_n1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_file_choose
 * @{
 * <tr>
 * <td>image_editor_file_choose</td>
 * <td>image_editor_file_choose_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 call wm_widgets_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL, "not_correct_image_name"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_n2)
{
   elm_init(0,0);
   setup("image_editor_file_choose_test_n2");

   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./image_editor_file_choose_test_n2/image_editor_file_choose_test_n2.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->mmap_file);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(NULL, "not_correct_image_name") == EINA_FALSE,
                 "Selected image that not exist");

   evas_object_del(images);
   app_shutdown();
   teardown("./image_editor_file_choose_test_n2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_file_choose
 * @{
 * <tr>
 * <td>image_editor_file_choose</td>
 * <td>image_editor_file_choose_test_n3</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 call wm_widgets_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>image_editor, NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_n3)
{
   elm_init(0,0);
   setup("image_editor_file_choose_test_n3");

   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./image_editor_file_choose_test_n3/image_editor_file_choose_test_n3.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->mmap_file);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(images, NULL) == EINA_FALSE, "Selected image that not exist");

   evas_object_del(images);
   app_shutdown();
   teardown("./image_editor_file_choose_test_n3");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_file_choose
 * @{
 * </TABLE>
 * @}
 * @}
 */
