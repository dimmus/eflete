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
 * @addtogroup test_widget_manager
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
 * @step 3 Widget list filled with data (Widgets, Classes, Styles etc).
 * @step 4 Group existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(widget_list, "elm/radio/base/test").
 * @step 2 Check returned object.
 * </td>
 * <td>Eina_Inlist *widget_list; char *full_style_name = "elm/radio/base/test"</td>
 * <td>Not NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_p1)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   const char *full_style_name = "elm/radio/base/test";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   ck_assert_msg(wm_style_object_find(widget_list, full_style_name) != NULL, "Group wasn't found");

   wm_widget_list_free(widget_list);
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
 * @step 3 widget list filled with data (Widgets, Classes, Styles etc).
 * @step 4 Group not existing in edj.
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(widget_list, "elm/widget/base/test").
 * @step 2 Check returned object.
 * </td>
 * <td>Eina_Inlist *widget_list; char *full_style_name = "elm/widget/base/test"</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_p2)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   const char *full_style_name = "elm/widget/base/test";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   ck_assert_msg(wm_style_object_find(widget_list, full_style_name) == NULL, "Unexisted Group was found");

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_object_find
 * @{
 * <tr>
 * <td>wm_style_object_find</td>
 * <td>wm_style_object_find_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(NULL, "elm/widget/base/test").
 * @step 2 Check returned object.
 * </td>
 * <td>NULL, char *full_style_name = "elm/widget/base/test"</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_n1)
{
   elm_init(0,0);
   const char *full_style_name = "elm/widget/base/test";
   ck_assert_msg(wm_style_object_find(NULL, full_style_name) == NULL, "Group was found from NULL list");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_object_find
 * @{
 * <tr>
 * <td>wm_style_object_find</td>
 * <td>wm_style_object_find_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 * @step 3 Widget list filled with data (Widgets, Classes, Styles etc).
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(widget_list, NULL).
 * @step 2 Check returned object.
 * </td>
 * <td>Eina_Inlist *widget_list, NULL</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_n2)
{
   elm_init(0,0);
   const char *file = "./edj_build/radio.edj";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, e, file);
   ck_assert_msg(wm_style_object_find(widget_list, NULL) == NULL, "NULL was found");

   wm_widget_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_object_find
 * @{
 * <tr>
 * <td>wm_style_object_find</td>
 * <td>wm_style_object_find_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_object_find(NULL, NULL).
 * @step 2 Check returned object.
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_object_find_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_style_object_find(NULL, NULL) == NULL, "Group was found");
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
