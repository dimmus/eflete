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
#include "ui_block.h"

/**
 * @addtogroup ui_block_add_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create parent
 * @step 2 Call ui_block_add()
 * @param parent The parent object
 *
 * @passcondition: Evas_Object "elm_layout" returned from function
 * @}
 */
START_TEST (ui_block_add_test_p)
{
   elm_init(0, 0);
   Evas_Object *result, *parent;
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);
   result = ui_block_add(parent);
   ck_assert_msg(!strcmp(evas_object_type_get(result),"elm_layout"),"Not layout returned\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_add_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create parent
 * @step 2 Call ui_block_add()
 * @param parent The parent object
 *
 * @passcondition: NULL returned from function
 * @}
 */
START_TEST (ui_block_add_test_n)
{
   elm_init(0, 0);
   Evas_Object *result, *parent;
   parent=NULL;
   result = ui_block_add(parent);
   ck_assert_msg(result == NULL,"Not NULL returned\n");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_title_visible_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ui_block
 * @step 2 Call ui_block_title_visible()
 * @param block The ui_block object
 *
 * @passcondition: nothing happend
 * @}
 */
START_TEST (ui_block_title_visible_n)
{
   elm_init(0, 0);
   ui_block_title_visible(NULL,EINA_TRUE);
   ui_block_title_visible(NULL,EINA_FALSE);
   elm_shutdown();
}
END_TEST

/*TODO:add positive UTC with signal hook*/

/**
 * @addtogroup ui_title_text_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Don't create ui_block
 * @step 2 Call ui_block_title_text_set()
 * @step 3 Call ui_block_title_text_get()
 * @param block The ui_block object
 * @param title title
 *
 * @passcondition: set:nothing happend, get: NULL returned
 * @}
 */
START_TEST (ui_block_title_text_n)
{
   elm_init(0, 0);
   ui_block_title_text_set(NULL,"test");
   ck_assert_msg(!ui_block_title_text_get(NULL), "Not NULL returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_title_text_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Create ui_block
 * @step 2 Call ui_block_title_text_set()
 * @step 3 Call ui_block_title_text_get()
 * @param block The ui_block object
 * @param title title
 *
 * @passcondition: Titles must match
 * @}
 */
START_TEST (ui_block_title_text_p)
{
   elm_init(0, 0);
   Evas_Object *parent, *block;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   mark_point();
   block = ui_block_add(parent);
   mark_point();
   ui_block_title_text_set(block,"test1");
   mark_point();
   const char * str = ui_block_title_text_get(block);
   ck_assert_msg(!strcmp(str,"test1"),"title mismatch");
   ui_block_title_text_set(block,"some say");
   str = ui_block_title_text_get(block);
   ck_assert_msg(!strcmp(str,"some say"),"title mismatch");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_block_content_test
 * @{
 * @objective Positive test case:
 *
 * @procedure
 * @step 1 Prepare app data and content
 * @step 2 Call func_set()
 * @step 3 Call func_get()
 *
 * @passcondition: Content objects must match
 * @}
 */

/**
 * @addtogroup ui_block_content_test
 * @{
 * @objective Negative test case:
 *
 * @procedure
 * @step 1 Prepare app data OR content
 * @step 2 Call func_set()
 * @step 3 Call func_get()
 *
 * @passcondition: NULL must be returned
 * @}
 */

#define TEST_UI_BLOCK_CONTENT_TC(func,tst,bl,block_obj,cont,result)\
START_TEST (func ## _test_ ## tst)\
{\
   elm_init(0, 0);\
   Evas_Object *parent, *content;\
   App_Data apd;\
   parent=elm_win_add(NULL, "test", ELM_WIN_BASIC);\
   apd.block.bl = block_obj;\
   content = cont;\
   func ## _set(&apd,content);\
   ck_assert_msg(func ## _get(&apd)==result,"Content isn't set or returned properly");\
   elm_shutdown();\
}\
END_TEST

#define TEST_UI_BLOCK_CONTENT(_func_,_block_)\
TEST_UI_BLOCK_CONTENT_TC(_func_,p,_block_,ui_block_add(parent),ui_block_add(parent),content)\
TEST_UI_BLOCK_CONTENT_TC(_func_,n1,_block_,NULL,ui_block_add(parent),NULL)\
TEST_UI_BLOCK_CONTENT_TC(_func_,n2,_block_,ui_block_add(parent),NULL,NULL)

TEST_UI_BLOCK_CONTENT(ui_block_widget_list,left_top)
TEST_UI_BLOCK_CONTENT(ui_block_signal_list,left_bottom)
TEST_UI_BLOCK_CONTENT(ui_block_state_list,bottom_left)
TEST_UI_BLOCK_CONTENT(ui_block_property,right_bottom)
TEST_UI_BLOCK_CONTENT(ui_block_graph_vision,right_top)
TEST_UI_BLOCK_CONTENT(ui_block_demo_view,bottom_right)

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
   Suite *suite = suite_create("ui_block_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_block_add_test_p);
   tcase_add_test(tcase, ui_block_add_test_n);
   tcase_add_test(tcase, ui_block_title_visible_n);
   tcase_add_test(tcase, ui_block_title_text_n);
   tcase_add_test(tcase, ui_block_title_text_p);
   tcase_add_test(tcase, ui_block_widget_list_test_p);
   tcase_add_test(tcase, ui_block_widget_list_test_n1);
   tcase_add_test(tcase, ui_block_widget_list_test_n2);
   tcase_add_test(tcase, ui_block_signal_list_test_p);
   tcase_add_test(tcase, ui_block_signal_list_test_n1);
   tcase_add_test(tcase, ui_block_signal_list_test_n2);
   tcase_add_test(tcase, ui_block_state_list_test_p);
   tcase_add_test(tcase, ui_block_state_list_test_n1);
   tcase_add_test(tcase, ui_block_state_list_test_n2);
   tcase_add_test(tcase, ui_block_property_test_p);
   tcase_add_test(tcase, ui_block_property_test_n1);
   tcase_add_test(tcase, ui_block_property_test_n2);
   tcase_add_test(tcase, ui_block_graph_vision_test_p);
   tcase_add_test(tcase, ui_block_graph_vision_test_n1);
   tcase_add_test(tcase, ui_block_graph_vision_test_n2);
   tcase_add_test(tcase, ui_block_demo_view_test_p);
   tcase_add_test(tcase, ui_block_demo_view_test_n1);
   tcase_add_test(tcase, ui_block_demo_view_test_n2);
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
   srunner_set_xml (runner, "test_uiblock.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
