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

/**
 * @addtogroup image_editor_test
 * @{
 * @addtogroup image_editor_callback_add
 * @{
 * image editor
 * <TABLE>
 * @}
 */

static void
_cb (void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
}

/**
 * @addtogroup image_editor_callback_add
 * @{
 * <tr>
 * <td>image_editor_callback_add</td>
 * <td>image_editor_callback_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create app
 * @step 4 create main window
 * @step 5 load project
 * @step 6 call wm_widget_list_objects_load
 * @step 7 add image editor
 *
 * @procedure
 * @step 1 call image_editor_callback_add
 * @step 2 check returned value
 *
 * </td>
 * <td>Evas_Object *image_editor, Evas_Smart_Cb callback, NULL</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_callback_add_test_p1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("./edj_build/image_editor_callback_add.edj");
   wm_widget_list_objects_load(app->project->widgets,
                               evas_object_evas_get(app->win),
                               app->project->swapfile);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_callback_add(images, _cb, NULL) == EINA_TRUE,
                 "cannot add callback");

   evas_object_del(images);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_callback_add
 * @{
 * <tr>
 * <td>image_editor_callback_add</td>
 * <td>image_editor_callback_add_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create app
 * @step 4 create main window
 * @step 5 load project
 * @step 6 call wm_widget_list_objects_load
 * @step 7 add image editor
 *
 * @procedure
 * @step 1 call image_editor_callback_add
 * @step 2 check returned value
 *
 * </td>
 * <td>Evas_Object *image_editor, Evas_Smart_Cb callback, valid pointer</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_callback_add_test_p2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("./edj_build/image_editor_callback_add.edj");
   wm_widget_list_objects_load(app->project->widgets,
                               evas_object_evas_get(app->win),
                               app->project->swapfile);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_callback_add(images, _cb, app) == EINA_TRUE,
                 "cannot add callback");

   evas_object_del(images);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_callback_add
 * @{
 * <tr>
 * <td>image_editor_callback_add</td>
 * <td>image_editor_callback_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create app
 * @step 4 create main window
 * @step 5 load project
 * @step 6 call wm_widget_list_objects_load
 * @step 7 add image editor
 *
 * @procedure
 * @step 1 call image_editor_callback_add
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL, Evas_Smart_Cb callback, valid pointer</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_callback_add_test_n1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("./edj_build/image_editor_callback_add.edj");
   wm_widget_list_objects_load(app->project->widgets,
                               evas_object_evas_get(app->win),
                               app->project->swapfile);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_callback_add(NULL, _cb, app) == EINA_FALSE,
                 "Callback added to NULL object");

   evas_object_del(images);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_callback_add
 * @{
 * <tr>
 * <td>image_editor_callback_add</td>
 * <td>image_editor_callback_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create app
 * @step 4 create main window
 * @step 5 load project
 * @step 6 call wm_widget_list_objects_load
 * @step 7 add image editor
 *
 * @procedure
 * @step 1 call image_editor_callback_add
 * @step 2 check returned value
 *
 * </td>
 * <td>Evas_Object *image editor, NULL, valid pointer</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_callback_add_test_n2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *images;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("./edj_build/image_editor_callback_add.edj");
   wm_widget_list_objects_load(app->project->widgets,
                               evas_object_evas_get(app->win),
                               app->project->swapfile);
   images = image_editor_window_add(app->project, SINGLE);

   ck_assert_msg(image_editor_callback_add(images, NULL, app) == EINA_FALSE,
                 "Null callback function added");

   evas_object_del(images);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_callback_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
