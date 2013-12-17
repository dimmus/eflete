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
#include "ui_ruler.h"

/**
 * @addtogroup ui_ruler_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parent
 * @step 2 Call ui_ruler_add()
 * @param parent The parent object
 *
 * @passcondition: Edje_Object returned from function
 * @}
 */
START_TEST (ui_ruler_add_test_p)
{
   elm_init(0, 0);
   Evas_Object *result, *parent;
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);
   result = ui_ruler_add(parent);
   ck_assert_msg(!strcmp(evas_object_type_get(result),"edje"),"Not layout returned\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create parent
 * @step 2 Call ui_ruler_add()
 * @param parent The parent object
 *
 * @passcondition: NULL returned from function
 * @}
 */
START_TEST (ui_ruler_add_test_n)
{
   elm_init(0, 0);
   Evas_Object *result, *parent;
   parent=NULL;
   result = ui_ruler_add(parent);
   ck_assert_msg(result == NULL,"Not NULL returned\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_GETTERS&SETTERS_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ruler
 * @step 2 Set val1
 * @step 3 Get val and compare with setted
 * @step 4 Set val2
 * @step 5 Get val and compare again
 *
 * @passcondition: val match
 * @}
 */
#define GETSET_POS_TEST(_FUNC_,_VAL1_,_VAL2_)\
START_TEST (_FUNC_ ## _test_p)\
{\
   elm_init(0, 0);\
   Evas_Object *ruler, *parent;\
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);\
   ruler = ui_ruler_add(parent);\
   _FUNC_ ## _set(ruler,_VAL1_);\
   ck_assert_msg(_FUNC_ ## _get(ruler)==_VAL1_,"Pointer position mismatch");\
   _FUNC_ ## _set(ruler,_VAL2_);\
   ck_assert_msg(_FUNC_ ## _get(ruler)==_VAL2_,"Pointer position mismatch");\
   elm_shutdown();\
}\
END_TEST

/**
 * @addtogroup ui_ruler_GETTERS&SETTERS_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ruler
 * @step 2 Get val
 *
 * @passcondition: nothing happend
 * @}
 */
#define GET_NEG_TEST(_FUNC_)\
START_TEST (_FUNC_ ## _get_test_n)\
{\
   elm_init(0, 0);\
   Evas_Object *ruler;\
   ruler = NULL;\
   _FUNC_ ## _get(ruler);\
   elm_shutdown();\
}\
END_TEST

/**
 * @addtogroup ui_ruler_GETTERS&SETTERS_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ruler
 * @step 2 Set val
 *
 * @passcondition: nothing happend
 * @}
 */
#define SET_NEG_TEST(_FUNC_,_VAL_)\
START_TEST (_FUNC_ ## _set_test_n)\
{\
   elm_init(0, 0);\
   Evas_Object *ruler;\
   ruler = NULL;\
   _FUNC_ ## _set(ruler,_VAL_);\
   elm_shutdown();\
}\
END_TEST

#define GETSET_TEST(_FUNC_,_VAL1_,_VAL2_)\
GETSET_POS_TEST(_FUNC_,_VAL1_,_VAL2_)\
GET_NEG_TEST(_FUNC_)\
SET_NEG_TEST(_FUNC_,_VAL1_)


GETSET_TEST(ui_ruler_orient,HORIZONTAL,VERTICAL)
GETSET_TEST(ui_ruler_pointer_pos,10,99)
GETSET_TEST(ui_ruler_scale_absolute_position_zero,10,99)
GETSET_TEST(ui_ruler_scale_absolute_dashes_step,10,99)
GETSET_TEST(ui_ruler_step_relative,10,42)
GETSET_TEST(ui_ruler_scale_relative_visible,EINA_TRUE,EINA_FALSE)
GETSET_TEST(ui_ruler_scale_absolute_visible,EINA_TRUE,EINA_FALSE)


/**
 * @addtogroup ui_ruler_scale_relative_position_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ruler
 * @step 2 Set pos
 * @step 3 Get pos and compare with setted
 * @step 4 Set another pos
 * @step 5 Get pos and compare again
 *
 * @passcondition: pos match
 * @}
 */
START_TEST (ui_ruler_scale_relative_position_test_p)
{
   elm_init(0, 0);
   Evas_Object *ruler, *parent;
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ruler = ui_ruler_add(parent);
   int a=0,b=0;
   ui_ruler_scale_relative_position_set(ruler,10,20);
   ui_ruler_scale_relative_position_get(ruler,&a,&b);
   ck_assert_msg(a==10 && b==20,"Position mismatch");
   ui_ruler_scale_relative_position_set(ruler,42,45);
   ui_ruler_scale_relative_position_get(ruler,&a,&b);
   ck_assert_msg(a==42 && b==45,"Position mismatch");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_scale_relative_position_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't reate ruler
 * @step 2 Set pos
 *
 * @passcondition: nothing happend
 * @}
 */
START_TEST (ui_ruler_scale_relative_position_set_test_n)
{
   elm_init(0, 0);
   Evas_Object *ruler;
   ruler = NULL;
   ui_ruler_scale_relative_position_set(ruler,10,20);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_scale_relative_position_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't reate ruler
 * @step 2 Get pos
 * @step 3 Create ruler
 * @step 4 Get pos into incorrect variables
 *
 * @passcondition: nothing happend
 * @}
 */
START_TEST (ui_ruler_scale_relative_position_get_test_n)
{
   elm_init(0, 0);
   Evas_Object *ruler, *parent;
   ruler = NULL;
   int a=0,b=0;
   ui_ruler_scale_relative_position_get(ruler,&a,&b);
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ruler = ui_ruler_add(parent);
   ui_ruler_scale_relative_position_get(ruler,NULL,NULL);
   elm_shutdown();
}
END_TEST



/**
 * @addtogroup ui_ruler_show_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ruler
 * @step 2 Try to show it
 *
 * @passcondition: nothing happend
 * @}
 */
START_TEST (ui_ruler_show_test_n)
{
   elm_init(0, 0);
   Evas_Object *ruler;
   ruler = NULL;
   ui_ruler_show(ruler);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_hide_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ruler
 * @step 2 Try to hide it
 *
 * @passcondition: nothing happend
 * @}
 */
START_TEST (ui_ruler_hide_test_n)
{
   elm_init(0, 0);
   Evas_Object *ruler;
   ruler = NULL;
   ui_ruler_hide(ruler);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_redraw_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ruler
 * @step 2 Try to redraw
 *
 * @passcondition: nothing happend
 * @}
 */
START_TEST (ui_ruler_redraw_test_n)
{
   elm_init(0, 0);
   Evas_Object *ruler;
   ruler = NULL;
   ui_ruler_redraw(ruler);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_visible_get_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ruler
 * @step 2 Show ruler
 * @step 3 Get ruler visible state and compare with setted
 * @step 4 Hide ruler
 * @step 5 Get ruler visible state and compare again
 *
 * @passcondition: State match
 * @}
 */
START_TEST (ui_ruler_visible_get_test_p)
{
   elm_init(0, 0);
   Evas_Object *ruler, *parent;
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ruler = ui_ruler_add(parent);
   ui_ruler_show(ruler);
   ck_assert_msg(ui_ruler_visible_get(ruler)==EINA_TRUE,"Visible state mismatch\n");
   ui_ruler_hide(ruler);
   ck_assert_msg(ui_ruler_visible_get(ruler)==EINA_FALSE,"Pointer position mismatch\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_ruler_visible_get_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ruler
 * @step 2 Get ruler visible state and compare with setted
 *
 * @passcondition: nothing happens
 * @}
 */
START_TEST (ui_ruler_visible_get_test_n)
{
   elm_init(0, 0);
   Evas_Object *ruler;
   ruler = NULL;
   ui_ruler_visible_get(ruler);
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
   Suite *suite = suite_create("ui_ruler_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_ruler_add_test_p);
   tcase_add_test(tcase, ui_ruler_add_test_n);
   tcase_add_test(tcase, ui_ruler_orient_test_p);
   tcase_add_test(tcase, ui_ruler_orient_get_test_n);
   tcase_add_test(tcase, ui_ruler_orient_set_test_n);
   tcase_add_test(tcase, ui_ruler_pointer_pos_test_p);
   tcase_add_test(tcase, ui_ruler_pointer_pos_get_test_n);
   tcase_add_test(tcase, ui_ruler_pointer_pos_set_test_n);
   tcase_add_test(tcase, ui_ruler_show_test_n);
   tcase_add_test(tcase, ui_ruler_hide_test_n);
   tcase_add_test(tcase, ui_ruler_visible_get_test_p);
   tcase_add_test(tcase, ui_ruler_visible_get_test_n);
   tcase_add_test(tcase, ui_ruler_scale_absolute_position_zero_test_p);
   tcase_add_test(tcase, ui_ruler_scale_absolute_position_zero_get_test_n);
   tcase_add_test(tcase, ui_ruler_scale_absolute_position_zero_set_test_n);
   tcase_add_test(tcase, ui_ruler_scale_absolute_dashes_step_test_p);
   tcase_add_test(tcase, ui_ruler_scale_absolute_dashes_step_get_test_n);
   tcase_add_test(tcase, ui_ruler_scale_absolute_dashes_step_set_test_n);
   tcase_add_test(tcase, ui_ruler_step_relative_test_p);
   tcase_add_test(tcase, ui_ruler_step_relative_get_test_n);
   tcase_add_test(tcase, ui_ruler_step_relative_set_test_n);
   tcase_add_test(tcase, ui_ruler_scale_relative_visible_test_p);
   tcase_add_test(tcase, ui_ruler_scale_relative_visible_get_test_n);
   tcase_add_test(tcase, ui_ruler_scale_relative_visible_set_test_n);
   tcase_add_test(tcase, ui_ruler_scale_absolute_visible_test_p);
   tcase_add_test(tcase, ui_ruler_scale_absolute_visible_get_test_n);
   tcase_add_test(tcase, ui_ruler_scale_absolute_visible_set_test_n);
   tcase_add_test(tcase, ui_ruler_redraw_test_n);
   tcase_add_test(tcase, ui_ruler_scale_relative_position_test_p);
   tcase_add_test(tcase, ui_ruler_scale_relative_position_set_test_n);
   tcase_add_test(tcase, ui_ruler_scale_relative_position_get_test_n);
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
   srunner_set_xml (runner, "test_uiruler.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
