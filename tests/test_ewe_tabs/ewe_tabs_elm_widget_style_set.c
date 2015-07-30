/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#include "test_ewe_tabs.h"

/**
 * @addtogroup ewe_tabs_test
 * @{
 * @addtogroup ewe_tabs_elm_widget_style_set
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_elm_widget_style_set
 * @{
 * <tr>
 * <td>elm_object_style_set</td>
 * <td>ewe_tabs_elm_widget_style_set_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 *
 * @procedure
 * @step 1 call elm_widget_style_set(tabs, "style");
 * @step 2 call elm_widget_style_get(tabs);
 * @step 3 check that returned value is "style"
 * @step 4 call elm_widget_style_set(tabs, NULL);
 * @step 5 call elm_widget_style_get(tabs);
 * @step 6 check that returned value is "default"
 * </td>
 * <td> Evas_Object *tabs, const char *str; Evas_Object *tabs, NULL;</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_elm_widget_style_set_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   ck_assert_msg(elm_object_style_set(tabs, "style") == EINA_TRUE, "New style value should be set, but it isnt");
   Eina_Bool result = !strcmp(elm_object_style_get(tabs), "style");
   ck_assert_msg(result == EINA_TRUE, "Wrong value of style received");

   ck_assert_msg(elm_object_style_set(tabs, NULL) == EINA_TRUE, "Default style value should be set, but it isnt");

   result = !strcmp(elm_object_style_get(tabs), "default");
   ck_assert_msg(result == EINA_TRUE, "Default style value should be set, but it isnt");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_elm_widget_style_set
 * @{
 * <tr>
 * <td>elm_object_style_set</td>
 * <td>ewe_tabs_elm_widget_style_set_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call elm_widget_style_set(NULL, NULL);
 * @step 2 check that returned value is EINA_FALSE;
 * @step 3 call elm_widget_style_set(NULL, "style");
 * @step 4 check that returned value is EINA_FALSE;
 * </td>
 * <td>NULL, NULL; NULL, const char *str</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_elm_widget_style_set_n)
{
   ewe_init(0, 0);

   ck_assert_msg(elm_object_style_set(NULL, NULL) == EINA_FALSE, "Wrong returned data on passing NULL");
   ck_assert_msg(elm_object_style_set(NULL, NULL) == EINA_FALSE, "Wrong returned data on passing NULL");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_elm_widget_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
