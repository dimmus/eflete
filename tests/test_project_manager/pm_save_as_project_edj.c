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
 * @addtogroup pm_save_as_project_edj
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_save_as_project_edj
 * @{
 * <tr>
 * <td>pm_save_as_project_edj</td>
 * <td>pm_save_as_project_edj_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Open edj project.
 *
 * @procedure
 * @step 1 Call function pm_save_as_project_edj(project, "./edj_build/NEWpm_save_as_project_edj.edj")
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project, (const char *)dest = "./edj_build/NEWpm_save_as_project_edj.edj"</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_save_as_project_edj_test_p)
{
   elm_init(0,0);
   char *name, *path, *dest;
   name = "radio_test";
   path = "./edj_build/pm_save_as_project_edj.edj";
   dest = "./edj_build/NEWpm_save_as_project_edj.edj";
   Project* pro = pm_open_project_edj(name, path);

   ck_assert_msg(pm_save_as_project_edj(pro, dest) == EINA_TRUE, "Can't save project to edj");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup pm_save_as_project_edj
 * @{
 * <tr>
 * <td>pm_save_as_project_edj</td>
 * <td>pm_save_as_project_edj_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Open edj project.
 *
 * @procedure
 * @step 1 Call function pm_save_as_project_edj(project, NULL)
 * @step 2 Check returned value.
 * @step 3 Call function pm_save_as_project_edj(NULL, "./edj_build/NEWpm_save_as_project_edj.edj")
 * @step 4 Check returned value.
 * @step 5 Call function pm_save_as_project_edj(NULL, NULL)
 * @step 6 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_save_as_project_edj_test_n)
{
   elm_init(0,0);
   const char *name, *path, *dest;
   name = "radio_test";
   path = "./edj_build/pm_save_as_project_edj.edj";
   dest = "./edj_build/NEWpm_save_as_project_edj.edj";
   Project* pro = pm_open_project_edj(name, path);

   ck_assert_msg(pm_save_as_project_edj(pro, NULL) == EINA_FALSE, "Project was saved without path to file");
   ck_assert_msg(pm_save_as_project_edj(NULL, dest) == EINA_FALSE, "Saved NULL project");
   ck_assert_msg(pm_save_as_project_edj(NULL, NULL) == EINA_FALSE, "Saved NULL project");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_as_project_edj
 * @{
 * </TABLE>
 * @}
 * @}
 */
