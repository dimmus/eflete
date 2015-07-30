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
 * @addtogroup ewe_statusbar_item_remove
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_remove
 * @{
 * <tr>
 * <td>ewe_statusbar_item_remove</td>
 * <td>ewe_statusbar_item_remove_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Add new item to statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_remove.
 * @step 2 Check returned value;
 * @step 3 check count of items in statusbar.
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_remove_test_p1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   const Eina_List *items_list = NULL;
   unsigned int items_count = 0;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_remove(statusbar, item);
   ck_assert_msg(ret == EINA_TRUE, "Failed to delete item from statusbar");
   items_list = ewe_statusbar_items_list_get(statusbar);
   items_count = eina_list_count(items_list);
   ck_assert_msg(items_count == 0, "Returned list contain not 0 items");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_remove
 * @{
 * <tr>
 * <td>ewe_statusbar_item_remove</td>
 * <td>ewe_statusbar_item_remove_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Add new item to statusbar widget.
 * @step 6 Add new item to statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_remove with item getted at step 5 precondition.
 * @step 2 Check returned value;
 * @step 3 check count of items in statusbar.
 * @step 4 Call ewe_statusbar_item_remove with item getted at step 6 precondition.
 * @step 5 Check returned value;
 * @step 6 Call ewe_statusbar_item_remove with item getted at step 6 precondition.
 * @step 7 Check returned value;
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_remove_test_p2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Ewe_Statusbar_Item *item_2 = NULL;
   const Eina_List *items_list = NULL;
   unsigned int items_count = 0;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_2 = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_remove(statusbar, item);
   ck_assert_msg(ret == EINA_TRUE, "Failed to delete item from statusbar");
   items_list = ewe_statusbar_items_list_get(statusbar);
   items_count = eina_list_count(items_list);
   ck_assert_msg(items_count == 1, "Returned list contain not one item");
   ret = ewe_statusbar_item_remove(statusbar, item_2);
   ck_assert_msg(ret == EINA_TRUE, "Failed to delete second item from statusbar");
   ret = ewe_statusbar_item_remove(statusbar, item_2);
   ck_assert_msg(ret == EINA_FALSE, "Delete already deleted item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_remove
 * @{
 * <tr>
 * <td>ewe_statusbar_item_remove</td>
 * <td>ewe_statusbar_item_remove_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_remove with NULL pointer of item.
 * @step 2 Check returned value;
 * </td>
 * <td>(Evas_Object *)statusbar, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_remove_test_n1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);

   ret = ewe_statusbar_item_remove(statusbar, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Delete NULL item from statusbar");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_remove
 * @{
 * <tr>
 * <td>ewe_statusbar_item_remove</td>
 * <td>ewe_statusbar_item_remove_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Add new item to statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_remove with NULL pointer to statusbar widget.
 * @step 2 Check returned value;
 * </td>
 * <td>NULL, (Ewe_Statusbar_Item *)item </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_remove_test_n2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_remove(NULL, item);
   ck_assert_msg(ret == EINA_FALSE, "Delete item from NULL statusbar");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_remove
 * @{
 * </TABLE>
 * @}
 * @}
 */
