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
#include "editor.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_project_save
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_project_save
 * @{
 * <tr>
 * <td>pm_project_save</td>
 * <td>pm_project_save_test_p</td>
 * <td>
 * This test check the save project procedure.<br>
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 * @step 3 import test_project_manager, so create a new project.
 * @step 4 open the imported project.
 * @step 5 set to all parts of all group min_x value 10;
 *
 * @procedure
 * @step 1 Call pm_project_save;
 * @step 2 Check returned value.
 * </td>
 * <td>(Project *)project</td>
 * <td>Project must be saved</td>
 * </tr>
 * @}
 */
static void
_test_end_cb(void *data __UNUSED__,
             PM_Project_Result result __UNUSED__)
{
   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_save_test_p)
{
   Project *pro;

   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   pm_project_import_edj("UTC", ".", "./edj_build/test_project_manager.edj",
                         NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();

   pro = pm_project_thread_project_get();
   pm_project_thread_free();

   pm_project_save(pro, NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();
   pm_project_thread_free();

   pm_project_close(pro);

   ck_assert_msg(ecore_file_exists("./UTC/UTC.edj"), "Project does't saved.");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_save
 * @{
 * </TABLE>
 * @}
 * @}
 */
