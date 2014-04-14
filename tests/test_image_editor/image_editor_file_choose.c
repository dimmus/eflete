/* Edje Theme Editor
* Copyright (C) 2013-2014 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "test_image_editor.h"

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
 * @step 5 call wm_widget_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>image_editor, "radio.png"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");
   wm_widget_list_objects_load(app->project->widgets,
                        evas_object_evas_get(app->win), app->project->swapfile);
   Evas_Object *ie = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(ie, "radio.png") == EINA_TRUE,
                 "failure: cannot select file");

   app_shutdown();
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
 * @step 5 call wm_widget_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>image_editor, "bla-bla-bla-there is no such image"</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_n1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");
   wm_widget_list_objects_load(app->project->widgets, evas_object_evas_get(app->win), app->project->swapfile);
   Evas_Object *ie = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(ie, "bla-bla-bla-there is no such image") == EINA_FALSE, "Selected image that not exist");

   app_shutdown();
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
 * @step 5 call wm_widget_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL, "radio.png"</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_n2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");
   wm_widget_list_objects_load(app->project->widgets, evas_object_evas_get(app->win), app->project->swapfile);
   image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(NULL, "bla-bla-bla-there is no such image") == EINA_FALSE, "Selected image that not exist");

   app_shutdown();
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
 * @step 5 call wm_widget_list_objects_load
 * @step 6 add image editor
 *
 * @procedure
 * @step 1 call image_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>image_editor, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_file_choose_test_n3)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");
   wm_widget_list_objects_load(app->project->widgets, evas_object_evas_get(app->win), app->project->swapfile);
   Evas_Object *ie = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_file_choose(ie, NULL) == EINA_FALSE, "Selected image that not exist");

   app_shutdown();
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
