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
 * @addtogroup ui_block_content_visible
 * @{
 * ui_block
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_block_content_visible
 * @{
 * <tr>
 * <td>ui_block_content_visible</td>
 * <td>ui_block_content_visible_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Created window.
 * @step 3 block created
 *
 * @procedure
 * @step 1 Call ui_block_content_visible(block, EINA_FALSE)
 * @step 2 Call ui_block_content_visible(block, EINA_TRUE)
 * @step 3 Call ui_block_content_visible(block, EINA_TRUE)
 * </td>
 * <td>Evas_Object *block, Eina_Bool</td>
 * <td>all checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_block_content_visible_p)
{
   Evas_Object *win, *block;

   elm_init(0, 0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   block = ui_block_add(win);
   ck_assert_msg(ui_block_content_visible(block, EINA_TRUE), "Cannot set visibility\n");
   ck_assert_msg(ui_block_content_visible(block, EINA_FALSE), "Cannot set visibility\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_content_visible
 * @{
 * <tr>
 * <td>ui_block_content_visible</td>
 * <td>ui_block_content_visible_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call ui_block_content_visible(NULL, EINA_FALSE)
 * @step 2 Call ui_block_content_visible(NULL, EINA_TRUE)
 * </td>
 * <td>NULL, EINA_TRUE</td>
 * <td>all checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_block_content_visible_n)
{
   elm_init(0, 0);
   ck_assert_msg(!ui_block_content_visible(NULL, EINA_TRUE), "Set visible to not existing object\n");
   ck_assert_msg(!ui_block_content_visible(NULL, EINA_FALSE), "Set visible to not existing object\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_content_visible
 * @{
 * </TABLE>
 * @}
 * @}
 */
