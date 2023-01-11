#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_get
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_get
 * @{
 * <tr>
 * <td>config_get</td>
 * <td>config_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call config_get
 * </td>
 * <td>void</td>
 * <td>NULL</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_get_test_p1)
{
   // elm_init(0,0);

   ck_assert_msg(config_get() == NULL, "Config not NULL");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_get
 * @{
 * <tr>
 * <td>config_get</td>
 * <td>config_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application data
 * @step 3 load config
 *
 * @procedure
 * @step 1 call config_get
 * </td>
 * <td>void</td>
 * <td>not NULL pointer</td>
 * </tr>
 * @}
 */
EFL_START_TEST(config_get_test_p2)
{
   // elm_init(0,0);
   app_init();
   config_load();

   ck_assert_msg(config_get() != NULL, "Config not loaded.");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_get_test(TCase *tc)
{
   tcase_add_test(tc, config_get_test_p1);
   tcase_add_test(tc, config_get_test_p2);
}
