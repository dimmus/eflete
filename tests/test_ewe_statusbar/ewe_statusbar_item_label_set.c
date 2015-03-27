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
 * @addtogroup ewe_statusbar_item_label_set
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_label_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_label_set</td>
 * <td>ewe_statusbar_item_label_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 * @step 5 Add new item into statusbar. Item content is progresbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_label_set.
 * @step 2 Check returned value;
 * @step 3 Get label from item, using ewe_statusbar_item_label_get
 * @step 4 Compare getted label with setted label at step 1 of procedure.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, (char *)"statusbar"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_label_set_test_p1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   char *label = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_label_set(item, "statusbar");
   ck_assert_msg(ret == EINA_TRUE, "Failed to set label for item");
   label = ewe_statusbar_item_label_get(item);
   ck_assert_msg(strcmp(label, "statusbar") == 0, "Setted and getted labels "
                                                  "are not equals");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_label_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_label_set</td>
 * <td>ewe_statusbar_item_label_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 * @step 5 Add new item into statusbar. Item content is progresbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_label_set with empty string param.
 * @step 2 Check returned value;
 * @step 3 Get label from item, using ewe_statusbar_item_label_get
 * @step 4 Compare getted label with setted label at step 1 of procedure.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, (char *)""</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_label_set_test_p2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   char *label = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_label_set(item, "");
   ck_assert_msg(ret == EINA_TRUE, "Failed to set label for item");
   label = ewe_statusbar_item_label_get(item);
   ck_assert_msg(strcmp(label, "") == 0, "Setted and getted labels "
                                                  "are not equals");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_label_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_label_set</td>
 * <td>ewe_statusbar_item_label_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 * @step 5 Add new item into statusbar. Item content is progresbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_label_set.
 * @step 2 Check returned value;
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_label_set_test_n1)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_label_set(item, NULL);
   ck_assert_msg(ret == EINA_FALSE, "Set label with NULL string");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_label_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_label_set</td>
 * <td>ewe_statusbar_item_label_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_label_set.
 * @step 2 Check returned value;
 * </td>
 * <td>NULL, (char *)"statusbar"</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_label_set_test_n2)
{
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);

   ret = ewe_statusbar_item_label_set(NULL, "statusbar");
   ck_assert_msg(ret == EINA_FALSE, "Set label with NULL object item");

   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_label_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
