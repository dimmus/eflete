#include <check.h>
#include "ui_signal_list.h"

/**
 * @addtogroup ui_signal_list
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object parent
 * @step 2 Add object to _add() function
 * @step 3 Create Evas_Object as signal list
 * @step 4 Create Group object
 * @step 5 Add list and group to _data_set() function
 *
 * @passcondition: new 'signal list' widget handle should return _list_add() and EINA_TRUE from _data_set() function
 * @}
 */
START_TEST (ui_signal_test1)
{
   elm_init(0,0);
   Evas_Object *par, *list;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   list = ui_signal_list_add(par);
   if(!list)
   {
      ck_abort_msg("failure: cannot create signal list");
   }
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   if (!ui_signal_list_data_set(list, group))
   {
      ck_abort_msg("failure: cannot set data of signal to signal list");
   }
   wm_group_free(group);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_signal_list
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to functions NULL objects
 *
 * @passcondition:  EINA_FALSE should return from functions
 * @}
 */
START_TEST (ui_signal_test2)
{
   elm_init(0,0);
   Evas_Object *par, *list;
   par = NULL;
   list = ui_signal_list_add(par);
   if(list)
   {
      ck_abort_msg("failure: cannot create signal list from NULL parent");
   }
   if (ui_signal_list_data_set(list, NULL))
   {
      ck_abort_msg("failure: cannot set data signal to NULL signal list");
   }
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
   Suite *suite = suite_create("uisignal_list_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_signal_test1);
   tcase_add_test(tcase, ui_signal_test2);
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
   srunner_set_xml (runner, "test_uisignal_list.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
