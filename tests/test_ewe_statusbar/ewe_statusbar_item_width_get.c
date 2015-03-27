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
 * @addtogroup ewe_statusbar_item_width_get
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_width_get
 * @{
 * <tr>
 * <td>ewe_statusbar_item_width_get</td>
 * <td>ewe_statusbar_item_width_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Add new item to statusbar widget.
 * @step 6 Add new item to statusbar widget.
 * @step 7 Set width '200' for item from step 6.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_width_get with item from step 5 of precondition.
 * @step 2 Check returned value (should equal default - 150);
 * @step 3 Check width of item from step 6 of precondition (should equal 200).
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, (int) 200 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_width_get_test_p)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Ewe_Statusbar_Item *item_2 = NULL;
   int width = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_2 = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_width_set(item_2, 200);

   width = ewe_statusbar_item_width_get(item);
   ck_assert_msg(width == 150, "Getted width value, for item, is not default");
   width = ewe_statusbar_item_width_get(item_2);
   ck_assert_msg(width == 200, "Getted width not equal with setted");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_width_get
 * @{
 * <tr>
 * <td>ewe_statusbar_item_width_get</td>
 * <td>ewe_statusbar_item_width_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_width_get0.
 * @step 2 Check returned value (should equal '-2');
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, (int) 200 </td>
 * <td>'-2' value returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_width_get_test_n)
{
   int width = 0;

   ewe_init(0, 0);

   width = ewe_statusbar_item_width_get(NULL);
   ck_assert_msg(width == -2, "Getted width value, for NULL item");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_width_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
