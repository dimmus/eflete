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
 * @addtogroup ewe_statusbar_item_label_get
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_label_get
 * @{
 * <tr>
 * <td>ewe_statusbar_item_label_get</td>
 * <td>ewe_statusbar_item_label_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 * @step 5 Add new item into statusbar. Item content is progresbar widget.
 * @step 6 Add label to item.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_label_get.
 * @step 2 Check returned value on NULL;
 * @step 3 Compare returned value with setted label at step 6 of precondition.
 * </td>
 * <td>(Ewe_Statusbar_Item *)item</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_label_get_test_p)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   char *label = NULL;
   Ewe_Statusbar_Item *item = NULL;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   item = ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_label_set(item, "statusbar");

   label = ewe_statusbar_item_label_get(item);
   ck_assert_msg(label != NULL, "NULL label pointer returned");
   ck_assert_msg(strcmp(label, "statusbar") == 0, "Setted and getted labels "
                                                  "are not equals");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_label_get
 * @{
 * <tr>
 * <td>ewe_statusbar_item_label_get</td>
 * <td>ewe_statusbar_item_label_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_label_get.
 * @step 2 Check returned value on NULL;
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_label_get_test_n)
{
   char *label = NULL;
   ewe_init(0, 0);

   label = ewe_statusbar_item_label_get(NULL);
   ck_assert_msg(label == NULL, "Not NULL label pointer returned");

   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_label_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
