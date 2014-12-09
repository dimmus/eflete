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
 * @addtogroup sound_editor_file_choose
 * @{
 * sound editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup sound_editor_file_choose
 * @{
 * <tr>
 * <td>sound_editor_file_choose</td>
 * <td>sound_editor_file_choose_test_p</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 add sound editor
 *
 * @procedure
 * @step 1 call sound_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>(Evas_Object *) sound_editor, (const char *) "key-tap"</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_file_choose_test_p)
{
   elm_init(0,0);
   setup("sound_editor_file_choose_test_p");

   App_Data *app;
   app_init();
   Evas_Object *sound_editor;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./sound_editor_file_choose_test_p/sound_editor_file_choose_test_p.pro");
   wm_widgets_list_objects_load(app->project->widgets, evas_object_evas_get(app->win), app->project->dev);
   sound_editor = sound_editor_window_add(app->project, SOUND_EDITOR_EDIT);

   ck_assert_msg(sound_editor_file_choose(sound_editor, "key-tap") == EINA_TRUE,
                 "failure: cannot select file");

   evas_object_del(sound_editor);
   app_shutdown();
   teardown("./sound_editor_file_choose_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_file_choose
 * @{
 * <tr>
 * <td>sound_editor_file_choose</td>
 * <td>sound_editor_file_choose_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 add sound editor
 *
 * @procedure
 * @step 1 call sound_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>(Evas_Object *) sound_editor, (const char *) "not_correct_sound_name"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_file_choose_test_n1)
{
   elm_init(0,0);
   setup("sound_editor_file_choose_test_n1");

   App_Data *app;
   app_init();
   Evas_Object *sound_editor;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./sound_editor_file_choose_test_n1/sound_editor_file_choose_test_n1.pro");
   sound_editor = sound_editor_window_add(app->project, SOUND_EDITOR_EDIT);

   ck_assert_msg(sound_editor_file_choose(sound_editor, "not_correct_sound_name") == EINA_FALSE,
                 "Selected sound that not exist");

   evas_object_del(sound_editor);
   app_shutdown();
   teardown("./sound_editor_file_choose_test_n1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_file_choose
 * @{
 * <tr>
 * <td>sound_editor_file_choose</td>
 * <td>sound_editor_file_choose_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 *
 * @procedure
 * @step 1 call sound_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL, (const char *) "not_correct_sound_name"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_file_choose_test_n2)
{
   elm_init(0,0);
   app_init();

   ck_assert_msg(sound_editor_file_choose(NULL, "not_correct_sound_name") == EINA_FALSE,
                 "Selected sound that not exist");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_file_choose
 * @{
 * <tr>
 * <td>sound_editor_file_choose</td>
 * <td>sound_editor_file_choose_test_n3</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 add sound editor
 *
 * @procedure
 * @step 1 call sound_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>sound_editor, NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_file_choose_test_n3)
{
   elm_init(0,0);
   setup("sound_editor_file_choose_test_n3");

   App_Data *app;
   app_init();
   Evas_Object *sound_editor;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./sound_editor_file_choose_test_n3/sound_editor_file_choose_test_n3.pro");
   sound_editor = sound_editor_window_add(app->project, SOUND_EDITOR_EDIT);

   ck_assert_msg(sound_editor_file_choose(sound_editor, NULL) == EINA_FALSE, "Selected sound that not exist");

   evas_object_del(sound_editor);
   app_shutdown();
   teardown("./sound_editor_file_choose_test_n3");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_file_choose
 * @{
 * <tr>
 * <td>sound_editor_file_choose</td>
 * <td>sound_editor_file_choose_test_n4</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 * @step 5 add button (not correct evas object)
 *
 * @procedure
 * @step 1 call sound_editor_file_choose
 * @step 2 check returned value
 *
 * </td>
 * <td>(Evas_Object *) not_correct_object, (const char *) "key-tap"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (sound_editor_file_choose_test_n4)
{
   elm_init(0,0);
   setup("sound_editor_file_choose_test_n4");

   App_Data *app;
   app_init();
   Evas_Object *not_corect_object;

   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_project_open("./sound_editor_file_choose_test_n4/sound_editor_file_choose_test_n4.pro");
   not_corect_object = elm_button_add(app->win);

   ck_assert_msg(sound_editor_file_choose(not_corect_object, "not_correct_sound_name") == EINA_FALSE,
                 "Selected sound from not correct object");

   evas_object_del(not_corect_object);
   app_shutdown();
   teardown("./sound_editor_file_choose_test_n4");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_file_choose
 * @{
 * </TABLE>
 * @}
 * @}
 */
