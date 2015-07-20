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
 * @addtogroup live_view_widget_style_unset
 * @{
 * live_view
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_widget_style_unset
 * @{
 * <tr>
 * <td>live_view_widget_style_unset</td>
 * <td>live_view_widget_style_unset_test_p</td>
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
 * @step 8 Load style into live view.
 *
 * @procedure
 * @step 1 Call live_view_widget_style_unset.
 * @step 2 Check returned value.
 * @step 3 Check object in live view on NULL pointer.
 * </td>
 * <td>(Live_View *)live</td>
 * <td>All check passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_unset_test_p)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Project *project = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   setup("live_view_widget_style_unset_test_p");

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   project = pm_project_open("./live_view_widget_style_unset_test_p/live_view_widget_style_unset_test_p.pro");
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, project->mmap_file);
   project->current_style = style;
   live = live_view_add(parent, false);
   live_view_widget_style_set(live, project, style);

   res = live_view_widget_style_unset(live);
   ck_assert_msg(res == EINA_TRUE, "Failed to unset style from live view.");
   ck_assert_msg(live->object == NULL, "Style wasn't unsetted from live view");

   live_view_free(live);
   wm_style_free(style);
   evas_object_del(parent);
   teardown("./live_view_widget_style_unset_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_unset
 * @{
 * <tr>
 * <td>live_view_widget_style_unset</td>
 * <td>live_view_widget_style_unset_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create live view object.
 *
 * @procedure
 * @step 1 Call live_view_widget_style_unset.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_widget_style_unset_test_n1)
{
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Eina_Bool res = EINA_TRUE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   live = live_view_add(parent, false);

   res = live_view_widget_style_unset(live);
   ck_assert_msg(res == EINA_FALSE, "Unset style from live view without object.");

   live_view_free(live);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_widget_style_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
