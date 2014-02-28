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
 * @addtogroup wm_part_del
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_part_del
 * @{
 * <tr>
 * <td>wm_part_del</td>
 * <td>wm_part_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 Create Part with a name of existing part.
 *
 * @procedure
 * @step 1 Call function wm_part_del(style, "elm.swallow.content").
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style, "elm.swallow.content"</td>
 * <td>true returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFLETE_TEST (wm_part_del_test_p1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);
   ck_assert_msg(wm_part_del(style, part) == true, "cannot delete Part object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_del
 * @{
 * <tr>
 * <td>wm_part_del</td>
 * <td>wm_part_del_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 Create Part but don't add it to Style structure (without using wm_part_add).
 *
 * @procedure
 * @step 1 Call function wm_part_del(style, "elm.double.eflete").
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style, "elm.double.eflete"</td>
 * <td>true returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_del_test_p2)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj", "elm/radio/base/def");

   part = (Part *)mem_malloc(sizeof(Part));
   part->name = eina_stringshare_add("elm.double.eflete");
   ck_assert_msg(wm_part_del(style, part) == false, "Part was found in Style and deleted");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_del
 * @{
 * <tr>
 * <td>wm_part_del</td>
 * <td>wm_part_del_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 Create Part with a name of existing part.
 *
 * @procedure
 * @step 1 Call function wm_part_del(NULL, part).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, "elm.double.eflete"</td>
 * <td>false returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_del_test_n1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);
   ck_assert_msg(wm_part_del(NULL, part) == false, "Part object deleted from NULL Style");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_del
 * @{
 * <tr>
 * <td>wm_part_del</td>
 * <td>wm_part_del_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 *
 * @procedure
 * @step 1 Call function wm_part_del(style, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style, NULL</td>
 * <td>false returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_del_test_n2)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL;
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj", "elm/radio/base/def");
   ck_assert_msg(wm_part_del(style, NULL) == false, "deleting not existing Part");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_del
 * @{
 * <tr>
 * <td>wm_part_del</td>
 * <td>wm_part_del_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_free(NULL, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>false returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_del_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_part_del(NULL, NULL) == false, "NULL Part object was deleted");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
