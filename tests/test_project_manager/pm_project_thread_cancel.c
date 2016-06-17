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
 * @addtogroup pm_project_thread_cancel
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

static Eina_Bool res;

/**
 * @addtogroup pm_project_thread_cancel
 * @{
 * <tr>
 * <td>pm_project_thread_cancel</td>
 * <td>pm_project_thread_cancel_test_p</td>
 * <td>
 * This test check that the thread is ran.
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 * @step 3 start a some Project thread
 *
 * @procedure
 * @step 1 Call pm_project_thread_cancel;
 * @step 2 Check returned value.
 * </td>
 * <td>(Project_Thread *)worker</td>
 * <td>The and func must be called with param PM_PROJECT_CANCEL</td>
 * </tr>
 * @}
 */
static void
_test_end_cb(void *data __UNUSED__,
             PM_Project_Result result,
             Eina_List *widgets __UNUSED__)
{
   if (result == PM_PROJECT_CANCEL)
     res = EINA_TRUE;

   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_thread_cancel_test_p)
{
   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   res = EINA_FALSE;
   pm_project_import_edj("UTC", ".", "./edj_build/test_project_manager_thread_cancel.edj",
                         NULL, NULL, _test_end_cb, NULL);
   pm_project_thread_cancel();
   ecore_main_loop_begin();

   ck_assert_msg(res, "Project thread did't canceled!");

   ecore_file_recursive_rm("./UTC");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_thread_cancel
 * @{
 * </TABLE>
 * @}
 * @}
 */
