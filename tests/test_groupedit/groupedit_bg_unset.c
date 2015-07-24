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
 * @addtogroup groupedit_bg_unset
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_bg_unset
 * @{
 * <tr>
 * <td>groupedit_bg_unset</td>
 * <td>groupedit_bg_unset_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create evas image
 *
 * @procedure
 * @step 1 call groupedit_add
 * @step 2 call groupedit_bg_set
 * @step 3 call groupedit_bg_unset
 * @step 4 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit</td>
 * <td>Image pointer equal to setted image pointer</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_bg_unset_test_p)
{
   Evas_Object *parent, *groupedit, *image;
   Evas_Object *res_image;

   elm_init(0, 0);

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   image = evas_object_image_add(evas_object_evas_get(parent));

   groupedit = groupedit_add(parent);
   groupedit_bg_set(groupedit, image);
   res_image = groupedit_bg_unset(groupedit);
   ck_assert_msg(image == res_image,
                 "Failed to unset the bg image from groupedit.");
   evas_object_del(image);
   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_bg_unset
 * @{
 * </TABLE>
 * @}
 * @}
 */
