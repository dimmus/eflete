
#include "test_change.h"

static const Efl_Test_Case etc[] = {
  { "Add", change_add_test },
  { "Free", change_free_test },
  { "Diff_Add", change_diff_add_test },
  { "Diff_Merge_Add", change_diff_merge_add_test },
  { NULL, NULL }
};

SUITE_INIT(eina)
{
   ck_assert_int_eq(eina_init(), 1);
}

SUITE_SHUTDOWN(eina)
{
   ck_assert_int_eq(eina_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Change", etc, SUITE_INIT_FN(eina), SUITE_SHUTDOWN_FN(eina));

   return (failed_count == 0) ? 0 : 255;
}
