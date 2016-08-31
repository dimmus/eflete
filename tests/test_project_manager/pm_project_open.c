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

#include "test_project_manager.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_project_open
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

static Project *pro;

/**
 * @addtogroup pm_project_open
 * @{
 * <tr>
 * <td>pm_project_open</td>
 * <td>pm_project_open_test_p</td>
 * <td>
 * This test check the open project procedure.<br>
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 * @step 3 import test_project_manager, so create a new project.
 *
 * @procedure
 * @step 1 Call pm_project_open;
 * @step 2 Check returned value.
 * </td>
 * <td>(char *)"./UTC/UTC.pro"</td>
 * <td>Project object</td>
 * </tr>
 * @}
 */
static void
_test_end_cb(void *data __UNUSED__,
             PM_Project_Result result __UNUSED__,
             Project *project)
{
   pro = project;
   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_open_test_p)
{
   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   pro = NULL;

   pm_project_import_edj("UTC", ".", "./edj_build/test_project_manager.edj",
                         NULL, NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();

   ck_assert_msg(pro != NULL, "Project does't opened.");
   pm_project_close(pro);
   pro = NULL;

   pm_project_open("./UTC/UTC.pro", NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();
   ck_assert_msg(pro != NULL, "Project does't opened.");
   pm_project_close(pro);
   ecore_file_recursive_rm("./UTC");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_open
 * @{
 * </TABLE>
 * @}
 * @}
 */
