/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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
 * @addtogroup groupedit_container_size_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_container_size_set
 * @{
 * <tr>
 * <td>groupedit_container_size_set</td>
 * <td>groupedit_container_size_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set the new size for groupedit container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, 100, 100</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_container_size_set_test_p1)
{
   Evas_Object *parent, *groupedit;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   groupedit = groupedit_add(parent);
   ck_assert_msg(groupedit_container_size_set(groupedit, 100, 100),
                 "Failed to set the groupedit container size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_container_size_set
 * @{
 * <tr>
 * <td>groupedit_container_size_set</td>
 * <td>groupedit_container_size_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set the new size for groupedit container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)groupedit, 0, 100</td>
 * <td>EINA_TRUE (0, 100)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_container_size_set_test_p2)
{
   Evas_Object *parent, *groupedit;
   int w, h;
   Eina_Bool res;

   elm_init(0, 0);
   w = h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   groupedit = groupedit_add(parent);
   groupedit_min_size_set(groupedit, 50, 200);
   res = groupedit_container_size_set(groupedit, 0, 300);
   if (res)
     groupedit_container_size_get(groupedit, &w, &h);
   ck_assert_msg((w == 50) && (h == 300),
                 "Failed to set the groupedit containers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_container_size_set
 * @{
 * <tr>
 * <td>groupedit_container_size_set</td>
 * <td>groupedit_container_size_set_test_p3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set the new size for groupedit container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)groupedit, 100, 0</td>
 * <td>EINA_TRUE (100, 0)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_container_size_set_test_p3)
{
   Evas_Object *parent, *groupedit;
   int w, h;
   Eina_Bool res;

   elm_init(0, 0);
   w = h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   groupedit = groupedit_add(parent);
   groupedit_min_size_set(groupedit, 200, 50);
   res = groupedit_container_size_set(groupedit, 300, 0);
   if (res)
     groupedit_container_size_get(groupedit, &w, &h);
   ck_assert_msg((w == 300) && (h == 50),
                 "Failed to set the groupedit containers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST



/**
 * @addtogroup groupedit_container_size_set
 * @{
 * <tr>
 * <td>groupedit_container_size_set</td>
 * <td>groupedit_container_size_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 set the new size for groupedit container
 * @step 2 check returned value
 * </td>
 * <td>NULL, 100, 100</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_container_size_set_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!groupedit_container_size_set(NULL, 100, 100),
                 "Func returned true, though was used NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_container_size_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
