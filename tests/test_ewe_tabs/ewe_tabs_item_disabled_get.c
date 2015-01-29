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
 * @addtogroup ewe_tabs_item_disabled_get
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_disabled_get
 * @{
 * <tr>
 * <td>ewe_tabs_item_disabled_get</td>
 * <td>ewe_tabs_item_disabled_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 item added to tabs
 *
 * @procedure
 * @step 1 call ewe_tabs_item_dsiabled_get(item);
 * @step 2 check that returned value is EINA_FALSE
 * @step 3 dsiabble item
 * @step 4 call ewe_tabs_item_dsiabled_get(item);
 * @step 5 check that returned value is EINA_TRUE
 * </td>
 * <td>item, EINA_TRUE;item, EINA_FALSE;item, EINA_FALSE;</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_disabled_get_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);

   ck_assert_msg(ewe_tabs_item_disabled_get(tabs, it) == EINA_FALSE, "Enabled item was marked as disabled");
   ewe_tabs_item_disabled_set(tabs, it, EINA_TRUE);
   ck_assert_msg(ewe_tabs_item_disabled_get(tabs, it) == EINA_TRUE, "Disabled item was marked as enabled");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_disabled_get
 * @{
 * <tr>
 * <td>ewe_tabs_item_disabled_get</td>
 * <td>ewe_tabs_item_disabled_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_item_disabled_get(tabs, NULL);
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_disabled_get_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_item_disabled_get(NULL, NULL) == EINA_FALSE, "NULL tabs item was disabled");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_disabled_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
