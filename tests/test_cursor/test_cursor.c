
#include "test_cursor.h"

static const Efl_Test_Case etc[] = {
  { "Main_Set", cursor_main_set_test },
  { "Main_Get", cursor_main_get_test },
  { "Type_Set", cursor_type_set_test },
  { "Type_Get", cursor_type_get_test },
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
                                           "Cursor", etc, SUITE_INIT_FN(elm), SUITE_SHUTDOWN_FN(elm));

   return (failed_count == 0) ? 0 : 255;
}
