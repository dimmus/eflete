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
 * @addtogroup ewe_tabs_item_title_get
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_title_get
 * @{
 * <tr>
 * <td>ewe_tabs_item_title_get</td>
 * <td>ewe_tabs_item_title_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs created
 * @step 4 items added to tabs: "first", "second"
 *
 * @procedure
 * @step 1 call ewe_tabs_item_title_get on item1
 * @step 2 check that returned title "first"
 * @step 3 call ewe_tabs_item_title_get on item2
 * @step 4 check that returned title "second"
 * </td>
 * <td>Evas_Object *tabs</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_title_get_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it1 = ewe_tabs_item_append(tabs, NULL, "first", NULL);
   Ewe_Tabs_Item *it2 = ewe_tabs_item_append(tabs, NULL, "second", NULL);

   ck_assert_msg(!strcmp(ewe_tabs_item_title_get(tabs, it1), "first"), "Wrong title for first item");
   ck_assert_msg(!strcmp(ewe_tabs_item_title_get(tabs, it2), "second"), "Wrong title for first item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_title_get
 * @{
 * <tr>
 * <td>ewe_tabs_item_title_get</td>
 * <td>ewe_tabs_item_title_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_item_title_get
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_title_get_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_item_title_get(NULL, NULL) == NULL, "Returned title for NULL item");

   ewe_shutdown();
}
END_TEST

/**
  * @addtogroup ewe_tabs_item_title_get
  * @{
  * </TABLE>
  * @}
  * @}
  */
