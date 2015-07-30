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

#include "test_style_editor.h"
#include "test_common.h"

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
 * </tr>
 * @}
 */
EFLETE_TEST(style_editor_window_add_test_p)
{
   App_Data *app = NULL;

   elm_init(0,0);
   setup("style_editor_window_add_test_p");

   app_init();
   app = app_data_get();
   app->project = pm_project_open("./style_editor_window_add_test_p/style_editor_window_add_test_p.pro");

   ui_main_window_add(app);
   ck_assert_msg(style_editor_window_add(app->project) != NULL,
                 "Unable to create style editor");

   elm_shutdown();
   teardown("./style_editor_window_add_test_p");
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
