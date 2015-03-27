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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "test_ewe_ruler.h"

Suite* test_suite (void)
{
   Suite *suite = suite_create("ewe_ruler_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ewe_ruler_add_test_p);
   tcase_add_test(tcase, ewe_ruler_add_test_n);
   tcase_add_test(tcase, ewe_ruler_scale_add_test_p);
   tcase_add_test(tcase, ewe_ruler_scale_add_test_n);
   tcase_add_test(tcase, ewe_ruler_scale_del_test_p);
   tcase_add_test(tcase, ewe_ruler_scale_del_test_n);
   tcase_add_test(tcase, ewe_ruler_scale_visible_get_test_p);
   tcase_add_test(tcase, ewe_ruler_scale_visible_get_test_n);
   tcase_add_test(tcase, ewe_ruler_scale_visible_set_test_p);
   tcase_add_test(tcase, ewe_ruler_scale_visible_set_test_n);
   tcase_add_test(tcase, ewe_ruler_scale_middle_mark_get_test_p);
   tcase_add_test(tcase, ewe_ruler_scale_middle_mark_get_test_n);
   tcase_add_test(tcase, ewe_ruler_scale_middle_mark_set_test_p);
   tcase_add_test(tcase, ewe_ruler_scale_middle_mark_set_test_n);
   tcase_add_test(tcase, ewe_ruler_horizontal_get_test_p);
   tcase_add_test(tcase, ewe_ruler_horizontal_get_test_n);
   tcase_add_test(tcase, ewe_ruler_horizontal_set_test_p1);
   tcase_add_test(tcase, ewe_ruler_horizontal_set_test_p2);
   tcase_add_test(tcase, ewe_ruler_horizontal_set_test_n);
   tcase_add_test(tcase, ewe_ruler_step_set_test_p);
   tcase_add_test(tcase, ewe_ruler_step_set_test_n);
   tcase_add_test(tcase, ewe_ruler_step_get_test_p);
   tcase_add_test(tcase, ewe_ruler_step_get_test_n);
   tcase_add_test(tcase, ewe_ruler_value_step_set_test_p);
   tcase_add_test(tcase, ewe_ruler_value_step_set_test_n);
   tcase_add_test(tcase, ewe_ruler_value_step_get_test_p);
   tcase_add_test(tcase, ewe_ruler_value_step_get_test_n);
   tcase_add_test(tcase, ewe_ruler_zero_offset_set_test_p);
   tcase_add_test(tcase, ewe_ruler_zero_offset_set_test_n);
   tcase_add_test(tcase, ewe_ruler_zero_offset_get_test_p);
   tcase_add_test(tcase, ewe_ruler_zero_offset_get_test_n);
   tcase_add_test(tcase, ewe_ruler_format_get_test_p);
   tcase_add_test(tcase, ewe_ruler_format_get_test_n);
   tcase_add_test(tcase, ewe_ruler_format_set_test_p);
   tcase_add_test(tcase, ewe_ruler_format_set_test_n);
   tcase_add_test(tcase, ewe_ruler_style_get_test_p);
   tcase_add_test(tcase, ewe_ruler_style_get_test_n);
   tcase_add_test(tcase, ewe_ruler_style_set_test_p);
   tcase_add_test(tcase, ewe_ruler_style_set_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_visible_get_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_visible_get_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_visible_set_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_visible_set_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_style_get_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_style_get_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_style_set_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_style_set_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_absolute_get_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_absolute_get_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_absolute_set_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_absolute_set_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_relative_get_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_relative_get_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_relative_set_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_relative_set_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_add_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_add_test_n);
   tcase_add_test(tcase, ewe_ruler_marker_del_test_p);
   tcase_add_test(tcase, ewe_ruler_marker_del_test_n);
   suite_add_tcase(suite, tcase);
   return suite;
}

int main(void)
{
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_ewe_ruler.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
