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
 * @addtogroup wm_style_copy
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_style_copy
 * @{
 * <tr>
 * <td>wm_style_copy</td>
 * <td>wm_style_copy_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Widget list filled with data (Widgets, Classes, Styles etc).
 * @step 5 New group is not exists and Old Group is exists in edj.
 * @step 6 New Style structure with new name (for copied group).
 *
 * @procedure
 * @step 1 Add new style.
 * @step 2 Call function wm_style_copy(edje_edit_obj, "elm/radio/base/undef", "./edj_build/wm_style_copy.edj", Style).
 * @step 3 Check if group was added into a file successfully.
 * @step 4 Load all data of new group from the file
 * @step 5 Get signal list from old and new style
 * @step 6 Compare old and new style by comparing it's signal from their signal lists.
 * </td>
 * <td>Evas_Object *edje_edit_obj, char *group_name = "elm/radio/base/undef", char *file = "./edj_build/wm_style_copy.edj", Style *style</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_copy_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_copy.edj";
   const char *group_name = "elm/radio/base/def";
   const char *copy_name = "elm/radio/base/undef";
   const char *style_name = "undef";

   Eina_Inlist *widget_list = NULL;
   Eina_List *sig_list = NULL, *sig_new_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL, *new_style = NULL;
   Class *dest_class = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);
   widget_list = wm_widgets_list_new(file);
   wm_widgets_list_objects_load(widget_list, e, mmap_file);
   style = wm_style_object_find(widget_list, group_name);

   /* before copy we need to do something with new style */
   dest_class = EINA_INLIST_CONTAINER_GET(
                   EINA_INLIST_CONTAINER_GET(widget_list, Widget)->classes,
                   Class);
   new_style = wm_style_add(style_name, copy_name, STYLE, NULL);
   dest_class->styles = eina_inlist_append(dest_class->styles,
                                           EINA_INLIST_GET(new_style));

   new_style = EINA_INLIST_CONTAINER_GET(dest_class->styles, Style);
   wm_style_data_load(style, e, mmap_file);
   /* ---------------------------------------- */

   ck_assert_msg(wm_style_copy(new_style->obj, group_name, copy_name, file, new_style), "Cannot copy.");
   ck_assert_msg(edje_edit_group_exist(new_style->obj, copy_name), "Group is not exist.");
   edje_edit_group_del(new_style->obj, copy_name);
   wm_style_data_load(new_style, e, mmap_file);
   sig_new_list = wm_program_signals_list_get(new_style);
   sig_list = wm_program_signals_list_get(new_style);
   ck_assert_str_eq((char *)sig_list->next->data, (char *)sig_new_list->next->data);

   wm_widgets_list_free(widget_list);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_copy
 * @{
 * </TABLE>
 * @}
 * @}
 */
