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

#include "test_style_manager.h"
#include "test_common.h"

/**
 * @addtogroup style_manager_test
 * @{
 * @addtogroup style_manager_add
 * @{
 * style_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup style_manager_add
 * @{
 * <tr>
 * <td>style_manager_add</td>
 * <td>style_manager_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm and app
 * @step 2 main window created
 * @step 3 project opened
 *
 * @procedure
 * @step 1 Call style_manager_add()
 * @step 2 Check  returned pointer
 * </td>
 * <td>Project *project</td>
 * <td>returned pointer is not NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(style_manager_add_test_p)
{
   elm_init(0,0);

   app_init();
   ap.project = setup("style_manager_add_test_p");

   ui_main_window_add();
   ck_assert_msg(style_manager_add(ap.project) != NULL,
                 "Unable to create style editor");

   elm_shutdown();
   teardown("./style_manager_add_test_p");
   app_shutdown();
}
END_TEST

/**
 * @addtogroup style_manager_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
