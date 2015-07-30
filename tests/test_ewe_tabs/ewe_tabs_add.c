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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */
#include "test_ewe_tabs.h"

/**
 * @addtogroup ewe_tabs_test
 * @{
 * @addtogroup ewe_tabs_add
 * @{
 * ewe_tabs
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_tabs_add
 * @{
 * <tr>
 * <td>ewe_tabs_add</td>
 * <td>ewe_tabs_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 * @step 2 parent window created
 *
 * @procedure
 * @step 1 call ewe_tabs_add
 * @step 2 check returned pointer
 * </td>
 * <td>Evas_Object *win</td>
 * <td>Not NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_add_test_p)
{
   ewe_init(0, 0);
   Evas_Object *win = elm_win_util_standard_add("test", "test");

   Evas_Object *obj = ewe_tabs_add(win);
   ck_assert_msg(obj != NULL, "Tab not added");

   evas_object_del(win);
   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_add
 * @{
 * <tr>
 * <td>ewe_tabs_add</td>
 * <td>ewe_tabs_add</td>
 * <td>
 * @precondition
 * @step 1 initialized ewe
 *
 * @procedure
 * @step 1 call ewe_tabs_add
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(ewe_tabs_add_test_n)
{
   ewe_init(0, 0);

   Evas_Object *obj = ewe_tabs_add(NULL);
   ck_assert_msg(obj == NULL, "Added tabs with NULL parent");

   ewe_shutdown();
}
END_TEST

/**
 * @addtogroup ewe_tabs_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
