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
 * @addtogroup container_content_set
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_content_set
 * @{
 * <tr>
 * <td>container_content_set</td>
 * <td>container_content_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 * @step 4 create a content, elm_button
 *
 * @procedure
 * @step 1 set to the container the elm_buttom object
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, (Evas_Object) *content<td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_content_set_test_p)
{
   Evas_Object *parent, *container, *content;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   container = container_add(parent);
   content = elm_button_add(container);

   ck_assert_msg(container_content_set(container, content),
                 "The content object not setted to the container.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_content_set
 * @{
 * <tr>
 * <td>container_content_set</td>
 * <td>container_content_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a content, elm_button
 *
 * @procedure
 * @step 1 set to the container editable object
 * @step 2 check returned value
 * </td>
 * <td>NULL, (Evas_Object) *content</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_content_set_test_n1)
{
   Evas_Object *parent, *content;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   content = elm_button_add(parent);

   ck_assert_msg(!container_content_set(NULL, content),
                 "The content object setted to the NULL pointer.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_content_set
 * @{
 * <tr>
 * <td>container_content_set</td>
 * <td>container_content_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set to the container editable object
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_content_set_test_n2)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   container = container_add(parent);

   ck_assert_msg(!container_content_set(container, NULL),
                 "The NULL object setted to the container.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_content_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
