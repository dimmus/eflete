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

#include "test_style_dialog.h"
#include "test_common.h"

/**
 * @addtogroup style_dialog_test
 * @{
 * @addtogroup style_dialog_add
 * @{
 * style_dialog
 * <TABLE>
 * @}
 */

#define _BEFORE_DOUBLE_CLICK \
   eoi = elm_naviframe_top_item_get(widget_list); \
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content"); \
   glit = elm_genlist_first_item_get(glist); \
   glit = elm_genlist_item_next_get(glit);

/**
 * @addtogroup style_dialog_add
 * @{
 * <tr>
 * <td>style_dialog_add</td>
 * <td>style_dialog_add_test_p1</td>
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
 * @step 9 select style in widget list
 *
 * @procedure
 * @step 1 Call function style_dialog_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (style_dialog_add_test_p1)
{
   elm_init(0, 0);
   setup("style_dialog_add_test_p1");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list, *glist;
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;
   Elm_Object_Item *glit, *eoi;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   app_data->project = pm_project_open("./style_dialog_add_test_p1/style_dialog_add_test_p1.pro");
   widget_list = ui_block_widget_list_get(app_data);

   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   _BEFORE_DOUBLE_CLICK
   /*************************************************************/

   result = style_dialog_add(app_data);
   ck_assert_msg(result == EINA_FALSE, "Style dialog wasn't created");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./style_dialog_add_test_p1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup style_dialog_add
 * @{
 * <tr>
 * <td>style_dialog_add</td>
 * <td>style_dialog_add_test_p2</td>
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
 *
 * @procedure
 * @step 1 Call function style_dialog_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (style_dialog_add_test_p2)
{
   elm_init(0, 0);
   setup("style_dialog_add_test_p2");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *widget_list, *glist;
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;
   Elm_Object_Item *glit, *eoi;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   app_data->project = pm_project_open("./style_dialog_add_test_p2/style_dialog_add_test_p2.pro");
   widget_list = ui_block_widget_list_get(app_data);

   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/

   result = style_dialog_add(app_data);
   ck_assert_msg(result == EINA_FALSE, "Style dialog wasn't created");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./style_dialog_add_test_p2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup style_dialog_add
 * @{
 * <tr>
 * <td>style_dialog_add</td>
 * <td>style_dialog_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 initialize application with app_init() function
 * @step 4 create application data
 * @step 5 create main window
 * @step 6 open new project and create widget list
 * @step 7 add callbacks to widget list
 *
 * @procedure
 * @step 1 Call function style_dialog_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>App_Data *app_data</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (style_dialog_add_test_n1)
{
   elm_init(0, 0);
   setup("style_dialog_add_test_n1");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app_data;
   Eina_Bool result = EINA_FALSE;

   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   app_data->project = pm_project_open("./style_dialog_add_test_n1/style_dialog_add_test_n1.pro");

   result = style_dialog_add(app_data);
   ck_assert_msg(result == EINA_FALSE, "Style dialog was added");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("style_dialog_add_test_n1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup style_dialog_add
 * @{
 * <tr>
 * <td>style_dialog_add</td>
 * <td>style_dialog_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call function style_dialog_add(app_data).
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (style_dialog_add_test_n2)
{
   elm_init(0, 0);
   Eina_Bool result;

   result = style_dialog_add(NULL);
   ck_assert_msg(result == EINA_FALSE, "Style dialog was added");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup style_dialog_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

#undef _BEFORE_DOUBLE_CLICK
