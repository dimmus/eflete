
#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup profile_save
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup profile_save
 * @{
 * <tr>
 * <td>profile_save</td>
 * <td>profile_save_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 load config
 *
 * @procedure
 * @step 1 call profile_save
 * </td>
 * <td>(const char *)"default"</td>
 * <td>Profile will be loaded</td>
 * </tr>
 * @}
 */
EFL_START_TEST(profile_save_test_p)
{

   // elm_init(0,0);
   // app_init();

   config_load();

   ck_assert_msg(profile_save("default"), "Profile not saved.");

   config_shutdown();
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup profile_save
 * @{
 * </TABLE>
 * @}
 * @}
 */

void profile_save_test(TCase *tc)
{
   tcase_add_test(tc, profile_save_test_p);
}