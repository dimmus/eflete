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
 * <td>sound_editor_window_add_test_p</td>
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
 * <td>No data</td>
 * <td>returned pointer is not NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(sound_editor_window_add_test_p)
{
   elm_init(0,0);

   Evas_Object *sounds;

   app_init();
   ui_main_window_add();
   ap.project = setup("sound_editor_window_add_test_p");

   sounds = sound_editor_window_add();
   ck_assert_msg(sounds != NULL, "cannot create sound editor window");

   evas_object_del(sounds);
   app_shutdown();
   teardown("./sound_editor_window_add_test_p");
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
