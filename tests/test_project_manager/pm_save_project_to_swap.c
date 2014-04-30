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

#include "test_project_manager.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_save_project_to_swap
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_save_project_to_swap
 * @{
 * <tr>
 * <td>pm_save_project_to_swap</td>
 * <td>pm_save_project_to_swap_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Open edj project.
 *
 * @procedure
 * @step 1 Call function pm_save_project_to_swap(project)
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_save_project_to_swap_test_p1)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/pm_save_project_to_swap.edj";
   Project* pro = pm_open_project_edj(name, path);

   ck_assert_msg(pm_save_project_to_swap(pro) == EINA_TRUE, "Can't save project to swap");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_to_swap
 * @{
 * <tr>
 * <td>pm_save_project_to_swap</td>
 * <td>pm_save_project_to_swap_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elm.
 * @step 2 Open edj project.
 * @step 3 Get style for modifying.
 * @step 4 Set style flag into 'modified'
 *
 * @procedure
 * @step 1 Call function pm_save_project_to_swap(project).
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_save_project_to_swap_test_p2)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/pm_project_close.edj";
   Project* pro = pm_open_project_edj(name, path);
   Style *style = wm_style_object_find(pro->widgets, "elm/radio/base/def");
   style->isModify = true;

   ck_assert_msg(pm_save_project_to_swap(pro) == EINA_TRUE, "Can't save project to swap");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup pm_save_project_to_swap
 * @{
 * <tr>
 * <td>pm_save_project_to_swap</td>
 * <td>pm_save_project_to_swap_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function pm_save_project_to_swap(NULL)
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_save_project_to_swap_test_n)
{
   elm_init(0,0);

   ck_assert_msg(pm_save_project_to_swap(NULL) == EINA_FALSE, "Saved NULL project");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_to_swap
 * @{
 * </TABLE>
 * @}
 * @}
 */
