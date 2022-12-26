#include "test_ewe_ruler.h"

static const Efl_Test_Case etc[] = {
  { "Ewe_Ruler", ewe_ruler_add_test },
  { NULL, NULL }
};

SUITE_INIT(elm)
{
   ck_assert_int_eq(elm_init(0, 0), 1);
}

SUITE_SHUTDOWN(elm)
{
   ck_assert_int_eq(elm_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Ewe_Ruler", etc, SUITE_INIT_FN(elm), SUITE_SHUTDOWN_FN(elm));

   return (failed_count == 0) ? 0 : 255;
}


// Suite* test_suite (void)
// {
//    Suite *suite = suite_create("ewe_ruler_test");
//    TCase *tcase = tcase_create("TCase");
//    tcase_add_test(tcase, ewe_ruler_add_test_p);
   // tcase_add_test(tcase, ewe_ruler_add_test_n);
   // tcase_add_test(tcase, ewe_ruler_scale_add_test_p);
   // tcase_add_test(tcase, ewe_ruler_scale_add_test_n);
   // tcase_add_test(tcase, ewe_ruler_scale_del_test_p);
   // tcase_add_test(tcase, ewe_ruler_scale_del_test_n);
   // tcase_add_test(tcase, ewe_ruler_scale_visible_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_scale_visible_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_scale_visible_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_scale_visible_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_scale_middle_mark_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_scale_middle_mark_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_scale_middle_mark_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_scale_middle_mark_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_horizontal_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_horizontal_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_horizontal_set_test_p1);
   // tcase_add_test(tcase, ewe_ruler_horizontal_set_test_p2);
   // tcase_add_test(tcase, ewe_ruler_horizontal_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_step_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_step_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_step_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_step_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_value_step_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_value_step_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_value_step_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_value_step_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_zero_offset_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_zero_offset_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_zero_offset_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_zero_offset_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_format_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_format_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_format_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_format_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_style_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_style_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_style_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_style_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_visible_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_visible_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_visible_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_visible_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_style_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_style_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_style_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_style_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_absolute_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_absolute_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_absolute_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_absolute_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_relative_get_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_relative_get_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_relative_set_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_relative_set_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_add_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_add_test_n);
   // tcase_add_test(tcase, ewe_ruler_marker_del_test_p);
   // tcase_add_test(tcase, ewe_ruler_marker_del_test_n);
//    suite_add_tcase(suite, tcase);
//    return suite;
// }

// int main(void)
// {
//    int number_failed;
//    Suite *suite = test_suite();
//    SRunner *runner = srunner_create(suite);
//    srunner_set_xml (runner, "test_ewe_ruler.xml");
//    srunner_run_all(runner, CK_VERBOSE);
//    number_failed = srunner_ntests_failed(runner);
//    srunner_free(runner);
//    return number_failed;
// }
