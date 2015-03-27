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
 * @addtogroup ewe_statusbar_item_append
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * <tr>
 * <td>ewe_statusbar_item_append</td>
 * <td>ewe_statusbar_item_append_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_append. With EWE_STATUSBAR_ITEM_TYPE_OBJECT.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get item from items list.
 * @step 6 Compare item getted from step 1 with item from step 5.
 * @step 7 Compare type of item getted from step 1 with type of item from step 5.
 * @step 8 Compare pointer of progressbar of item getted from step 1 with
  *        content of item from step 5.
 * @step 9 Call ewe_statusbar_item_append.
 * @step 10 Call ewe_statusbar_items_list_get
 * @step 11 Check list of items on NULL.
 * @step 12 Check count nodes in returned list.
 * @step 13 Get last item from items list and compare it with item
 *          returned on step 7.
 * </td>
 * <td>(Evas_Object *)statusbar, (Evas_Object *) progressbar,
       (Ewe_Statusbar_Items_Type) EWE_STATUSBAR_ITEM_TYPE_OBJECT,
       (Evas_Smart_Cb) NULL, (const void *) NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_append_test_p1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *sb_item_f = NULL;
   Ewe_Statusbar_Item *sb_item_s = NULL;
   const Eina_List *items_list = NULL;
   unsigned int items_count = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);

   sb_item_f = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ck_assert_msg(sb_item_f != NULL, "Failed to append new item to statusbar");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "After append new item to statusbar, NULL "
                                     "list of items returned");
   sb_item_s = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(sb_item_f == sb_item_s, "Setted and getted items are not equal");
   ck_assert_msg(sb_item_f->type == EWE_STATUSBAR_ITEM_TYPE_OBJECT,
                 "Types of setted and getted items are not equal");
   content = ewe_statusbar_item_content_get(sb_item_f);
   ck_assert_msg(content == progressbar, "Setted and getted item contents are not equal");
   sb_item_f = ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "After append new item to statusbar, NULL "
                                     "list of items returned");
   items_count = eina_list_count(items_list);
   ck_assert_msg(items_count == 2, "Returned list contain different count of "
                                   " setted items");
   sb_item_s = (Ewe_Statusbar_Item *)eina_list_data_get(eina_list_last(items_list));
   ck_assert_msg(sb_item_f == sb_item_s, "Last setted and last getted items are "
                                         " not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * <tr>
 * <td>ewe_statusbar_item_append</td>
 * <td>ewe_statusbar_item_append_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_append. With EWE_STATUSBAR_ITEM_TYPE_SEPARATOR.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get item from items list.
 * @step 6 Compare item getted from step 1 with item from step 5.
 * @step 7 Compare type of item getted from step 1 with type of item from step 5.
 * @step 8 Check content from created item, it should be NULL;
 * </td>
 * <td>(Evas_Object *)statusbar, (Evas_Object *) progressbar,
       (Ewe_Statusbar_Items_Type) EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
       (Evas_Smart_Cb) NULL, (const void *) NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_append_test_p2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *sb_item_f = NULL;
   Ewe_Statusbar_Item *sb_item_s = NULL;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);

   sb_item_f = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, NULL, NULL);
   ck_assert_msg(sb_item_f != NULL, "Failed to append new separator to statusbar");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "After append new separator to statusbar, "
                                     "NULL list of items returned");
   sb_item_s = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(sb_item_f == sb_item_s, "Setted and getted separators are not equal");
   ck_assert_msg(sb_item_f->type == EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                 "Getted item is not a separator");
   content = ewe_statusbar_item_content_get(sb_item_f);
   ck_assert_msg(content == NULL, "Separator item have a content object");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * <tr>
 * <td>ewe_statusbar_item_append</td>
 * <td>ewe_statusbar_item_append_test_p3</td>
 * <td>
 * @precondition
 * @step 1 Create callback function _test_callback. This method change value of
 *         global static variable 'return_code'. If 'return_code' is EINA_TRUE -
           all is ok, otherwise value mean that error happens.
 * @step 2 Initialize EWE library.
 * @step 3 Create parent window.
 * @step 4 Create statusbar widget.
 * @step 5 Create progressbar widget.
 * @step 6 Create string "test" for callback data check.
 * @step 7 Initialize 'return_code' with EINA_FALSE value.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_append. With EWE_STATUSBAR_ITEM_TYPE_OBJECT.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get item from items list.
 * @step 6 Compare item getted from step 1 with item from step 5.
 * @step 7 Compare type of item getted from step 1 with type of item from step 5.
 * @step 8 Check content from created item, it should be NULL.
 * @step 9 Call callback test_callback.
 * @step 10 Check 'return_code'.
 * @step 11 Check progressbar state value, it must be equal 0.5.
 * </td>
 * <td>(Evas_Object *) statusbar, (Evas_Object *) progressbar, (Ewe_Statusbar_Items_Type) EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, (Evas_Smart_Cb) test_callback, (const void *) cb_data</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
