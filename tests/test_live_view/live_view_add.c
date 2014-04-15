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

#include "test_live_view.h"

/**
 * @addtogroup live_view_test
 * @{
 * @addtogroup live_view_add
 * @{
 * live_view
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_add
 * @{
 * <tr>
 * <td>live_view_add</td>
 * <td>live_view_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 *
 * @procedure
 * @step 1 Call live_view_add.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(Evas_Object *)parent</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_add_test_p)
{
   Evas_Object *parent = NULL;
   Live_View *live = NULL;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   live = live_view_add(parent);
   ck_assert_msg(live != NULL, "Failed to create live view object.");

   live_view_free(live);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_add
 * @{
 * <tr>
 * <td>live_view_add</td>
 * <td>live_view_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call live_view_add.
 * @step 2 Check returned pointer.
 * </td>
 * <td>NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_add_test_n)
{
   Live_View *live = NULL;

   elm_init(0, 0);

   live = live_view_add(NULL);
   ck_assert_msg(live == NULL, "Live view object was created "
                               "with NULL pointer parent");

   elm_shutdown();
}
END_TEST
/**
 * @addtogroup live_view_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
