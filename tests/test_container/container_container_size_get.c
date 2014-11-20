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
 * @addtogroup container_container_size_get
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_container_size_get
 * @{
 * <tr>
 * <td>container_container_size_get</td>
 * <td>container_container_size_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container container
 * @step 2 get the new size for container container
 * @step 3 check returned value
 * </td>
 * <td>(Evas_Object *) container, 100, 100</td>
 * <td>EINA_TRUE, 100, 100</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_container_size_get_test_p)
{
   Evas_Object *parent, *container;
   int w, h;
   Eina_Bool res;

   elm_init(0, 0);
   w = h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_container_size_set(container, 100, 100);
   if (res)
     container_container_size_get(container, &w, &h);
   ck_assert_msg(((w == 100) && (h == 100)),
                 "Failed to get the container containers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_container_size_get
 * @{
 * <tr>
 * <td>container_container_size_get</td>
 * <td>container_container_size_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 get the new size for container container
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_container_size_get_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!container_container_size_get(NULL, NULL, NULL),
                 "Success to get the container container size.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_container_size_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
