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
 * @addtogroup live_view_theme_update
 * @{
 * live_view
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_theme_update
 * @{
 * <tr>
 * <td>live_view_theme_update</td>
 * <td>live_view_theme_update_test_p1</td>
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
 * @step 8 Set style for live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_theme_update.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, (Project *)project</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_theme_update_test_p1)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   setup("live_view_theme_update_test_p1");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_theme_update_test_p1/live_view_theme_update_test_p1.pro");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, project->mmap_file);
   project->current_style = style;
   live = live_view_add(parent, false);
   live_view_widget_style_set(live, project, style);

   res = live_view_theme_update(live, project);
   ck_assert_msg(res == EINA_TRUE, "Failed update style for widget in live view.");

   live_view_free(live);
   wm_style_free(style);
   evas_object_del(parent);
   teardown("./live_view_theme_update_test_p1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_theme_update
 * @{
 * <tr>
 * <td>live_view_theme_update</td>
 * <td>live_view_theme_update_test_p2</td>
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
 * @step 8 Set style for live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_theme_update.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, (Project *)project</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_theme_update_test_p2)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Style *layout = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   setup("live_view_theme_update_test_p2");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_theme_update_test_p2/live_view_theme_update_test_p2.pro");
   e = evas_object_evas_get(parent);
   layout = wm_style_add("load/layout/test", "load/layout/test", LAYOUT, NULL);
   wm_style_data_load(layout, e, project->mmap_file);
   project->current_style = layout;
   live = live_view_add(parent, false);
   live_view_widget_style_set(live, project, layout);

   res = live_view_theme_update(live, project);
   ck_assert_msg(res == EINA_TRUE, "Failed update style for layout in live view.");

   live_view_free(live);
   wm_style_free(layout);
   evas_object_del(parent);
   teardown("./live_view_theme_update_test_p2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_theme_update
 * @{
 * <tr>
 * <td>live_view_theme_update</td>
 * <td>live_view_theme_update_test_n1</td>
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
 * @step 1 Call live_view_theme_update.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Project *)project</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_theme_update_test_n1)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Project *project = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   setup("live_view_theme_update_test_n1");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_theme_update_test_n1/live_view_theme_update_test_n1.pro");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, project->mmap_file);
   project->current_style = style;

   res = live_view_theme_update(NULL, project);
   ck_assert_msg(res == EINA_FALSE, "Update style in NULL pointer live view.");

   wm_style_free(style);
   evas_object_del(parent);
   teardown("./live_view_theme_update_test_n1");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_theme_update
 * @{
 * <tr>
 * <td>live_view_theme_update</td>
 * <td>live_view_theme_update_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_theme_update.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_theme_update_test_n2)
{
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   live = live_view_add(parent, false);

   res = live_view_theme_update(live, NULL);
   ck_assert_msg(res == EINA_FALSE, "Update style, withoud load project.");

   live_view_free(live);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_theme_update
 * @{
 * <tr>
 * <td>live_view_theme_update</td>
 * <td>live_view_theme_update_test_n3</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Open project into Project structure. Use edj file,
 *         wich contain widgets and layout groups.
 * @step 4 Create style object as layout.
 * @step 5 Load style data from edje file.
 * @step 6 Create live view object.
 *
 *
 * @procedure
 * @step 1 Call live_view_theme_update.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live, (Project *)project</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_theme_update_test_n3)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Style *layout = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   setup("live_view_theme_update_test_n3");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_theme_update_test_n3/live_view_theme_update_test_n3.pro");
   e = evas_object_evas_get(parent);
   layout = wm_style_add("load/layout/test", "load/layout/test", LAYOUT, NULL);
   wm_style_data_load(layout, e, project->mmap_file);
   live = live_view_add(parent, false);

   res = live_view_theme_update(live, project);
   ck_assert_msg(res == EINA_FALSE, "Update style for layout in live view, "
                 "without load style as current in project");

   live_view_free(live);
   wm_style_free(layout);
   evas_object_del(parent);
   teardown("./live_view_theme_update_test_n3");
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup live_view_theme_update
 * @{
 * </TABLE>
 * @}
 * @}
 */
