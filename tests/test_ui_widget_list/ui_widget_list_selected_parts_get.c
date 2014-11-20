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

#include "test_ui_widget_list.h"
#include "test_common.h"

#define _BEFORE_DOUBLE_CLICK \
   eoi = elm_naviframe_top_item_get(widget_list); \
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content"); \
   glit = elm_genlist_first_item_get(glist); \
   glit = elm_genlist_item_next_get(glit);

/**
 * @addtogroup ui_widget_list_test
 * @{
 * @addtogroup ui_widget_list_selected_parts_get
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_selected_parts_get
 * @{
 * <tr>
 * <td>ui_widget_list_selected_parts_get</td>
 * <td>ui_widget_list_selected_parts_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 5 create widget list.
 * @step 6 set data into widget list.
 * @step 7 double click on widget in widget list
 * @step 8 double click on class/style in widget list
 * @step 9 Switch genlist of widget list into multiselect mode.
 * @step 10 Select second part, third and last (fifth) part.
 *
 * @procedure
 * @step 1 Call ui_widget_list_selected_parts_get
 * @step 2 Check returned value
 * @step 3 Check count of returned selected parts.
 * </td>
 * <td>(Evas_Object *) widget_list</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_selected_parts_get_test_p)
{
   elm_init(0, 0);
   setup("ui_widget_list_selected_parts_get_test_p");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list, *glist;
   Project *project = NULL;
   Eina_List *selected_parts = NULL;
   Elm_Object_Item *glit, *eoi;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_selected_parts_get_test_p/ui_widget_list_selected_parts_get_test_p.pro");
   widget_list = ui_widget_list_add(parent);
   ui_widget_list_data_set(widget_list, project);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* Double-click on style*/
   _BEFORE_DOUBLE_CLICK
   glit = elm_genlist_item_next_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* select second part, so it can be move above or below */
   _BEFORE_DOUBLE_CLICK
   elm_genlist_multi_select_set(glist, EINA_TRUE);
   elm_genlist_item_selected_set(glit, EINA_TRUE);
   glit = elm_genlist_item_next_get(glit);
   elm_genlist_item_selected_set(glit, EINA_TRUE);
   glit = elm_genlist_last_item_get(glist);
   elm_genlist_item_selected_set(glit, EINA_TRUE);
   /*************************************************************/

   selected_parts = ui_widget_list_selected_parts_get(widget_list);
   ck_assert_msg(selected_parts != NULL, "Can't return any selected parts");
   ck_assert_msg(eina_list_count(selected_parts) == 3, "Wrong number of selected parts were selected");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./ui_widget_list_selected_parts_get_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_selected_parts_get
 * @{
 * <tr>
 * <td>ui_widget_list_selected_parts_get</td>
 * <td>ui_widget_list_selected_parts_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 5 create widget list.
 * @step 6 set data into widget list.
 * @step 7 double click on widget in widget list
 * @step 8 double click on class/style in widget list
 * @step 9 Switch genlist of widget list into multiselect mode.
 *
 * @procedure
 * @step 1 Call ui_widget_list_selected_parts_get
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_selected_parts_get_test_n1)
{
   elm_init(0, 0);
   setup("ui_widget_list_selected_parts_get_test_n1");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list, *glist;
   Project *project = NULL;
   Eina_List *selected_parts = NULL;
   Elm_Object_Item *glit, *eoi;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_selected_parts_get_test_n1/ui_widget_list_selected_parts_get_test_n1.pro");
   widget_list = ui_widget_list_add(parent);
   ui_widget_list_data_set(widget_list, project);
   /********Choosing widget, so widget list contain parts********/
   /* Double-click on widget*/
   _BEFORE_DOUBLE_CLICK
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /* Double-click on style*/
   _BEFORE_DOUBLE_CLICK
   glit = elm_genlist_item_next_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /*************************************************************/

   selected_parts = ui_widget_list_selected_parts_get(widget_list);
   ck_assert_msg(selected_parts == NULL, "List of parts was returned!");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./ui_widget_list_selected_parts_get_test_n1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_selected_parts_get
 * @{
 * <tr>
 * <td>ui_widget_list_selected_parts_get</td>
 * <td>ui_widget_list_selected_parts_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call ui_widget_list_selected_parts_get
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_selected_parts_get_test_n2)
{
   elm_init(0, 0);
   Eina_List *selected_parts = NULL;

   selected_parts = ui_widget_list_selected_parts_get(NULL);
   ck_assert_msg(selected_parts == NULL, "List of parts was returned!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_selected_parts_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

#undef _BEFORE_DOUBLE_CLICK
