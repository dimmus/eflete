
#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup app_init
 * @{
 * eflete
 * <TABLE>
 * @}
 */

/**
 * @addtogroup app_init
 * @{
 * <tr>
 * <td>app_init</td>
 * <td>app_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call app_init
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST (app_init_test_p)
{
   // elm_init(0,0);

   ck_assert_msg(app_init() == EINA_TRUE, "failure: libraries was failed to init");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup app_init
 * @{
 * </TABLE>
 * @}
 * @}
 */

void app_init_test(TCase *tc)
{
   tcase_add_test(tc, app_init_test_p);
}

