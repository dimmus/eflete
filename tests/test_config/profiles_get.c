
#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup profiles_get
 * @{
 * Config
 * <TABLE>
 * @}
 */
/**
 * @addtogroup profiles_get
 * @{
 * <tr>
 * <td>profiles_get</td>
 * <td>profiles_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initializeapplication data
 * @step 3 load config
 *
 * @procedure
 * @step 1 call profiles_get
 * </td>
 * <td>void</td>
 * <td>not NULL pointer</td>
 * </tr>
 * @}
 */
EFL_START_TEST(profiles_get_test_p)
{
   Eina_List *profiles;

   // elm_init(0,0);
   // app_init();
   config_load();

   profiles = profiles_get();
   ck_assert_msg(profiles != NULL, "Config not loaded.");

   EINA_LIST_STRINGSHARE_FREE(profiles);
   config_shutdown();
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup profiles_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void profiles_get_test(TCase *tc)
{
   tcase_add_test(tc, profiles_get_test_p);
}
