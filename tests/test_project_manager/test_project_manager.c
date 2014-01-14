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

#include <check.h>
#include "project_manager.h"
#include "eflete.h"

#define M_ mark_point();

/**
 * @addtogroup pm_open_project_edc_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call pm_open_project_edc()
 * @step 2 Check returned pointer
 * @step 3 Check setted name, img, fnt, snd dirs, edc/edj paths
 *
 * @passcondition: Filled Project *returned
 * @}
 */
START_TEST (pm_open_project_edc_test_p1)
{
   elm_init(0,0);
   const char *name, *path, *img, *font, *sound;
   name = "radio_test";
   path = "./edj_build/radio.edc";
   img = "./edj_build/";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
   Project* pro = pm_open_project_edc(name, path, img, font, sound);
M_ ck_assert_msg(pro != NULL, "failure: cannot open project from edc-file");

M_ ck_assert_msg(!strcmp(pro->name, "radio_test"), "project name is incorrect");
M_ ck_assert_msg(!strcmp(pro->edc, "./edj_build/radio.edc"), "edc path is incorrect");
M_ ck_assert_msg(pro->edj == NULL, "edj path is not NULL");
M_ ck_assert_msg(!strcmp(pro->image_directory, "./edj_build/"), "image dir is incorrect");
M_ ck_assert_msg(!strcmp(pro->font_directory, "./edj_build/fnt"), "font dir is incorrect");
M_ ck_assert_msg(!strcmp(pro->sound_directory, "./edj_build/snd"), "sound dir is incorrect");

M_ pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edc_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 call pm_open_project_edc() without img, snd and fnt dirs
 * @step 2 check returned pointer
 * @step 3 check setted name, img, fnt, snd dirs, edc/edj paths
 *
 * @passcondition: Filled Project *returned
 * @}
 */
START_TEST (pm_open_project_edc_test_p2)
{
   elm_init(0,0);
   const char *name, *path;
   name = "radio_test";
   path = "./edj_build/radio.edc";
   Project* pro = pm_open_project_edc(name, path, NULL, NULL, NULL);
M_ ck_assert_msg(pro != NULL, "failure: cannot open project from edc-file");

M_ ck_assert_msg(!strcmp(pro->name, "radio_test"), "project name is incorrect");
M_ ck_assert_msg(!strcmp(pro->edc, "./edj_build/radio.edc"), "edc path is incorrect");
M_ ck_assert_msg(pro->edj == NULL, "edj path is not NULL");
M_ ck_assert_msg(pro->image_directory == NULL, "image dir is not NULL");
M_ ck_assert_msg(pro->font_directory == NULL, "font dir is not NULL");
M_ ck_assert_msg(pro->sound_directory == NULL, "sound dir is not NULL");

M_ pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edc_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 call pm_open_project_edc() with all params set to NULL
 * @step 2 check returned pointer
 * @step 3 call pm_open_project_edc() with all params set to NULL but name
 * @step 4 check returned pointer
 * @step 5 call pm_open_project_edc() with all params set to NULL but path
 * @step 6 check returned pointer
 *
 * @passcondition: NULL returned
 * @}
 */
