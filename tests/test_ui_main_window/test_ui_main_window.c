/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#include <check.h>
#include "ui_main_window.h"
#include "efl_ete.h"


/*  Notify! The methods  ui_part_back(), ui_group_back(), ui_state_select(), ui_part_select(),
 ui_group_clicked(), ui_edc_load_done() can not be checked out working project.
 There are some bugs when ui_edj_load_done() and ui_edc_load_done() calling without first calling elm_main().
*/

/**
 * @addtogroup ui_main_window_add
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 *
 * @passcondition: EINA_TRUE should returned of a function
 * @}
 */
START_TEST (ui_main_window_add_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_main_window_add
 * @{
 * @oobjective Negative test case: Calling function with NULL argument
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (ui_main_window_add_test_n2)
{
   elm_init(0,0);
   fail_unless(ui_main_window_add(NULL) == EINA_FALSE, "error: 'ui_main_window_add(NULL)' = EINA_TRUE");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_main_window_del
 * @{
 * @oobjective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Delete main window using ui_main_window_del()
 *
 * @passcondition:  test passed
 * @}
 */
START_TEST (ui_main_window_del_test)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_main_window_del();
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Create panes using ui_main_window_add()
 *
 * @passcondition: EINA_TRUE should returned of a function
 * @}
 */
START_TEST (ui_panes_add_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   fail_unless(ui_panes_add(app) == EINA_TRUE, "failure: cannot create panes");
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @oobjective Negative test case: Calling function with NULL argument
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (ui_panes_add_test_n2)
{
   fail_unless(ui_panes_add(NULL) == EINA_FALSE, "error: 'ui_panes_add(NULL)' = EINA_TRUE");
}
END_TEST

/**
 * @addtogroup ui_panes_settings_load
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Load settings using ui_panes_settings_load()
 *
 * @passcondition:  test passed
 * @}
 */
START_TEST (ui_panes_settings_load_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_panes_settings_load();
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_settings_load
 * @{
 * @oobjective Negative test case: Calling function without initializing
 *
 * @procedure
 * @step 1 Call function.
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_settings_load_test_n2)
{
   ui_panes_settings_load();
}
END_TEST

/**
 * @addtogroup ui_panes_settings_save
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Save settings using ui_panes_settings_save()
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_settings_save_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_panes_settings_save();
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_settings_load
 * @{
 * @oobjective Negative test case: Calling function without initializing
 *
 * @procedure
 * @step 1 Call function.
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_settings_save_test_n2)
{
   ui_panes_settings_save();
}
END_TEST

/**
 * @addtogroup ui_menu_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Create menu using ui_menu_add_test()
 *
 * @passcondition: EINA_TRUE should returned of a function
 * @}
 */
START_TEST (ui_menu_add_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   fail_unless(ui_menu_add(app) == EINA_TRUE, "failure: cannot create menu");
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_menu_add
 * @{
 * @oobjective Negative test case: Calling function without initializing
 *
 * @procedure
 * @step 1 Call function with.
 *
 * @passcondition: EINA_FALSE returned from function
 * @}
 */
START_TEST (ui_menu_add_test_n2)
{
   fail_unless(ui_menu_add(NULL) == EINA_FALSE, "error: 'ui_panes_add(NULL)' = EINA_TRUE");
}
END_TEST

/**
 * @addtogroup ui_resize_pans
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Resize panes using ui_resize_pans(100, 100);
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_resize_pans_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_resize_pans(100, 100);
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @oobjective Negative test case: Calling function with negative arguments
 *
 * @procedure
 * @step 1 Call function with values -100 -100.
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_resize_pans_test_n2)
{
   ui_resize_pans(-100, -100);
}
END_TEST

/**
 * @addtogroup ui_panes_show
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Show panes using ui_panes_show
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_show_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_panes_show(app);
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @oobjective Negative test case: Calling function with NULL argument
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_show_test_n2)
{
   ui_panes_show(NULL);
}
END_TEST

/**
 * @addtogroup ui_panes_hide
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Hide panes using ui_panes_hide(app);
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_hide_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_panes_hide(app);
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_panes_hide
 * @{
 * @oobjective Negative test case: Calling function with NULL argument
 *
 * @procedure
 * @step 1 Call function with NULL parameter.
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_panes_hide_test_n2)
{
   ui_panes_hide(NULL);
}
END_TEST


/**
 * @addtogroup ui_part_back
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Calling  ui_part_back(app);
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_part_back_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_part_back(app);
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create App_Data structure app
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Open edc file using  ui_edc_load_done
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edc_load_done_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_edc_load_done(app, "first", "./tests/ui_main_window/data/naviframe.edc","","","");
   elm_shutdown();
   app_free(app);
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Negative test case: Calling function with NULL argument App_Data *ap
 *
 * @procedure
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edc_load_done_test_n2)
{
   elm_init(0,0);
   ui_edc_load_done(NULL, "first", "./tests/ui_main_window/data/naviframe.edc","","","");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Negative test case: Open bad edc file
 *
 * @procedure
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edc_load_done_test_n3)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   if(app == NULL)
   {
      ck_abort_msg("uncorrect work function 'app_create'");
   }
   fail_unless(ui_main_window_add(app) == EINA_TRUE, "failure: cannot create window");
   ui_edc_load_done(app, "first", "./tests/ui_main_window/data/bad.edc","","","");
   elm_shutdown();
   app_free(app);
}
END_TEST


/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Call app_init();
 * @step 2 Call app_create() function to initialize app
 * @step 3 Create main window using ui_main_window_add()
 * @step 4 Create fileselector using elm_fileselector_add
 * @step 5 Load edj file using ui_edj_load_done
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edj_load_done_test_n1)
{
   elm_init(0,0);
   app_init();
   App_Data * ap = app_create();
   ui_main_window_add(ap);
   ap->inwin = elm_win_inwin_add(ap->win);
   Evas_Object * fs = elm_fileselector_add(ap->inwin);
   ui_edj_load_done(ap, fs, "./tests/ui_main_window/data/naviframe.edj");
   elm_shutdown();
   app_free(ap);
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case:
 *
 * @procedure: Calling function with NULL argument App_Data * ap
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edj_load_done_test_n2)
{
   elm_init(0,0);
   app_init();
   App_Data * ap = app_create();
   ui_main_window_add(ap);
   ap->inwin = elm_win_inwin_add(ap->win);
   Evas_Object * fs = elm_fileselector_add(ap->inwin);
   ui_edj_load_done(NULL, fs, "./tests/ui_main_window/data/navifram.edj");
   elm_shutdown();
   app_free(ap);
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case: Calling function with NULL argument App_Data * ap
 *
 * @procedure: Calling function with NULL argument Evas_Object* obj
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edj_load_done_test_n3)
{
   elm_init(0,0);
   app_init();
   App_Data * ap = app_create();
   ui_main_window_add(ap);
   ui_edj_load_done(ap, NULL, "./tests/ui_main_window/data/naviframe.edj");
   elm_shutdown();
   app_free(ap);
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case: Opening bad file
 *
 * @procedure: Calling function with addres of bad file.
 *
 * @passcondition: test passed
 * @}
 */
START_TEST (ui_edj_load_done_test_n4)
{
   elm_init(0,0);
   app_init();
   App_Data * ap = app_create();
   ui_main_window_add(ap);
   ap->inwin = elm_win_inwin_add(ap->win);
   Evas_Object * fs = elm_fileselector_add(ap->inwin);
   ui_edj_load_done(ap, fs, "./tests/ui_main_window/data/bad.edj");
   elm_shutdown();
   app_free(ap);
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
   Suite *suite = suite_create("ui_main_window_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_main_window_add_test_n1);
   tcase_add_test(tcase, ui_main_window_add_test_n2);
   tcase_add_test(tcase, ui_main_window_del_test);
   tcase_add_test(tcase, ui_panes_add_test_n1);
   tcase_add_test(tcase, ui_panes_add_test_n2);
   tcase_add_test(tcase, ui_panes_settings_load_test_n1);
   tcase_add_test(tcase, ui_panes_settings_load_test_n2);
   tcase_add_test(tcase, ui_panes_settings_save_test_n1);
   tcase_add_test(tcase, ui_panes_settings_save_test_n2);
   tcase_add_test(tcase, ui_menu_add_test_n1);
   tcase_add_test(tcase, ui_menu_add_test_n2);
   tcase_add_test(tcase, ui_resize_pans_test_n1);
   tcase_add_test(tcase, ui_resize_pans_test_n2);
   tcase_add_test(tcase, ui_panes_show_test_n1);
   tcase_add_test(tcase, ui_panes_show_test_n2);
   tcase_add_test(tcase, ui_panes_hide_test_n1);
   tcase_add_test(tcase, ui_panes_hide_test_n2);
   tcase_add_test(tcase, ui_part_back_test_n1);
   tcase_add_test(tcase, ui_edc_load_done_test_n1);
   tcase_add_test(tcase, ui_edc_load_done_test_n2);
   tcase_add_test(tcase, ui_edc_load_done_test_n3);
   tcase_add_test(tcase, ui_edj_load_done_test_n1);
   tcase_add_test(tcase, ui_edj_load_done_test_n2);
   tcase_add_test(tcase, ui_edj_load_done_test_n3);
   tcase_add_test(tcase, ui_edj_load_done_test_n4);

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
   srunner_set_xml (runner, "test_ui_main_window.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
