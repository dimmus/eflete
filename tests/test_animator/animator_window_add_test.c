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

#include "test_animator.h"
#include "main_window.h"
#include "test_common.h"

/**
 * @addtogroup animator_test
 * @{
 * @addtogroup animator_window_add
 * @{
 * animator
 * <TABLE>
 * @}
 */

/**
 * @addtogroup animator_window_add
 * @{
 * <tr>
 * <td>animator_window_add</td>
 * <td>animator_window_add_test_p</td>
 * <td>
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 Mmap edj file.
 * @step 4 Style filled with parts
 *
 * @procedure
 * @step 1 Call animator_window_add(Style)
 * @step 2 Check returned pointer
 * </td>
 * <td>Style *style</td>
 * <td>Not NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (animator_window_add_test_p)
{
   //Style *style = NULL;
   //Evas_Object *manager;

ck_assert(false);
   elm_init(0, 0);
   app_init();

   ui_main_window_add();
   ap.project = setup("animator_window_add_test_p");
/*   wm_widgets_list_objects_load(ap.project->widgets,
                                evas_object_evas_get(ap.win), ap.project->mmap_file);
   blocks_show();*/
   //style = wm_style_object_find(ap.project->widgets, "elm/radio/base/def");
//   ui_style_clicked(style);

   //manager = animator_window_add(style);
   ck_assert_msg(NULL != NULL, "cannot create new Animator");

   //evas_object_del(manager);
   pm_project_close(ap.project);
   ap.project = NULL;
   ui_main_window_del();
   app_shutdown();
   teardown("./animator_window_add_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup animator_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
