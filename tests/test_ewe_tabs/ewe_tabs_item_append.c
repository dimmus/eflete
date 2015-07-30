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
 * @addtogroup ewe_tabs_item_append
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_append
 * @{
 * <tr>
 * <td>ewe_tabs_item_append</td>
 * <td>ewe_tabs_item_append_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs created
 *
 * @procedure
 * @step 1 append "first" item to tabs
 * @step 2 check that returned pointer is not NULL
 * @step 3 append "second" item to tabs
 * @step 4 check that returned pointer is not NULL
 * @step 5 append "new item" after "first" item
 * @step 6 check that returned pointer is not NULL
 * @step 7 check that there are 3 items in tabs
 * @step 8 check that items order is "first", "new item", "second"
 * </td>
 * <td>Evas_Object *tabs, Ewe_Tabs_Item *item1, "new item", NULL;</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_append_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   Ewe_Tabs_Item *it1 = ewe_tabs_item_append(tabs, NULL, "first item", NULL);
   ck_assert_msg(it1 != NULL, "Tabs item 1 not added");
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "second item", NULL);
   ck_assert_msg(it2 != NULL, "Tabs item 2 not added");
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, it1, "new item", NULL);
   ck_assert_msg(it != NULL, "Tabs item 3 not added");
   const Eina_List * items = ewe_tabs_items_list_get(tabs);
   ck_assert_msg(eina_list_count(items) == 3, "Wrong number of tabs added");
   ck_assert_msg((eina_list_data_get(items) == it1) &&
                 (eina_list_data_get(items->next) == it) &&
                 (eina_list_data_get(items->next->next) == it2),
                 "Wrong items order");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_append
 * @{
 * <tr>
 * <td>ewe_tabs_item_append</td>
 * <td>ewe_tabs_item_append_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs created
 *
 * @procedure
 * @step 1 append "first" item to tabs
 * @step 2 check that returned pointer is not NULL
 * @step 3 append "second" item to tabs
 * @step 4 check that returned pointer is not NULL
 * @step 5 append new item without title after "first" item
 * @step 6 check that returned pointer is not NULL
 * @step 7 check that there are 3 items in tabs
 * @step 8 check that items order is "first", new item, "second"
 * </td>
 * <td>Evas_Object *tabs, Ewe_Tabs_Item *item1, NULL, NULL;</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_append_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   Ewe_Tabs_Item *it1 = ewe_tabs_item_append(tabs, NULL, "first item", NULL);
   ck_assert_msg(it1 != NULL, "Tabs item 1 not added");
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "second item", NULL);
   ck_assert_msg(it2 != NULL, "Tabs item 2 not added");
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, it1, NULL, NULL);
   ck_assert_msg(it != NULL, "Tabs item 3 not added");
   const Eina_List * items = ewe_tabs_items_list_get(tabs);
   ck_assert_msg(eina_list_count(items) == 3, "Wrong number of tabs added");
   ck_assert_msg((eina_list_data_get(items) == it1) &&
                 (eina_list_data_get(items->next) == it) &&
                 (eina_list_data_get(items->next->next) == it2),
                 "Wrong items order");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_append
 * @{
 * <tr>
 * <td>ewe_tabs_item_append</td>
 * <td>ewe_tabs_item_append_test_p3</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs created
 *
 * @procedure
 * @step 1 append "first" item to tabs
 * @step 2 check that returned pointer is not NULL
 * @step 3 append "second" item to tabs
 * @step 4 check that returned pointer is not NULL
 * @step 5 append "new item" with style "red" after "first" item
 * @step 6 check that returned pointer is not NULL
 * @step 7 check that there are 3 items in tabs
 * @step 8 check that items order is "first", "new item", "second"
 * </td>
 * <td>Evas_Object *tabs, Ewe_Tabs_Item *item1, "new item", "red";</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_append_test_p3)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   Ewe_Tabs_Item *it1 = ewe_tabs_item_append(tabs, NULL, "first item", NULL);
   ck_assert_msg(it1 != NULL, "Tabs item 1 not added");
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "second item", NULL);
   ck_assert_msg(it2 != NULL, "Tabs item 2 not added");
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, it1, "new item", "red");
   ck_assert_msg(it != NULL, "Tabs item 3 not added");
   const Eina_List * items = ewe_tabs_items_list_get(tabs);
   ck_assert_msg(eina_list_count(items) == 3, "Wrong number of tabs added");
   ck_assert_msg((eina_list_data_get(items) == it1) &&
                 (eina_list_data_get(items->next) == it) &&
                 (eina_list_data_get(items->next->next) == it2),
                 "Wrong items order");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_append
 * @{
 * <tr>
 * <td>ewe_tabs_item_append</td>
 * <td>ewe_tabs_item_append_test_p4</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs created
 *
 * @procedure
 * @step 1 append "first" item to tabs
 * @step 2 check that returned pointer is not NULL
 * @step 3 append "second" item to tabs
 * @step 4 check that returned pointer is not NULL
 * @step 5 append "new item" after NULL item
 * @step 6 check that returned pointer is not NULL
 * @step 7 check that there are 3 items in tabs
 * @step 8 check that items order is "first", "second", "new item"
 * </td>
 * <td>Evas_Object *tabs, Ewe_Tabs_Item *item1, "new item", "red";</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_append_test_p4)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);

   Ewe_Tabs_Item *it1 = ewe_tabs_item_append(tabs, NULL, "first item", NULL);
   ck_assert_msg(it1 != NULL, "Tabs item 1 not added");
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "second item", NULL);
   ck_assert_msg(it2 != NULL, "Tabs item 2 not added");
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "new item", NULL);
   ck_assert_msg(it != NULL, "Tabs item 3 not added");
   const Eina_List * items = ewe_tabs_items_list_get(tabs);
   ck_assert_msg(eina_list_count(items) == 3, "Wrong number of tabs added");
   ck_assert_msg((eina_list_data_get(items) == it1) &&
                 (eina_list_data_get(items->next) == it2) &&
                 (eina_list_data_get(items->next->next) == it),
                 "Wrong items order");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_append
 * @{
 * <tr>
 * <td>ewe_tabs_item_append</td>
 * <td>ewe_tabs_item_append_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_item_append
 * @step 3 check returned pointer
 * </td>
 * <td>NULL, NULL , "first item", "red"</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_append_test_n)
{
   ewe_init(0, 0);

   Ewe_Tabs_Item *it = ewe_tabs_item_append(NULL, NULL, "first item", "red");
   ck_assert_msg(it == NULL, "Tabs item not added");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_append
 * @{
 * </TABLE>
 * @}
 * @}
 */
