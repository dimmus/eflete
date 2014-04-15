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

#include "test_style_editor.h"

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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ui_edj_load_done(app, "./edj_build/radio.edj");
   ck_assert_msg(style_editor_window_add(app->project) != NULL,
                 "Style_edtor not added");
   elm_shutdown();
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_n2)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_edj_load_done(app, "./edj_build/radio.edj");
   ck_assert_msg(style_editor_window_add(app->project) == NULL,
                 "Style_edtor added to non-existing window");
   elm_shutdown();
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_n3)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
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
