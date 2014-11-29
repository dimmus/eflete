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

#include "test_live_view.h"

/**
 * @addtogroup live_view_test
 * @{
 * @addtogroup live_view_free
 * @{
 * live_view
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_free
 * @{
 * <tr>
 * <td>live_view_add</td>
 * <td>live_view_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create live view object.
 *
 * @procedure
 * @step 1 Call live_view_free.
 * @step 2 Check returned value.
 * </td>
 * <td>(Live_View *)live</td>
 * <td>function returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_free_test_p)
{
   Evas_Object *parent = NULL;
   Live_View *live = NULL;
   Eina_Bool ret = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   live = live_view_add(parent, false);

   ret = live_view_free(live);
   ck_assert_msg(ret == EINA_TRUE, "Failed freeing live view object");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup live_view_free
 * @{
 * <tr>
 * <td>live_view_add</td>
 * <td>live_view_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 None
 *
 * @procedure
 * @step 1 Call live_view_free.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>function returned EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_free_test_n)
{
   Eina_Bool ret = EINA_TRUE;

   ret = live_view_free(NULL);
   ck_assert_msg(ret == EINA_FALSE, "Freeing NULL pointer");
}
END_TEST

/**
 * @addtogroup live_view_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
