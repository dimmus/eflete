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

#include "test_image_manager.h"
#include "test_common.h"

/**
 * @addtogroup image_manager_test
 * @{
 * @addtogroup image_manager_add
 * @{
 * image manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup image_manager_add
 * @{
 * <tr>
 * <td>image_manager_add</td>
 * <td>image_manager_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 *
 * @procedure
 * @step 1 call image_manager_add
 * @step 2 check returned value
 *
 * </td>
 * <td>Nothing</td>
 * <td>Not NULL Pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (image_manager_add_test_p)
{
   elm_init(0,0);

   app_init();
   Evas_Object *images;
   ui_main_window_add();
   ap.project = setup("image_manager_add_test_p");

   images = image_manager_add();
   ck_assert_msg(images != NULL,
                 "cannot create image editor window in SINGLE mode");

   evas_object_del(images);
   app_shutdown();
   teardown("image_manager_add_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup image_manager_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
