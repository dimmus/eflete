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
 * @addtogroup ewe_statusbar_item_width_set
 * @{
 * ewe_statusbar
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_statusbar_item_width_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_width_set</td>
 * <td>ewe_statusbar_item_width_set_test_p</td>
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
 * @step 1 Call ewe_statusbar_item_width_set with item from step 5 of precondition.
 * @step 2 Check returned value;
 * @step 3 Check width of item from step 5 of precondition (should equal setted width).
 * @step 4 Check width of item from step 6 of precondition (should equal 150).
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, (int) 200 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_width_set_test_p)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Ewe_Statusbar_Item *item_2 = NULL;
   Eina_Bool ret = EINA_FALSE;
   int width = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   item_2 = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_width_set(item, 300);
   ck_assert_msg(ret == EINA_TRUE, "Failed to set item width");
   width = ewe_statusbar_item_width_get(item);
   ck_assert_msg(width == 300, "Setted width not equal with getted");
   width = ewe_statusbar_item_width_get(item_2);
   ck_assert_msg(width == 150, "Getted width for second item not default");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_width_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_width_set</td>
 * <td>ewe_statusbar_item_width_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_width_set with item from step 5 of precondition.
 * @step 2 Check returned value;
 * </td>
 * <td>NULL, (int) 200 </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_width_set_test_n1)
{
   Evas_Object *win = NULL;
   Eina_Bool ret = EINA_TRUE;

   ewe_init(0, 0);

   ret = ewe_statusbar_item_width_set(NULL, 200);
   ck_assert_msg(ret == EINA_FALSE, "Set new width for NULL item");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_statusbar_item_width_set
 * @{
 * <tr>
 * <td>ewe_statusbar_item_width_set</td>
 * <td>ewe_statusbar_item_width_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize EWE library.
 * @step 2 Create parent window.
 * @step 3 Create statusbar widget.
 * @step 4 Create progressbar widget.
 * @step 5 Add new item to statusbar widget.
 *
 * @procedure
 * @step 1 Call ewe_statusbar_item_width_set with negative value.
 * @step 2 Check returned value;
 * @step 3 Check width of item, it should be equal default value (150)
 * </td>
 * <td>(Ewe_Statusbar_Item *)item, (int) -50 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_statusbar_item_width_set_test_n2)
{
   Evas_Object *win = NULL;
   Evas_Object *statusbar = NULL;
   Ewe_Statusbar_Item *item = NULL;
   Eina_Bool ret = EINA_TRUE;
   int width = 0;

   ewe_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   statusbar = ewe_statusbar_add(win);
   item = ewe_statusbar_item_append(statusbar, NULL,
                             EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);

   ret = ewe_statusbar_item_width_set(item, -50);
   ck_assert_msg(ret == EINA_FALSE, "Setted negative value as item width");
   width = ewe_statusbar_item_width_get(item);
   ck_assert_msg(width == 150, "Getted width not equal default value");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST


/**
 * @addtogroup ewe_statusbar_item_width_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
