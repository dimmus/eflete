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
 * @addtogroup wm_styles_build_alias
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_styles_build_alias
 * @{
 * <tr>
 * <td>wm_styles_build_alias</td>
 * <td>wm_styles_build_alias_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Layouts list filled and loaded with data which storred into edj file.
 * @step 5 Widgets list filled and loaded with data which storred into edj file.
 *
 * @procedure
 * @step 1 Call function wm_styles_build_alias(widget_list, layouts_list).
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *widgets_list, Eina_Inlist *layouts_list</td>
 * <td>Returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_styles_build_alias_test_p1)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_styles_build_alias.edj";
   Eina_Inlist *layouts_list = NULL, *widgets_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);

   /* loading layouts */
   layouts_list = wm_layouts_list_new(file);
   wm_layouts_list_objects_load(layouts_list, e, mmap_file);

   /* loading widgets */
   widgets_list = wm_widgets_list_new(file);
   wm_widgets_list_objects_load(widgets_list, e, mmap_file);

   ck_assert_msg(wm_styles_build_alias(widgets_list, layouts_list),
                 "Failed load aliases.");

   wm_widgets_list_free(widgets_list);
   wm_layouts_list_free(layouts_list);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_styles_build_alias
 * @{
 * <tr>
 * <td>wm_styles_build_alias</td>
 * <td>wm_styles_build_alias_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Mmap edj file.
 * @step 4 Layouts list filled and loaded with data which storred into edj file.
 * @step 5 Widgets list filled and loaded with data which storred into edj file.
 *
 * @procedure
 * @step 1 Call function wm_styles_build_alias(NULL, layouts_list).
 * @step 2 Check returned bool.
 * @step 3 Call function wm_styles_build_alias(widgets_list, NULL).
 * @step 4 Check returned bool.
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_styles_build_alias_test_p2)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_styles_build_alias.edj";
   Eina_Inlist *layouts_list = NULL, *widgets_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Eina_File *mmap_file = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   mmap_file = eina_file_open(file, EINA_FALSE);

   /* loading layouts */
   layouts_list = wm_layouts_list_new(file);
   wm_layouts_list_objects_load(layouts_list, e, mmap_file);

   /* loading widgets */
   widgets_list = wm_widgets_list_new(file);
   wm_widgets_list_objects_load(widgets_list, e, mmap_file);

   ck_assert_msg(wm_styles_build_alias(NULL, layouts_list),
                 "Failed load aliases.");
   ck_assert_msg(wm_styles_build_alias(widgets_list, NULL),
                 "Failed load aliases.");

   wm_widgets_list_free(widgets_list);
   wm_layouts_list_free(layouts_list);
   eina_file_close(mmap_file);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_styles_build_alias
 * @{
 * <tr>
 * <td>wm_styles_build_alias</td>
 * <td>wm_styles_build_alias_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_styles_build_alias(NULL, layouts_list).
 * @step 2 Check returned bool.
 * </td>
 * <td>NULL, NULL</td>
 * <td>Returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_styles_build_alias_test_n)
{
   elm_init(0,0);

   ck_assert_msg(!wm_styles_build_alias(NULL, NULL),
                 "Aliases loaded but shoudn't.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_styles_build_alias
 * @{
 * </TABLE>
 * @}
 * @}
 */
