
#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_init
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_init
 * @{
 * <tr>
 * <td>config_init</td>
 * <td>config_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 *
 * @procedure
 * @step 1 call config_init
 * </td>
 * <td>(App_Data *)ap</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_init_test_p)
{
   // elm_init(0,0);
   // app_init();

   ck_assert_msg(config_get() == NULL, "Config not initialized.");

   // app_shutdown();
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_init
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_init_test(TCase *tc)
{
   tcase_add_test(tc, config_init_test_p);
}