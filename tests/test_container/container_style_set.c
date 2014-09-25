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
 * @addtogroup container_style_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup container_style_set
 * @{
 * <tr>
 * <td>container_style_set</td>
 * <td>container_style_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 set the new style to container
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, "default"</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_style_set_test_p)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   container_style_set(container, "wrong_style");
   ck_assert_msg(container_style_set(container, "default"),
                 "Style 'default' not setted to the conteiner.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_style_set
 * @{
 * <tr>
 * <td>container_style_set</td>
 * <td>container_style_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 set the a new style name
 * @step 2 check returned value
 * </td>
 * <td>NULL, "default"</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_style_set_test_n1)
{
   elm_init(0, 0);

   ck_assert_msg(!container_style_set(NULL, "default"),
                 "Setted to NULL object style.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup container_style_set
 * @{
 * <tr>
 * <td>container_style_set</td>
 * <td>container_style_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a container
 *
 * @procedure
 * @step 1 call container_style_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) container, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(container_style_set_test_n2)
{
   Evas_Object *parent, *container;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   container = container_add(parent);
   ck_assert_msg(!container_style_set(container, NULL),
                 "Setted NULL style to container object");

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup container_style_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
