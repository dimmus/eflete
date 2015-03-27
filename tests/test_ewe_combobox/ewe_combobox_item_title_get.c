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
 * @addtogroup ewe_combobox_item_title_get
 * @{
 * ewe_combobox
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_combobox_item_title_get
 * @{
 * <tr>
 * <td>ewe_combobox_item_title_get</td>
 * <td>ewe_combobox_item_title_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox created
 * @step 4 items added to combobox: "first", "second"
 *
 * @procedure
 * @step 1 call ewe_combobox_item_title_get on item1
 * @step 2 check that returned title "first"
 * @step 3 call ewe_combobox_item_title_get on item2
 * @step 4 check that returned title "second"
 * </td>
 * <td>Evas_Object *combobox</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_title_get_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   ewe_combobox_item_add(combobox, "first");
   ewe_combobox_item_add(combobox, "second");

   ck_assert_msg(!strcmp(ewe_combobox_item_title_get(combobox, 0), "first"), "Wrong title for first item");
   ck_assert_msg(!strcmp(ewe_combobox_item_title_get(combobox, 1), "second"), "Wrong title for first item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_combobox_item_title_get
 * @{
 * <tr>
 * <td>ewe_combobox_item_title_get</td>
 * <td>ewe_combobox_item_title_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox created
 * @step 4 item added to combobox "first"
 *
 * @procedure
 * @step 1 call ewe_combobox_item_title_get on item without index
 * @step 2 check returned pointer
 * </td>
 * <td>Evas_Object *combobox, NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_title_get_test_n1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);
   ewe_combobox_item_add(combobox, "first");

   ck_assert_msg(ewe_combobox_item_title_get(combobox, -1) == NULL, "Wrong title for first item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST
/**
 * @addtogroup ewe_combobox_item_title_get
 * @{
 * <tr>
 * <td>ewe_combobox_item_title_get</td>
 * <td>ewe_combobox_item_title_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_combobox_item_title_get
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_title_get_test_n2)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_combobox_item_title_get(NULL, 1) == NULL, "Returned title for NULL item");

   ewe_shutdown();
}
END_TEST

/**
  * @addtogroup ewe_combobox_item_title_get
  * @{
  * </TABLE>
  * @}
  * @}
  */
