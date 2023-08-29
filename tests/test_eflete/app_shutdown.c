
#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup app_shutdown
 * @{
 * eflete
 * <TABLE>
 * @}
*/

/**
 * @addtogroup app_shutdown
 * @{
 * <tr>
 * <td>app_shutdown</td>
 * <td>app_shutdown_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 * @step 3 Create application data.
 *
 * @procedure
 * @step 1 call app_shutdown
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST (app_shutdown_test_p1)
{
   // elm_init(0,0);
   app_init();

   ck_assert_msg(app_shutdown() == EINA_TRUE, "Could not shutdown some of libraries");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup app_shutdown
 * @{
 * <tr>
 * <td>app_shutdown</td>
 * <td>app_shutdown_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize requred libraries.
 *
 * @procedure
 * @step 1 call app_shutdown
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFL_START_TEST (app_shutdown_test_p2)
{
   // elm_init(0,0);
   app_init();

   ck_assert_msg(app_shutdown() == EINA_TRUE, "Could not shutdown some of libraries");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup app_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */

void app_shutdown_test(TCase *tc)
{
   tcase_add_test(tc, app_shutdown_test_p1);
   tcase_add_test(tc, app_shutdown_test_p2);
}
