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
 * @step 3 Widget list filled with data (Widgets, Classes, Styles etc).
 * @step 4 Group existing in edj and there is an alias group.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_objects_load(widget_list, e, "./edj_build/wm_widget_list_objects_load.edj").
 * @step 2 Check returned bool.
 * @step 3 Find alias group "elm/radio/base/aliased"
 * @step 4 Check if alias group has correct name
 * @step 5 Check if the group is marked as alias
 * @step 6 Check that main group of that aliased one has correct hame.
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
   const char *main_group_name = "elm/radio/base/def";
   const char *full_alias_name = "elm/radio/base/aliased";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Style *style = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widgets_list_new(file);
   ck_assert_msg(wm_widgets_list_objects_load(widget_list, e, file), "Cannot load.");
   style = wm_style_object_find(widget_list, full_alias_name);
   ck_assert_str_eq(style->full_group_name, full_alias_name);
   ck_assert_msg(style->isAlias, "This group was loaded as not alias, but it actually is alias.");
   ck_assert_str_eq(style->main_group->full_group_name, main_group_name);

   wm_widgets_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_objects_load
 * @{
 * <tr>
 * <td>wm_widget_list_objects_load</td>
 * <td>wm_widget_list_objects_load_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_objects_load(NULL, e, "./edj_build/wm_widget_list_objects_load.edj").
 * @step 2 Check returned bool.
 * </td>
 * <td>NULL, Evas *e, char *file = "./edj_build/wm_widget_list_objects_load.edj"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_list_objects_load_test_n1)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_widget_list_objects_load.edj";
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   ck_assert_msg(!wm_widgets_list_objects_load(NULL, e, file), "Widget List loaded.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_objects_load
 * @{
 * <tr>
 * <td>wm_widget_list_objects_load</td>
 * <td>wm_widget_list_objects_load_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_objects_load(widget_list, NULL, "./edj_build/wm_widget_list_objects_load.edj").
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *widget_list, NULL, char *file = "./edj_build/wm_widget_list_objects_load.edj"</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_list_objects_load_test_n2)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_widget_list_objects_load.edj";
   Eina_Inlist *widget_list = NULL;

   widget_list = wm_widgets_list_new(file);
   ck_assert_msg(!wm_widgets_list_objects_load(widget_list, NULL, file), "Widget List loaded.");

   wm_widgets_list_free(widget_list);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_widget_list_objects_load
 * @{
 * <tr>
 * <td>wm_widget_list_objects_load</td>
 * <td>wm_widget_list_objects_load_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created.
 *
 * @procedure
 * @step 1 Call function wm_widget_list_objects_load(widget_list, e, NULL).
 * @step 2 Check returned bool.
 * </td>
 * <td>Eina_Inlist *widget_list, Evas *e, NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_widget_list_objects_load_test_n3)
{
   elm_init(0,0);
   const char *file = "./edj_build/wm_widget_list_objects_load.edj";
   Eina_Inlist *widget_list = NULL;
   Evas_Object *win = NULL;
   Evas *e = NULL;

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   widget_list = wm_widgets_list_new(file);
   ck_assert_msg(!wm_widgets_list_objects_load(widget_list, e, NULL), "Widget List loaded.");

   wm_widgets_list_free(widget_list);
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
