/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "test_ui_block.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("ui_block_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_block_add_test_p);
   tcase_add_test(tcase, ui_block_add_test_n);
   tcase_add_test(tcase, ui_block_title_visible_p);
   tcase_add_test(tcase, ui_block_title_visible_n);
   tcase_add_test(tcase, ui_block_content_visible_p);
   tcase_add_test(tcase, ui_block_content_visible_n);
   tcase_add_test(tcase, ui_block_title_text_p);
   tcase_add_test(tcase, ui_block_title_text_n);
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
   tcase_add_test(tcase, ui_block_history_test_p);
   tcase_add_test(tcase, ui_block_history_test_n1);
   tcase_add_test(tcase, ui_block_history_test_n2);
   tcase_add_test(tcase, ui_block_live_view_test_p);
   tcase_add_test(tcase, ui_block_live_view_test_n1);
   tcase_add_test(tcase, ui_block_live_view_test_n2);
   tcase_add_test(tcase, ui_block_ws_test_p);
   tcase_add_test(tcase, ui_block_ws_test_n1);
   tcase_add_test(tcase, ui_block_ws_test_n2);
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_ui_block.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
