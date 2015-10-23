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
 * @addtogroup image_editor_window_add
 * @{
 * image editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup image_editor_window_add
 * @{
 * <tr>
 * <td>image_editor_window_add</td>
 * <td>image_editor_window_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 *
 * @procedure
 * @step 1 call image_editor_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>project, SINGLE</td>
 * <td>Not NULL Pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_window_add_test_p1)
{
   elm_init(0,0);

   app_init();
   Evas_Object *images;
   ui_main_window_add();
   ap.project = setup("image_editor_window_add_test_p1");

   images = image_editor_window_add(SINGLE);
   ck_assert_msg(images != NULL,
                 "cannot create image editor window in SINGLE mode");

   evas_object_del(images);
   app_shutdown();
   teardown("image_editor_window_add_test_p1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_window_add
 * @{
 * <tr>
 * <td>image_editor_window_add</td>
 * <td>image_editor_window_add_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 *
 * @procedure
 * @step 1 call image_editor_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>project, MULTIPLE</td>
 * <td>Not NULL Pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_window_add_test_p2)
{
   elm_init(0,0);

   app_init();
   Evas_Object *images;

   ui_main_window_add();
   //Project *project = setup("image_editor_window_add_test_p2");

   images = image_editor_window_add(MULTIPLE);
   ck_assert_msg(images != NULL,
                 "cannot create image editor window in MULTIPLE mode");

   evas_object_del(images);
   app_shutdown();
   //teardown("./image_editor_window_add_test_p2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
