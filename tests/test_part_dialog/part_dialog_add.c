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

#include "test_part_dialog.h"

#define _BEFORE_DOUBLE_CLICK \
   eoi = elm_naviframe_top_item_get(widget_list); \
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content"); \
   glit = elm_genlist_first_item_get(glist); \
   glit = elm_genlist_item_next_get(glit);

/**
 * @addtogroup part_dialog_test
 * @{
 * @addtogroup part_dialog_add
 * @{
 * part_dialog
 * <TABLE>
 * @}
 */

/**
 * @addtogroup part_dialog_add
 * @{
 * <tr>
 * <td>part_dialog_add</td>
 * <td>part_dialog_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extension theme from EFLETE_THEME file
 * @step 3 initialize required libraries
 * @step 4 create main window
 * @step 5 open edj file
 * @step 6 create widget list
 * @step 7 set data into widget list.
 * @step 8 set widget list into his block on main layout
 * @step 9 double click on widget in widget list
 * @step 10 Get currently selected Style structure
 * @step 11 double click on class/style in widget list
 * @step 12 set current style into workspace as edited object.
 *
 * @procedure
 * @step 1 Call part_dialog_add
 * @step 2 Check returned pointer
 * </td>
 * <td>App_Data *app</td>
 * <td>Not NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (part_dialog_add_test_p)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app = NULL;
   Evas_Object *widget_list, *dialog, *glist;
   Elm_Object_Item *glit, *eoi;
   const char *edj_path = "./edj_build/part_dialog_add.edj";

   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj(edj_path);
   widget_list = ui_widget_list_add(app->win);
   ui_widget_list_data_set(widget_list, app->project);
   ui_block_widget_list_set(app, widget_list);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* Double-click on style*/
   _BEFORE_DOUBLE_CLICK
   Style *style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/
   workspace_edit_object_set(app->workspace, style, edj_path);

   dialog = part_dialog_add(app);
   ck_assert_msg(dialog != NULL, "Creating part dialog failed.");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * <tr>
 * <td>part_dialog_add</td>
 * <td>part_dialog_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extension theme from EFLETE_THEME file
 * @step 3 initialize required libraries
 * @step 4 create main window
 * @step 5 open edj file
 * @step 6 create widget list
 * @step 7 set data into widget list.
 * @step 8 double click on widget in widget list
 * @step 9 Get currently selected Style structure
 * @step 10 double click on class/style in widget list
 * @step 11 set current style into workspace as edited object.
 *
 * @procedure
 * @step 1 Call part_dialog_add
 * @step 2 Check returned pointer
 * </td>
 * <td>App_Data *app</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (part_dialog_add_test_n1)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app = NULL;
   Evas_Object *widget_list, *dialog, *glist;
   Elm_Object_Item *glit, *eoi;
   const char *edj_path = "./edj_build/part_dialog_add.edj";

   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj(edj_path);
   widget_list = ui_widget_list_add(app->win);
   ui_widget_list_data_set(widget_list, app->project);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* Double-click on style*/
   _BEFORE_DOUBLE_CLICK
   Style *style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/
   workspace_edit_object_set(app->workspace, style, edj_path);

   dialog = part_dialog_add(app);
   ck_assert_msg(dialog == NULL, "Part dialog was created.");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * <tr>
 * <td>part_dialog_add</td>
 * <td>part_dialog_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extension theme from EFLETE_THEME file
 * @step 3 initialize required libraries
 * @step 4 create window (not project's one)
 * @step 5 open edj file
 * @step 6 create widget list
 * @step 7 set data into widget list.
 * @step 8 set widget list into his block on main layout
 * @step 9 double click on widget in widget list
 * @step 10 Get currently selected Style structure
 * @step 11 double click on class/style in widget list
 * @step 12 set current style into workspace as edited object.
 *
 * @procedure
 * @step 1 Call part_dialog_add
 * @step 2 Check returned pointer
 * </td>
 * <td>App_Data *app</td>
 * <td>Not NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (part_dialog_add_test_n2)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app = NULL;
   Evas_Object *widget_list, *dialog, *glist;
   Elm_Object_Item *glit, *eoi;
   const char *edj_path = "./edj_build/part_dialog_add.edj";

   app_init();
   app = app_data_get();
   app->win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   app->project = pm_open_project_edj(edj_path);
   widget_list = ui_widget_list_add(app->win);
   ui_widget_list_data_set(widget_list, app->project);
   ui_block_widget_list_set(app, widget_list);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* Double-click on style*/
   _BEFORE_DOUBLE_CLICK
   Style *style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/
   workspace_edit_object_set(app->workspace, style, edj_path);

   dialog = part_dialog_add(app);
   ck_assert_msg(dialog == NULL, "Part dialog was created.");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST
/**
 * @addtogroup part_dialog_add
 * @{
 * <tr>
 * <td>part_dialog_add</td>
 * <td>part_dialog_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 load extension theme from EFLETE_THEME file
 * @step 3 initialize required libraries
 * @step 4 create main window
 * @step 5 create widget list
 * @step 6 set widget list into his block on main layout
 * @step 7 double click on widget in widget list
 * @step 8 Get currently selected Style structure
 * @step 9 double click on class/style in widget list
 * @step 10 set current style into workspace as edited object.
 *
 * @procedure
 * @step 1 Call part_dialog_add
 * @step 2 Check returned pointer
 * </td>
 * <td>App_Data *app</td>
 * <td>Not NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (part_dialog_add_test_n3)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   App_Data *app = NULL;
   Evas_Object *widget_list, *dialog, *glist;
   Elm_Object_Item *glit, *eoi;
   const char *edj_path = "./edj_build/part_dialog_add.edj";

   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   widget_list = ui_widget_list_add(app->win);
   ui_block_widget_list_set(app, widget_list);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* Double-click on style*/
   _BEFORE_DOUBLE_CLICK
   Style *style = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/
   workspace_edit_object_set(app->workspace, style, edj_path);

   dialog = part_dialog_add(app);
   ck_assert_msg(dialog == NULL, "Part dialog was created.");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST
/**
 * @addtogroup part_dialog_add
 * @{
 * <tr>
 * <td>part_dialog_add</td>
 * <td>part_dialog_add_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call part_dialog_add
 * @step 2 Check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (part_dialog_add_test_n4)
{
   elm_init(0, 0);

   ck_assert_msg(part_dialog_add(NULL) == NULL, "Part dialog was created.");

   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_dialog_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

#undef _BEFORE_DOUBLE_CLICK
