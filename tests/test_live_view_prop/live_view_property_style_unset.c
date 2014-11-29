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

#include "test_live_view_prop.h"

/**
 * @addtogroup live_view_prop_test
 * @{
 * @addtogroup live_view_property_style_unset
 * @{
 * live_view_prop
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_property_style_unset
 * @{
 * <tr>
 * <td>live_view_property_style_unset</td>
 * <td>live_view_property_style_unset_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Load style data from edje file.
 * @step 5 Create live view property object.
 * @step 6 Load object into live view property
 *
 * @procedure
 * @step 1 Call live_view_property_style_unset.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)live_view_prop</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_unset_test_p)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL, *live_view_prop = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_property_style_unset.edj");
   live_view_prop = live_view_property_add(parent, false);
   live_view_property_style_set(live_view_prop, parent, style, "win");

   res = live_view_property_style_unset(live_view_prop);
   ck_assert_msg(res == EINA_TRUE, "Failed to unset style from live view.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_unset
 * @{
 * <tr>
 * <td>live_view_property_style_unset</td>
 * <td>live_view_property_style_unset_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create style object as style.
 * @step 4 Load style data from edje file.
 * @step 5 Create object that is not live view property.
 * @step 6 Load object into live view property
 *
 * @procedure
 * @step 1 Call live_view_property_style_unset.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)live_view_prop</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_unset_test_n1)
{
   Evas *e = NULL;
   Evas_Object *parent = NULL, *live_view_prop = NULL;
   Style *style = NULL;
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(parent);
   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   wm_style_data_load(style, e, "./edj_build/live_view_property_style_unset.edj");
   live_view_prop = elm_bg_add(parent);
   live_view_property_style_set(live_view_prop, parent, style, "win");

   res = live_view_property_style_unset(parent);
   ck_assert_msg(res == EINA_FALSE, "Style was unset but shouldn't.");

   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_unset
 * @{
 * <tr>
 * <td>live_view_property_style_unset</td>
 * <td>live_view_property_style_unset_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call live_view_property_style_unset.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_style_unset_test_n2)
{
   Eina_Bool res = EINA_FALSE;

   elm_init(0, 0);

   res = live_view_property_style_unset(NULL);
   ck_assert_msg(res == EINA_FALSE, "Style was unset but shouldn't.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_style_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
