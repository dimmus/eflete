#include <check.h>
#include "ui_groupspace.h"
#include "ui_widget_list.h"

/**
 * @addtogroup groupspace_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Parent object
 * @step 2 Add object to tests functions.
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (groupspace_add_test1)
{
   elm_init(0,0);
   Evas_Object *par, *gs;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gs = ui_groupspace_add(par);
   if (!gs)
   {
      ck_abort_msg("failure(groupspace_add): cannot return pointer to groupspace Evas_Object");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE should returned from function
 * @}
 */
START_TEST (groupspace_add_test2)
{
   elm_init(0,0);
   Evas_Object *res, *par = NULL;
   res = ui_groupspace_add(par);
   if (res)
   {
      ck_abort_msg("failure: return NULL pointer to groupspace");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspase_set_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to workspace
 * @step 2 Add Project object
 * @step 3 Create Group object
 * @step 4 Add parameters to set() function
 * @step 5 Add parameters to update() function
 * @step 6 Add parameters to unset() function
 * @param part The name of a part.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (groupspace_set_up_unset_test)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_ui_groupspace/data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./test_ui_groupspace/data";
   path = "./test_ui_groupspace/data/check.edj";
   Project *pro = pm_open_project_edj(name, path);
   Evas_Object *gs = ui_groupspace_add(par);
   ui_groupspace_set(ws, pro, group);
   ui_groupspace_update(gs);
   ui_groupspace_unset(gs);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspace_part_update_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Parent object
 * @step 2 Create Part object
 * @step 3 Add object to function
 *
 * @passcondition: EINA_TRUE should returned from function
 * @}
 */
START_TEST (part_update_test)
{
   elm_init(0,0);
   Evas_Object *par, *gs;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   gs = ui_groupspace_add(par);
   Evas_Object *obj, *win;
   elm_theme_extension_add(NULL, "./test_ui_groupspace/data/check.edj");
   win = elm_win_add(NULL, "check", ELM_WIN_BASIC);
   obj = elm_check_add(win);
   const char *name, *path, *pname = "defaul";
   Part *part = wm_part_add(obj, pname);
   name = "./test_ui_groupspace/data/";
   path = "./test_ui_groupspace/data/default.edj";
   Project *pro = pm_open_project_edj(name, path);
   Evas_Object *list = ui_widget_list_add(par);
   pro->widgets = wm_widget_list_new(pro->swapfile);
   ui_widget_list_data_set(list, pro);
   ui_groupspace_part_state_update(gs, part);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspase_separate_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to workspace
 * @step 2 Add ws to function.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (separate_test)
{
   elm_init(0,0);
   Evas_Object *par, *layout;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   Workspace *ws = ws_add(layout);
   ui_groupspace_separate(ws);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupspase_separate_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to workspace
 * @step 2 Add ws to function.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (group_get_test1)
{
   elm_init(0,0);
   Evas_Object *par, *layout, *gs;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   gs = ui_groupspace_add(layout);
   Workspace *ws = ws_add(gs);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_ui_groupspace/data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./test_ui_groupspace/data";
   path = "./test_ui_groupspace/data/check.edj";
   Project *pro = pm_open_project_edj(name, path);
   ui_groupspace_set(ws, pro, group);
   Group *gr = ui_groupspace_group_get(gs);
   if (!gr)
   {
      ck_abort_msg("failure: cannot return group structure pointer");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup group_get_test
 * @{
 * @objective negative test case:
 *
 * @procedure
 * @step 1 call function with null parameter.
 *
 * @passcondition: eina_false should returned from function
 * @}
 */
START_TEST (group_get_test2)
{
   elm_init(0,0);
   Evas_Object *gs =  NULL;
   Group *group = ui_groupspace_group_get(gs);
   if (group)
   {
      ck_abort_msg("failure: return null pointer to groupspace");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_state_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to groupspace layout
 * @step 2 Create Part Object
 * @step 2 Add parameters to function.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (part_state_test1)
{
   elm_init(0,0);
   Evas_Object *obj, *win;
   elm_theme_extension_add(NULL, "./test_ui_groupspace/data/check.edj");
   win = elm_win_add(NULL, "check", ELM_WIN_BASIC);
   obj = elm_check_add(win);
   const char *pname = "defaul";
   Part *part = wm_part_add(obj, pname);
   Evas_Object *par, *layout, *gs;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   gs = ui_groupspace_add(layout);
   char *stname = "new_state";
   double stvalue = 3.6;
   if(!ui_groupspace_part_state_add(gs, part, stname, stvalue))
   {
      ck_abort_msg("failure: cannot add state for part");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup part_state_test
 * @{
 * @objective negative test case:
 *
 * @procedure
 * @step 1 call function with null parameter.
 *
 * @passcondition: eina_false should returned from function
 * @}
 */
START_TEST (part_state_test2)
{
   elm_init(0,0);
   Evas_Object *gs =  NULL;
   Part *part = NULL;
   double value = 0;
   if (ui_groupspace_part_state_add(gs, part, NULL, value))
   {
      ck_abort_msg("failure: return state for NULL part");
   }
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup box_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create pointer to groupspace layout
 * @step 2 Create Part Object
 * @step 2 Add parameters to function.
 *
 * @passcondition: not NULL object should returned from function
 * @}
 */
START_TEST (box_get_test)
{
   elm_init(0,0);
   Evas_Object *par, *layout, *gs;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   layout = elm_layout_add(par);
   gs = ui_groupspace_add(layout);
   Workspace *ws = ws_add(gs);
   Evas *e = evas_new();
   Evas_Object *obj = edje_edit_object_add(e);
   edje_object_file_set(obj, "./test_ui_groupspace/data/check.edj", "elm/check/base/defaul");
   const char *grname = "defaul";
   const char *full_grname = "elm/check/base/defaul";
   Group *group = wm_group_add(grname, full_grname);
   group->obj = obj;
   char *name, *path;
   name = "./test_ui_groupspace/data";
   path = "./test_ui_groupspace/data/check.edj";
   Project *pro = pm_open_project_edj(name, path);
   ui_groupspace_set(ws, pro, group);
   ui_groupspace_box_get(gs);
   elm_shutdown();
}
END_TEST

/* @addtogroup test_suite
 * @{
 * @objective Creating above to the test case:
 *
 * @procedure
 * @step 1 Create suite
 * @step 2 Create test case
 * @step 3 Add unit tests to the test case
 * @step 4 Add test case to the suite.
 *
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("ui_groupspace_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, groupspace_add_test1);
   tcase_add_test(tcase, groupspace_add_test2);
   tcase_add_test(tcase, groupspace_set_up_unset_test);
   tcase_add_test(tcase, part_update_test);
   tcase_add_test(tcase, separate_test);
   tcase_add_test(tcase, group_get_test1);
   tcase_add_test(tcase, group_get_test2);
   tcase_add_test(tcase, part_state_test1);
   tcase_add_test(tcase, part_state_test2);
   tcase_add_test(tcase, box_get_test);
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
 * @step 5 Create int object for list of the failures.
 *
 * @passcondition: Print a summary of the run unit tests.
 * @}
 */
int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_groupspace.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
