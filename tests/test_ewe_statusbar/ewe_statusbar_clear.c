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
 * @addtogroup ewe_statusbar_clear
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_clear
 * @{
 * <tr>
 * <td>ewe_statusbar_clear</td>
 * <td>ewe_statusbar_clear_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget
 * @step 5 Add new item into statusbar. Item content is progresbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_clear.
 * @step 2 Check returned value.
 * @step 3 Call ewe_statusbar_items_list_get
 * @step 4 Check list of items on NULL.
 * </td>
 * <td>(Evas_Object *)statusbar</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_clear_test_p)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Evas_Object *progressbar = NULL;
   const Eina_List *items_list = NULL;
   Eina_Bool ret = EINA_FALSE;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   progressbar = elm_progressbar_add(statusbar);
   ewe_statusbar_item_append(statusbar, progressbar,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_clear(statusbar);
   ck_assert_msg(ret == EINA_TRUE, "Failed to clean statusbar items");
   items_list = ewe_statusbar_items_list_get(statusbar);
   ck_assert_msg(items_list == NULL, "After cleaning statusbar, not NULL list of "
                                     "items returned");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_clear
 * @{
 * <tr>
 * <td>ewe_statusbar_clear</td>
 * <td>ewe_statusbar_clear_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_clear.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_clear_test_n)
{
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);

   ret = ewe_statusbar_clear(NULL);
   ck_assert_msg(ret == EINA_FALSE, "Clear statusbar items, without statusbar "
                                    " object");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_clear
 * @{
 * </TABLE>
 * @}
 * @}
 */
