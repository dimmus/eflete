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

#include "test_ui_main_window.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("ui_main_window_test");
   TCase *tcase = tcase_create("TCase");

   tcase_add_test(tcase, ui_main_window_add_test_p);
   tcase_add_test(tcase, ui_main_window_add_test_n);

   tcase_add_test(tcase, ui_main_window_del_test_p);
   tcase_add_test(tcase, ui_main_window_del_test_n);

   tcase_add_test(tcase, ui_panes_add_test_p);
   tcase_add_test(tcase, ui_panes_add_test_n1);
   tcase_add_test(tcase, ui_panes_add_test_n2);

   tcase_add_test(tcase, ui_panes_show_test_p);
   tcase_add_test(tcase, ui_panes_show_test_n1);
   tcase_add_test(tcase, ui_panes_show_test_n2);

   tcase_add_test(tcase, ui_panes_hide_test_p);
   tcase_add_test(tcase, ui_panes_hide_test_n1);
   tcase_add_test(tcase, ui_panes_hide_test_n2);

   tcase_add_test(tcase, ui_menu_add_test_p);
   tcase_add_test(tcase, ui_menu_add_test_n1);
   tcase_add_test(tcase, ui_menu_add_test_n2);

   tcase_add_test(tcase, new_theme_create_test_p1);
   tcase_add_test(tcase, new_theme_create_test_p2);
   tcase_add_test(tcase, new_theme_create_test_n);

   tcase_add_test(tcase, register_callbacks_test_p);
   tcase_add_test(tcase, register_callbacks_test_n1);
   tcase_add_test(tcase, register_callbacks_test_n2);

   tcase_add_test(tcase, ui_edj_load_test_p);
   tcase_add_test(tcase, ui_edj_load_test_n);

   tcase_add_test(tcase, ui_menu_base_disabled_set_test_p);
   tcase_add_test(tcase, ui_menu_base_disabled_set_test_n);

   tcase_add_test(tcase, ui_menu_disable_set_test_p);
   tcase_add_test(tcase, ui_menu_disable_set_test_n1);
   tcase_add_test(tcase, ui_menu_disable_set_test_n2);

   tcase_add_test(tcase, ui_menu_locked_set_test_p);
   tcase_add_test(tcase, ui_menu_locked_set_test_n);

   tcase_add_test(tcase, add_callbacks_wd_test_p1);
   tcase_add_test(tcase, add_callbacks_wd_test_p2);
   tcase_add_test(tcase, add_callbacks_wd_test_n);

/* tcase_add_test(tcase, ui_style_clicked_test_p);
   tcase_add_test(tcase, ui_style_clicked_test_n1);
   tcase_add_test(tcase, ui_style_clicked_test_n2);   TODO: separate functional tests*/

   suite_add_tcase(suite, tcase);

   return suite;
}

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
