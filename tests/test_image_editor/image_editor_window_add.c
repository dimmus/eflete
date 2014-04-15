/**
 * Edje Theme Editor
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_window_add_test_p1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("test", "./edj_build/radio.edj");

   ck_assert_msg(image_editor_window_add(app->project, SINGLE) != NULL,
                 "failure: cannot create image editor window in SINGLE mode");

   app_shutdown();
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_editor_window_add_test_p2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   Project *project = pm_open_project_edj("test", "./edj_build/radio.edj");

   ck_assert_msg(image_editor_window_add(project, MULTIPLE) != NULL,
                 "failure: cannot create image editor window in MULTIPLE mode");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_window_add
 * @{
 * <tr>
 * <td>image_editor_window_add</td>
 * <td>image_editor_window_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 *
 * @procedure
 * @step 1 call image_editor_window_add
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
EFLETE_TEST (image_editor_window_add_test_n1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);

   ck_assert_msg(image_editor_window_add(NULL, SINGLE) == NULL,
                 "Created image editor in SINGLE mode for NULL project");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_editor_window_add
 * @{
 * <tr>
 * <td>image_editor_window_add</td>
 * <td>image_editor_window_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 *
 * @procedure
 * @step 1 call image_editor_window_add
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
EFLETE_TEST (image_editor_window_add_test_n2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);

   ck_assert_msg(image_editor_window_add(NULL, MULTIPLE) == NULL,
                 "Created image editor in MULTIPLE mode for NULL project");

   app_shutdown();
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
