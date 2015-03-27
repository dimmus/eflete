/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */
#include "test_ewe_tabs.h"

/**
 * @addtogroup ewe_tabs_test
 * @{
 * @addtogroup ewe_tabs_orient_horizontal_get
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_orient_horizontal_get
 * @{
 * <tr>
 * <td>ewe_tabs_orient_horizontal_get</td>
 * <td>ewe_tabs_orient_horizontal_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 * @step 3 ewe_tabs added
 * @step 4 Item added to tabs
 *
 * @procedure
 * @step 1 call ewe_tabs_orient_horizontal_set(tabs, EINA_FALSE);
 * @step 2 call ewe_tabs_orient_horizontal_get(tabs);
 * @step 3 check that returned value is EINA_FALSE
 * </td>
 * <td> Ewe_Tabs_Item *tabs</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_orient_horizontal_get_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *tabs = ewe_tabs_add(win);
   ewe_tabs_item_append(tabs, NULL, "item", NULL);
   ewe_tabs_orient_horizontal_set(tabs, EINA_FALSE);
   ck_assert_msg(ewe_tabs_orient_horizontal_get(tabs) == EINA_FALSE, "Cannot get orient from tabs");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_orient_horizontal_get
 * @{
 * <tr>
 * <td>ewe_tabs_orient_horizontal_get</td>
 * <td>ewe_tabs_orient_horizontal_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_orient_horizontal_get(NULL, EINA_TRUE);
 * </td>
 * <td>NULL, EINA_TRUE; NULL, EINA_FALSE;/td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_orient_horizontal_get_test_n)
{
   ewe_init(0, 0);

   ck_assert_msg(ewe_tabs_orient_horizontal_get(NULL) == EINA_FALSE, "Orient was getted from NULL object");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_orient_horizontal_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
