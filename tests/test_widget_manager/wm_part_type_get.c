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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_part_type_get
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_part_type_get
 * @{
 * <tr>
 * <td>wm_part_type_get</td>
 * <td>wm_part_type_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Edje Part Type for TEXT prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(EDJE_PART_TYPE_TEXT).
 * @step 2 Check returned object.
 * </td>
 * <td>Edje_Part_Type type = EDJE_PART_TYPE_TEXT</td>
 * <td>"TEXT" string returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_type_get_test_p1)
{
   elm_init(0,0);
   Edje_Part_Type type = EDJE_PART_TYPE_TEXT;
   ck_assert_str_eq(wm_part_type_get(type), "TEXT");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get
 * @{
 * <tr>
 * <td>wm_part_type_get</td>
 * <td>wm_part_type_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Edje Part Type for NONE prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(EDJE_PART_TYPE_NONE).
 * @step 2 Check returned object.
 * </td>
 * <td>Edje_Part_Type type = EDJE_PART_TYPE_NONE</td>
 * <td>"NONE" string returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_type_get_test_p2)
{
   elm_init(0,0);
   Edje_Part_Type type = EDJE_PART_TYPE_NONE;
   ck_assert_str_eq(wm_part_type_get(type), "NONE");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
