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
 * @addtogroup ui_block_title_text
 * @{
 * ui_block
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ui_block_title_text
 * @{
 * <tr>
 * <td>ui_block_title_text</td>
 * <td>ui_block_title_text_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Created window.
 * @step 3 Create ui_block
 *
 * @procedure
 * @step 1 Call ui_block_title_text_set(block, "test1")
 * @step 2 Call ui_block_title_text_get(block)
 * @step 3 Call ui_block_title_text_set(block, "some say")
 * @step 4 Call ui_block_title_text_get(block)
 * </td>
 * <td>Evas_Object *block, const char *title</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_block_title_text_p)
{
   elm_init(0, 0);
   Evas_Object *parent, *block;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   block = ui_block_add(parent);
   ui_block_title_text_set(block,"test1");
   ck_assert_msg(ui_block_title_text_set(block, "test1"), "cannot save title to block.");
   const char *str = ui_block_title_text_get(block);
   ck_assert_msg(!strcmp(str, "test1"), "title mismatch");
   ui_block_title_text_set(block, "some say");
   str = ui_block_title_text_get(block);
   ck_assert_msg(!strcmp(str,"some say"), "title mismatch");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_title_text
 * @{
 * <tr>
 * <td>ui_block_title_text</td>
 * <td>ui_block_title_text_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call ui_block_title_text_set(NULL, "test1")
 * @step 2 Call ui_block_title_text_get(NULL)
 * </td>
 * <td>NULL, const char *title</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (ui_block_title_text_n)
{
   elm_init(0, 0);
   ck_assert_msg(!ui_block_title_text_set(NULL, "test1"), "Successfully set title to unexistend block");
   ck_assert_msg(!ui_block_title_text_get(NULL), "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_title_text
 * @{
 * </TABLE>
 * @}
 * @}
 */
