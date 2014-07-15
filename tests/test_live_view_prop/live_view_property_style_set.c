/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "test_live_view_prop.h"

/**
 * @addtogroup live_view_prop_test
 * @{
 * @addtogroup live_view_property_style_set
 * @{
 * live_view_prop
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_property_style_set
 * @{
 * <tr>
 * <td>live_view_property_style_set</td>
 * <td>live_view_property_style_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Load style data from edje file.
 * @step 5 Create live view property object.
 *
 * @procedure
 * @step 1 Call live_view_property_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)live_view_prop, (Evas_Object *)object, (Style *)style, (const char *) widget = "win"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_set_test_p)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL, *live_view_prop = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_property_style_set.edj");
   live_view_prop = live_view_property_add(parent);

   res = live_view_property_style_set(live_view_prop, parent, style, "win");
   ck_assert_msg(res == EINA_TRUE, "Failed to set style into live view prop.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_set
 * @{
 * <tr>
 * <td>live_view_property_style_set</td>
 * <td>live_view_property_style_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Load style data from edje file.
 * @step 5 Create live view property object.
 *
 * @procedure
 * @step 1 Call live_view_property_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)live_view_prop, (Evas_Object *)object, (Style *)style, (const char *) widget = "win"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_set_test_n1)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL, *live_view_prop = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_property_style_set.edj");

   res = live_view_property_style_set(parent, parent, style, "win");
   ck_assert_msg(res == EINA_FALSE, "Given, not live view prop, object contain live view prop data in it.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_set
 * @{
 * <tr>
 * <td>live_view_property_style_set</td>
 * <td>live_view_property_style_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Create live view property object.
 *
 * @procedure
 * @step 1 Call live_view_property_style_set.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)live_view_prop, (Evas_Object *)object, (Style *)style, (const char *) widget = "win"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_set_test_n2)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL, *live_view_prop = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   live_view_prop = live_view_property_add(parent);

   res = live_view_property_style_set(live_view_prop, parent, style, "win");
   ck_assert_msg(res == EINA_FALSE, "Style was set into live view prop.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_set
 * @{
 * <tr>
 * <td>live_view_property_style_set</td>
 * <td>live_view_property_style_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Create live view property object.
 *
 * @procedure
 * @step 1 Call live_view_property_style_set with first parameter as NULL.
 * @step 2 Check returned value.
 * @step 1 Call live_view_property_style_set with second parameter as NULL.
 * @step 2 Check returned value.
 * @step 1 Call live_view_property_style_set with third parameter as NULL.
 * @step 2 Check returned value.
 * @step 1 Call live_view_property_style_set with fourth parameter as NULL.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL, NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_set_test_n3)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL, *live_view_prop = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   live_view_prop = live_view_property_add(parent);

   res = live_view_property_style_set(NULL, parent, style, "win");
   ck_assert_msg(res == EINA_FALSE, "Style was set into live view prop with live_view_prop = NULL.");
   res = live_view_property_style_set(live_view_prop, NULL, style, "win");
   ck_assert_msg(res == EINA_FALSE, "Style was set into live view prop with object = NULL.");
   res = live_view_property_style_set(live_view_prop, parent, NULL, "win");
   ck_assert_msg(res == EINA_FALSE, "Style was set into live view prop with style = NULL.");
   res = live_view_property_style_set(live_view_prop, parent, style, NULL);
   ck_assert_msg(res == EINA_FALSE, "Style was set into live view prop with widget = NULL.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
