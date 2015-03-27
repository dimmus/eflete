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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; If not, see www.gnu.org/licenses/lgpl.html.
*/
#include "test_ewe_statusbar.h"

/**
 * @addtogroup ewe_statusbar_test
 * @{
 * @addtogroup ewe_statusbar_items_list_get
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_items_list_get
 * @{
 * <tr>
 * <td>ewe_statusbar_items_list_get</td>
 * <td>ewe_statusbar_items_list_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 * @step 5 Add new item into statusbar. Item content is progresbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_items_list_get.
 * @step 2 Check returned value.
 * @step 3 Check count of items in received list.
 * @step 4 Check item from list with appended item at step 5 in precondition.
 * </td>
 * <td>(Evas_Object *)statusbar</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_items_list_get_test_p1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   const Eina_List *items_list = NULL;
   Ewe_Statusbar_Item *item = NULL, *item_received = NULL;
   int count = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "NULL list of items returned");
   count = eina_list_count(items_list);
   ck_assert_msg(count == 1, "Number of list nodes not equal 1");
   item_received = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(item == item_received, "Getted item from list not equal "
                                        "to added item");
   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_items_list_get
 * @{
 * <tr>
 * <td>ewe_statusbar_items_list_get</td>
 * <td>ewe_statusbar_items_list_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 *
 * @procedure
 * @step 1 Call ewe_statusbar_items_list_get.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)statusbar</td>
 * <td>NULL pointer retuned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_items_list_get_test_p2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);

   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list == NULL, "Not NULL list returned");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_items_list_get
 * @{
 * <tr>
 * <td>ewe_statusbar_items_list_get</td>
 * <td>ewe_statusbar_items_list_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_items_list_get.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer retuned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_items_list_get_test_n)
{
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);

   items_list = ewe_statusbar_items_list_get(NULL);
   ck_assert_msg(items_list == NULL, "Not NULL list returned");

   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_items_list_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
