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
* along with this program; If not, see .
*/

#include <check.h>
#include "edje_compile.h"
#include <Elementary.h>
/**
 * @addtogroup edje_cc_free
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create _Edje_CC object
 * @step 2 Add object to edje_cc_free() function
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (edje_cc_free_test_n1)
{
/*
   struct _Edje_CC *cc = calloc(1, sizeof(struct _Edje_CC));
   edje_cc_free(cc);
   if (cc == EINA_FALSE)
   {
      ck_abort_msg("uncorrect work function 'edje_cc_free'");
   }
*/
}
END_TEST

/**
 * @addtogroup edje_cc_free
 * @{
 * @objective Negative test case: Calling function with NULL argument
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (edje_cc_free_test_n2)
{
/*
   struct _Edje_CC *cc = NULL;
   edje_cc_free(cc);
   fail_unless(cc == EINA_FALSE, "error: 'edje_cc_free(NULL)' = EINA_TRUE");
*/
}
END_TEST

/**
 * @addtogroup decompile_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to decompile() function
 * @param edj Path to input edj file
 * @param edc Path to outut edc file
 *
 * @passcondition: Edje_DeCC object returned from function
 * @}
 */
START_TEST (decompile_test)
{
/*
   elm_init(0,0);
   char *edc, *edj;
   edc = "./test_edje_compile/data/";
   edj = "./test_edje_compile/data/radio.edj";
   if (decompile(edj, edc) == EINA_FALSE)
   {
      ck_abort_msg("error in function 'decompile': cannot creating Edje_DeCC object");
   }

   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup compile_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to compile() function
 * @param edc Path to input edc file.
 * @param edj Path to output edj file.
 * @param img Path to a image directory of a project.
 * @param font Path to a font directory of a project.
 * @param sound Path to a sound directory of a project.
 *
 * @passcondition: Edje_CC object returned from function
 * @}
 */
START_TEST (compile_test)
{
/*
   elm_init(0,0);
   const char *edc, *edj, *img, *font, *sound;
   edc = "./test_edje_compile/data/radio.edc";
   edj = "./test_edje_compile/data";
   img = "./test_edje_compile/data";
   sound = "./test_edje_compile/data";
   font = "./test_edje_compile/data";
   if (compile(edc, edj, img, font, sound) == EINA_FALSE)
   {
      ck_abort_msg("error in function 'compile': cannot creating Edje_CC object");
   }
   elm_shutdown();
*/
}
END_TEST

/**
 * @addtogroup test_suite
 * @{
 * @objective Creating above to the test case:
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
   Suite *suite = suite_create("edje_compile_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, edje_cc_free_test_n1);
   tcase_add_test(tcase, edje_cc_free_test_n2);
   tcase_add_test(tcase, compile_test);
   tcase_add_test(tcase, decompile_test);
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
   srunner_set_xml (runner, "test_compile.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
