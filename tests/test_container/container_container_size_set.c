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
 * @addtogroup container_container_size_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_container_size_set
 * @{
 * <tr>
 * <td>container_container_size_set</td>
 * <td>container_container_size_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, 100, 100</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_container_size_set_test_p1)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   ck_assert_msg(container_container_size_set(container, 100, 100),
                 "Failed to set the container container size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_container_size_set
 * @{
 * <tr>
 * <td>container_container_size_set</td>
 * <td>container_container_size_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)container, 0, 100</td>
 * <td>EINA_TRUE (0, 100)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_container_size_set_test_p2)
{
   Evas_Object *parent, *container;
   int w, h;
   Eina_Bool res;

   elm_init(0, 0);
   w = h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_min_size_set(container, 50, 200);
   res = container_container_size_set(container, 0, 300);
   if (res)
     container_container_size_get(container, &w, &h);
   ck_assert_msg((w == 50) && (h == 300),
                 "Failed to set the container containers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_container_size_set
 * @{
 * <tr>
 * <td>container_container_size_set</td>
 * <td>container_container_size_set_test_p3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)container, 100, 0</td>
 * <td>EINA_TRUE (100, 0)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_container_size_set_test_p3)
{
   Evas_Object *parent, *container;
   int w, h;
   Eina_Bool res;

   elm_init(0, 0);
   w = h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_min_size_set(container, 200, 50);
   res = container_container_size_set(container, 300, 0);
   if (res)
     container_container_size_get(container, &w, &h);
   ck_assert_msg((w == 300) && (h == 50),
                 "Failed to set the container containers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST



/**
 * @addtogroup container_container_size_set
 * @{
 * <tr>
 * <td>container_container_size_set</td>
 * <td>container_container_size_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 set the new size for container container
 * @step 2 check returned value
 * </td>
 * <td>NULL, 100, 100</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_container_size_set_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!container_container_size_set(NULL, 100, 100),
                 "Func returned true, though was used NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_container_size_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
