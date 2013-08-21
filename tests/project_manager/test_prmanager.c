#include <check.h>
#include "project_manager.h"
#include "efl_tet.h"
/**
 * @addtogroup pm_open_project_edc_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to pm_open_project_edc() function
 * @param name The name of a project
 * @param path Path to a edc-file
 * @param img Path to a image firectory of a project
 * @param font Path to a font directory of a project
 * @param sound Path to a sound directory of a project.
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (pm_open_project_edc_test_n1)
{
   elm_init(0,0);
   const char *name, *path, *img, *font, *sound;
   name = "./data";
   path = "./data/check.edc";
   img = "./data";
   font = "./data";
   sound = "./data";
   if(pm_open_project_edc(name, path, img, font, sound) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot open project from edc-file");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edc_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Calling NULL parameters to function
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (pm_open_project_edc_test_n2)
{
   fail_unless(pm_open_project_edc(NULL, NULL, NULL, NULL, NULL) == EINA_FALSE, "failure: uncorrect work function");
}
END_TEST

/**
 * @addtogroup pm_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call NULL parameter to function
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (pm_free_test_n2)
{
   Project *project = NULL;
   fail_unless(pm_free(project) == EINA_FALSE, "failure: uncorrect work function");
}
END_TEST

/**
 * @addtogroup pm_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Project object
 * @step 2 Add object to function
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (pm_free_test_n1)
{
   elm_init(0,0);
   Project *pro;
   const char *name, *path, *img, *font, *sound;
   name = "./data";
   path = "../tests/edje_compile/data/check.edc";
   img = "./data";
   font = "./data";
   sound = "./data";
   pro = pm_open_project_edc(name, path, img, font, sound);
   fail_unless(pm_free(pro) == EINA_TRUE, "failure: uncorrect work function");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edj_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to pm_open_project_edj() function
 * @param name The name of a project
 * @param path Path to edj-file.
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (pm_open_project_edj_test_n1)
{
   elm_init(0,0);
   char *name, *path;
   name = "./data";
   path = "./data/check.edj";
   if(pm_open_project_edj(name, path) == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot open project from edj-file");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_open_project_edj_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Calling NULL parameters to function
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (pm_open_project_edj_test_n2)
{
   fail_unless(pm_open_project_edj(NULL, NULL) == EINA_FALSE, "failure: uncorrect work function");
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
   Suite *suite = suite_create("pr_manager_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, pm_free_test_n1);
   tcase_add_test(tcase, pm_free_test_n2);
   tcase_add_test(tcase, pm_open_project_edj_test_n1);
   tcase_add_test(tcase, pm_open_project_edj_test_n2);
   tcase_add_test(tcase, pm_open_project_edc_test_n1);
   tcase_add_test(tcase, pm_open_project_edc_test_n2);
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
