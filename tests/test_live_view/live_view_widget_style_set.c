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
#include "test_common.h"

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
 * @step 4 Check object type, which created in live_view. Here type will be
 *         elm_box, because 3 radio widgets are packed into elementary box.
 * </tdhutdown();
 *
 * <td>(Live_View *)live, (Project *)project, (Style *)style</td>
 * <td>All check passed</td>
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
   setup("live_view_widget_style_set_test_p1");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_widget_style_set_test_p1/live_view_widget_style_set_test_p1.pro");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, project->mmap_file);
   project->current_style = style;
   live = live_view_add(parent, false);

   res = live_view_widget_style_set(live, project, style);
   ck_assert_msg(res == EINA_TRUE, "Failed set style into live view.");

   style_set = elm_object_style_get(live->object);
   ck_assert_msg(strcmp(style_set, "default") == 0, "Style setted for object in "
                 " live view not equal with style name loaded in project");

   style_set = elm_object_widget_type_get(live->object);
   ck_assert_msg(strcmp(style_set, "elm_box") == 0, "Object created into "
                 " live view not equal elm_box");

   live_view_free(live);
   wm_style_free(style);
   evas_object_del(parent);
   teardown("./live_view_widget_style_set_test_p1");
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
 * </td>
 * <td>(Live_View *)live, (Project *)project, (Style *)style</td>
 * <td>All checks are passed</td>
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
   setup("live_view_widget_style_set_test_p2");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_widget_style_set_test_p2/live_view_widget_style_set_test_p2.pro");
   e = evas_object_evas_get(parent);
   layout = wm_style_add("load/layout/test", "load/layout/test", LAYOUT, NULL);
   wm_style_data_load(layout, e, project->mmap_file);
   project->current_style = layout;
   live = live_view_add(parent, false);

   res = live_view_widget_style_set(live, project, layout);
   ck_assert_msg(res == EINA_TRUE, "Failed set style into live view.");
   edje_object_file_get(live->object, NULL, &style_set);
   ck_assert_msg(strcmp(style_set, "load/layout/test") == 0, "Style setted for "
                 " object in live view not equal with style name loaded in project");

   live_view_free(live);
   wm_style_free(layout);
   evas_object_del(parent);

   teardown("./live_view_widget_style_set_test_p2");
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
   setup("live_view_widget_style_set_test_n1");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_widget_style_set_test_n1/live_view_widget_style_set_test_n1.pro");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, project->mmap_file);
   project->current_style = style;

   res = live_view_widget_style_set(NULL, project, style);
   ck_assert_msg(res == EINA_FALSE, "Set style into NULL pointer of live view.");

   wm_style_free(style);
   evas_object_del(parent);
   teardown("./live_view_widget_style_set_test_n1");
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
   setup("live_view_widget_style_set_test_n2");
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_widget_style_set_test_n2/live_view_widget_style_set_test_n2.pro");
   live = live_view_add(parent, false);

   res = live_view_widget_style_set(live, project, NULL);
   ck_assert_msg(res == EINA_FALSE, "Set NULL pointer style into live view.");

   live_view_free(live);
   evas_object_del(parent);
   teardown("live_view_widget_style_set_test_n2");
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
 * @step 4 Mmap edj file.
 * @step 5 Load style data from edje file.
 * @step 6 Set loaded style as current loaded into project.
 * @step 7 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_widget_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, NULL, (Style *)style</td>
 * <td>EINA_FALSE returned</td>
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
   Eina_File *mmap_file = NULL;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mmap_file = eina_file_open("./edj_build/"TEST_NAME".edj", EINA_FALSE);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, mmap_file);
   live = live_view_add(parent, false);

   res = live_view_widget_style_set(live, NULL, style);
   ck_assert_msg(res == EINA_FALSE, "Set style into live view, without open project");

   live_view_free(live);
   wm_style_free(style);
   eina_file_close(mmap_file);
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
