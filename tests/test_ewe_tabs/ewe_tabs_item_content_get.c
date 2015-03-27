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
 * @addtogroup ewe_tabs_item_content_get
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_content_get
 * @{
 * <tr>
 * <td>ewe_tabs_item_content_get</td>
 * <td>ewe_tabs_item_content_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 item added to tabs
 * @step 5 elm_label added
 * @step 6 set label as content to item
 *
 * @procedure
 * @step 1 call ewe_tabs_item_content_get
 * @step 2 check that returned pointer is pointer to setted label
 * </td>
 * <td>(Evas_Object *) tabs, (Ewe_Tabs_Item *)it</td>
 * <td>Getted content equal to setted label</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_content_get_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);
   Evas_Object *lbl = elm_label_add(win);
   ewe_tabs_item_content_set(tabs, it, lbl);

   ck_assert_msg(ewe_tabs_item_content_get(tabs, it) == lbl, "Wrong content returned");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_content_get
 * @{
 * <tr>
 * <td>ewe_tabs_item_content_get</td>
 * <td>ewe_tabs_item_content_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_item_content_get
 * @step 2 check returned pointer
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_content_get_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_item_content_get(NULL, NULL) == NULL, "Content getted from NULL object");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_content_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
