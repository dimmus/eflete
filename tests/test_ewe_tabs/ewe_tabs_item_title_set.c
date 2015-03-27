/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#include "test_ewe_tabs.h"

/**
 * @addtogroup ewe_tabs_test
 * @{
 * @addtogroup ewe_tabs_item_title_set
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_title_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_title_set</td>
 * <td>ewe_tabs_item_title_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 tabs widget created
 * @step 4 item with title added to tabs widget
 *
 * @procedure
 * @step 1 call ewe_tabs_item_title_set
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that correct title is setted to the item
 * </td>
 * <td>Ewe_Tabs_Item *it, "NEW Title"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_title_set_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);

   ck_assert_msg(ewe_tabs_item_title_set(tabs, it, "NEW Title") == EINA_TRUE, "Cannot set tabs item title");
   ck_assert_msg(!strcmp(it->title, "NEW Title"), "Title not changed");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_title_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_title_set</td>
 * <td>ewe_tabs_item_title_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 tabs widget created
 * @step 4 item without title added to tabs widget
 *
 * @procedure
 * @step 1 call ewe_tabs_item_title_set
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that correct title is setted to the item
 * </td>
 * <td>Ewe_Tabs_Item *it, "NEW Title"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_title_set_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, NULL, NULL);

   ck_assert_msg(ewe_tabs_item_title_set(tabs, it, "NEW Title") == EINA_TRUE, "Cannot set tabs item title");
   ck_assert_msg(!strcmp(it->title, "NEW Title"), "Title not changed");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_title_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_title_set</td>
 * <td>ewe_tabs_item_title_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 tabs widget created
 * @step 4 item without title added to tabs widget
 *
 * @procedure
 * @step 1 call ewe_tabs_item_title_set
 * @step 2 check that returned value
 * </td>
 * <td>Ewe_Tabs_Item *it, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_title_set_test_n1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, NULL, NULL);

   ck_assert_msg(ewe_tabs_item_title_set(tabs, it, NULL) == EINA_FALSE, "NULL title was set to item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_title_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_title_set</td>
 * <td>ewe_tabs_item_title_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_item_title_set
 * @step 2 check that returned value
 * </td>
 * <td>NULL, "title"</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_title_set_test_n2)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_item_title_set(NULL, NULL, "title") == EINA_FALSE, "Title was set to NULL item");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_title_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
