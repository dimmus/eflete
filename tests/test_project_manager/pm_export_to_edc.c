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
 * @addtogroup pm_export_to_edc
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

static int cb_flag = 0;

static void log_decc_cb (time_t time __UNUSED__,
                         Eina_Stringshare* msg,
                         int type __UNUSED__)
{
   if (strstr(msg, "edje_decc:"))
      cb_flag = 1;
}


/**
 * @addtogroup pm_export_to_edc
 * @{
 * <tr>
 * <td>pm_export_to_edc</td>
 * <td>pm_export_to_edc_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 open edj project
 *
 * @procedure
 * @step 1 Call function pm_export_to_edc(project, dest, NULL).
 * @step 2 Check returned value.
 * </td>
 * <td>Project *project, (const char *)dest = "./edj_build/RADIO_DECC", NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_export_to_edc_test_p1)
{
   elm_init(0,0);
   char *name, *path, *dest;
   name = "radio_test";
   path = "./edj_build/pm_export_to_edc.edj";
   dest = "./edj_build/RADIO_DECC";
   Project* pro = pm_open_project_edj(name, path);

   ck_assert_msg(pm_export_to_edc(pro, dest, NULL) == EINA_TRUE, "something is wrong");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_export_to_edc
 * @{
 * <tr>
 * <td>pm_export_to_edc</td>
 * <td>pm_export_to_edc_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 open edj project
 * @step 3 callback function that checks log for "edje_decc:" line
 *
 * @procedure
 * @step 1 Call function pm_export_to_edc(project, dest, log_decc_cb).
 * @step 2 Check returned value.
 * @step 3 Check callback function.
 * </td>
 * <td>Project *project, (const char *)dest = "./edj_build/RADIO_DECC", Edje_Compile_Log_Cb log_decc_cb</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_export_to_edc_test_p2)
{
   elm_init(0,0);
   char *name, *path, *dest;
   name = "radio_test";
   path = "./edj_build/pm_export_to_edc.edj";
   dest = "./edj_build/RADIO_DECC";
   Project* pro = pm_open_project_edj(name, path);

   ck_assert_msg(pm_export_to_edc(pro, dest, log_decc_cb) == EINA_TRUE, "something is wrong");
   ck_assert_msg(cb_flag == 1, "Wrong callback output");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_export_to_edc
 * @{
 * <tr>
 * <td>pm_export_to_edc</td>
 * <td>pm_export_to_edc_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 open edj project
 *
 * @procedure
 * @step 1 Call function pm_export_to_edc(project, NULL, log_decc_cb).
 * @step 2 Check returned value.
 * @step 3 Call function pm_export_to_edc(NULL, NULL, log_decc_cb).
 * @step 4 Check returned value.
 * @step 5 Call function pm_export_to_edc(NULL, NULL, NULL).
 * @step 6 Check returned value.
 * </td>
 * <td>NULL, NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_export_to_edc_test_n)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/pm_export_to_edc.edj";
   Project* pro = pm_open_project_edj(name, path);

   ck_assert_msg(pm_export_to_edc(pro, NULL, log_decc_cb) == EINA_FALSE, "something is wrong");
   ck_assert_msg(pm_export_to_edc(NULL, NULL, log_decc_cb) == EINA_FALSE, "something is wrong");
   ck_assert_msg(pm_export_to_edc(NULL, NULL, NULL) == EINA_FALSE, "something is wrong");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup pm_export_to_edc
 * @{
 * </TABLE>
 * @}
 * @}
 */
