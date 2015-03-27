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
 * @addtogroup ewe_tabs_active_item_set
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_active_item_set
 * @{
 * <tr>
 * <td>ewe_tabs_active_item_set</td>
 * <td>ewe_tabs_active_item_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 item added to tabs
 *
 * @procedure
 * @step 1 call ewe_tabs_active_item_set
 * @step 2 check returned value
 * </td>
 * <td>Ewe_Tabs_Item *it</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_active_item_set_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);

   ck_assert_msg(ewe_tabs_active_item_set(tabs, it) == EINA_TRUE, "Cannot activate tabs item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_tabs_active_item_set
 * @{
 * <tr>
 * <td>ewe_tabs_active_item_set</td>
 * <td>ewe_tabs_active_item_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 two items added to tabs: it1, it2
 * @step 5 add activate/deactivate callbacks
 *
 * @procedure
 * @step 1 call ewe_tabs_active_item_set(tabs, it2)
 * @step 2 check that deactivated callback was called for it1
 * @step 3 check that activated callback was called for it2
 * @step 4 call ewe_tabs_active_item_set(tabs, it2) again
 * @step 5 check that deactivated callback was not called
 * @step 6 check that activated callback was not called
 * </td>
 * <td>Ewe_Tabs_Item *it</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
static void
_callback(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ewe_Tabs_Item **it = (Ewe_Tabs_Item **) data;
   *it = (Ewe_Tabs_Item *) event_info;
}

EFLETE_TEST(ewe_tabs_active_item_set_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *deactivated = NULL;
   Ewe_Tabs_Item *activated = NULL;
   Ewe_Tabs_Item *it1 = ewe_tabs_item_append(tabs, NULL, "item1", NULL);
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "item2", NULL);
   evas_object_smart_callback_add(tabs, "ewe,tabs,item,activated", _callback, &activated);
   evas_object_smart_callback_add(tabs, "ewe,tabs,item,deactivated", _callback, &deactivated);

   ewe_tabs_active_item_set(tabs, it2);
   ck_assert_msg(deactivated == it1, "Wrong item deactivated");
   ck_assert_msg(activated == it2, "Wrong item activated");

   activated = NULL;
   deactivated = NULL;
   ewe_tabs_active_item_set(tabs, it2);
   ck_assert_msg((deactivated == NULL) && (activated == NULL), "Active item reactivated");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_active_item_set
 * @{
 * <tr>
 * <td>ewe_tabs_active_item_set</td>
 * <td>ewe_tabs_active_item_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_active_item_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_active_item_set_test_n1)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_active_item_set(NULL, NULL) == EINA_FALSE, "NULL tabs item was activated");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_active_item_set
 * @{
 * <tr>
 * <td>ewe_tabs_active_item_set</td>
 * <td>ewe_tabs_active_item_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 item added to tabs
 * @step 5 another tabs object added
 *
 * @procedure
 * @step 1 call ewe_tabs_active_item_set(NULL, it)
 * @step 2 check that returned value is EINA_FALSE
 * @step 3 call ewe_tabs_active_item_set(tabs, NULL)
 * @step 4 check that returned value is EINA_FALSE
 * @step 5 call ewe_tabs_active_item_set(tabs_other, it)
 * @step 6 check that returned value is EINA_FALSE
 * </td>
 * <td>NULL, Ewe_Tabs_Item *item; Ewe_Tabs tabs, NULL; another tabs, item</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_active_item_set_test_n2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Evas_Object *tabs_other = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);

   ck_assert_msg(ewe_tabs_active_item_set(NULL, it) == EINA_FALSE, "Tabs item was activated without tabs object");
   ck_assert_msg(ewe_tabs_active_item_set(tabs, NULL) == EINA_FALSE, "NULL tabs item was activated");
   ck_assert_msg(ewe_tabs_active_item_set(tabs_other, it) == EINA_FALSE, "Tabs item was activated with another tabs object");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_tabs_active_item_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
