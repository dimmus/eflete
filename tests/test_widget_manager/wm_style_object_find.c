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
 * @addtogroup wm_style_object_find
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_style_object_find
 * @{
 * <tr>
 * <td>wm_style_object_find</td>
 * <td>wm_style_object_find_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Widget list filled with data (Widgets, Classes, Styles etc).
 * @step 5 Group existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(widget_list, "elm/radio/base/test").
 * @step 2 Check returned object.
 * </td>
 * <td>Eina_Inlist *widget_list; char *full_style_name = "elm/radio/base/test"</td>
 * <td>Not NULL object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_p1)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_object_find.edj";
   const char *full_style_name = "elm/radio/base/test";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open(file, EINA_FALSE);
   e = evas_object_evas_get(win);
   widget_list = wm_widgets_list_new(file);
   wm_widgets_list_objects_load(widget_list, e, mmap_file);
   ck_assert_msg(wm_style_object_find(widget_list, full_style_name) != NULL, "Group wasn't found");

   wm_widgets_list_free(widget_list);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_object_find
 * @{
 * <tr>
 * <td>wm_style_object_find</td>
 * <td>wm_style_object_find_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 widget list filled with data (Widgets, Classes, Styles etc).
 * @step 5 Group not existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(widget_list, "elm/widget/base/test").
 * @step 2 Check returned object.
 * </td>
 * <td>Eina_Inlist *widget_list; char *full_style_name = "elm/widget/base/test"</td>
 * <td>NULL object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_p2)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_object_find.edj";
   const char *full_style_name = "elm/widget/base/test";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);
   widget_list = wm_widgets_list_new(file);
   wm_widgets_list_objects_load(widget_list, e, mmap_file);
   ck_assert_msg(wm_style_object_find(widget_list, full_style_name) == NULL, "Unexisted Group was found");

   wm_widgets_list_free(widget_list);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_object_find
 * @{
 * </TABLE>
 * @}
 * @}
 */
