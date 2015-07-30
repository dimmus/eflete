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
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_content_set</td>
 * <td>ewe_statusbar_item_content_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Create statusbar item with progressbar as content.
 * @step 6 Create label widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_content_set. With label widget as content.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_container_get.
 * @step 4 Check returned object with label from step 6 of precondition.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item,(Evas_Object *)label</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_content_set_test_p1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Eina_Bool ret = EINA_FALSE;
   Ewe_Statusbar_Item *item = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   label = elm_label_add(statusbar);

   ret = ewe_statusbar_item_content_set(item, label);
   ck_assert_msg(ret == EINA_TRUE, "Failed to set new content");
   content = ewe_statusbar_item_content_get(item);
   ck_assert_msg(content == label, "Setted and getted objects are not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_content_set</td>
 * <td>ewe_statusbar_item_content_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create statusbar item without content.
 * @step 5 Create label widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_content_set. With label widget as content.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_container_get.
 * @step 4 Check returned object with label from step 6 of precondition.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item,(Evas_Object *)label</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_content_set_test_p2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Eina_Bool ret = EINA_FALSE;
   Ewe_Statusbar_Item *item = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   label = elm_label_add(statusbar);

   ret = ewe_statusbar_item_content_set(item, label);
   ck_assert_msg(ret == EINA_TRUE, "Failed to set new content");
   content = ewe_statusbar_item_content_get(item);
   ck_assert_msg(content == label, "Setted and getted objects are not equal");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_content_set</td>
 * <td>ewe_statusbar_item_content_set_test_p3</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create statusbar item with type EWE_STATUSBAR_ITEM_TYPE_SEPARATOR.
 * @step 5 Create label widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_content_set. With label widget as content.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_container_get.
 * @step 4 Check returned object with label from step 6 of precondition.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item,(Evas_Object *)label</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_content_set_test_p3)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *label = NULL;
   Evas_Object *content = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, NULL, NULL);
   label = elm_label_add(statusbar);

   ret = ewe_statusbar_item_content_set(item, label);
   ck_assert_msg(ret == EINA_FALSE, "Set content for separator item");
   content = ewe_statusbar_item_content_get(item);
   ck_assert_msg(content == NULL, "Separator item store container");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_content_set</td>
 * <td>ewe_statusbar_item_content_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create statusbar item without content.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_content_set. Without content object.
 * @step 2 Check returned value.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_content_set_test_n1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_SEPARATOR, NULL, NULL);

   ret = ewe_statusbar_item_content_set(item, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Set content without content object");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_content_set</td>
 * <td>ewe_statusbar_item_content_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create label widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_content_set. With label widget as content.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Evas_Object *)label</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_content_set_test_n2)
{
   Evas_Object *win = NULL;
   Evas_Object *label = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   label = elm_label_add(win);

   ret = ewe_statusbar_item_content_set(NULL, label);
   ck_assert_msg(ret == EINA_FALSE, "Set content without item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_content_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
