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
#include "font_viewer_dialog.h"

/**
 * @addtogroup font_viewer_dialog
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Evas_Object parent as main window
 * @step 2 Create Evas_Object from font_viewer_add() function
 * @step 3 Add objects to font_viewer_init() function
 *
 * @passcondition: should return result without error
 * @}
 */
START_TEST (font_dialog_test1)
{
   elm_init(0,0);
   Evas_Object *par, *font;
   par = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   font = font_viewer_add(par);
   if (!font)
   {
      ck_abort_msg("failure: cannot create pointer to inwin object");
   }
   char *name, *path;
   name = "./test_font_viewer_dialog/data";
   path = "./test_font_viewer_dialog/data/check.edj";
   Project *pro = pm_open_project_edj(name, path);
   font_viewer_init(font, pro);
   pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup font_viewer_dialog
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Call to function NULL object
 *
 * @passcondition: NULL should return
 * @}
 */
START_TEST (font_dialog_test2)
{
   elm_init(0,0);
   Evas_Object *par, *font;
   par = NULL;
   font = font_viewer_add(par);
   if (font)
   {
      ck_abort_msg("failure: cannot create pointer to inwin object");
   }
   char *name, *path;
   name = "";
   path = "";
   Project *pro = pm_open_project_edj(name, path);
   font_viewer_init(font, pro);
   pm_free(pro);
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
   Suite *suite = suite_create("font_dialog_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, font_dialog_test1);
   tcase_add_test(tcase, font_dialog_test2);
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
   srunner_set_xml (runner, "test_font_viewer_dialog.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
