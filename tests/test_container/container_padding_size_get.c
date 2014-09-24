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
 * @addtogroup container_padding_size_get
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_padding_size_get
 * @{
 * <tr>
 * <td>container_padding_size_get</td>
 * <td>container_padding_size_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container paddings
 * @step 2 get the new size of container paddings
 * @step 3 check size of container paddings
 * </td>
 * <td>(Evas_Object *) container</td>
 * <td>EINA_TRUE, (5, 6, 7, 8)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_get_test_p1)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_padding_size_set(container, 5, 6, 7, 8);

   ck_assert_msg(container_padding_size_get(container, &lt_w, &lt_h, &br_w, &br_h),
                 "Failed to get the container paddings size.");

   ck_assert_msg(((lt_w == 5) || (lt_h == 6) || (br_w == 7) || (br_h == 8)),
                 "Returned container paddings sizes are wrong.");

   evas_object_del(container);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_get
 * @{
 * <tr>
 * <td>container_padding_size_get</td>
 * <td>container_padding_size_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new size for container paddings
 * @step 2 get the new size of container's top left handler
 * @step 3 get the new size of container's bottom right handler
 * @step 4 check size of container paddings
 * </td>
 * <td>(Evas_Object *) container</td>
 * <td>EINA_TRUE, (25, 26, 27, 28)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_get_test_p2)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_padding_size_set(container, 25, 26, 27, 28);

   ck_assert_msg(container_padding_size_get(container, &lt_w, &lt_h, NULL, NULL),
                 "Failed to get the container paddings size.");

   ck_assert_msg(((lt_w == 25) || (lt_h == 26) || (br_w == 0) || (br_h == 0)),
                 "Returned container TL handler's size is wrong.");

   ck_assert_msg(container_padding_size_get(container, NULL, NULL, &br_w, &br_h),
                 "Failed to get the container paddings size.");

   ck_assert_msg(((lt_w == 25) || (lt_h == 26) || (br_w == 27) || (br_h == 28)),
                 "Returned container BR handler's size is wrong.");

   evas_object_del(container);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_get
 * @{
 * <tr>
 * <td>container_padding_size_get</td>
 * <td>container_padding_size_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 get the new size of paddings from different, not container, object
 * @step 2 check size of container paddings
 * </td>
 * <td>(Evas_Object *) win</td>
 * <td>EINA_FALSE, (0, 0, 0, 0)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_get_test_n1)
{
   Evas_Object *parent;
   int lt_w, lt_h, br_w, br_h;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(!container_padding_size_get(parent, &lt_w, &lt_h, &br_w, &br_h),
                 "Not container object returned paddings size.");

   ck_assert_msg(((lt_w == 0) || (lt_h == 0) || (br_w == 0) || (br_h == 0)),
                 "Returned container paddings sizes are wrong.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_get
 * @{
 * <tr>
 * <td>container_padding_size_get</td>
 * <td>container_padding_size_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 get the new size of paddings from different, not container, object
 * @step 2 check size of container paddings
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE, (0, 0, 0, 0)</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_padding_size_get_test_n2)
{
   int lt_w, lt_h, br_w, br_h;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   ck_assert_msg(!container_padding_size_get(NULL, &lt_w, &lt_h, &br_w, &br_h),
                 "NULL object returned paddings size.");

   ck_assert_msg(((lt_w == 0) || (lt_h == 0) || (br_w == 0) || (br_h == 0)),
                 "Returned container paddings sizes are wrong.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_padding_size_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
