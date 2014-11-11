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
 * @addtogroup pm_project_import_edc
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_project_import_edc
 * @{
 * <tr>
 * <td>pm_project_import_edc</td>
 * <td>pm_project_import_edc_test_p</td>
 * <td>
 * This test check that the thread is ran.
 * @precondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 *
 * @procedure
 * @step 1 Call pm_project_import_edc;
 * @step 2 Check returned value.
 * </td>
 * <td>(char *)"UTC", (char *)".", (char *)"radio.edc",
 * (char *)"-id ./edj_build/ -fd ./edj_build/fnt -sd ./edj_build/snd",
 * NULL, NULL, NULL </td>
 * <td>Project_Thead object must be created</td>
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

EFLETE_TEST (pm_project_import_edc_test_p)
{
   Project_Thread *thread;

   elm_init(0,0);
   app_init();

   thread = pm_project_import_edc("UTC", ".", "./edj_build/radio.edc",
                                  "-id ./edj_build/ -fd ./edj_build/fnt -sd ./edj_build/snd",
                                  NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();

   ck_assert_msg(thread != NULL, "Thread for import radio.edc to new project not started!");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_import_edc
 * @{
 * </TABLE>
 * @}
 * @}
 */
