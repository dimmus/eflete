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
 * @addtogroup container_confine_set
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_confine_set
 * @{
 * <tr>
 * <td>container_confine_set</td>
 * <td>container_confine_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 * @step 4 create a rectangle for confining.
 *
 * @procedure
 * @step 1 confine container with different object.
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, (Evas_Object *)rect</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_confine_set_test_p)
{
   Evas_Object *parent, *container, *rect;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   container = container_add(parent);
   rect = evas_object_rectangle_add(evas_object_evas_get(parent));
   ck_assert_msg(container_confine_set(container, rect) == EINA_TRUE,
                 "Confining the container to thr rectangle object FAILED.");

   evas_object_del(container);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_confine_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
