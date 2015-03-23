/**
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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
 * @addtogroup wm_style_data_reload
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_style_data_reload
 * @{
 * <tr>
 * <td>wm_style_data_reload</td>
 * <td>wm_style_data_reload_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Create Style structute for load data.
 * @step 5 Load data into style structure.
 *
 * @procedure
 * @step 1 Add new part directly into edj file.
 * @step 2 Reload data with function wm_style_data_reload.
 * @step 3 Check part structure in reloaded style.
 * @step 4 Check type of the loaded part structure,
 * </td>
 * <td>(Style *)style, (Eina_File *) mmap_file</td>
 * <td>All Checks passed successfuly</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_data_reload_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_data_reload.edj";
   const char *group_name = "elm/radio/base/def";
   Eina_Stringshare *part_name = eina_stringshare_add("new_part");

   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL;
   Part *part = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Bool result = EINA_FALSE;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);
   style = wm_style_add(group_name, group_name, LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);

   edje_edit_part_add(style->obj, part_name, EDJE_PART_TYPE_RECTANGLE);
   result = wm_style_data_reload(style, mmap_file);
   ck_assert_msg(result, "Failed reload data of the style");
   part = wm_part_by_name_find(style, part_name);
   ck_assert_msg(part != NULL, "Can not find part instance in style");
   ck_assert_msg(part->type == EDJE_PART_TYPE_RECTANGLE, "Wrong part type");

   eina_stringshare_del(part_name);
   wm_style_free(style);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_data_reload
 * @{
 * <tr>
 * <td>wm_style_data_reload</td>
 * <td>wm_style_data_reload_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Mmap edj file.
 * @step 3 Create Style structute for load data.
 *
 * @procedure
 * @step 1 Reload data with function wm_style_data_reload.
 * </td>
 * <td>(Style *)style, (Eina_File *)mmap_file</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_data_reload_test_n1)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_data_reload.edj";
   const char *group_name = "elm/radio/base/def";

   Style *style = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Bool result = EINA_TRUE;

   mmap_file = eina_file_open(file, EINA_FALSE);
   style = wm_style_add(group_name, group_name, LAYOUT, NULL);

   result = wm_style_data_reload(style, mmap_file);
   ck_assert_msg(!result, "Reload data of the style, without preload");

   wm_style_free(style);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_data_reload
 * @{
 * <tr>
 * <td>wm_style_data_reload</td>
 * <td>wm_style_data_reload_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Create Style structute for load data.
 * @step 5 Load data into style structure.
 *
 * @procedure
 * @step 1 Reload data with function wm_style_data_reload, without file pointer.
 * </td>
 * <td>(Style *)style, (Eina_File *)NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_data_reload_test_n2)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_data_reload.edj";
   const char *group_name = "elm/radio/base/def";

   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL;
   Eina_File *mmap_file = NULL;
   Eina_Bool result = EINA_TRUE;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);
   style = wm_style_add(group_name, group_name, LAYOUT, NULL);
   wm_style_data_load(style, e, mmap_file);

   result = wm_style_data_reload(style, NULL);
   ck_assert_msg(!result, "Reload data of the style without source file");

   wm_style_free(style);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_data_reload
 * @{
 * <tr>
 * <td>wm_style_data_reload</td>
 * <td>wm_style_data_reload_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 3 Mmap edj file.
 *
 * @procedure
 * @step 1 Reload data with function wm_style_data_reload, without style pointer
 * </td>
 * <td>(Style *)NULL, (Eina_File *) mmap_file</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_data_reload_test_n3)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_style_data_reload.edj";

   Eina_File *mmap_file = NULL;
   Eina_Bool result = EINA_TRUE;

   mmap_file = eina_file_open(file, EINA_FALSE);

   result = wm_style_data_reload(NULL, mmap_file);
   ck_assert_msg(!result, "Reload data of the style, without style pointer");

   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup wm_style_data_reload
 * @{
 * </TABLE>
 * @}
 * @}
 */
