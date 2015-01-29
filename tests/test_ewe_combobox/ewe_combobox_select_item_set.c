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
 * @addtogroup ewe_combobox_select_item_set
 * @{
 * ewe_combobox
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_combobox_select_item_set
 * @{
 * <tr>
 * <td>ewe_combobox_select_item_set</td>
 * <td>ewe_combobox_select_item_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 combobox widget created
 * @step 4 items added to combobox widget: "first", "second"
 *
 * @procedure
 * @step 1 call ewe_combobox_select_item_set
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that select item setted correctly by index
 * </td>
 * <td>Evas_Object *combobox, 1</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_select_item_set_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   ewe_combobox_item_add(combobox, "first");
   Ewe_Combobox_Item *it2 = ewe_combobox_item_add(combobox, "second");

   ck_assert_msg(ewe_combobox_select_item_set(combobox, 1) == EINA_TRUE, "Cannot set combobox item select");
   ck_assert_msg((ewe_combobox_select_item_get(combobox) == it2), "Incorrectly setted item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_select_item_set
 * @{
 * <tr>
 * <td>ewe_combobox_select_item_set</td>
 * <td>ewe_combobox_select_item_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 combobox widget created
 * @step 4 item added to combobox widget
 *
 * @procedure
 * @step 1 call ewe_combobox_select_item_set
 * @step 2 check that returned value
 * </td>
 * <td>Evas_Object *combobox, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_select_item_set_test_n1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   ewe_combobox_item_add(combobox, NULL);

   ck_assert_msg(ewe_combobox_select_item_set(combobox, -1) == EINA_FALSE, "NULL index was set to select item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_select_item_set
 * @{
 * <tr>
 * <td>ewe_combobox_select_item_set</td>
 * <td>ewe_combobox_select_item_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_combobox_select_item_set
 * @step 2 check that returned value
 * </td>
 * <td>NULL, 1</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_select_item_set_test_n2)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_combobox_select_item_set(NULL, 1) == EINA_FALSE, "Select was setted to NULL");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_select_item_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
