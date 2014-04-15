/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
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

#include "test_groupedit.h"

/**
 * @addtogroup groupedit_test
 * @{
 * @addtogroup groupedit_min_size_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_min_size_set
 * @{
 * <tr>
 * <td>groupedit_min_size_set</td>
 * <td>groupedit_min_size_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set min size to container of the groupedit
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, 100, 100</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_min_size_set_test_p)
{
   Evas_Object *parent, *groupedit;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   groupedit = groupedit_add(parent);
   ck_assert_msg(groupedit_min_size_set(groupedit, 100, 100),
                 "Failed to set minimun size to container of the groupedit.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_min_size_set
 * @{
 * <tr>
 * <td>groupedit_min_size_set</td>
 * <td>groupedit_min_size_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 set min size to container of the groupedit
 * @step 2 check returned value
 * </td>
 * <td>NULL, 100, 100</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_min_size_set_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!groupedit_min_size_set(NULL, 100, 100),
                 "Failed to set minimun size to container of the groupedit.");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup groupedit_min_size_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
