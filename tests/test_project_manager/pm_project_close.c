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

#include "test_project_manager.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_project_close
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_project_close
 * @{
 * <tr>
 * <td>pm_project_close</td>
 * <td>pm_project_close_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elm.
 * @step 2 Open edj project.
 *
 * @procedure
 * @step 1 Call function pm_project_close(project).
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_project_close_test_p1)
{
   elm_init(0,0);
   char *path;
   path = "./edj_build/pm_project_close.edj";
   Project* pro = pm_open_project_edj(path);

   ck_assert_msg(pm_project_close(pro) == EINA_TRUE, "Can't close project!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_close
 * @{
 * <tr>
 * <td>pm_project_close</td>
 * <td>pm_project_close_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elm.
 * @step 2 Open edj project.
 * @step 3 Get style for modifying.
 * @step 4 Set style flag into 'modified'
 * @step 5 Save project into swap file
 * @step 6 Save project
 *
 * @procedure
 * @step 1 Call function pm_project_close(project).
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_project_close_test_p2)
{
   elm_init(0,0);
   char *path;
   path = "./edj_build/pm_project_close.edj";
   Project* pro = pm_open_project_edj(path);
   Style *style = wm_style_object_find(pro->widgets, "elm/radio/base/def");
   style->isModify = true;
   pm_save_project_to_swap(pro);
   pm_save_project_edj(pro);

   ck_assert_msg(pm_project_close(pro) == EINA_TRUE, "Can't close project!");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_close
 * @{
 * <tr>
 * <td>pm_project_close</td>
 * <td>pm_project_close_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize elm.
 *
 * @procedure
 * @step 1 Call function pm_project_close(project).
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_project_close_test_n)
{
   elm_init(0,0);

   ck_assert_msg(pm_project_close(NULL) == EINA_FALSE, "NULL project deleted");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_close
 * @{
 * </TABLE>
 * @}
 * @}
 */
