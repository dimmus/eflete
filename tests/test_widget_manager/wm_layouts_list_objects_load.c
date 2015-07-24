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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_layouts_list_objects_load
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_layouts_list_objects_load
 * @{
 * <tr>
 * <td>wm_layouts_list_objects_load</td>
 * <td>wm_layouts_list_objects_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Layouts list filled with data which storred into edj file.
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_objects_load(widget_list, e, mmap_file).
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *layouts_list, Evas *e, (Eina_File *)mmap_file</td>
 * <td>Returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_layouts_list_objects_load_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_layouts_list_objects_load.edj";
   Eina_Inlist *layouts_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);
   layouts_list = wm_layouts_list_new(file);
   ck_assert_msg(wm_layouts_list_objects_load(layouts_list, e, mmap_file),
                 "Failed load layout.");

   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_layouts_list_objects_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
