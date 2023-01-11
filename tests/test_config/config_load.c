#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_load
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_load
 * @{
 * <tr>
 * <td>config_load</td>
 * <td>config_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create App_Data
 * @step 3 create Eflete window
 *
 * @procedure
 * @step 1 call config_load
 * </td>
 * <td>void</td>
 * <td>Config  will be loaded</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_load_test_p)
{
   Config *config;

   // elm_init(0,0);
   // app_init();
   config_load();

   config = config_get();
   ck_assert_msg(config != NULL, "Config not been loaded.");

   // app_shutdown();
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_load
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_load_test(TCase *tc)
{
   tcase_add_test(tc, config_load_test_p);
}