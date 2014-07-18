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

#include "test_live_view.h"

/**
 * @addtogroup live_view_test
 * @{
 * @addtogroup live_view_widget_style_set
 * @{
 * live_view
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_widget_style_set
 * @{
 * <tr>
 * <td>live_view_widget_style_set</td>
 * <td>live_view_widget_style_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 4 Create style object as style.
 * @step 5 Load style data from edje file.
 * @step 6 Set loaded style as current loaded into project.
 * @step 7 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_widget_style_set.
 * @step 2 Check returned value.
 * @step 3 Check style name, which was set to object into live view.
 * @step 4 Check object type, which created in live_view. Here type will be Elm_Box,
 *         because 3 radio widgets are packed into elementary box.
 * </td>
 * <td>(Live_View *)live, (Project *)project, (Style *)style</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_set_test_p1)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Style *style = NULL;
   const char *style_set = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_open_project_edj("UTC", "./edj_build/live_view_widget_style_set.edj");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_widget_style_set.edj");
   project->current_style = style;
   live = live_view_add(parent);

   res = live_view_widget_style_set(live, project, style);
   ck_assert_msg(res == EINA_TRUE, "Failed set style into live view.");
   style_set = elm_object_style_get(live->object);
   ck_assert_msg(strcmp(style_set, "def") == 0, "Style setted for object in "
                 " live view not equal with style name loaded in project");
   style_set = elm_object_widget_type_get(live->object);
   ck_assert_msg(strcmp(style_set, "Elm_Box") == 0, "Object created into "
                 " live view not equal Elm_Box");

   live_view_free(live);
   wm_style_free(style);
   pm_project_close(project);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_set
 * @{
 * <tr>
 * <td>live_view_widget_style_set</td>
 * <td>live_view_widget_style_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 4 Create style object as layout.
 * @step 5 Load style data from edje file.
 * @step 6 Set loaded style as current loaded into project.
 * @step 7 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_widget_style_set.
 * @step 2 Check returned value.
 * @step 3 Check style name, which was set to object into live view.
 * @step 4 Check object type, which created in live_view.
 *         Here type will be Elm_Layout.
 * </td>
 * <td>(Live_View *)live, (Project *)project, (Style *)style</td>
 * <td>All checks are passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_set_test_p2)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Style *layout = NULL;
   Eina_Bool res = EINA_FALSE;
   const char *style_set = NULL;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_open_project_edj("UTC", "./edj_build/live_view_widget_style_set.edj");
   e = evas_object_evas_get(parent);
   layout = wm_style_add("load/layout/test", "load/layout/test", LAYOUT, NULL);
   wm_style_data_load(layout, e, "./edj_build/live_view_widget_style_set.edj");
   project->current_style = layout;
   live = live_view_add(parent);

   res = live_view_widget_style_set(live, project, layout);
   ck_assert_msg(res == EINA_TRUE, "Failed set style into live view.");
   style_set = elm_object_style_get(live->object);
   ck_assert_msg(strcmp(style_set, "load/layout/test") == 0, "Style setted for "
                 " object in live view not equal with style name loaded in project");
   style_set = elm_object_widget_type_get(live->object);
   ck_assert_msg(strcmp(style_set, "Elm_Layout") == 0, "Object created into "
                 " live view not equal Elm_Layout");

   live_view_free(live);
   wm_style_free(layout);
   pm_project_close(project);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_set
 * @{
 * <tr>
 * <td>live_view_widget_style_set</td>
 * <td>live_view_widget_style_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 4 Create style object as style.
 * @step 5 Load style data from edje file.
 * @step 6 Set loaded style as current loaded into project.
 *
 *
 * @procedure
 * @step 1 Call live_view_widget_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Project *)project, (Style *)style</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_set_test_n1)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Project *project = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_open_project_edj("UTC", "./edj_build/live_view_widget_style_set.edj");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_widget_style_set.edj");
   project->current_style = style;

   res = live_view_widget_style_set(NULL, project, style);
   ck_assert_msg(res == EINA_FALSE, "Set style into NULL pointer of live view.");

   wm_style_free(style);
   pm_project_close(project);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_set
 * @{
 * <tr>
 * <td>live_view_widget_style_set</td>
 * <td>live_view_widget_style_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 4 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_widget_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, (Project *)project, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_set_test_n2)
{
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_open_project_edj("UTC", "./edj_build/live_view_widget_style_set.edj");
   live = live_view_add(parent);

   res = live_view_widget_style_set(live, project, NULL);
   ck_assert_msg(res == EINA_FALSE, "Set NULL pointer style into live view.");

   live_view_free(live);
   pm_project_close(project);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_set
 * @{
 * <tr>
 * <td>live_view_widget_style_set</td>
 * <td>live_view_widget_style_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Load style data from edje file.
 * @step 5 Set loaded style as current loaded into project.
 * @step 6 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_widget_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, NULL, (Style *)style</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_set_test_n3)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_widget_style_set.edj");
   live = live_view_add(parent);

   res = live_view_widget_style_set(live, NULL, style);
   ck_assert_msg(res == EINA_FALSE, "Set style into live view, without open project");

   live_view_free(live);
   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
