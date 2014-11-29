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
 * @addtogroup container_border_hide
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_border_hide
 * @{
 * <tr>
 * <td>container_border_hide</td>
 * <td>container_border_hide_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 hide layout of the container.
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_border_hide_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   container = container_add(parent);
   ck_assert_msg(container_border_hide(container) == EINA_TRUE,
                 "Failed to hide layout of the container.");

   evas_object_del(container);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_border_hide
 * @{
 * <tr>
 * <td>container_border_hide</td>
 * <td>container_border_hide_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 hide layout of the different object that is not container.
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *)win</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_border_hide_test_n1)
{
   Evas_Object *parent;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ck_assert_msg(container_border_hide(parent) == EINA_FALSE,
                 "Given object is not container but function was successfull anyway.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_border_hide
 * @{
 * <tr>
 * <td>container_border_hide</td>
 * <td>container_border_hide_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 hide layout of the NULL object.
 * @step 2 check returned value
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_border_hide_test_n2)
{
   elm_init(0, 0);

   ck_assert_msg(!container_border_hide(NULL),
                 "Hiding layout of the NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_border_hide
 * @{
 * </TABLE>
 * @}
 * @}
 */
