#include "test_ewe_ruler.h"

static const Efl_Test_Case etc[] = {
  { "Ruler_add", ewe_ruler_add_test },
  { "Ruler_scale_add", ewe_ruler_scale_add_test },
  { "Ruler_scale_del", ewe_ruler_scale_del_test },
  { "Ruler_scale_visible_get", ewe_ruler_scale_visible_get_test },
  { "Ruler_scale_visible_set", ewe_ruler_scale_visible_set_test },
  { "Ruler_scale_middle_mark_get", ewe_ruler_scale_middle_mark_get_test },
  { "Ruler_scale_middle_mark_set", ewe_ruler_scale_middle_mark_set_test },
  { "Ruler_horizontal_get", ewe_ruler_horizontal_get_test },
  { "Ruler_horizontal_set", ewe_ruler_horizontal_set_test },
  { "Ruler_step_get", ewe_ruler_step_get_test },
  { "Ruler_step_set", ewe_ruler_step_set_test },
  { "Ruler_value_step_get", ewe_ruler_value_step_get_test },
  { "Ruler_value_step_set", ewe_ruler_value_step_set_test },
  { "Ruler_zero_offset_get", ewe_ruler_zero_offset_get_test },
  { "Ruler_zero_offset_set", ewe_ruler_zero_offset_set_test },
  { "Ruler_format_get", ewe_ruler_format_get_test },
  { "Ruler_format_set", ewe_ruler_format_set_test },
  { "Ruler_style_get", ewe_ruler_style_get_test },
  { "Ruler_style_set", ewe_ruler_style_set_test },
  { "Ruler_marker_visible_get", ewe_ruler_marker_visible_get_test },
  { "Ruler_marker_visible_set", ewe_ruler_marker_visible_set_test },
  { "Ruler_marker_style_get", ewe_ruler_marker_style_get_test },
  { "Ruler_marker_style_set", ewe_ruler_marker_style_set_test },
  { "Ruler_marker_absolute_get", ewe_ruler_marker_absolute_get_test },
  { "Ruler_marker_absolute_set", ewe_ruler_marker_absolute_set_test },
  { "Ruler_marker_relative_get", ewe_ruler_marker_relative_get_test },
  { "Ruler_marker_relative_set", ewe_ruler_marker_relative_set_test },
  { "Ruler_marker_add", ewe_ruler_marker_add_test },
  { "Ruler_marker_del", ewe_ruler_marker_del_test },
  { NULL, NULL }
};

SUITE_INIT(elm)
{
   ck_assert_int_eq(logger_init(), 1);
   ck_assert_int_eq(elm_init(0, 0), 1);
   ck_assert_int_eq(app_init(), 1);
}

SUITE_SHUTDOWN(elm)
{
   ck_assert_int_eq(app_shutdown(), 1);
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