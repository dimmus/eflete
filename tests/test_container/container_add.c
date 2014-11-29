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
 * @addtogroup container_add
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_add
 * @{
 * <tr>
 * <td>container_add</td>
 * <td>container_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 *
 * @procedure
 * @step 1 call container_add
 * @step 2 check returned pointer
 * </td>
 * <td>(Evas_Object *) window</td>
 * <td>Returned pointer not NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST (container_add_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   ck_assert_msg(container != NULL, "Failed create container object.");
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_add
 * @{
 * <tr>
 * <td>container_add</td>
 * <td>container_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call container_add
 * @step 2 check returned pointer
 * </td>
 * <td>NULL</td>
 * <td>Returned pointer is NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST (container_add_test_n)
{
   Evas_Object *container;

   elm_init(0, 0);

   container = container_add(NULL);
   ck_assert_msg(container == NULL, "Failed create container object.");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup container_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
