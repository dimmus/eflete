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

#include "test_groupedit.h"

/**
 * @addtogroup groupedit_test
 * @{
 * @addtogroup groupedit_style_get
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_style_get
 * @{
 * <tr>
 * <td>groupedit_style_get</td>
 * <td>groupedit_style_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set the new style to groupedit
 * @step 2 get the style name from groupedit
 * @step 3 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, "default"</td>
 * <td>EINA_TRUE, "default"</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_style_get_test_p)
{
   Evas_Object *parent, *groupedit;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   groupedit = groupedit_add(parent);
   groupedit_style_set(groupedit, "default");
   ck_assert_msg(strcmp(groupedit_style_get(groupedit), "default") == 0,
                 "Failed to get the groupedit a style.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_style_get
 * @{
 * <tr>
 * <td>groupedit_style_get</td>
 * <td>groupedit_style_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 get the a new style name
 * @step 2 check returned value
 * </td>
 * <td>NULL, "default"</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_style_get_test_n)
{
   elm_init(0, 0);

   ck_assert_msg(!groupedit_style_get(NULL),
                 "Getting a style from NULL object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_style_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
