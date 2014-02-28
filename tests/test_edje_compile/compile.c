/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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
 * @addtogroup compile
 * @{
 * edje_compile
 * <TABLE>
 * @}
 */

static int cb_flag = 0;

static void log_cb (time_t time __UNUSED__,
                    Eina_Stringshare* msg,
                    int type)
{
   /* on succesfull compilation "Summary:" should be outputed to stdout */
   if (!strcmp(msg, "Summary:") && (type == 13)) /* 13 - stdout id in elementary */
      cb_flag = 1;
}
/**
 * @addtogroup compile
 * @{
 * <tr>
 * <td>compile</td>
 * <td>compile_callback_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edc file and resources
 * @step 3 callback function that checks log for "Summary:" line in stdout
 *
 * @procedure
 * @step 1 If output file already exists remove it
 * @step 2 Call compile()
 * @step 3 Check that return code is 0
 * @step 4 Check that callback function found "Summary:" in output
 * @step 5 Check that output file exists
 * </td>
 * <td>char *edc, char *edj, char *img, char *font, char *sound, Edje_Compile_Log_Cb log_callback</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (compile_callback_test_p)
{
   elm_init(0,0);
   const char *edc, *edj, *img, *font, *sound;
   edc = "./edj_build/radio.edc";
   edj = "./edj_build/radio_COMP.edj";
   img = "./edj_build/";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
   cb_flag = 0;
   if (ecore_file_exists(edj))
      ecore_file_remove(edj);
M_ int exit_code = compile (edc, edj, img, font, sound, log_cb);
   ck_assert_msg(exit_code == 0, "Exit code is not 0");
   ck_assert_msg(cb_flag == 1, "Wrong callback output");
M_ ck_assert_msg(ecore_file_exists(edj), "Output file doesn't exist");
}
END_TEST

/**
 * @addtogroup compile
 * @{
 * <tr>
 * <td>compile</td>
 * <td>compile_callback_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edc file
 * @step 3 no resources specified
 * @step 4 callback function that checks log for "Summary:" line in stdout
 *
 * @procedure
 * @step 1 Call compile()
 * @step 2 Check that return code is not 0
 * @step 3 Check that callback function found no "Summary:" in output
 * </td>
 * <td>char *edc, char *edj, NULL, NULL, NULL, Edje_Compile_Log_Cb log_cb</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (compile_callback_test_n)
{
   elm_init(0,0);
   const char *edc, *edj;
   edc = "./edj_build/radio.edc";
   edj = "./edj_build/radio_COMP.edj";
   cb_flag = 0;
M_ int exit_code = compile (edc, edj, NULL, NULL, NULL, log_cb);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
   ck_assert_msg(cb_flag == 0, "Wrong callback output");
}
END_TEST

/**
 * @addtogroup compile
 * @{
 * <tr>
 * <td>compile</td>
 * <td>compile_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edc file and resources
 *
 * @procedure
 * @step 1 If output file already exists remove it
 * @step 2 Call compile()
 * @step 3 Check that return code is 0
 * @step 4 Check that output file exists
 * </td>
 * <td>char *edc, char *edj, char *img, char *font, char *sound, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (compile_test_p)
{
   elm_init(0,0);
   const char *edc, *edj, *img, *font, *sound;
   edc = "./edj_build/radio.edc";
   edj = "./edj_build/radio_COMP.edj";
   img = "./edj_build/";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
   if (ecore_file_exists(edj))
      ecore_file_remove(edj);
M_ int exit_code = compile (edc, edj, img, font, sound, NULL);
   ck_assert_msg(exit_code == 0, "Exit code is not 0");
M_ ck_assert_msg(ecore_file_exists(edj), "Output file doesn't exist");
}
END_TEST

/**
 * @addtogroup compile
 * @{
 * <tr>
 * <td>compile</td>
 * <td>compile_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edc file
 * @step 3 no resources specified
 *
 * @procedure
 * @step 1 Call compile() with NULL pointer to fonts/sounds/images dirs
 * @step 2 Check that return code is not 0
 * @step 3 Call compile() with "" as fonts/sounds/images dirs
 * @step 4 Check that return code is not 0
 * </td>
 * <td>char *edc, char *edj, NULL, NULL, NULL, NULL;
       char *edc, char *edj, "", "", "", NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (compile_test_n1)
{
   elm_init(0,0);
   const char *edc, *edj;
   edc = "./edj_build/radio.edc";
   edj = "./edj_build/radio_COMP.edj";
M_ int exit_code = compile (edc, edj, NULL, NULL, NULL, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
M_ exit_code = compile (edc, edj, "", "", "", NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
}
END_TEST

/**
 * @addtogroup compile
 * @{
 * <tr>
 * <td>compile</td>
 * <td>compile_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edc file and resources
 *
 * @procedure
 * @step 1 Call compile() with NULL pointer to output file
 * @step 2 Check that return code is not 0
 * @step 3 Call compile() with "" as output file
 * @step 4 Check that return code is not 0
 * </td>
 * <td>char *edc, NULL, char *img, char *font, char *sound, NULL;
       char *edc, "", char *img, char *font, char *sound, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (compile_test_n2)
{
   elm_init(0,0);
   const char *edc, *img, *font, *sound;
   edc = "./edj_build/radio.edc";
   img = "./edj_build/";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
M_ int exit_code = compile (edc, NULL, img, font, sound, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
M_ exit_code = compile (edc, "", img, font, sound, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
}
END_TEST

/**
 * @addtogroup compile
 * @{
 * <tr>
 * <td>compile</td>
 * <td>compile_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 correct input edc file and resources
 *
 * @procedure
 * @step 1 Call compile() with NULL pointer to input file
 * @step 2 Check that return code is not 0
 * @step 3 Call compile() with "" as input file
 * @step 4 Check that return code is not 0
 * </td>
 * <td>NULL, char *edj, char *img, char *font, char *sound, NULL;
       "", char *edj, char *img, char *font, char *sound, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (compile_test_n3)
{
   elm_init(0,0);
   const char *edj, *img, *font, *sound;
   edj = "./edj_build/radio_COMP.edj";
   img = "./edj_build/";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
M_ int exit_code = compile (NULL, edj, img, font, sound, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
M_ exit_code = compile ("", edj, img, font, sound, NULL);
   ck_assert_msg(exit_code != 0, "Exit code is 0");
}
END_TEST

/**
 * @addtogroup compile
 * @{
 * </TABLE>
 * @}
 * @}
 */
