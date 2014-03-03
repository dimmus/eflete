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
 * @addtogroup wm_part_add
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_part_add
 * @{
 * <tr>
 * <td>wm_part_add</td>
 * <td>wm_part_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 edj file contain requested part
 *
 * @procedure
 * @step 1 Call function wm_part_add(style, "elm.swallow.content").
 * @step 2 Check returned Part
 * @step 3 Check Part's type.
 * </td>
 * <td>Style *style, const char *part_name = "elm.swallow.content"</td>
 * <td>All check's passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_add_test_p)
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
   ck_assert_msg(part != NULL, "cannot create Part object");
   ck_assert_msg(part->type == EDJE_PART_TYPE_SWALLOW, "Part has wrong type");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add
 * @{
 * <tr>
 * <td>wm_part_add</td>
 * <td>wm_part_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested group
 * @step 5 edj file doesn't contain requested part
 *
 * @procedure
 * @step 1 Call function wm_part_add(style, "eflete.part").
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style, const char *part_name = "eflete.part"</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_add_test_n1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "eflete.part";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/radio.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);
   ck_assert_msg(part == NULL, "Unexisting Part was successfully added");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add
 * @{
 * <tr>
 * <td>wm_part_add</td>
 * <td>wm_part_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested group
 *
 * @procedure
 * @step 1 Call function wm_part_add(style, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style, NULL</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_add_test_n2)
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
   part = wm_part_add(style, NULL);
   ck_assert_msg(part == NULL, "Part without a name was successfully added");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add
 * @{
 * <tr>
 * <td>wm_part_add</td>
 * <td>wm_part_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_add(NULL, "elm.swallow.content").
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, const char *part = "elm.swallow.content"</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_add_test_n3)
{
   elm_init(0,0);
   const char *part = "elm.swallow.content";
   ck_assert_msg(wm_part_add(NULL, part) == NULL, "Part was added successfuly without edje edit object.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_add
 * @{
 * <tr>
 * <td>wm_part_add</td>
 * <td>wm_part_add_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_part_add(NULL, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_add_test_n4)
{
   elm_init(0,0);
   ck_assert_msg(wm_part_add(NULL, NULL) == NULL, "Part was added successfuly with NULL parametres");
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup wm_part_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
