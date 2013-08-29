#include <check.h>
#include "alloc.h"

/**
 * @addtogroup mem_malloc
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameter
 * @step 2 Add parameter to function
 * @param size Size of the memory block, in bites.
 *
 * @passcondition: should returned a pointer to the memory block allocated
 * @}
 */
START_TEST (mem_malloc_test)
{
   size_t size = 1024;
   int *mm = mem_malloc(size);
   if(mm == NULL)
   {
      ck_abort_msg("failure: cannot return pointer to the memory block allocated");
   }
   free(mm);
}
END_TEST

/**
 * @addto group mem_calloc
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parameters
 * @step 2 Add parameters to function
 * @param num Number of a elements to allocate
 * @param size Size of each element.
 *
 * @passcondition: should return a pointer to the memory block allocated
 */
START_TEST (mem_calloc_test)
{
   size_t num = 1;
   size_t size = sizeof(1025);
   int *mc = mem_calloc(num, size);
   if(mc == NULL)
   {
      ck_abort_msg("failure: cannot return pointer to the memory block allocated");
   }
   free(mc);
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
   tcase_add_test(tcase, mem_malloc_test);
   tcase_add_test(tcase, mem_calloc_test);
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
