#include "test_config.h"

/**
 * @addtogroup config_test
 * @{
 * @addtogroup config_panes_sizes_data_update
 * @{
 * Config
 * <TABLE>
 * @}
 */

/**
 * @addtogroup config_panes_sizes_data_update
 * @{
 * <tr>
 * <td>config_panes_sizes_data_update</td>
 * <td>config_panes_sizes_data_update_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 *
 * @procedure
 * @step 1 Call config_panes_sizes_data_update(NULL)
 * @step 2 Check returned value
 * </td>
 * <td></td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST (config_panes_sizes_data_update_test_p)
{
   // elm_init(0, 0);
   Eina_Bool result = EINA_FALSE;

   // app_init();
   config_load();

   result = config_panes_sizes_data_update();
   ck_assert_msg(result == EINA_TRUE, "Could not update Panes sizes data");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup config_panes_sizes_data_update
 * @{
 * </TABLE>
 * @}
 * @}
 */

void config_panes_sizes_data_update_test(TCase *tc)
{
   tcase_add_test(tc, config_panes_sizes_data_update_test_p);
}
