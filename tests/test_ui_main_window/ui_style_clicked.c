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

#include "test_ui_main_window.h"
#include "test_common.h"

/**
 * @addtogroup ui_main_window_test
 * @{
 * @addtogroup ui_style_clicked
 * @{
 * ui_main_window
 * <TABLE>
 * @}
 */

#define _BEFORE_DOUBLE_CLICK \
   eoi = elm_naviframe_top_item_get(widget_list); \
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content"); \
   glit = elm_genlist_first_item_get(glist); \
   glit = elm_genlist_item_next_get(glit);

/**
 * @addtogroup ui_style_clicked
 * @{
 * <tr>
 * <td>ui_style_clicked</td>
 * <td>ui_style_clicked_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 * @step 6 open new project and create widget list
 * @step 7 add callbacks to widget list
 * @step 8 double click on widget in widget list
 * @step 9 double click on style in widget list
 * @step 10 get Style structure of the clicked style
 *
 * @procedure
 * @step 1 Call function ui_style_clicked(app_data, style).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data, Style *style</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_style_clicked_test_p)
{
   elm_init(0, 0);
   setup("ui_style_clicked_test_p");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list, *glist;
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;
   Elm_Object_Item *glit, *eoi;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   app_data->project = pm_project_open("./ui_style_clicked_test_p/ui_style_clicked_test_p.pro");
   widget_list = ui_block_widget_list_get(app_data);

   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   _BEFORE_DOUBLE_CLICK
   Style *_style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/

   result = ui_style_clicked(app_data, _style);
   ck_assert_msg(result == EINA_TRUE, "Function failed");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./ui_style_clicked_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_style_clicked
 * @{
 * <tr>
 * <td>ui_style_clicked</td>
 * <td>ui_style_clicked_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create window (but not set into application data)
 * @step 6 open new project
 * @step 7 create widget list
 * @step 8 fill widget list with data
 * @step 9 double click on widget in widget list
 * @step 10 double click on style in widget list
 * @step 11 get Style structure of the clicked style
 *
 * @procedure
 * @step 1 Call function ui_style_clicked(app_data, style).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data, Style *style</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_style_clicked_test_n1)
{
   elm_init(0, 0);
   setup("ui_style_clicked_test_n1");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list, *glist, *parent;
   App_Data *app_data;
   Project *project;
   Eina_Bool result = EINA_FALSE;
   Elm_Object_Item *glit, *eoi;

   app_init();
   app_data = app_data_get();
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_style_clicked_test_n1/ui_style_clicked_test_n1.pro");
   widget_list = ui_widget_list_add(parent);
   ui_widget_list_data_set(widget_list, project);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   _BEFORE_DOUBLE_CLICK
   Style *_style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/

   result = ui_style_clicked(app_data, _style);
   ck_assert_msg(result == EINA_FALSE, "Function succeed but shouldn't");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./ui_style_clicked_test_n1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_style_clicked
 * @{
 * <tr>
 * <td>ui_style_clicked</td>
 * <td>ui_style_clicked_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 * @step 6 open new project and create widget list
 * @step 7 add callbacks to widget list
 * @step 8 double click on widget in widget list
 * @step 9 double click on style in widget list
 * @step 10 get Style structure of the clicked style
 *
 * @procedure
 * @step 1 Call function ui_style_clicked(app_data, NULL).
 * @step 2 Check returned value.
 * @step 3 Call function ui_style_clicked(NULL, style).
 * @step 4 Check returned value.
 * @step 5 Call function ui_style_clicked(NULL, NULL).
 * @step 6 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_style_clicked_test_n2)
{
   elm_init(0, 0);
   setup("ui_style_clicked_test_n2");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list, *glist;
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;
   Elm_Object_Item *glit, *eoi;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   app_data->project = pm_project_open("./ui_style_clicked_test_n2/ui_style_clicked_test_n2.pro");
   widget_list = ui_block_widget_list_get(app_data);

   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   _BEFORE_DOUBLE_CLICK
   Style *_style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/

   result = ui_style_clicked(app_data, NULL);
   ck_assert_msg(result == EINA_FALSE, "Function succeed but shouldn't");
   result = ui_style_clicked(NULL, _style);
   ck_assert_msg(result == EINA_FALSE, "Function succeed but shouldn't");
   result = ui_style_clicked(NULL, NULL);
   ck_assert_msg(result == EINA_FALSE, "Function succeed but shouldn't");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./ui_style_clicked_test_n2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_style_clicked
 * @{
 * </TABLE>
 * @}
 * @}
 */

#undef _BEFORE_DOUBLE_CLICK
