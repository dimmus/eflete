#include <check.h>
#include "logger.h"

/**
 * @addtogroup logger_init
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function for test
 *
 * @passcondition: EINA_TRUE should returned of a function
 * @}
 */
START_TEST (logger_init_test)
{
  fail_unless(logger_init() == EINA_TRUE, "failure: cannot init logger");
}
END_TEST

/**
 * @addto group logger_shutdown
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function for test
 *
 * @passcondition: test passed
 */
START_TEST (logger_shutdown_test)
{
   logger_shutdown();
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
   Suite *suite = suite_create("alloc_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, logger_init_test);
   tcase_add_test(tcase, logger_shutdown_test);
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
   srunner_set_xml (runner, "test_alloc.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
