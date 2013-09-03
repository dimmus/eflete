#include <check.h>
#include "notify.h"

/**
 * @addtogroup noti_error
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object
 * @param error A error massage
 * @step 7 Add object and parameters to functions
 *
 * @passcondition: EINA_TRUE should return from function
 * @}
 */
START_TEST (error_test1)
{
   elm_init(0,0);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_notify/data/check.edj", "elm/check/base/defaul");
   const char *error;
   error = "error show";
   noti_error_show(obj, error);
   elm_shutdown(); 
}
END_TEST

/**
 * @addtogroup noti_error
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to functions NULL objects
 *
 * @passcondition: error should return from functions
 * @}
 */
START_TEST (error_test2)
{
   elm_init(0,0);
   Evas_Object *obj = NULL;
   const char *error;
   error = NULL;
   noti_error_show(obj, error);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_warning
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object
 * @param warning A warning massage
 * @step 7 Add object and parameters to functions
 *
 * @passcondition: EINA_TRUE should return from function
 * @}
 */
START_TEST (warning_test1)
{
   elm_init(0,0);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_notify/data/check.edj", "elm/check/base/defaul");
   const char *warning;
   warning = "warning show";
   noti_warning_show(obj, warning);
   elm_shutdown(); 
}
END_TEST

/**
 * @addtogroup noti_warning
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to functions NULL objects
 *
 * @passcondition: error should return from functions
 * @}
 */
START_TEST (warning_test2)
{
   elm_init(0,0);
   Evas_Object *obj = NULL;
   const char *warning;
   warning = NULL;
   noti_warning_show(obj, warning);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object
 * @param info A information massage
 * @param time The time for show information
 * @step 7 Add object and parameters to functions
 *
 * @passcondition: EINA_TRUE should return from function
 * @}
 */
START_TEST (info_test1)
{
   elm_init(0,0);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_notify/data/check.edj", "elm/check/base/defaul");
   const char *info;
   double time = 2.5;
   info = "info show";
   noti_info_show(obj, info, time);
   elm_shutdown(); 
}
END_TEST

/**
 * @addtogroup noti_info
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to functions NULL objects
 *
 * @passcondition: error should return from functions
 * @}
 */
START_TEST (info_test2)
{
   elm_init(0,0);
   Evas_Object *obj = NULL;
   const char *info;
   info = NULL;
   double time = 0;
   noti_info_show(obj, info, time);
   elm_shutdown();
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
   Suite *suite = suite_create("notify_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, error_test1);
   tcase_add_test(tcase, error_test2);
   tcase_add_test(tcase, warning_test1);
   tcase_add_test(tcase, warning_test2);
   tcase_add_test(tcase, info_test1);
   tcase_add_test(tcase, info_test2);
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
   srunner_set_xml (runner, "test_notify.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
