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
 * @addtogroup pm_project_import_edc
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

Eina_Bool res;

/**
 * @addtogroup pm_project_import_edc
 * @{
 * <tr>
 * <td>pm_project_import_edc</td>
 * <td>pm_project_import_edc_test_p1</td>
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
 * </tr>
 * @}
 */
static void
_test_end_p1_cb(void *data __UNUSED__,
                PM_Project_Result result __UNUSED__,
                Eina_List *widgets __UNUSED__)
{
   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_import_edc_test_p1)
{
   Project *pro;

   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   pm_project_import_edc("UTC", ".", "./edj_build/radio.edc",
                         "-id ./edj_build -fd ./edj_build -sd ./edj_build",
                         NULL, _test_end_p1_cb, NULL);
   ecore_main_loop_begin();

   pro = pm_project_thread_project_get();
   pm_project_close(pro);
   ecore_file_recursive_rm("./UTC");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_import_edc
 * @{
 * <tr>
 * <td>pm_project_import_edc</td>
 * <td>pm_project_import_edc_test_p2</td>
 * <td>
 * This test check that all specific file are created.
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
 * NULL, "_test_end_p2_cb", NULL </td>
 * <td>Specific files must be created</td>
 * </tr>
 * @}
 */
static void
_test_end_p2_cb(void *data __UNUSED__,
                PM_Project_Result result __UNUSED__,
                Eina_List *widgets __UNUSED__)
{
   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_import_edc_test_p2)
{
   Project *pro;
   //Eina_Bool files_is = EINA_FALSE;
   //Eet_File *ef;

   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   pm_project_import_edc("UTC", ".", "./edj_build/radio.edc",
                         "-id ./edj_build -fd ./edj_build -sd ./edj_build",
                         NULL, _test_end_p2_cb, NULL);
   ecore_main_loop_begin();

   /*
   ef = eet_open("./UTC/UTC.pro", EET_FILE_MODE_READ);
   if (ef) files_is = EINA_TRUE;
   eet_close(ef);
   ef = eet_open("./UTC/UTC.dev", EET_FILE_MODE_READ);
   if (ef) files_is &= EINA_TRUE;
   eet_close(ef);
   ck_assert_msg(files_is != EINA_FALSE, "Specific project file not created.");
   */

   pro = pm_project_thread_project_get();
   pm_project_close(pro);
   ecore_file_recursive_rm("./UTC");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_import_edc
 * @{
 * <tr>
 * <td>pm_project_import_edc</td>
 * <td>pm_project_import_edc_test_p3</td>
 * <td>
 * This test check the progress message from import edc thread.
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
 * "_test_progress_cb", NULL, NULL </td>
 * <td>Specific files must be created</td>
 * </tr>
 * @}
 */
static Eina_Bool
_test_progress_cb(void *data __UNUSED__,
                  Eina_Stringshare *message __UNUSED__)
{
   res = EINA_TRUE;

   return EINA_TRUE;
}

EFLETE_TEST (pm_project_import_edc_test_p3)
{
   Project *pro;

   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   res = EINA_FALSE;
   pm_project_import_edc("UTC", ".", "./edj_build/radio.edc",
                         "-id ./edj_build -fd ./edj_build -sd ./edj_build",
                         _test_progress_cb, _test_end_p2_cb, NULL);
   ecore_main_loop_begin();

   ck_assert_msg(res, "Progress callback did't called!");

   pro = pm_project_thread_project_get();
   pm_project_close(pro);
   ecore_file_recursive_rm("./UTC");

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
