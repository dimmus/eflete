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

#include "test_live_view_prop.h"

/**
 * @addtogroup live_view_prop_test
 * @{
 * @addtogroup live_view_property_free
 * @{
 * live_view_prop
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_view_property_free
 * @{
 * <tr>
 * <td>live_view_property_free</td>
 * <td>live_view_property_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create live view property.
 *
 * @procedure
 * @step 1 Call live_view_property_free.
 * @step 2 Check result.
 * </td>
 * <td>(Evas_Object *)property</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_view_property_free_test_p)
{
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;
   Eina_Bool result = EINA_FALSE;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   //live = live_view_property_add(parent, false);
   ck_assert_msg(live != NULL, "Failed to create live view property object.");

   result = live_view_property_free(live);
   ck_assert_msg(result == EINA_TRUE, "Could not delete live-view's property.");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_view_property_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
