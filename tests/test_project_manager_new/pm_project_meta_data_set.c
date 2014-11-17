/*
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

#include "test_project_manager_new.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_project_meta_data_set
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_project_meta_data_set
 * @{
 * <tr>
 * <td>pm_project_meta_data_set</td>
 * <td>pm_project_meta_data_set_test_p</td>
 * <td>
 * This test check the open project procedure.<br>
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 * @step 3 import pm_open_project_edj, so create a new project.
 *
 * @procedure
 * @step 1 Call pm_project_meta_data_set;
 * @step 2 Check returned value.
 * </td>
 * <td>(Project *) pro, (char *)"UTC", (char *)"authors", (char *)"version",
 * (char *)"license", (char *)"comment"</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
static void
_test_end_cb(void *data __UNUSED__,
             PM_Project_Result result __UNUSED__)
{
   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_meta_data_set_test_p)
{
   Project_Thread *thread;
   Project *pro;
   Eina_Bool ret;

   elm_init(0,0);
   app_init();

   thread = pm_project_import_edj("UTC", ".", "./edj_build/pm_open_project_edj.edj",
                                  NULL, _test_end_cb, NULL);
   if (!thread)
     ck_abort_msg("Project thread is not runned!");
   ecore_main_loop_begin();

   pro = pm_project_thread_project_get(thread);
   if (!pro)
     ck_abort_msg("Project thread not returned the Project. Maybe thread not finished yet.");

   ret = pm_project_meta_data_set(pro, "UTC", "authors", "version", "license", "comment");
   ck_assert_msg(ret, "Meta data not setted to project.");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_meta_data_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
