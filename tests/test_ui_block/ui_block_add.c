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

#include "test_ui_block.h"

/**
 * @addtogroup ui_block_test
 * @{
 * @addtogroup ui_block_add
 * @{
 * ui_block
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_block_add
 * @{
 * <tr>
 * <td>ui_block_add</td>
 * <td>ui_block_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Created window.
 *
 * @procedure
 * @step 1 Call ui_block_add(parent)
 * </td>
 * <td>Evas_Object *parent</td>
 * <td>Not NULL returned value</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_block_add_test_p)
{
   elm_init(0, 0);
   Evas_Object *result, *parent;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   result = ui_block_add(parent);
   ck_assert_msg(!strcmp(evas_object_type_get(result), "elm_layout"), "Not layout returned\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_add
 * @{
 * <tr>
 * <td>ui_block_add</td>
 * <td>ui_block_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call ui_block_add(NULL)
 * </td>
 * <td>NULL</td>
 * <td>NULL returned value</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_block_add_test_n)
{
   elm_init(0, 0);
   ck_assert_msg(!ui_block_add(NULL), "Not NULL returned\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
