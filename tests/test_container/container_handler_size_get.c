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
 * @addtogroup container_handler_size_get
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_handler_size_get
 * @{
 * <tr>
 * <td>container_handler_size_get</td>
 * <td>container_handler_size_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 get the new size for container handlers
 * @step 2 check size of container handlers
 * </td>
 * <td>(Evas_Object *) container</td>
 * <td>5, 5, 5, 5</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_get_test_p)
{
   Evas_Object *parent, *container;
   int lt_w, lt_h, br_w, br_h;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_handler_size_get(container, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((lt_w == 5) || (lt_h == 5) || (br_w == 5) || (br_h == 5)),
                 "Failed to set the container handlers size.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_get
 * @{
 * <tr>
 * <td>container_handler_size_get</td>
 * <td>container_handler_size_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 get the new size for container handlers
 * @step 2 check size of container handlers
 * </td>
 * <td>NULL</td>
 * <td>0, 0, 0, 0</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_handler_size_get_test_n)
{
   int lt_w, lt_h, br_w, br_h;

   elm_init(0, 0);
   lt_w = lt_h = br_w = br_h = 0;

   container_handler_size_get(NULL, &lt_w, &lt_h, &br_w, &br_h);
   ck_assert_msg(((lt_w == 0) || (lt_h == 0) || (br_w == 0) || (br_h == 0)),
                 "Failed to set the container handlers size.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_handler_size_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
