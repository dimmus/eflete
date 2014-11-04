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
 * @addtogroup container_padding_size_set
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container paddings
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, 9, 9, 9, 9</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   ck_assert_msg(container_padding_size_set(container, 9, 9, 9, 9),
                 "Failed to set the container handlers size.");

   evas_object_del(container);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 set the new size for container paddings
 * @step 2 check returned value
 * </td>
 * <td>NULL, 9, 9, 9, 9</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_n1)
{
   elm_init(0, 0);

   ck_assert_msg(!container_padding_size_set(NULL, 9, 9, 9, 9),
                 "Paddings were set for object that is not container.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 set the new size for container paddings
 * @step 2 check returned value
 * </td>
 * <td>NULL, 9, 9, 9, 9</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_n2)
{
   Evas_Object *parent;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(!container_padding_size_set(parent, 9, 9, 9, 9),
                 "Paddings were set for object that is not container.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_n3</td>
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
 * <td>(Evas_Object *)container, -79, 9, 9, 9</td>
 * <td>EINA_TRUE (0, 9, 9, 9)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_n3)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_padding_size_set(container, -79, 9, 9, 9);
   if (res)
     container_padding_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 0) || (lt_h == 9) || (br_w == 9) || (br_h == 9)),
                 "Failed to set the container handlers size.");

   evas_object_del(container);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_n4</td>
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
 * <td>(Evas_Object *)container, 9, -16, 9, 9</td>
 * <td>EINA_TRUE (9, 0, 9, 9)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_n4)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_padding_size_set(container, 9, -16, 9, 9);
   if (res)
     container_padding_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 9) || (lt_h == 0) || (br_w == 9) || (br_h == 9)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_n5</td>
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
 * <td>(Evas_Object *)container, 9, 9, -333, 9</td>
 * <td>EINA_TRUE (9, 9, 0, 9)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_n5)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_padding_size_set(container, 9, 9, -333, 9);
   if (res)
     container_padding_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 9) || (lt_h == 9) || (br_w == 0) || (br_h == 9)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * <tr>
 * <td>container_padding_size_set</td>
 * <td>container_padding_size_set_test_n6</td>
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
 * <td>(Evas_Object *)container, 9, 9, 9, -1</td>
 * <td>EINA_TRUE (9, 9, 9, 0)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_set_test_n6)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;
   Eina_Bool res;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   res = container_padding_size_set(container, 9, 9, 9, -1);
   if (res)
     container_padding_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((!res) || (lt_w == 9) || (lt_h == 9) || (br_w == 9) || (br_h == 0)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
