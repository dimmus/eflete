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
 * @addtogroup pm_project_import_edj
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

static Eina_Bool res;

/**
 * @addtogroup pm_project_import_edj
 * @{
 * <tr>
 * <td>pm_project_import_edj</td>
 * <td>pm_project_import_edj_test_p</td>
 * <td>
 * This test check that the thread is ran.
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 *
 * @procedure
 * @step 1 Call pm_project_import_edj;
 * @step 2 Check returned value.
 * </td>
 * <td>(char *)"UTC", (char *)".", (char *)"./edj_build/pm_open_project_edj.edj",
 * NULL, NULL, NULL </td>
 * <td>Project_Thead object must be created</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_project_import_edj_test_p)
{
   Project_Thread *thread;

   elm_init(0,0);
   app_init();

   thread = pm_project_import_edj("UTC", ".", "./edj_build/pm_open_project_edj.edj", NULL, NULL, NULL);
   ck_assert_msg(thread != NULL, "Thread for import pm_open_project_edj.edj to new project not started!");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_import_edj
 * @{
 * <tr>
 * <td>pm_project_import_edj</td>
 * <td>pm_project_import_edj_test_p1</td>
 * <td>
 * This test check feedback from thread.
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 *
 * @procedure
 * @step 1 Call pm_project_import_edj;
 * @step 2 Check returned value.
 * </td>
 * <td>(char *)"UTC", (char *)".", (char *)"./edj_build/pm_open_project_edj.edj",
 * "_test_progress_cb", NULL, NULL </td>
 * <td>Static variable 'res' must be EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
static Eina_Bool
_test_progress_cb(void *data __UNUSED__,
                  Eina_Stringshare *message __UNUSED__)
{
   res = EINA_TRUE;

   ecore_main_loop_quit();
   return EINA_TRUE;
}

EFLETE_TEST (pm_project_import_edj_test_p1)
{
   Project_Thread *thread;

   elm_init(0,0);
   app_init();

   res = EINA_FALSE;
   thread = pm_project_import_edj("UTC", ".", "./edj_build/pm_open_project_edj.edj",
                                  _test_progress_cb, NULL, NULL);
   if (!thread)
     ck_abort_msg("Project thread is not runned!");
   ecore_main_loop_begin();
   ck_assert_msg(res, "Progress callback did't called!");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_import_edj
 * @{
 * <tr>
 * <td>pm_project_import_edj</td>
 * <td>pm_project_import_edj_test_p2</td>
 * <td>
 * This test check callback of end event from thread.
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 *
 * @procedure
 * @step 1 Call pm_project_import_edj;
 * @step 2 Check returned value.
 * </td>
 * <td>(char *)"UTC", (char *)".", (char *)"./edj_build/pm_open_project_edj.edj",
 * NULL, "_test_end_cb", NULL </td>
 * <td>Static variable 'res' must be EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
static void
_test_end_cb(void *data __UNUSED__,
             PM_Project_Result result)
{
   if (result == PM_PROJECT_SUCCESS)
     res = EINA_TRUE;

   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_import_edj_test_p2)
{
   Project_Thread *thread;

   elm_init(0,0);
   app_init();

   res = EINA_FALSE;
   thread = pm_project_import_edj("UTC", ".", "./edj_build/pm_open_project_edj.edj",
                                  NULL, _test_end_cb, NULL);
   if (!thread)
     ck_abort_msg("Project thread is not runned!");
   ecore_main_loop_begin();
   ck_assert_msg(res, "End callback did't called!");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_import_edj
 * @{
 * </TABLE>
 * @}
 * @}
 */
