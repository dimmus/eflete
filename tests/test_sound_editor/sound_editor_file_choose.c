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

   ck_assert(false);
   app_init();
   Evas_Object *sound_editor;

   ui_main_window_add();
   ap.project = setup("sound_editor_file_choose_test_p");

   //wm_widgets_list_objects_load(ap.project->widgets, evas_object_evas_get(ap.win), ap.project->mmap_file);
   sound_editor = sound_editor_window_add(SOUND_EDITOR_EDIT);

   ck_assert_msg(sound_editor_file_choose(sound_editor, "key-tap") == EINA_TRUE,
                 "failure: cannot select file");

   pm_project_close(ap.project);
   ap.project = NULL;

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

   app_init();
   Evas_Object *sound_editor;

   ui_main_window_add();
   ap.project = setup("sound_editor_file_choose_test_n3");
   sound_editor = sound_editor_window_add(SOUND_EDITOR_EDIT);

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
 * </TABLE>
 * @}
 * @}
 */
