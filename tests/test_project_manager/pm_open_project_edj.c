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

#include "test_project_manager.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_open_project_edj
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_open_project_edj
 * @{
 * <tr>
 * <td>pm_open_project_edj</td>
 * <td>pm_open_project_edj_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function pm_open_project_edj(name, path).
 * @step 2 Check returned value.
 * @step 3 Check project's name.
 * @step 4 Check edc path.
 * @step 5 Check edj path.
 * @step 6 Check swap path.
 * @step 7 Check image directory.
 * @step 8 Check font directory.
 * @step 9 Check sound directory.
 * </td>
 * <td>(const char *)name = "radio_test", (const char *)path = "./edj_build/pm_open_project_edj.edj"</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_open_project_edj_test_p)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/pm_open_project_edj.edj";

   Project* pro = pm_open_project_edj(name, path);
   ck_assert_msg(pro != NULL, "failure: cannot open project from edj-file");
   ck_assert_msg(!strcmp(pro->name, "radio_test"), "project name is incorrect");
   ck_assert_msg(!strcmp(pro->edc, "./edj_build/pm_open_project_edj.edc"), "edc path is incorrect");
   ck_assert_msg(!strcmp(pro->edj, "./edj_build/pm_open_project_edj.edj"), "edj path is incorrect");
   ck_assert_msg(!strcmp(pro->swapfile, "./edj_build/pm_open_project_edj.edj.swap"), "swapfile path is incorrect");
   ck_assert_msg(pro->image_directory == NULL, "image dir is incorrect");
   ck_assert_msg(pro->font_directory == NULL, "font dir is incorrect");
   ck_assert_msg(pro->sound_directory == NULL, "sound dir is incorrect");

   pm_project_close(pro);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup pm_open_project_edj
 * @{
 * <tr>
 * <td>pm_open_project_edj</td>
 * <td>pm_open_project_edj_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function pm_open_project_edj(NULL, NULL).
 * @step 2 Check returned value.
 * @step 3 Call function pm_open_project_edj(name, NULL).
 * @step 4 Check returned value.
 * @step 5 Call function pm_open_project_edj(NULL, path).
 * @step 6 Check returned value.
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_open_project_edj_test_n1)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/pm_open_project_edj.edj";

   ck_assert_msg(pm_open_project_edj(NULL, NULL) == NULL, "Project created without path");
   ck_assert_msg(pm_open_project_edj(name, NULL) == NULL, "Project created without path");
   ck_assert_msg(pm_open_project_edj(NULL, path) == NULL, "Project created without name");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edj
 * @{
 * <tr>
 * <td>pm_open_project_edj</td>
 * <td>pm_open_project_edj_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 *
 * @procedure
 * @step 1 Call function pm_open_project_edj(name, path1).
 * @step 2 Check returned value.
 * @step 3 Call function pm_open_project_edj(name, path2).
 * @step 4 Check returned value.
 * @step 5 Call function pm_open_project_edj(name, path3).
 * @step 6 Check returned value.
 * </td>
 * <td>(const char *)name = "radio_test", (const char *)path</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (pm_open_project_edj_test_n2)
{
   elm_init(0,0);
   char *name, *path1, *path2, *path3;
   name = "radio_test";
   path1 = "_totally_wrong_path_project_manager_probably_going_to_ignore! >;3";
   path2 = "./edj_build/Makefile";
   path3 = "./edj_build/radio.png";

   ck_assert_msg(pm_open_project_edj(name, path1) == NULL, "Project created with path that is not exist.");
   ck_assert_msg(pm_open_project_edj(name, path2) == NULL, "Project created with file that doesn't have any extensions");
   ck_assert_msg(pm_open_project_edj(name, path3) == NULL, "Project created with wrong file");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edj
 * @{
 * </TABLE>
 * @}
 * @}
 */
