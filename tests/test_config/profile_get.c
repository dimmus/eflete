
#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup profile_get
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup profile_get
 * @{
 * <tr>
 * <td>profile_get</td>
 * <td>profile_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 load config
 *
 * @procedure
 * @step 1 call profile_get
 * </td>
 * <td>void</td>
 * <td>not NULL pointer</td>
 * </tr>
 * @}
 */
EFL_START_TEST(profile_get_test_p2)
{
   // elm_init(0,0);
   // app_init();

   config_load();
   ck_assert_msg(profile_get() != NULL, "Profile not loaded.");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup profile_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void profile_get_test(TCase *tc)
{
   tcase_add_test(tc, profile_get_test_p2);
}