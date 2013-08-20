#include <check.h>
#include "efl_tet.h"

/**
 * @addtogroup app_init_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function for test
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (app_init_test)
{
   if (app_init() == EINA_FALSE)
   {
      ck_abort_msg("failure: libraries was failed to init");
   }
}
END_TEST

/**
 * @addtogroup app_layout_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data object
 * @step 2 Call function for test
 *
 * @passcondition: win_layout should return of function
 * @}
 */
START_TEST (win_layout_get_test)
{
   App_Data *ap = NULL;
   Evas_Object *obj = win_layout_get();
   fail_unless(obj == ap->win_layout, "failure: win_layout not exist");
}
END_TEST

/**
 * @addtogroup app_shutdown_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function fot test
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (app_shutdown_test)
{
   app_shutdown();
}
END_TEST

/**
 * @addtogroup app_create_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data object with app_create() function
 *
 * @passcondition: link to create emplty App_Data should return
 * @}
 */
START_TEST (app_create_test)
{
   App_Data *data = app_create();
   if(data == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot link to create empty App_Data");
   }
}
END_TEST

/**
 * @addtogroup app_free_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data object
 * @step 2 Add to the function
 *
 * @passcondition: EINA_TRUE returned from function
 * @}
 */
START_TEST (app_free_test1)
{
   App_Data *ap = calloc(1, sizeof(ap));
   app_free(ap);
   if (ap == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot remove App_Data");
   }
}
END_TEST

/**
 * @addtogroup app_free_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Calling NULL parameters to function
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (app_free_test2)
{
   App_Data *ap = NULL;
   app_free(ap);
   fail_unless(ap == EINA_FALSE, "failure: cannot delete NULL App_Data");
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
   Suite *suite = suite_create("efltet_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, app_init_test);
   tcase_add_test(tcase, app_shutdown_test);
   tcase_add_test(tcase, win_layout_get_test);
   tcase_add_test(tcase, app_create_test);
   tcase_add_test(tcase, app_free_test1);
   tcase_add_test(tcase, app_free_test2);
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
   elm_init(0, 0);
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_prmanager.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   elm_shutdown();
   return number_failed;
}