static Eina_Bool return_code;
static void
test_callback(void *data, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *cb_data = (Eina_Stringshare *)data;
   Ewe_Statusbar_Item *sb_item = (Ewe_Statusbar_Item *) event_info;
   Evas_Object *content = NULL;

   if ((!sb_item) || (!obj)) return;

   if (strcmp("test", cb_data)) return;

   content = ewe_statusbar_item_content_get(sb_item);
   if (!content) return;
   elm_progressbar_value_set(content, 0.5);

   return_code = EINA_TRUE;
}


EFLETE_TEST(ewe_statusbar_item_append_test_p3)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *content = NULL;
   Eina_Stringshare *cb_data = NULL;
   Ewe_Statusbar_Item *sb_item = NULL;
   Ewe_Statusbar_Item *sb_item_s = NULL;
   const Eina_List *items_list = NULL;
   double pb_state = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   cb_data = eina_stringshare_add("test");
   return_code = EINA_FALSE;

   sb_item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, test_callback,
                             (char *)cb_data);
   ck_assert_msg(sb_item != NULL, "Failed to append new item to statusbar");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "After append new item to statusbar, "
                                     "NULL list of items returned");
   sb_item_s = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(sb_item == sb_item_s, "Setted and getted items are not equal");
   ck_assert_msg(sb_item->type == EWE_STATUSBAR_ITEM_TYPE_OBJECT,
                 "Getted type of item is not a EWE_STATUSBAR_ITEM_TYPE_OBJECT");
   content = ewe_statusbar_item_content_get(sb_item);
   ck_assert_msg(content == progressbar, "Setted and getted item contents are "
                                         " not equal");
   sb_item->func(sb_item->cb_data, sb_item->parent, sb_item);
   ck_assert_msg(return_code == EINA_TRUE, "Something wrong with 'clicked' callback");
   pb_state = elm_progressbar_value_get(progressbar);
   ck_assert_msg(pb_state == 0.5, "Content parameters wasn't changed");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * <tr>
 * <td>ewe_statusbar_item_append</td>
 * <td>ewe_statusbar_item_append_test_p4</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_append. With EWE_STATUSBAR_ITEM_TYPE_OBJECT.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get item from items list.
 * @step 6 Compare item getted from step 1 with item from step 5.
 * @step 7 Compare type of item getted from step 1 with type of item from step 5.
 * @step 8 Check content of item from step 5.
 * </td>
 * <td>(Evas_Object *)statusbar, NULL,
       (Ewe_Statusbar_Items_Type) EWE_STATUSBAR_ITEM_TYPE_OBJECT,
       (Evas_Smart_Cb) NULL, (const void *) NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_append_test_p4)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *sb_item_f = NULL;
   Ewe_Statusbar_Item *sb_item_s = NULL;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);

   sb_item_f = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ck_assert_msg(sb_item_f != NULL, "Failed to append new item to statusbar");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "After append new item to statusbar, NULL "
                                     "list of items returned");
   sb_item_s = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(sb_item_f == sb_item_s, "Setted and getted items are not equal");
   ck_assert_msg(sb_item_f->type == EWE_STATUSBAR_ITEM_TYPE_OBJECT,
                 "Types of setted and getted items are not equal");
   content = ewe_statusbar_item_content_get(sb_item_f);
   ck_assert_msg(content == NULL, "Not NULL content returned");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * <tr>
 * <td>ewe_statusbar_item_append</td>
 * <td>ewe_statusbar_item_append_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_append. With EWE_STATUSBAR_ITEM_TYPE_OBJECT.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, NULL, (Ewe_Statusbar_Items_Type) EWE_STATUSBAR_ITEM_TYPE_OBJECT,
       (Evas_Smart_Cb) NULL, (const void *) NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_append_test_n1)
{
   Ewe_Statusbar_Item *sb_item = NULL;

   ewe_init(0, 0);

   sb_item = ewe_statusbar_item_append(NULL, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ck_assert_msg(sb_item == NULL, "Append new item without statusbar");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * <tr>
 * <td>ewe_statusbar_item_append</td>
 * <td>ewe_statusbar_item_append_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_append.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) statusbar, NULL, -11, (Evas_Smart_Cb) NULL,
 *     (const void *) NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_append_test_n2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Ewe_Statusbar_Item *sb_item = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);

   sb_item = ewe_statusbar_item_append(statusbar, NULL, -1, NULL, NULL);
   ck_assert_msg(sb_item == NULL, "Append new item with wrong type");

   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_append
 * @{
 * </TABLE>
 * @}
 * @}
 */
