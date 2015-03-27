/* Elementary Widgets Extension
* Copyright (C) 2014 Samsung Electronics.
*
* This file is part of Elementary Widgets Extension.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; If not, see www.gnu.org/licenses/lgpl.html.
*/
#include "test_ewe_combobox.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("ewe_init_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ewe_combobox_add_test_p);
   tcase_add_test(tcase, ewe_combobox_add_test_n);
   tcase_add_test(tcase, ewe_combobox_item_add_test_p1);
   tcase_add_test(tcase, ewe_combobox_item_add_test_p2);
   tcase_add_test(tcase, ewe_combobox_item_add_test_n);
   tcase_add_test(tcase, ewe_combobox_item_del_test_p);
   tcase_add_test(tcase, ewe_combobox_item_del_test_n);
   tcase_add_test(tcase, ewe_combobox_item_title_get_test_p);
   tcase_add_test(tcase, ewe_combobox_item_title_get_test_n1);
   tcase_add_test(tcase, ewe_combobox_item_title_get_test_n2);
   tcase_add_test(tcase, ewe_combobox_item_title_set_test_p1);
   tcase_add_test(tcase, ewe_combobox_item_title_set_test_p2);
   tcase_add_test(tcase, ewe_combobox_item_title_set_test_p3);
   tcase_add_test(tcase, ewe_combobox_item_title_set_test_n1);
   tcase_add_test(tcase, ewe_combobox_item_title_set_test_n2);
   tcase_add_test(tcase, ewe_combobox_items_list_get_test_p);
   tcase_add_test(tcase, ewe_combobox_items_list_get_test_n);
   tcase_add_test(tcase, ewe_combobox_select_item_set_test_p);
   tcase_add_test(tcase, ewe_combobox_select_item_set_test_n1);
   tcase_add_test(tcase, ewe_combobox_select_item_set_test_n2);
   tcase_add_test(tcase, ewe_combobox_select_item_get_test_p1);
   tcase_add_test(tcase, ewe_combobox_select_item_get_test_p2);
   tcase_add_test(tcase, ewe_combobox_select_item_get_test_n);
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml(runner, "test_ewe_combobox.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
