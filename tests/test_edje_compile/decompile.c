/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "test_edje_compile.h"

/**
 * @addtogroup edje_compile_test
 * @{
 * @addtogroup decompile
 * @{
 * edje_compile
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
 * @addtogroup decompile
 * @{
 * <tr>
 * <td>decompile</td>
 * <td>decompile_callback_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edj file
 * @step 3 callback function that checks log for "edje_decc:" line
 *
 * @procedure
 * @step 1 If output dir exists remove it
 * @step 2 Call decompile()
 * @step 3 Check that return code is 0
 * @step 4 Check that callback function found "edje_decc:" in output
 * @step 5 Check that output directory exists
 * </td>
 * <td>char *edj, char *dest, Edje_Compile_Log_Cb log_decc_cb</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (decompile_callback_test_p)
{
   elm_init(0,0);
   const char *edj, *dest;
   edj = "./edj_build/decompile.edj";
   dest = "./edj_build/RADIO_DECC";
   cb_flag = 0;
   if (ecore_file_exists(dest))
      ecore_file_recursive_rm(dest);
M_ int exit_code = decompile (edj, dest, log_decc_cb);
   ck_assert_msg(exit_code == 0, "Exit code is not 0");
   ck_assert_msg(cb_flag == 1, "Wrong callback output");
   ck_assert_msg(ecore_file_exists(dest), "Output dir doesn't exist");
}
END_TEST

/**
 * @addtogroup decompile
 * @{
 * <tr>
 * <td>decompile</td>
 * <td>decompile_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edj file
 *
 * @procedure
 * @step 1 If output dir exists remove it
 * @step 2 Call decompile()
 * @step 3 Check that return code is 0
 * @step 4 Check that output directory exists
 * </td>
 * <td>char *edj, char *dest, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (decompile_test_p)
{
   elm_init(0,0);
   const char *edj, *dest;
   edj = "./edj_build/decompile.edj";
   dest = "./edj_build/RADIO_DECC";
   if (ecore_file_exists(dest))
      ecore_file_recursive_rm(dest);
M_ int exit_code = decompile (edj, dest, NULL);
   ck_assert_msg(exit_code == 0, "Exit code is not 0");
   ck_assert_msg(ecore_file_exists(dest), "Output dir doesn't exist");
}
END_TEST

/**
 * @addtogroup decompile
 * @{
 * <tr>
 * <td>decompile</td>
 * <td>decompile_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edj file
 *
 * @procedure
 * @step 1 Call decompile() with NULL pointer to output dir
 * @step 2 Check that return code is not 0
 * @step 3 Call decompile() with "" as output dir
 * @step 4 Check that return code is not 0
 * </td>
 * <td>char *edj, NULL, NULL; char *edj, "", NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (decompile_test_n1)
{
   elm_init(0,0);
   const char *edj;
   edj = "./edj_build/decompile.edj";
M_ int exit_code = decompile (edj, NULL, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
M_ exit_code = decompile (edj, "", NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
}
END_TEST

/**
 * @addtogroup decompile
 * @{
 * <tr>
 * <td>decompile</td>
 * <td>decompile_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call decompile() with NULL pointer to input file
 * @step 2 Check that return code is not 0
 * @step 3 Call decompile() with "" as input file
 * @step 4 Check that return code is not 0
 * </td>
 * <td>NULL, char *dest, NULL; "", char *dest, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (decompile_test_n2)
{
   elm_init(0,0);
   const char *dest;
   dest = "./edj_build/RADIO_DECC";
M_ int exit_code = decompile (NULL, dest, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
M_ exit_code = decompile ("", dest, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
}
END_TEST

/**
 * @addtogroup decompile
 * @{
 * </TABLE>
 * @}
 * @}
 */
