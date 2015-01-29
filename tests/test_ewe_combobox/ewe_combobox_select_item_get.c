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
 * @addtogroup ewe_combobox_select_item_get
 * @{
 * ewe_combobox
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_combobox_select_item_get
 * @{
 * <tr>
 * <td>ewe_combobox_select_item_get</td>
 * <td>ewe_combobox_select_item_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox created
 * @step 4 item added to combobox: "selected"
 * @step 5 call ewe_combobox_select_item_set for first item
 *
 * @procedure
 * @step 1 call ewe_combobox_select_item_get on combobox
 * @step 2 check that "selected" item returned
 * </td>
 * <td>Evas_Object *combobox</td>
 * <td>Check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_select_item_get_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   Ewe_Combobox_Item *it1 = ewe_combobox_item_add(combobox, "selected");
   ewe_combobox_select_item_set(combobox, 0);

   ck_assert_msg((ewe_combobox_select_item_get(combobox) == it1), "Wrong selected item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_select_item_get
 * @{
 * <tr>
 * <td>ewe_combobox_select_item_get</td>
 * <td>ewe_combobox_select_item_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox created
 * @step 4 items added to combobox: "first", "second", "third"
 * @step 5 call ewe_combobox_select_item_set on third item
 *
 * @procedure
 * @step 1 call ewe_combobox_select_item_get on combobox
 * @step 2 check that "third" item returned
 * @step 3 check that returned item title is "third"
 * </td>
 * <td>Evas_Object *combobox</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_select_item_get_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   ewe_combobox_item_add(combobox, "first");
   ewe_combobox_item_add(combobox, "second");
   Ewe_Combobox_Item *it3 = ewe_combobox_item_add(combobox, "third");

   ewe_combobox_select_item_set(combobox, 2);

   ck_assert_msg((ewe_combobox_select_item_get(combobox) == it3), "Wrong selected item");
   ck_assert_msg(!strcmp(ewe_combobox_select_item_get(combobox)->title, "third"), "Wrong item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_select_item_get
 * @{
 * <tr>
 * <td>ewe_combobox_select_item_get</td>
 * <td>ewe_combobox_select_item_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_combobox_select_item_get
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_select_item_get_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_combobox_select_item_get(NULL) == NULL, "Returned title for NULL item");

   ewe_shutdown();
}
END_TEST

/**
  * @addtogroup ewe_combobox_select_item_get
  * @{
  * </TABLE>
  * @}
  * @}
  */
