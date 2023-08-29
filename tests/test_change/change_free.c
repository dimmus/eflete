
#include "test_change.h"

/**
 * @addtogroup change_test
 * @{
 * @addtogroup change_free
 * @{
 * Change
 * <TABLE>
 * @}
 */

/**
 * @addtogroup change_free
 * @{
 * <tr>
 * <td>change_free</td>
 * <td>change_free_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 add change
 *
 * @procedure
 * @step 1 call change_free
 * </td>
 * <td>Change *change</td>
 * <td>Not aborted</td>
 * </tr>
 * @}
 */
EFL_START_TEST (change_free_test_p)
{
   Change *change;
   // eina_init();
   change = change_add("test");

   change_free(change);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup change_free
 * @{
 * </TABLE>
 * @}
 * @}
 */

void change_free_test(TCase *tc)
{
   tcase_add_test(tc, change_free_test_p);
}