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

#include "test_sound_editor.h"
#include "test_common.h"

/**
 * @addtogroup sound_editor_test
 * @{
 * @addtogroup sound_editor_window_add
 * @{
 * sound_editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup sound_editor_window_add
 * @{
 * <tr>
 * <td>sound_editor_window_add</td>
 * <td>sound_editor_window_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 *
 * @procedure
 * @step 1 Call sound_editor_window_add()
 * @step 2 Check returned pointer
 * </td>
 * <td>(Project *) project, SINGLE</td>
 * <td>returned pointer is not NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(sound_editor_window_add_test_p1)
{
   elm_init(0,0);
   setup("sound_editor_window_add_test_p1");

   Evas_Object *sounds;
   App_Data *app = NULL;

   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./sound_editor_window_add_test_p1/sound_editor_window_add_test_p1.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->dev);

   sounds = sound_editor_window_add(app->project, SINGLE);
   ck_assert_msg(sounds != NULL, "cannot create sound editor window in SINGLE mode");

   evas_object_del(sounds);
   ui_main_window_del(app);
   app_shutdown();
   teardown("./sound_editor_window_add_test_p1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_window_add
 * @{
 * <tr>
 * <td>sound_editor_window_add</td>
 * <td>sound_editor_window_add_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 *
 * @procedure
 * @step 1 call sound_editor_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>(Project *) project, MULTIPLE</td>
 * <td>Not NULL Pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_window_add_test_p2)
{
   elm_init(0,0);
   setup("sound_editor_window_add_test_p2");

   App_Data *app;
   app_init();
   Evas_Object *sounds;

   app = app_data_get();
   app->project = pm_project_open("./sound_editor_window_add_test_p2/sound_editor_window_add_test_p2.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                               evas_object_evas_get(app->win),
                               app->project->dev);
   ui_main_window_add(app);

   sounds = sound_editor_window_add(app->project, MULTIPLE);
   ck_assert_msg(sounds != NULL, "cannot create sound editor window in MULTIPLE mode");

   evas_object_del(sounds);
   ui_main_window_del(app);
   app_shutdown();
   teardown("./sound_editor_window_add_test_p2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_window_add
 * @{
 * <tr>
 * <td>sound_editor_window_add</td>
 * <td>sound_editor_window_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 *
 * @procedure
 * @step 1 call sound_editor_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL, SINGLE</td>
 * <td>NULL Pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_window_add_test_n1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_data_get();
   ui_main_window_add(app);

   ck_assert_msg(sound_editor_window_add(NULL, SINGLE) == NULL,
                 "Created sound editor in SINGLE mode for NULL project");

   ui_main_window_del(app);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_window_add
 * @{
 * <tr>
 * <td>sound_editor_window_add</td>
 * <td>sound_editor_window_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 *
 * @procedure
 * @step 1 call sound_editor_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL, MULTIPLE</td>
 * <td>NULL Pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_window_add_test_n2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *sounds;

   app = app_data_get();
   ui_main_window_add(app);

   sounds = sound_editor_window_add(NULL, MULTIPLE);
   ck_assert_msg(sounds == NULL,
                 "Created sound editor in MULTIPLE mode for NULL project");

   ui_main_window_del(app);
   evas_object_del(sounds);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_window_add
 * @{
 * <tr>
 * <td>sound_editor_window_add</td>
 * <td>sound_editor_window_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 load project without creating main window
 *
 * @procedure
 * @step 1 call sound_editor_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>(Project *) project, SINGLE</td>
 * <td>NULL Pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_window_add_test_n3)
{
   App_Data *app;
   Evas_Object *sounds;

   elm_init(0,0);
   setup("sound_editor_window_add_test_n3");
   app_init();

   app = app_data_get();
   app->project = pm_project_open("./sound_editor_window_add_test_n3/sound_editor_window_add_test_n3.pro");
   wm_widgets_list_objects_load(app->project->widgets,
                                evas_object_evas_get(app->win),
                                app->project->dev);

   sounds = sound_editor_window_add(app->project, SINGLE);
   ck_assert_msg(sounds == NULL,
                 "Created sound editor in SINGLE  mode without creating main window");

   evas_object_del(sounds);
   app_shutdown();
   teardown("./sound_editor_window_add_test_n3");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
