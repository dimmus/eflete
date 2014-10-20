/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
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

#include "test_groupedit.h"

/**
 * @addtogroup groupedit_test
 * @{
 * @addtogroup groupedit_container_size_get
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_container_size_get
 * @{
 * <tr>
 * <td>groupedit_container_size_get</td>
 * <td>groupedit_container_size_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set the new size for groupedit container
 * @step 2 get the new size for groupedit container
 * @step 3 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, 100, 100</td>
 * <td>EINA_TRUE, 100, 100</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_container_size_get_test_p)
{
   Evas_Object *parent, *groupedit;
   int w, h;
   Eina_Bool res;

   elm_init(0, 0);
   w = h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   groupedit = groupedit_add(parent);
   res = groupedit_container_size_set(groupedit, 100, 100);
   if (res)
     groupedit_container_size_get(groupedit, &w, &h);
   ck_assert_msg(((w == 100) && (h == 100)),
                 "Failed to get the groupedit containers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_container_size_get
 * @{
 * <tr>
 * <td>groupedit_container_size_get</td>
 * <td>groupedit_container_size_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 get the new size for groupedit container
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_container_size_get_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!groupedit_container_size_get(NULL, NULL, NULL),
                 "Success to get the groupedit container size.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_container_size_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
