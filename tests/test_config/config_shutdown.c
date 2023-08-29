
#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_shutdown
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_shutdown
 * @{
 * <tr>
 * <td>config_shutdown</td>
 * <td>config_shutdown_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call config_shutdown
 * </td>
 * <td>void</td>
 * <td>config is NULL</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_shutdown_test_p)
{
   // elm_init(0,0);
   // app_init();

   config_shutdown();
   ck_assert_msg(config_get() == NULL, "Config not initialized.");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_shutdown_test(TCase *tc)
{
   tcase_add_test(tc, config_shutdown_test_p);
}
