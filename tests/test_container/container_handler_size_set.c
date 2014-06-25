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

#include "test_container.h"

/**
 * @addtogroup container_test
 * @{
 * @addtogroup container_handler_size_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_handler_size_set
 * @{
 * <tr>
 * <td>container_handler_size_set</td>
 * <td>container_handler_size_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container handlers
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, 9, 9, 9, 9</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_set_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   ck_assert_msg(container_handler_size_set(container, 9, 9, 9, 9),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_set
 * @{
 * <tr>
 * <td>container_handler_size_set</td>
 * <td>container_handler_size_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 set the new size for container handlers
 * @step 2 check returned value
 * </td>
 * <td>NULL, 9, 9, 9, 9</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_set_test_n1)
{
   elm_init(0, 0);

   ck_assert_msg(!container_handler_size_set(NULL, 9, 9, 9, 9),
                 "Failed to set the container handlers size.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_set
 * @{
 * <tr>
 * <td>container_handler_size_set</td>
 * <td>container_handler_size_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container handlers
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)container, 0, 9, 9, 9</td>
 * <td>EINA_TRUE (5, 9, 9, 9)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_set_test_n2)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_handler_size_set(container, 0, 9, 9, 9);
   if (res)
     container_handler_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 5) || (lt_h == 9) || (br_w == 9) || (br_h == 9)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_set
 * @{
 * <tr>
 * <td>container_handler_size_set</td>
 * <td>container_handler_size_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container handlers
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)container, 9, 0, 9, 9</td>
 * <td>EINA_TRUE (9, 5, 9, 9)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_set_test_n3)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_handler_size_set(container, 9, 0, 9, 9);
   if (res)
     container_handler_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 9) || (lt_h == 5) || (br_w == 9) || (br_h == 9)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_set
 * @{
 * <tr>
 * <td>container_handler_size_set</td>
 * <td>container_handler_size_set_test_n4</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container handlers
 * @step 2 check return value
 * </td>
 * <td>(Evas_Object *)container, 9, 9, 0, 9</td>
 * <td>EINA_TRUE (9, 9, 5, 9)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_set_test_n4)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_handler_size_set(container, 9, 9, 0, 9);
   if (res)
     container_handler_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 9) || (lt_h == 9) || (br_w == 5) || (br_h == 9)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_set
 * @{
 * <tr>
 * <td>container_handler_size_set</td>
 * <td>container_handler_size_set_test_n5</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container handlers
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)container, 9, 9, 9, 0</td>
 * <td>EINA_TRUE (9, 9, 9, 5)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_set_test_n5)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_handler_size_set(container, 9, 9, 9, 0);
   if (res)
     container_handler_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 9) || (lt_h == 9) || (br_w == 9) || (br_h == 5)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
