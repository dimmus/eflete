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
 * @addtogroup pm_project_meta_data_get
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_project_meta_data_get
 * @{
 * <tr>
 * <td>pm_project_meta_data_get</td>
 * <td>pm_project_meta_data_get_test_p</td>
 * <td>
 * This test check the open project procedure.<br>
 * @paramrecondition
 * @step 1 initialized elm;
 * @step 2 initialized eflete, need for logger.
 * @step 3 import radio.edj, so create a new project.
 *
 * @paramrocedure
 * @step 1 Call pm_project_meta_data_get;
 * @step 2 Check returned value.
 * </td>
 * <td>(Project *) pro, NULL, NULL, NULL, (char *)"license", NULL</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
static void
_test_end_cb(void *data __UNUSED__,
             PM_Project_Result result __UNUSED__)
{
   ecore_main_loop_quit();
}

EFLETE_TEST (pm_project_meta_data_get_test_p)
{
   Project *pro;
   Eina_Stringshare *license;

   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");

   pm_project_import_edj("UTC", ".", "./edj_build/radio.edj",
                         NULL, NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();

   pro = pm_project_thread_project_get();
   if (!pro)
     ck_abort_msg("Project thread not returned the Project. Maybe thread not finished yet.");

   pm_project_meta_data_get(pro, NULL, NULL, NULL, &license, NULL);
   ck_assert_msg(license != NULL, "Meta data is missing in the project.");

   pm_project_close(pro);
   ecore_file_recursive_rm("./UTC");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_project_meta_data_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
