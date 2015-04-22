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

/**
 * @addtogroup ui_widget_list_test
 * @{
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * <tr>
 * <td>ui_widget_list_class_data_reload</td>
 * <td>ui_widget_list_class_data_reload_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 5 create widget list.
 * @step 6 set data into widget list.
 * @step 7 get GenList object from widget list (that is actually just a Naviframe).
 * @step 8 get the first Item from GenList (its widget's list so,
 *         first item is just a label "Widgets").
 * @step 9 get next Item (that is widgeti, this item can be clicked,
 *         so widget list will contain it's classes and styles).
 * @step 10 do a double click on that Item so it will change gelist container to
 *          list of classes and styles.
 * @step 11 create two Styles "test33" and "test55".
 * @step 12 create Class "qqqqq".
 * @step 13 append new Class to list of project widget's class.
 * @step 14 get GenList object from widget list (that is actually just a Naviframe).
 *
 * @procedure
 * @step 1 Call ui_widget_list_class_data_reload
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) genlist, (Eina_List *) classes</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_class_data_reload_test_p1)
{
   elm_init(0, 0);
   setup("ui_widget_list_class_data_reload_test_p1");

   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;
   Eina_List *groups = NULL;
   Class *class = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_class_data_reload_test_p1/ui_widget_list_class_data_reload_test_p1.pro");
   widget_list = ui_widget_list_add(parent);
   result = ui_widget_list_data_set(widget_list, project);
   /********Choosing widget, so widget list contain classes and styles********/
   Elm_Object_Item *eoi = elm_naviframe_top_item_get(widget_list);
   Evas_Object *glist = elm_object_item_part_content_get(eoi,
                                               "elm.swallow.content");
   Elm_Object_Item *glit = elm_genlist_first_item_get(glist);
   glit = elm_genlist_item_next_get(glit);
   Widget *_widget = elm_object_item_data_get(glit);
   evas_object_smart_callback_call(glist, "clicked,double", glit);
   /**************************************************************************/
   groups = eina_list_append(groups, "elm/radio/qqqqq/test33");
   groups = eina_list_append(groups, "elm/radio/qqqqq/test55");
   class = wm_class_add("qqqqq", groups, _widget);
   _widget->classes = eina_inlist_append(_widget->classes,
                                         EINA_INLIST_GET(class));
   eoi = elm_naviframe_top_item_get(widget_list);
   glist = elm_object_item_part_content_get(eoi, "elm.swallow.content");

   result = ui_widget_list_class_data_reload(glist, _widget->classes);
   ck_assert_msg(result, "Failed to reload data into widget list");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   teardown("./ui_widget_list_class_data_reload_test_p1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * <tr>
 * <td>ui_widget_list_class_data_reload</td>
 * <td>ui_widget_list_class_data_reload_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 create widget list
 * @step 5 create two Styles "test33" and "test55".
 * @step 6 create Class "qqqqq".
 * @step 7 Create Widget filled with classes and styles.
 * @step 8 get GenList object from widget list (that is actually just a Naviframe).
 *
 * @procedure
 * @step 1 Call ui_widget_list_class_data_reload
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) genlist, (Eina_List *) classes</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_class_data_reload_test_p2)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list;
   Widget *_widget;
   Eina_Bool result = EINA_FALSE;
   Eina_List *groups = NULL;
   Class *class = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);
   groups = eina_list_append(groups, "elm/radio/qqqqq/test33");
   groups = eina_list_append(groups, "elm/radio/qqqqq/test55");
   _widget = (Widget *)mem_malloc(sizeof(Widget));
   _widget->name = eina_stringshare_add("radio");
   _widget->__type = WIDGET;
   class = wm_class_add("qqqqq", groups, _widget);
   _widget->classes = eina_inlist_append(_widget->classes,
                                         EINA_INLIST_GET(class));
   Elm_Object_Item *eoi = elm_naviframe_top_item_get(widget_list);
   Evas_Object *glist = elm_object_item_part_content_get(eoi, "elm.swallow.content");

   result = ui_widget_list_class_data_reload(glist, _widget->classes);
   ck_assert_msg(result, "Failed to reload data into widget list");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * <tr>
 * <td>ui_widget_list_class_data_reload</td>
 * <td>ui_widget_list_class_data_reload_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 create widget list
 * @step 5 get GenList object from widget list (that is actually just a Naviframe).
 *
 * @procedure
 * @step 1 Call ui_widget_list_class_data_reload
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) genlist, NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_class_data_reload_test_n1)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent, *widget_list;
   Eina_Bool result = EINA_FALSE;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);
   Elm_Object_Item *eoi = elm_naviframe_top_item_get(widget_list);
   Evas_Object *glist = elm_object_item_part_content_get(eoi, "elm.swallow.content");

   result = ui_widget_list_class_data_reload(glist, NULL);
   ck_assert_msg(!result, "Data in widget list was reloaded");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * <tr>
 * <td>ui_widget_list_class_data_reload</td>
 * <td>ui_widget_list_class_data_reload_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 add theme extension "eflete theme".
 * @step 3 create parent window
 * @step 4 create list with names two Styles "test33" and "test55".
 * @step 5 create widget filled with styles from step 4.
 *
 * @procedure
 * @step 1 Call ui_widget_list_class_data_reload
 * @step 2 Check returned value
 * </td>
 * <td>NULL, (Eina_List *) classes</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_class_data_reload_test_n2)
{
   elm_init(0, 0);
   elm_theme_extension_add(NULL, EFLETE_THEME);
   Evas_Object *parent;
   Widget *_widget;
   Eina_Bool result = EINA_FALSE;
   Eina_List *groups = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   groups = eina_list_append(groups, "elm/radio/qqqqq/test33");
   groups = eina_list_append(groups, "elm/radio/qqqqq/test55");
   _widget = wm_widget_add("radio", &groups);

   result = ui_widget_list_class_data_reload(NULL, _widget->classes);
   ck_assert_msg(result == EINA_FALSE, "Data in widget list was reloaded");

   evas_object_del(parent);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * <tr>
 * <td>ui_widget_list_class_data_reload</td>
 * <td>ui_widget_list_class_data_reload_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 Call ui_widget_list_class_data_reload
 * @step 2 Check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_class_data_reload_test_n3)
{
   elm_init(0, 0);

   ck_assert_msg(!ui_widget_list_class_data_reload(NULL, NULL),
                 "Data in widget list was reloaded");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_class_data_reload
 * @{
 * </TABLE>
 * @}
 * @}
 */
