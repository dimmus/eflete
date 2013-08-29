#include <check.h>
#include "ui_states_list.h"

/**
 * @addtogroup ui_states_list
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object parent
 * @step 2 Add object to _add() function
 * @step 3 Create Evas_Object as state list
 * @step 4 Add list, part and group to _data_set() function
 * @step 5 Create Part object
 * @step 6 Create Group object
 * @step 7 Add list, to _part_get() function
 *
 * @passcondition: EINA_TRUE should return from function
 * @}
 */
START_TEST (ui_states_test1)
{
   elm_init(0,0);
   Evas_Object *par, *list;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   list = ui_states_list_add(par);
   if(!list)
   {
      ck_abort_msg("failure(list_add): cannot create widget list");
   }
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   const char *pname = "bg";
   Part *part = wm_part_add(obj, pname);
   if (!part) printf("----------no part----------\n");
   if (!ui_states_list_data_set(list, group, part))
   {
      ck_abort_msg("failure(list_data_set): cannot set data of part states to state list");
   }
   wm_part_free(part);
   part = ui_state_list_part_get(list);
   if (!part)
   {
      ck_abort_msg("failure(list_part_get): cannot create Part object");
   }
   elm_shutdown();
   wm_group_free(group);
}
END_TEST

/**
 * @addtogroup ui_states_list
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to functions NULL objects
 *
 * @passcondition:  EINA_FALSE should return from functions
 * @}
 */
START_TEST (ui_states_test2)
{
   elm_init(0,0);
   Evas_Object *par, *list;
   par = NULL;
   list = ui_states_list_add(par);
   if(list)
   {
      ck_abort_msg("failure(list_add): cannot create state list with NULL object");
   }
   const char *grname = "";
   const char *full_grname = "";
   Group *group = wm_group_add(grname, full_grname);
   Part *part;
   part = ui_state_list_part_get(list);
   if (part)
   {
      ck_abort_msg("failure(list_part_get): cannot create Part object with NULL state list");
   }
   if (ui_states_list_data_set(list, group, part))
   {
      ck_abort_msg("failure(list_data_set): cannot set data of part states to NULL state list");
   }
   elm_shutdown();
   wm_part_free(part);
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
   Suite *suite = suite_create("uistate_list_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_states_test1);
   tcase_add_test(tcase, ui_states_test2);
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
   srunner_set_xml (runner, "test_uistates_list.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
