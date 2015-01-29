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
 * @addtogroup ewe_combobox_item_add
 * @{
 * ewe_combobox
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_combobox_item_add
 * @{
 * <tr>
 * <td>ewe_combobox_item_add</td>
 * <td>ewe_combobox_item_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox created
 *
 * @procedure
 * @step 1 call ewe_combobox_item_add
 * @step 2 call ewe_combobox_item_add
 * @step 3 check returned pointers
 * </td>
 * <td>Evas_Object *combobox, "first item"; Evas_Object *combobox, "second item"</td>
 * <td>Not NULL pointers returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_add_test_p1)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);

   Ewe_Combobox_Item *it = ewe_combobox_item_add(combobox, "first item");
   Ewe_Combobox_Item *it2 = ewe_combobox_item_add(combobox, "second item");
   ck_assert_msg((it != NULL) && (it2 != NULL), "Combobox item not added");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_item_add
 * @{
 * <tr>
 * <td>ewe_combobox_item_add</td>
 * <td>ewe_combobox_item_add_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_combobox created
 *
 * @procedure
 * @step 1 call ewe_combobox_item_add
 * @step 2 call ewe_combobox_item_add
 * @step 3 check returned pointers
 * </td>
 * <td>Evas_Object *combobox, NULL; Evas_Object *combobox, NULL</td>
 * <td>Not NULL pointers returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_add_test_p2)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *combobox = ewe_combobox_add(win);

   Ewe_Combobox_Item *it = ewe_combobox_item_add(combobox, NULL);
   Ewe_Combobox_Item *it2 = ewe_combobox_item_add(combobox, NULL);
   ck_assert_msg((it != NULL) && (it2 != NULL), "Combobox item not added");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_combobox_item_add
 * @{
 * <tr>
 * <td>ewe_combobox_item_add</td>
 * <td>ewe_combobox_item_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_combobox_item_add
 * @step 2 check returned pointer
 * </td>
 * <td>NULL, "first item"</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_combobox_item_add_test_n)
{
   ewe_init(0, 0);

   Ewe_Combobox_Item *it = ewe_combobox_item_add(NULL, "first item");
   ck_assert_msg(it == NULL, "combobox item not added");

   ewe_shutdown();
}
END_TEST

/**
  * @addtogroup ewe_combobox_item_add
  * @{
  * </TABLE>
  * @}
  * @}
  */
