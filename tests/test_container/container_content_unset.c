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
 * @addtogroup container_content_unset
 * @{
 * Container
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_content_unset
 * @{
 * <tr>
 * <td>container_content_unset</td>
 * <td>container_content_unset_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 * @step 4 create a content, elm_button
 * @step 5 set the content object to the container
 *
 * @procedure
 * @step 1 try to unset content object from the container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container</td>
 * <td>Not NULL pointer</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_content_unset_test_p)
{
   Evas_Object *parent, *container, *content;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   container = container_add(parent);
   content = elm_button_add(container);
   container_content_set(container, content);

   ck_assert_msg(container_content_unset(container) != NULL,
                 "Can't unset the content object from the container.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_content_unset
 * @{
 * <tr>
 * <td>container_content_unset</td>
 * <td>container_content_unset_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 try to unset the content object from the container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container</td>
 * <td>NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_content_unset_test_n1)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   container = container_add(parent);

   ck_assert_msg(container_content_unset(container) == NULL,
                 "Unset the content object from the container without content.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_content_unset
 * @{
 * <tr>
 * <td>container_content_unset</td>
 * <td>container_content_unset_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 try to unset the content object from the containet
 * @step 2 check returned value
 * </td>
 * <td>NULL</td>
 * <td>NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_content_unset_test_n2)
{
   elm_init(0, 0);

   ck_assert_msg(container_content_unset(NULL) == NULL,
                 "Unset the content object from the NULL container.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_content_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
