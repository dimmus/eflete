/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
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
 * @step 3 Layouts list filled with data which storred into edj file.
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_objects_load(widget_list, e, "./edj_build/radio.edj").
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *layouts_list, Evas *e, char *file = "./edj_build/radio.edj"</td>
 * <td>Returned EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_layouts_list_objects_load_test_p)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Eina_Inlist *layouts_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   layouts_list = wm_widget_list_layouts_load(file);
   ck_assert_msg(wm_layouts_list_objects_load(layouts_list, e, file), "Failed load layout.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_layouts_list_objects_load
 * @{
 * <tr>
 * <td>wm_layouts_list_objects_load</td>
 * <td>wm_layouts_list_objects_load_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_objects_load(NULL, e, "./edj_build/radio.edj").
 * @step 2 Check returned bool.
 * </td>
 * <td>NULL, Evas *e, char *file = "./edj_build/radio.edj"</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_layouts_list_objects_load_test_n1)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   ck_assert_msg(!wm_layouts_list_objects_load(NULL, e, file), "Layouts list loaded into NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_layouts_list_objects_load
 * @{
 * <tr>
 * <td>wm_layouts_list_objects_load</td>
 * <td>wm_layouts_list_objects_load_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_objects_load(widget_list, NULL, "./edj_build/radio.edj").
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *layouts_list, NULL, char *file = "./edj_build/radio.edj"</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_layouts_list_objects_load_test_n2)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Eina_Inlist *layouts_list = NULL;

   layouts_list = wm_widget_list_layouts_load(file);
   ck_assert_msg(!wm_layouts_list_objects_load(layouts_list, NULL, file), "Layouts list loaded without canvas pointer.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_layouts_list_objects_load
 * @{
 * <tr>
 * <td>wm_layouts_list_objects_load</td>
 * <td>wm_layouts_list_objects_load_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 *
 * @procedure
 * @step 1 Call function wm_layouts_list_objects_load(widget_list, e, NULL).
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *layouts_list, Evas *e, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(wm_layouts_list_objects_load_test_n3)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Eina_Inlist *layouts_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   layouts_list = wm_widget_list_layouts_load(file);
   ck_assert_msg(!wm_layouts_list_objects_load(layouts_list, e, NULL), "Layouts List loaded without file name.");

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
