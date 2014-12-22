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

#include "test_sound_editor.h"

/**
 * @addtogroup sound_editor_test
 * @{
 * @addtogroup sound_editor_added_sounds_free
 * @{
 * sound_editor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup sound_editor_added_sounds_free
 * @{
 * <tr>
 * <td>sound_editor_added_sounds_free</td>
 * <td>sound_editor_added_sounds_free_test_n</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 *
 * @procedure
 * @step 1 Call sound_editor_added_sounds()
 * @step 2 Check if there is no segmentation fault
 * </td>
 * <td>NULL</td>
 * <td>there is no segmentation fault</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(sound_editor_added_sounds_free_test_n)
{
   elm_init(0,0);
   Eina_List *sounds = NULL;

   sound_editor_added_sounds_free(sounds);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup sound_editor_added_sounds_free
 * @{
 * </TABLE>
 * @}
 * @}
 */
