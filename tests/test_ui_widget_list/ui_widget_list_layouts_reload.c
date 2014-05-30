/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "test_ui_widget_list.h"

/**
 * @addtogroup ui_widget_list_test
 * @{
 * @addtogroup ui_widget_list_layouts_reload
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_layouts_reload
 * @{
 * <tr>
 * <td>ui_widget_list_layouts_reload</td>
 * <td>ui_widget_list_layouts_reload_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 5 create widget list.
 * @step 6 set data into widget list.
 * @step 7 create new Style called "newstyle".
 * @step 8 append new Style to project layouts.
 * @step 9 get GenList object from widget list (that is actually just a Naviframe).
 *
 * @procedure
 * @step 1 Call ui_widget_list_layouts_reload
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) genlist, (Project *) project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_layouts_reload_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list, *glist;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;
   Style *layout = NULL;
   Elm_Object_Item *eoi;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_open_project_edj("UTC", "./edj_build/ui_widget_list_layout_reload.edj");
   widget_list = ui_widget_list_add(parent);
   result = ui_widget_list_data_set(widget_list, project);
   layout = wm_style_add("newstyle", "newstyle", LAYOUT, NULL);
   project->layouts = eina_inlist_append(project->layouts,
                                         EINA_INLIST_GET(layout));
   eoi = elm_naviframe_top_item_get(widget_list);
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content");

   result = ui_widget_list_layouts_reload(glist, project);
   ck_assert_msg(result, "Failed to reload layouts data into widget list");

   pm_project_close(project);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_layouts_reload
 * @{
 * <tr>
 * <td>ui_widget_list_layouts_reload</td>
 * <td>ui_widget_list_layouts_reload_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 *
 * @procedure
 * @step 1 Call ui_widget_list_layouts_reload
 * @step 2 Check returned value
 * </td>
 * <td>NULL, (Project *) project</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_layouts_reload_test_n1)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_open_project_edj("UTC", "./edj_build/ui_widget_list_layout_reload.edj");

   result = ui_widget_list_layouts_reload(NULL, project);
   ck_assert_msg(!result, "Layouts data was reloaded in widget list");

   pm_project_close(project);
   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_layouts_reload
 * @{
 * <tr>
 * <td>ui_widget_list_layouts_reload</td>
 * <td>ui_widget_list_layouts_reload_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 create widget list.
 * @step 5 get GenList object from widget list (that is actually just a Naviframe).
 *
 * @procedure
 * @step 1 Call ui_widget_list_layouts_reload
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) genlist, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_layouts_reload_test_n2)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list, *glist;
   Eina_Bool result = EINA_FALSE;
   Elm_Object_Item *eoi;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);
   eoi = elm_naviframe_top_item_get(widget_list);
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content");

   result = ui_widget_list_layouts_reload(glist, NULL);
   ck_assert_msg(!result, "Layouts data was reloaded in widget list");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_layouts_reload
 * @{
 * <tr>
 * <td>ui_widget_list_layouts_reload</td>
 * <td>ui_widget_list_layouts_reload_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call ui_widget_list_layouts_reload
 * @step 2 Check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_layouts_reload_test_n3)
{
   elm_init(0, 0);

   ck_assert_msg(!ui_widget_list_layouts_reload(NULL, NULL),
                 "Layouts data was reloaded in widget list");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_layouts_reload
 * @{
 * </TABLE>
 * @}
 * @}
 */