START_TEST (pm_open_project_edc_test_n)
{
   elm_init(0,0);
   const char *name, *path;
   name = "radio_test";
   path = "./edj_build/radio.edc";
M_ ck_assert_msg(pm_open_project_edc(NULL, NULL, NULL, NULL, NULL) == NULL, "Not NULL returned");
M_ ck_assert_msg(pm_open_project_edc(name, NULL, NULL, NULL, NULL) == NULL, "Not NULL returned");
M_ ck_assert_msg(pm_open_project_edc(NULL, path, NULL, NULL, NULL) == NULL, "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_free_test
 * @{
 * @objective Positive test case
 *
 * @procedure
 * @step 1 Open Project
 * @step 2 Call pm_free()
 *
 * @passcondition: EINA_TRUE returned
 * @}
 */
START_TEST (pm_free_test_p)
{
   elm_init(0,0);
   Project *pro;
   const char *name, *path, *img, *font, *sound;
   name = "radio_test";
   path = "./edj_build/radio.edc";
   img = "./edj_build/img";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
M_ pro = pm_open_project_edc(name, path, img, font, sound);
M_ ck_assert_msg(pm_free(pro) == EINA_TRUE, "Can't delete project");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_free_test
 * @{
 * @objective Negative test case
 *
 * @procedure
 * @step 1 Call pm_free(NULL)
 *
 * @passcondition: EINA_FALSE returned
 * @}
 */
START_TEST (pm_free_test_n)
{
   elm_init(0,0);
   ck_assert_msg(pm_free(NULL) == EINA_FALSE, "NULL project deleted");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edj_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call pm_open_project_edj()
 * @step 2 Check returned pointer
 * @step 3 Check setted name, img, fnt, snd dirs, edc/edj paths
 *
 * @passcondition: Filled Project *returned
 * @}
 */
START_TEST (pm_open_project_edj_test_p)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/radio.edj";
M_ Project* pro = pm_open_project_edj(name, path);
M_ ck_assert_msg(pro != NULL, "failure: cannot open project from edj-file");

M_ ck_assert_msg(!strcmp(pro->name, "radio_test"), "project name is incorrect");
M_ ck_assert_msg(!strcmp(pro->edc, "./edj_build/radio.edc"), "edc path is incorrect");
M_ ck_assert_msg(!strcmp(pro->edj, "./edj_build/radio.edj"), "edj path is incorrect");
M_ ck_assert_msg(!strcmp(pro->swapfile, "./edj_build/radio.edj.swap"), "swapfile path is incorrect");
M_ ck_assert_msg(pro->image_directory == NULL, "image dir is incorrect");
M_ ck_assert_msg(pro->font_directory == NULL, "font dir is incorrect");
M_ ck_assert_msg(pro->sound_directory == NULL, "sound dir is incorrect");

M_ pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edj_test
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 call pm_open_project_edj() with all params set to NULL
 * @step 2 check returned pointer
 * @step 3 call pm_open_project_edj() with all params set to NULL but name
 * @step 4 check returned pointer
 * @step 5 call pm_open_project_edj() with all params set to NULL but path
 * @step 6 check returned pointer
 *
 * @passcondition: NULL returned
 * @}
 */
START_TEST (pm_open_project_edj_test_n)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/radio.edj";
M_ ck_assert_msg(pm_open_project_edj(NULL, NULL) == NULL, "Project created without path");
M_ ck_assert_msg(pm_open_project_edj(name, NULL) == NULL, "Project created without path");
M_ ck_assert_msg(pm_open_project_edj(NULL, path) == NULL, "Project created without name");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_edj_test
 * @{
 * @objective Positive test case
 *
 * @procedure
 * @step 1 Open project
 * @step 2 pm_save_project_edj()
 *
 * @passcondition: EINA_TRUE returned
 * @}
 */
START_TEST (pm_save_project_edj_test_p)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/radio.edj";
M_ Project* pro = pm_open_project_edj(name, path);
M_ ck_assert_msg(pm_save_project_edj(pro), "Can't save project to edj");
M_ pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_edj_test
 * @{
 * @objective Negative test case
 *
 * @procedure
 * @step 1 pm_save_project_edj(NULL)
 *
 * @passcondition: EINA_FALSE returned
 * @}
 */
START_TEST (pm_save_project_edj_test_n)
{
   elm_init(0,0);
   ck_assert_msg(!pm_save_project_edj(NULL), "Saved NULL project");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_edc_test
 * @{
 * @objective Positive test case
 *
 * @procedure
 * @step 1 Open project
 * @step 2 pm_save_project_edc()
 *
 * @passcondition: EINA_TRUE returned
 * @}
 */
START_TEST (pm_save_project_edc_test_p)
{
   elm_init(0,0);
   char *name, *path;
   name = "radio_test";
   path = "./edj_build/radio.edj";
M_ Project* pro = pm_open_project_edj(name, path);

M_ ck_assert_msg(pm_save_project_edc(pro), "Can't save project to edc");

M_ pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_edc_test
 * @{
 * @objective Negative test case
 *
 * @procedure
 * @step 1 pm_save_project_edc(NULL)
 *
 * @passcondition: EINA_FALSE returned
 * @}
 */
START_TEST (pm_save_project_edc_test_n1)
{
   elm_init(0,0);
   ck_assert_msg(!pm_save_project_edc(NULL), "Saved NULL project");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_save_project_edc_test
 * @{
 * @objective Positive test case
 *
 * @procedure
 * @step 1 Open EDC project
 * @step 2 pm_save_project_edc() without saving edj
 *
 * @passcondition: EINA_FALSE returned
 * @}
 */
START_TEST (pm_save_project_edc_test_n2)
{
   elm_init(0,0);
   Project *pro;
   const char *name, *path, *img, *font, *sound;
   name = "radio_test";
   path = "./edj_build/radio.edc";
   img = "./edj_build/img";
   font = "./edj_build/fnt";
   sound = "./edj_build/snd";
M_ pro = pm_open_project_edc(name, path, img, font, sound);
M_ ck_assert_msg(!pm_save_project_edc(pro), "Saved project without compiled EDJ");
M_ pm_free(pro);
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup test_suite
 * @{
 * @objective Creating above to the test case
 *
 * @procedure
 * @step 1 Create suite
 * @step 2 Create test case
 * @step 3 Add unit tests to the test case
 * @step 4 Add test case to the suite
 *
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("pr_manager_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, pm_free_test_p);
   tcase_add_test(tcase, pm_free_test_n);
   tcase_add_test(tcase, pm_open_project_edj_test_p);
   tcase_add_test(tcase, pm_open_project_edj_test_n);
   tcase_add_test(tcase, pm_open_project_edc_test_p1);
   tcase_add_test(tcase, pm_open_project_edc_test_p2);
   tcase_add_test(tcase, pm_open_project_edc_test_n);
   tcase_add_test(tcase, pm_save_project_edc_test_p);
   tcase_add_test(tcase, pm_save_project_edc_test_n1);
   tcase_add_test(tcase, pm_save_project_edc_test_n2);
   tcase_add_test(tcase, pm_save_project_edj_test_p);
   tcase_add_test(tcase, pm_save_project_edj_test_n);
   suite_add_tcase(suite, tcase);
   return suite;
}

/**
 * @addtogroup main
 * @{
 * @objective : Run a Check Unit Test
 *
 * @procedure
 * @step 1 Create a suite
 * @step 2 Create a suite runner object of type SRunner
 * @step 3 Add report of Check tests to the xml format
 * @step 4 Run the suite, using the CK_VERBOSE flag
 * @step 5 Create int object for list of the failures
 *
 * @passcondition: Print a summary of the run unit tests.
 * @}
 */
int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_prmanager.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
