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

#include "test_container.h"

/**
 * @addtogroup container_test
 * @{
 * @addtogroup container_min_size_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_min_size_set
 * @{
 * <tr>
 * <td>container_min_size_set</td>
 * <td>container_min_size_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set min size to container of the container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, 100, 100</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_min_size_set_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   ck_assert_msg(container_min_size_set(container, 100, 100),
                 "Failed to set minimun size to container of the container.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_min_size_set
 * @{
 * <tr>
 * <td>container_min_size_set</td>
 * <td>container_min_size_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 set min size to container of the container
 * @step 2 check returned value
 * </td>
 * <td>NULL, 100, 100</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_min_size_set_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!container_min_size_set(NULL, 100, 100),
                 "Failed to set minimun size to container of the container.");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup container_min_size_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
