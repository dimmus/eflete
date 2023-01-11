#include "test_config.h"

static const Efl_Test_Case etc[] = {
  { "Config_Get", config_get_test },
  { "Config_Init", config_init_test },
  { "Config_Load", config_load_test },
  { "Config_Panes_Size_Update", config_panes_sizes_data_update_test },
  { "Config_Save", config_save_test },
  { "Config_Shutdown", config_shutdown_test },
  { "Profile_Get", profile_get_test },
  { "Profile_Load", profile_load_test },
  { "Profile_Save", profile_save_test },
  { "Profiles_Get", profiles_get_test },
  { NULL, NULL }
};

SUITE_INIT(elm)
{
   ck_assert_int_eq(elm_init(0, 0), 1);
   ck_assert_int_eq(app_init(), EINA_TRUE);
}

SUITE_SHUTDOWN(elm)
{
   ck_assert_int_eq(elm_shutdown(), 0);
   ck_assert_int_eq(app_shutdown(), EINA_TRUE);
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Config and Profile", etc, SUITE_INIT_FN(elm), SUITE_SHUTDOWN_FN(elm));

   return (failed_count == 0) ? 0 : 255;
}
