#include <check.h>
#include "edje_compile.h"

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
   struct _Edje_CC *cc = calloc(1, sizeof(struct _Edje_CC));
   edje_cc_free(cc);
   if(cc == EINA_FALSE)
   {
      ck_abort_msg("uncorrect work function 'edje_cc_free'");
   }
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
   struct _Edje_CC *cc = NULL;
   edje_cc_free(cc);
   fail_unless(cc == EINA_FALSE, "error: 'edje_cc_free(NULL)' = EINA_TRUE");
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
   printf("test decc\n!!!!!!!\n");
   char *edc, *edj;
   edc = "../check_utc/edje_compile/data/new_edc_check.edc";
   edj = "../check_utc/edje_compile/data/check.edj";
   decompile(edj,edc);
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
   const char *edc, *edj, *img, *font, *sound;
   edc = "../check_utc/edje_compile/data";
   edj = "../check_utc/edje_compile/data/new_edj_check.edj";
   img = "../check_utc/edje_compile/data";
   sound = "../check_utc/edje_compile/data";
   font = "../check_utc/edje_compile/data";
   if(compile(edc, edj, img, font, sound) == EINA_FALSE)
   {
      ck_abort_msg("error in function 'compile': cannot creating Edje_CC object");
   }
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
   tcase_add_test(tcase, decompile_test);
   tcase_add_test(tcase, compile_test);
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
