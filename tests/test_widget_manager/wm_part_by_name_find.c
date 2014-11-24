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
 * @addtogroup wm_part_by_name_find
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_part_by_name_find
 * @{
 * <tr>
 * <td>wm_part_by_name_find</td>
 * <td>wm_part_by_name_find_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize Elementary.
 * @step 2 Create  Evas canvas.
 * @step 3 Create new widget list with using wm_widget_list_new.
 * @step 4 Fill widget list with data (Widgets, Classes, Styles, Parts).
 * @step 5 Find Style structure
 *
 * @procedure
 * @step 1 Call function wm_part_by_name_find.
 * @step 2 Check returned object.
 * @step 3 Check name, that stored in returned structure.
 * </td>
 * <td>(Style *) style, (Eina_Stringshare *) "elm.text"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_by_name_find_test_p1)
{
   const char *file = "./edj_build/wm_part_by_name_find.edj";
   Eina_Stringshare *group_name = NULL;
   Eina_Stringshare *part_name = NULL;
   Eina_Inlist *widget_list = NULL;
   Ecore_Evas *ee = NULL;
   Evas *canvas = NULL;
   Part *part = NULL;
   Style *style = NULL;

   elm_init(0, 0);
   group_name = eina_stringshare_add("elm/radio/base/def");
   part_name = eina_stringshare_add("elm.text");
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, canvas, file);
   style = wm_style_object_find(widget_list, group_name);

   part = wm_part_by_name_find(style, part_name);
   ck_assert_msg(part != NULL, "Part wasn't found");
   ck_assert_msg(part->name == part_name, "Founded strucutre isn't searching part");

   wm_widget_list_free(widget_list);
   eina_stringshare_del(group_name);
   eina_stringshare_del(part_name);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_by_name_find
 * @{
 * <tr>
 * <td>wm_part_by_name_find</td>
 * <td>wm_part_by_name_find_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize Elementary.
 * @step 2 Create  Evas canvas.
 * @step 3 Create new widget list with using wm_widget_list_new.
 * @step 4 Fill widget list with data (Widgets, Classes, Styles, Parts).
 * @step 5 Find Style structure
 *
 * @procedure
 * @step 1 Call function wm_part_by_name_find with wrong part name.
 * @step 2 Check returned object.
 * </td>
 * <td>(Style *) style, (Eina_Stringshare *) "wrong"</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_by_name_find_test_n1)
{
   const char *file = "./edj_build/wm_part_by_name_find.edj";
   Eina_Stringshare *group_name = NULL;
   Eina_Stringshare *part_name = NULL;
   Eina_Inlist *widget_list = NULL;
   Ecore_Evas *ee = NULL;
   Evas *canvas = NULL;
   Part *part = NULL;
   Style *style = NULL;

   elm_init(0, 0);
   group_name = eina_stringshare_add("elm/radio/base/def");
   part_name = eina_stringshare_add("wrong");
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, canvas, file);
   style = wm_style_object_find(widget_list, group_name);

   part = wm_part_by_name_find(style, part_name);
   ck_assert_msg(!part, "Founded part, that not exists in group");

   wm_widget_list_free(widget_list);
   eina_stringshare_del(group_name);
   eina_stringshare_del(part_name);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup wm_part_by_name_find
 * @{
 * <tr>
 * <td>wm_part_by_name_find</td>
 * <td>wm_part_by_name_find_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize Elementary.
 * @step 2 Create  Evas canvas.
 * @step 3 Create new widget list with using wm_widget_list_new.
 * @step 4 Fill widget list with data (Widgets, Classes, Styles, Parts).
 * @step 5 Find Style structure
 *
 * @procedure
 * @step 1 Call function wm_part_by_name_find withou part name.
 * @step 2 Check returned object.
 * </td>
 * <td>Style *style, NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_by_name_find_test_n2)
{
   const char *file = "./edj_build/wm_part_by_name_find.edj";
   Eina_Stringshare *group_name = NULL;
   Eina_Inlist *widget_list = NULL;
   Ecore_Evas *ee = NULL;
   Evas *canvas = NULL;
   Part *part = NULL;
   Style *style = NULL;

   elm_init(0, 0);
   group_name = eina_stringshare_add("elm/radio/base/def");
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, canvas, file);
   style = wm_style_object_find(widget_list, group_name);

   part = wm_part_by_name_find(style, NULL);
   ck_assert_msg(!part, "Founded part with NULL part param.");

   wm_widget_list_free(widget_list);
   eina_stringshare_del(group_name);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_by_name_find
 * @{
 * <tr>
 * <td>wm_part_by_name_find</td>
 * <td>wm_part_by_name_find_test_n3</td>
 * <td>
 * @precondition
 * @step 1 Initialize Elementary.
 *
 * @procedure
 * @step 1 Call function wm_part_by_name_find without style.
 * @step 2 Check returned object.
 * </td>
 * <td>NULL, (Eina_Stringshare *) "elm.text"</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_by_name_find_test_n3)
{
   Eina_Stringshare *part_name = NULL;
   Part *part = NULL;

   elm_init(0, 0);
   part_name = eina_stringshare_add("elm.text");

   part = wm_part_by_name_find(NULL, part_name);
   ck_assert_msg(!part, "Founded part, in non exists group");

   eina_stringshare_del(part_name);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_by_name_find
 * @{
 * <tr>
 * <td>wm_part_by_name_find</td>
 * <td>wm_part_by_name_find_test_n4</td>
 * <td>
 * @precondition
 * @step 1 Initialize Elementary.
 * @step 2 Create  Evas canvas.
 * @step 3 Create new widget list with using wm_widget_list_new.
 * @step 4 Fill widget list with data (Widgets, Classes, Styles, Parts).
 * @step 5 Find Style structure, that does not have part with name "elm.text"
 *
 * @procedure
 * @step 1 Call function wm_part_by_name_find withou part name.
 * @step 2 Check returned object.
 * </td>
 * <td>Style *style, (Eina_Stringshare *) "elm.text"</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_by_name_find_test_n4)
{
   const char *file = "./edj_build/wm_part_by_name_find.edj";
   Eina_Stringshare *group_name = NULL;
   Eina_Stringshare *part_name = NULL;
   Eina_Inlist *widget_list = NULL;
   Ecore_Evas *ee = NULL;
   Evas *canvas = NULL;
   Part *part = NULL;
   Style *style = NULL;

   elm_init(0, 0);
   group_name = eina_stringshare_add("elm/radio/base/test");
   part_name = eina_stringshare_add("elm.text");
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   widget_list = wm_widget_list_new(file);
   wm_widget_list_objects_load(widget_list, canvas, file);
   style = wm_style_object_find(widget_list, group_name);

   part = wm_part_by_name_find(style, part_name);
   ck_assert_msg(!part, "Founded part in group, that doesn't have part sith same name.");

   wm_widget_list_free(widget_list);
   eina_stringshare_del(group_name);
   eina_stringshare_del(part_name);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_by_name_find
 * @{
 * </TABLE>
 * @}
 * @}
 */
