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
 * @addtogroup ewe_tabs_item_icon_set
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_item_icon_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_icon_set</td>
 * <td>ewe_tabs_item_icon_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 item added to tabs
 * @step 5 elm_icon added
 *
 * @procedure
 * @step 1 call ewe_tabs_item_icon_set
 * @step 2 check returned value
 * </td>
 * <td>Ewe_Tabs_Item *it, Evas_Object *icon</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_icon_set_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);
   Evas_Object *lbl = elm_icon_add(win);

   ck_assert_msg(ewe_tabs_item_icon_set(tabs, it, lbl) == EINA_TRUE, "Cannot set icon to the tabs item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_icon_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_icon_set</td>
 * <td>ewe_tabs_item_icon_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 item added to tabs
 *
 * @procedure
 * @step 1 call ewe_tabs_item_icon_set
 * @step 2 check returned value
 * </td>
 * <td>Ewe_Tabs_Item *it, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_icon_set_test_n1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   Ewe_Tabs_Item *it = ewe_tabs_item_append(tabs, NULL, "item", NULL);

   ck_assert_msg(ewe_tabs_item_icon_set(tabs, it, NULL) == EINA_FALSE, "Setted NULL icon to the tabs item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_icon_set
 * @{
 * <tr>
 * <td>ewe_tabs_item_icon_set</td>
 * <td>ewe_tabs_item_icon_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 elm_icon added
 *
 * @procedure
 * @step 1 call ewe_tabs_item_icon_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, Evas_Object *icon</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_item_icon_set_test_n2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *lbl = elm_icon_add(win);

   ck_assert_msg(ewe_tabs_item_icon_set(NULL, NULL, lbl) == EINA_FALSE, "Setted icon to NULL tabs item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_item_icon_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
