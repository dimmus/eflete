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

#include "test_ui_workspace.h"

Suite* test_suite (void) {
   Suite *suite = suite_create("ui_workspace_test");
   TCase *tcase = tcase_create("TCase");

   tcase_add_test(tcase, workspace_add_test_p);
   tcase_add_test(tcase, workspace_edit_object_get_test_p);
   tcase_add_test(tcase, workspace_edit_object_get_test_p1);
   tcase_add_test(tcase, workspace_edit_object_part_add_test_p);
   tcase_add_test(tcase, workspace_edit_object_part_add_test_p1);
   tcase_add_test(tcase, workspace_edit_object_part_del_test_p);
   tcase_add_test(tcase, workspace_edit_object_part_state_set_test_n2);
   tcase_add_test(tcase, workspace_edit_object_part_state_set_test_p);
   tcase_add_test(tcase, workspace_edit_object_recalc_test_p);
   tcase_add_test(tcase, workspace_edit_object_visible_set_test_p);
   tcase_add_test(tcase, workspace_highlight_align_visible_get_test_p1);
   tcase_add_test(tcase, workspace_highlight_align_visible_get_test_p2);
   tcase_add_test(tcase, workspace_highlight_align_visible_set_test_p1);
   tcase_add_test(tcase, workspace_highlight_align_visible_set_test_p2);
   tcase_add_test(tcase, workspace_object_area_visible_get_test_p1);
   tcase_add_test(tcase, workspace_object_area_visible_get_test_p2);
   tcase_add_test(tcase, workspace_object_area_visible_set_test_p1);
   tcase_add_test(tcase, workspace_object_area_visible_set_test_p2);
   tcase_add_test(tcase, workspace_separate_mode_get_test_p1);
   tcase_add_test(tcase, workspace_separate_mode_get_test_p2);
   tcase_add_test(tcase, workspace_separate_mode_set_test_p1);
   tcase_add_test(tcase, workspace_separate_mode_set_test_p2);
   tcase_add_test(tcase, workspace_zoom_factor_set_test_p);
   tcase_add_test(tcase, workspace_zoom_factor_get_test_p);

   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void)
{
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_ui_workspace.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
