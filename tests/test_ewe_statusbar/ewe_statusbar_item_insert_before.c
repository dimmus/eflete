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
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 8 of
 *         precondition before item from step 6 of precondition.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get first item from items list.
 * @step 6 Compare item getted from step 8 of precondition with item from
  *        step 5 of procedure
 * @step 7 Compare pointer of label of item getted from step 8 of precondition
 *         with content of item from step 5 of procedure.
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_p1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *item_p = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Ewe_Statusbar_Item *control = NULL;
   Eina_Bool ret = EINA_FALSE;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   item_p = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_l, item_p);
   ck_assert_msg(ret == EINA_TRUE, "Failed to insert last item before first");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "List of items is NULL");
   control = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(item_l == control, "First item not equal to reordered item");
   content = ewe_statusbar_item_content_get(control);
   ck_assert_msg(content == label, "Setted and getted item contents are not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 6 of
 *         precondition before item from step 8 of precondition.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get second item from items list.
 * @step 6 Compare item getted from step 6 of precondition with item from
  *        step 5 of procedure
 * @step 7 Compare pointer of progressbar of item getted from step 6 of precondition
 *         with content of item from step 5 of procedure.
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_p2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *item_p = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Ewe_Statusbar_Item *control = NULL;
   Eina_Bool ret = EINA_FALSE;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   item_p = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar, label, EWE_STATUSBAR_ITEM_TYPE_OBJECT,
                             NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_p, item_l);
   ck_assert_msg(ret == EINA_TRUE, "Failed to insert first item before last");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "List of items is NULL");
   control = (Ewe_Statusbar_Item *)eina_list_nth(items_list, 1);
   ck_assert_msg(item_p == control, "Second item not equal to reordered item");
   content = ewe_statusbar_item_content_get(control);
   ck_assert_msg(content == progressbar, "Setted and getted item contents are not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_p3</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 8 of
 *         precondition before item from step 7 of precondition.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get second item from items list.
 * @step 6 Compare item getted from step 8 of precondition with item from
  *        step 5 of procedure
 * @step 7 Compare pointer of label item getted from step 8 of precondition
 *         with content of item from step 5 of procedure.
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_p3)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Ewe_Statusbar_Item *item_s = NULL;
   Ewe_Statusbar_Item *control = NULL;
   Eina_Bool ret = EINA_FALSE;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_s = ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_l, item_s);
   ck_assert_msg(ret == EINA_TRUE, "Failed to insert third item before second");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "List of items is NULL");
   control = (Ewe_Statusbar_Item *)eina_list_nth(items_list, 1);
   ck_assert_msg(item_l == control, "Second item not equal to reordered item");
   content = ewe_statusbar_item_content_get(control);
   ck_assert_msg(content == label, "Setted and getted item contents are not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_p4</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 7 of
 *         precondition before item from step 6 of precondition.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get first item from items list.
 * @step 6 Compare item getted from step 7 of precondition with item from
  *        step 5 of procedure.
 * @step 7 Check item type from step 5 of procedure.
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_p4)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_p = NULL;
   Ewe_Statusbar_Item *item_s = NULL;
   Ewe_Statusbar_Item *control = NULL;
   Eina_Bool ret = EINA_FALSE;
   const Eina_List *items_list = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   item_p = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_s = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, NULL, NULL);
   ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_s, item_p);
   ck_assert_msg(ret == EINA_TRUE, "Failed to insert separator item as first");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "List of items is NULL");
   control = (Ewe_Statusbar_Item *)eina_list_data_get(items_list);
   ck_assert_msg(item_s == control, "First item not equal to reordered item");
   ck_assert_msg(control->type == EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                 "Setted and getted items types are not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_n1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_s = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_s = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(NULL, item_s, item_l);
   ck_assert_msg(ret == EINA_FALSE, "Change order of items with statusbar NULL");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)statusbar, NULL, (Ewe_Statusbar_Item *)before</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_n2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, NULL, item_l);
   ck_assert_msg(ret == EINA_FALSE, "Change order of items with item NULL");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_n3</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_n3)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_l, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Change order of items with before item NULL");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_n4</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create one more statusbar widget.
 * @step 5 Create progressbar widget.
 * @step 6 Create label widget.
 * @step 7 Append new item with progressbar content. (Statusbar from step 3)
 * @step 8 Append new item with SEPARATOR type. (Statusbar from step 3)
 * @step 9 Append new item with label content. (Statusbar from step 4)
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 7 of
 *         precondition before item from step 9 of precondition. Using object
 *         received at step 3 of precondition as statusbar parameter.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get (Using object from step 3 of precondition )
 * @step 4 Check list of items on NULL.
 * @step 5 Get count of items in statusbar. (should be 2)
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_n4)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *statusbar_2 = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_p = NULL;
   Ewe_Statusbar_Item *item_l = NULL;
   Eina_Bool ret = EINA_TRUE;
   const Eina_List *items_list = NULL;
   int items_count = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   statusbar_2 = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   item_p = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   item_l = ewe_statusbar_item_append(statusbar_2, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_p, item_l);
   ck_assert_msg(ret == EINA_FALSE, "Inserted item from one statusbar to another");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "List of items frist statusbar is NULL");
   items_count = eina_list_count(items_list);
   ck_assert_msg(items_count == 2, "Items count is changed");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_n5</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create one more statusbar widget.
 * @step 5 Create progressbar widget.
 * @step 6 Create label widget.
 * @step 7 Append new item with progressbar content. (Statusbar from step 3)
 * @step 8 Append new item with SEPARATOR type. (Statusbar from step 3)
 * @step 9 Append new item with label content. (Statusbar from step 4)
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 8 of
 *         precondition before item from step 7 of precondition. Using object
 *         received at step 4 of precondition as statusbar parameter.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get (Using object from step 4 of precondition )
 * @step 4 Check list of items on NULL.
 * @step 5 Get count of items in statusbar. (should be 1)
 * </td>
 * <td>(Evas_Object *)statusbar_2, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)before</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_n5)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *statusbar_2 = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_p = NULL;
   Ewe_Statusbar_Item *item_s = NULL;
   Eina_Bool ret = EINA_FALSE;
   const Eina_List *items_list = NULL;
   int items_count = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   statusbar_2 = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   item_p = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_s = ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   ewe_statusbar_item_append(statusbar_2, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar_2, item_s, item_p);
   ck_assert_msg(ret == EINA_FALSE, "Items was moved in not parent widget");
   items_list = ewe_statusbar_items_list_get(statusbar_2);
   ck_assert_msg(items_list != NULL, "List of statusbar items is NULL");
   items_count = eina_list_count(items_list);
   ck_assert_msg(items_count == 1, "Items count is changed");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * <tr>
 * <td>ewe_statusbar_item_insert_before</td>
 * <td>ewe_statusbar_item_insert_before_test_n6</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create label widget.
 * @step 6 Append new item with progressbar content.
 * @step 7 Append new item with SEPARATOR type.
 * @step 8 Append new item with label content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_insert_before. Inserting item from step 7 of
 *         precondition before itself.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * @step 5 Get count of items in statusbar. (should be 3)
 * </td>
 * <td>(Evas_Object *)statusbar, (Ewe_Statusbar_Item *)item, (Ewe_Statusbar_Item *)item</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_insert_before_test_n6)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item_p = NULL;
   Eina_Bool ret = EINA_FALSE;
   const Eina_List *items_list = NULL;
   int items_count = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   label = elm_label_add(statusbar);
   item_p = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_append(statusbar, NULL, EWE_STATUSBAR_ITEM_TYPE_SEPARATOR,
                             NULL, NULL);
   ewe_statusbar_item_append(statusbar, label,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_insert_before(statusbar, item_p, item_p);
   ck_assert_msg(ret == EINA_FALSE, "Item was moved before itself");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list != NULL, "List of items is NULL");
   items_count = eina_list_count(items_list);
   ck_assert_msg(items_count == 3, "Items count is changed");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_insert_before
 * @{
 * </TABLE>
 * @}
 * @}
 */
