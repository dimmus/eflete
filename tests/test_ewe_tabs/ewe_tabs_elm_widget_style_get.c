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
 * @addtogroup ewe_tabs_elm_widget_style_get
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_elm_widget_style_get
 * @{
 * <tr>
 * <td>elm_object_style_get</td>
 * <td>ewe_tabs_elm_widget_style_get_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 *
 * @procedure
 * @step 1 call elm_widget_style_get(tabs);
 * @step 2 check that returned value is "default"
 * </td>
 * <td> Evas_Object *tabs</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_elm_widget_style_get_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   Eina_Bool result = !strcmp(elm_object_style_get(tabs), "default");
   ck_assert_msg(result == EINA_TRUE, "Wrong default name of the style");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_elm_widget_style_get
 * @{
 * <tr>
 * <td>elm_object_style_get</td>
 * <td>ewe_tabs_elm_widget_style_get_p2</td>
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
 * </td>
 * <td> Evas_Object *tabs</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_elm_widget_style_get_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   elm_object_style_set(tabs, "style");

   Eina_Bool result = !strcmp(elm_object_style_get(tabs), "style");
   ck_assert_msg(result == EINA_TRUE, "Wrong style name received");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_elm_widget_style_get
 * @{
 * <tr>
 * <td>elm_object_style_get</td>
 * <td>ewe_tabs_elm_widget_style_get_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call elm_widget_style_get(NULL);
 * @step 2 check that returned value is NULL;
 * </td>
 * <td>NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_elm_widget_style_get_n)
{
   ewe_init(0, 0);

   ck_assert_msg(elm_object_style_get(NULL) == NULL, "NULL should be returned");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_elm_widget_style_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
