
#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_save
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_save
 * @{
 * <tr>
 * <td>config_save</td>
 * <td>config_save_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 init config
 * @step 3 load config
 * @step 4 create App_Data
 * @step 5 create Eflete window
 *
 * @procedure
 * @step 1 call config_save
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_save_test_p)
{
   // elm_init(0,0);
   // app_init();
   config_load();

   ck_assert_msg(config_save(), "Config not saved.");

   // app_shutdown();
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_save
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_save_test(TCase *tc)
{
   tcase_add_test(tc, config_save_test_p);
}
