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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <tr>
 * <td>wm_part_type_get</td>
 * <td>wm_part_type_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Unexisting Edje Part Type prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(55).
 * @step 2 Check returned object.
 * </td>
 * <td>Edje_Part_Type type = 55</td>
 * <td>NULL string returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_type_get_test_n1)
{
   elm_init(0,0);
   Edje_Part_Type type = 55;
   ck_assert_msg(!wm_part_type_get(type), "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_type_get
 * @{
 * <tr>
 * <td>wm_part_type_get</td>
 * <td>wm_part_type_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Unexisting Edje Part Type prepared.
 *
 * @procedure
 * @step 1 Call function wm_part_type_get(-13).
 * @step 2 Check returned object.
 * </td>
 * <td>Edje_Part_Type type = -13</td>
 * <td>NULL string returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_part_type_get_test_n2)
{
   elm_init(0,0);
   Edje_Part_Type type = -13;
   ck_assert_msg(!wm_part_type_get(type), "Not NULL returned");
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
