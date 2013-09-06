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
#include "colorclasses_edit_api.h"
#include "image_edit_api.h"
#include "project_manager.h"

/**
 * @addtogroup colorclasses_edit_list_get
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Project object
 * @step 2 Add object to function
 *
 * @passcondition: List of strings, each being the name for an image should returned
 * @}
 */
START_TEST (edit_list_get_test)
{
   elm_init(0, 0);
   const char *edc = "./test_colorclasses_edit_api/data";
   const char *edj = "./test_colorclasses_edit_api/data/radio.edj";
   Project *pro = pm_open_project_edj(edc, edj);
   if (!colorclasses_edit_list_get(pro))
   {
      ck_abort_msg("failure: cannot return list of strings, each being the name for an image");
   }
   pm_free(pro);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorclasses_edit_colors
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create Project object
 * @step 2 Create parameters
 * @step 3 Add object and parameters to test functions
 * @param class_name Color class to fetch values
 * @param r Red component of main color
 * @param g Green component of main color
 * @param b Blue component of main color
 * @param a Alpha component of main color
 * @param r2 Red component of secondary color
 * @param g2 Green component of secondary color
 * @param b2 Blue component of secondary color
 * @param a2 Alpha component of secondary color
 * @param r3 Red component of tertiary color
 * @param g3 Green component of tertiary color
 * @param b3 Blue component of tertiary color
 * @param a3 Alpha component of tertiary coloor
 *
 * @passcondition: EINA_TRUE should returned
 * @}
 */
START_TEST (edit_colors_test)
{
   elm_init(0, 0);

   const char *edc, *edj;
   edc = "./test_colorclasses_edit_api/data";
   edj = "./test_colorclasses_edit_api/data/radio.edj";
   Project *pro = pm_open_project_edj(edc, edj);
   const char *class_name = "border_top";

   if (!colorclass_edit_add(pro, class_name))
   {
      ck_abort_msg("failure: cannot create new color class object");
   }

   int r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3;
   r = 150;
   g = 150;
   b = 150;
   a = 150;
   r2 = 150;
   g2 = 150;
   b2 = 150;
   a2 = 150;
   r3 = 150;
   g3 = 150;
   b3 = 150;
   a3 = 150;
   if (!colorclasses_edit_colors_set(pro, class_name, r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3))
   {
      ck_abort_msg("failure: error in colorclasses_edit_color_set() function");
   }

   int _r, _g, _b, _a, _r2, _g2, _b2, _a2, _r3, _g3, _b3, _a3;
   if (!colorclasses_edit_colors_get(pro, class_name, &_r, &_g, &_b, &_a, &_r2, &_g2, &_b2, &_a2, &_r3, &_g3, &_b3, &_a3))
   {
      ck_abort_msg("failure: error in colorclasses_edit_colors_get() function");
   }
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
   Suite *suite = suite_create("colorclases_edit_api_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, edit_list_get_test);
   tcase_add_loop_test(tcase, edit_colors_test, 0, 1);
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
   srunner_set_xml (runner, "test_colorclasses_api.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
