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
#include "test_ewe_combobox.h"

/**
 * @addtogroup ewe_combobox_test
 * @{
 * @addtogroup ewe_combobox_item_del
 * @{
 * ewe_combobox
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_combobox_item_del
 * @{
 * <tr>
 * <td>ewe_combobox_item_del</td>
 * <td>ewe_combobox_item_del_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox added
 * @step 4 Two items with title added to combobox
 * @step 5 3rd item without title added to combobox
 * @step 6 call ewe_combobox_select_item_set for first item
 *
 * @procedure
 * @step 1 remove second item
 * @step 2 check returned value
 * @step 3 remove 3rd item
 * @step 4 check returned value
 * @step 5 remove first item
 * @step 6 check returned value
 * </td>
 * <td>Ewe_Combobox_Item *it</td>
 * <td>EINA_TRUE returned in each check</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_del_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   Ewe_Combobox_Item *it1 = ewe_combobox_item_add(combobox, "item1");
   Ewe_Combobox_Item *it2 = ewe_combobox_item_add(combobox, "item2");
   Ewe_Combobox_Item *it3 = ewe_combobox_item_add(combobox, NULL);
   ewe_combobox_select_item_set(combobox, 1);

   ck_assert_msg(ewe_combobox_item_del(combobox, it2) == EINA_TRUE, "Cannot remove second item");
   ck_assert_msg(ewe_combobox_item_del(combobox, it3) == EINA_TRUE, "Cannot remove 3rd item");
   ck_assert_msg(ewe_combobox_item_del(combobox, it1) == EINA_TRUE, "Cannot remove first item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_item_del
 * @{
 * <tr>
 * <td>ewe_combobox_item_del</td>
 * <td>ewe_combobox_item_del_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_combobox_item_del
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_del_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_combobox_item_del(NULL, NULL) == EINA_FALSE, "NULL item removed");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_item_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
