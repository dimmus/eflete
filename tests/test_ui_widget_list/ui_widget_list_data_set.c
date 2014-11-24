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
 * @addtogroup ui_widget_list_data_set
 * @{
 * ui_widget_list
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * <tr>
 * <td>ui_widget_list_data_set</td>
 * <td>ui_widget_list_data_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 4 create widget list.
 *
 * @procedure
 * @step 1 Call ui_widget_list_data_set
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list, (Project *) project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_data_set_test_p1)
{
   elm_init(0, 0);
   setup("ui_widget_list_data_set_test_p1");

   Evas_Object *parent, *widget_list;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_data_set_test_p1/ui_widget_list_data_set_test_p1.pro");
   widget_list = ui_widget_list_add(parent);

   result = ui_widget_list_data_set(widget_list, project);
   ck_assert_msg(result, "Failed load data into widget list");

   evas_object_del(parent);
   teardown("./ui_widget_list_data_set_test_p1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * <tr>
 * <td>ui_widget_list_data_set</td>
 * <td>ui_widget_list_data_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups. Unset layouts list.
 * @step 4 create widget list.
 *
 * @procedure
 * @step 1 Call ui_widget_list_data_set
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list, (Project *) project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_data_set_test_p2)
{
   elm_init(0, 0);
   setup("ui_widget_list_data_set_test_p2");

   Evas_Object *parent, *widget_list;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;
   Style *layout = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_data_set_test_p2/ui_widget_list_data_set_test_p2.pro");
   project->layouts = NULL;
   while (project->layouts)
     {
        layout = EINA_INLIST_CONTAINER_GET(project->layouts, Style);
        project->layouts = eina_inlist_remove(project->layouts, project->layouts);
        wm_style_free(layout);
     }
   project->widgets = NULL;
   widget_list = ui_widget_list_add(parent);

   result = ui_widget_list_data_set(widget_list, project);
   ck_assert_msg(result, "Failed load data into widget list");

   evas_object_del(parent);
   teardown("./ui_widget_list_data_set_test_p2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * <tr>
 * <td>ui_widget_list_data_set</td>
 * <td>ui_widget_list_data_set_test_p3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups. Unset widgets list.
 * @step 4 create widget list.
 *
 * @procedure
 * @step 1 Call ui_widget_list_data_set
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list, (Project *) project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_data_set_test_p3)
{
   elm_init(0, 0);
   setup("ui_widget_list_data_set_test_p3");

   Evas_Object *parent, *widget_list;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;
   Widget *widget = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_data_set_test_p3/ui_widget_list_data_set_test_p3.pro");
   while (project->widgets)
     {
        widget = EINA_INLIST_CONTAINER_GET(project->widgets, Widget);
        project->widgets = eina_inlist_remove(project->widgets, project->widgets);
        wm_widget_free(widget);
     }
   project->widgets = NULL;
   widget_list = ui_widget_list_add(parent);

   result = ui_widget_list_data_set(widget_list, project);
   ck_assert_msg(result, "Failed load data into widget list");

   evas_object_del(parent);
   teardown("./ui_widget_list_data_set_test_p3");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * <tr>
 * <td>ui_widget_list_data_set</td>
 * <td>ui_widget_list_data_set_test_p4</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups. Unset widgets and layouts lists.
 * @step 4 create widget list.
 *
 * @procedure
 * @step 1 Call ui_widget_list_data_set
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list, (Project *) project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_data_set_test_p4)
{
   elm_init(0, 0);
   setup("ui_widget_list_data_set_test_p4");

   Evas_Object *parent, *widget_list;
   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;
   Widget *widget = NULL;
   Style *layout = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./ui_widget_list_data_set_test_p4/ui_widget_list_data_set_test_p4.pro");
   project->widgets = NULL;
   project->layouts = NULL;
   while (project->layouts)
     {
        layout = EINA_INLIST_CONTAINER_GET(project->layouts, Style);
        project->layouts = eina_inlist_remove(project->layouts, project->layouts);
        wm_style_free(layout);
     }
   project->widgets = NULL;
   while (project->widgets)
     {
        widget = EINA_INLIST_CONTAINER_GET(project->widgets, Widget);
        project->widgets = eina_inlist_remove(project->widgets, project->widgets);
        wm_widget_free(widget);
     }
   project->widgets = NULL;
   widget_list = ui_widget_list_add(parent);

   result = ui_widget_list_data_set(widget_list, project);
   ck_assert_msg(result, "Failed load data into widget list");

   evas_object_del(parent);
   teardown("./ui_widget_list_data_set_test_p4");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * <tr>
 * <td>ui_widget_list_data_set</td>
 * <td>ui_widget_list_data_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create widget list.
 *
 * @procedure
 * @step 1 Call ui_widget_list_data_set
 * @step 2 Check returned value
 * </td>
 * <td>(Evas_Object *) widget_list, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_data_set_test_n1)
{
   elm_init(0, 0);
   Evas_Object *parent, *widget_list;
   Eina_Bool result = EINA_FALSE;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   widget_list = ui_widget_list_add(parent);

   result = ui_widget_list_data_set(widget_list, NULL);
   ck_assert_msg(!result, "Data loaded into widget list, with NULL project");
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * <tr>
 * <td>ui_widget_list_data_set</td>
 * <td>ui_widget_list_data_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 *
 * @procedure
 * @step 1 Call ui_widget_list_data_set
 * @step 2 Check returned value
 * </td>
 * <td>NULL, (Project *) project</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_widget_list_data_set_test_n2)
{
   elm_init(0, 0);
   setup("ui_widget_list_data_set_test_n2");

   Project *project = NULL;
   Eina_Bool result = EINA_FALSE;

   project = pm_project_open("./ui_widget_list_data_set_test_n2/ui_widget_list_data_set_test_n2.pro");
   result = ui_widget_list_data_set(NULL, project);

   ck_assert_msg(!result, "Data loaded with NULL widget list object");

   teardown("./ui_widget_list_data_set_test_n2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list_data_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
