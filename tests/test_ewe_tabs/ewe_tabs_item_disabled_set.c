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
 * @addtogroup ewe_tabs_item_disabled_set
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_disabled_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_disabled_set</td>
 * <td>ewe_tabs_item_disabled_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 Two items added to tabs
 *
 * @procedure
 * @step 1 call ewe_tabs_item_disabled_set(tabs, item, EINA_TRUE);
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that item->disabled is EINA_TRUE
 * @step 4 call ewe_tabs_item_disabled_set(tabs, item, EINA_FALSE);
 * @step 5 check that returned value is EINA_TRUE
 * @step 6 check that item->disabled is EINA_FALSE
 * @step 7 call again ewe_tabs_item_disabled_set(tabs, item, EINA_FALSE);
 * @step 8 check that returned value is EINA_TRUE
 * @step 9 check that item->disabled is EINA_FALSE
 * @step 10 repeat steps 1-9 for inactive(second) item
 * </td>
 * <td>item, EINA_TRUE;item, EINA_FALSE;item, EINA_FALSE;</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_disabled_set_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "item2", NULL);

   ck_assert_msg(ewe_tabs_item_disabled_set(tabs, it, EINA_TRUE) == EINA_TRUE, "Cannot disable active tabs item");
   ck_assert_msg(it->disabled == EINA_TRUE, "Active item was not disabled");
   ck_assert_msg(ewe_tabs_item_disabled_set(tabs, it, EINA_FALSE) == EINA_TRUE, "Cannot enable active tabs item");
   ck_assert_msg(it->disabled == EINA_FALSE, "Active item was not enabled");
   ck_assert_msg(ewe_tabs_item_disabled_set(tabs, it, EINA_FALSE) == EINA_TRUE, "Cannot reenable active tabs item");
   ck_assert_msg(it->disabled == EINA_FALSE, "Active item was not reenabled");

   ck_assert_msg(ewe_tabs_item_disabled_set(tabs, it2, EINA_TRUE) == EINA_TRUE, "Cannot disable passive tabs item");
   ck_assert_msg(it2->disabled == EINA_TRUE, "Passive item was not disabled");
   ck_assert_msg(ewe_tabs_item_disabled_set(tabs, it2, EINA_FALSE) == EINA_TRUE, "Cannot enable passive tabs item");
   ck_assert_msg(it2->disabled == EINA_FALSE, "Passive item was not enabled");
   ck_assert_msg(ewe_tabs_item_disabled_set(tabs, it2, EINA_FALSE) == EINA_TRUE, "Cannot reenable passive tabs item");
   ck_assert_msg(it2->disabled == EINA_FALSE, "Passive item was not reenabled");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_disabled_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_disabled_set</td>
 * <td>ewe_tabs_item_disabled_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_item_disabled_set(tabs, NULL, EINA_TRUE);
 * @step 2 call ewe_tabs_item_disabled_set(tabs, NULL, EINA_FALSE);
 * </td>
 * <td>NULL, EINA_TRUE;NULL, EINA_FALSE;</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_disabled_set_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_item_disabled_set(NULL, NULL, EINA_TRUE) == EINA_FALSE, "NULL tabs item was disabled");
   ck_assert_msg(ewe_tabs_item_disabled_set(NULL, NULL, EINA_FALSE) == EINA_FALSE, "NULL tabs item was enabled");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_disabled_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
