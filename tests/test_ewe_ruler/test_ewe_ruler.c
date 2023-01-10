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