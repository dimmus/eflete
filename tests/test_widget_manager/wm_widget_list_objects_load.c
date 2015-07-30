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
 * @addtogroup wm_widget_list_objects_load
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_widget_list_objects_load
 * @{
 * <tr>
 * <td>wm_widget_list_objects_load</td>
 * <td>wm_widget_list_objects_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Widget list filled with data (Widgets, Classes, Styles etc).
 * @step 5 Group existing in edj and there is an alias group.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_objects_load(widget_list, e, "./edj_build/wm_widget_list_objects_load.edj").
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *widget_list, Evas *e, char *file = "./edj_build/wm_widget_list_objects_load.edj"</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_list_objects_load_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_widget_list_objects_load.edj";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);
   widget_list = wm_widgets_list_new(file);
   ck_assert_msg(wm_widgets_list_objects_load(widget_list, e, mmap_file), "Cannot load.");

   wm_widgets_list_free(widget_list);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_objects_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
