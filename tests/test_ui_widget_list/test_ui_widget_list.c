#include <check.h>
#include "ui_widget_list.h"

/**
 * @addtogroup ui_widget_list
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object parent
 * @step 2 Add object to _add() function
 * @step 3 Create Evas_Object as widget list
 * @step 4 Add parameters to _title_set() function
 * @step 5 Create Project object
 * @step 7 Add list and object to _data_set() function
 *
 * @passcondition: EINA_TRUE should return from function
 * @}
 */
START_TEST (ui_widget_test1)
{
   elm_init(0,0);
   Evas_Object *par, *list;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   list = ui_widget_list_add(par);
   if(!list)
   {
      ck_abort_msg("failure(list_add): cannot create widget list");
   }
   const char *title;
   title = "new_title";

   ui_widget_list_title_set(list, title);
   char *name, *path;
   name = "./test_ui_widget_list/data/";
   path = "./test_ui_widget_list/data/default.edj";
   Project *pro = pm_open_project_edj(name, path);
   pro->widgets = wm_widget_list_new(pro->swapfile);
   if (!ui_widget_list_data_set(list, pro))
   {
      ck_abort_msg("failure(list_data_set): cannot set data of widget to widget list");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_widget_list
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to functions NULL objects
 *
 * @passcondition:  EINA_FALSE should return from functions
 * @}
 */
START_TEST (ui_widget_test2)
{
   elm_init(0,0);
   Evas_Object *par, *list;
   par = NULL;
   list = ui_widget_list_add(par);
   if(list)
   {
      ck_abort_msg("failure(list_add): cannot create widget list with NULL parent");
   }
   const char *title;
   title = "";

   ui_widget_list_title_set(list, title);

   char *name, *path;
   name = "";
   path = "";
   Project *pro = pm_open_project_edj(name, path);
   if (ui_widget_list_data_set(list, pro))
   {
      ck_abort_msg("failure(list_data_set): cannot set data of widget to NULL widget list");
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
   Suite *suite = suite_create("uiwidget_list_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_widget_test1);
   tcase_add_test(tcase, ui_widget_test2);
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
   srunner_set_xml (runner, "test_uiwidget_list.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
