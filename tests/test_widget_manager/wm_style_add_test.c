/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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
 * @addtogroup test_widget_manager
 * @{
 * @addtogroup wm_style_add
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_style_add
 * @{
 * <tr>
 * <td>wm_style_add</td>
 * <td>wm_style_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_add("testclass", "eflete/testclass/testgroup").
 * @step 2 Check returned Style.
 * @step 3 Check Style's full name.
 * </td>
 * <td>char *style_name = "testclass"; char *full_style_name = "eflete/testclass/testgroup"</td>
 * <td>All check's passed.</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_add_test_p)
{
   elm_init(0,0);
   const char *style_name = "testclass";
   const char *full_style_name = "eflete/testclass/testgroup";
   Style *style = NULL;

   style = wm_style_add(style_name, full_style_name);
   ck_assert_msg(style != NULL, "cannot create new Style object");
   ck_assert_str_eq(style->full_group_name, full_style_name);

   wm_style_free(style);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add
 * @{
 * <tr>
 * <td>wm_style_add</td>
 * <td>wm_style_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_add(NULL, "eflete/testclass/testgroup").
 * @step 2 Check returned value.
 * </td>
 * <td>char *style_name = NULL; char *full_style_name = "eflete/testclass/testgroup"</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_add_test_n1)
{
   elm_init(0,0);
   const char *full_style_name = "eflete/testclass/testgroup";
   ck_assert_msg(wm_style_add(NULL, full_style_name) == NULL, "Style object was created with NULL parameter.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add
 * @{
 * <tr>
 * <td>wm_style_add</td>
 * <td>wm_style_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_add("testclass", NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>char *style_name = "testclass"; char *full_style_name = NULL</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_add_test_n2)
{
   elm_init(0,0);
   const char *style_name = "testclass";
   ck_assert_msg(wm_style_add(style_name, NULL) == NULL, "Style object was created with NULL parameter.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add
 * @{
 * <tr>
 * <td>wm_style_add</td>
 * <td>wm_style_add_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function wm_style_add(NULL, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>char *style_name = NULL; char *full_style_name = NULL</td>
 * <td>NULL object pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (wm_style_add_test_n3)
{
   elm_init(0,0);
   ck_assert_msg(wm_style_add(NULL, NULL) == NULL, "Style object was created with NULL parameters.");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_style_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
