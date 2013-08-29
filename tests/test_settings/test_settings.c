#include <check.h>
#include "settings.h"
#include "ui_main_window.h"

/**
 * @addtogroup ui_element_settings_init
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function for test
 *
 * @passcondition: should returned a pointer of UI_Elements_Settings
 * @}
 */
START_TEST (ui_element_settings_init_test)
{
   elm_init(0,0);
   UI_Elements_Settings *set = ui_element_settings_init();
   if(!set)
   {
      ck_abort_msg("failure: cannot return pointer of the UI_Elements_Settings");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addto group ui_settings_save
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function for test
 *
 * @passcondition: EINA_TRUE should return of a function
 */
START_TEST (ui_settings_save_test)
{
   elm_init(0,0);
   App_Data *app = app_create();
   app_init();
   ui_main_window_add(app);
   if(ui_settings_save() == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot save ui elements setings");
   }
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addto group ui_settings_load
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call function for test
 *
 * @passcondition: EINA_TRUE should return of a function
 */
START_TEST (ui_settings_load_test)
{
   elm_init(0,0);
   App_Data *app = app_create();
   app_init();
   ui_main_window_add(app);
   if(ui_settings_load() == EINA_FALSE)
   {
      ck_abort_msg("failure: cannot load setings from 'ui.set'");
   }
   elm_shutdown();
   app_free(app);
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
   Suite *suite = suite_create("setings_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_element_settings_init_test);
   tcase_add_test(tcase, ui_settings_save_test);
   tcase_add_test(tcase, ui_settings_load_test);
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
   srunner_set_xml (runner, "test_settings.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
