
#include "test_change.h"

/**
 * @addtogroup change_test
 * @{
 * @addtogroup change_add
 * @{
 * Change
 * <TABLE>
 * @}
 */

/**
 * @addtogroup change_add
 * @{
 * <tr>
 * <td>change_add</td>
 * <td>change_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 *
 * @procedure
 * @step 1 call change_add
 * </td>
 * <td>"test"</td>
 * <td>not NULL returned</td>
 * </tr>
 * @}
 */
EFL_START_TEST (change_add_test_p)
{
   // eina_init();

   ck_assert(change_add("test") != NULL);
   ck_assert(change_add(NULL) != NULL);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup change_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

void change_add_test(TCase *tc)
{
   tcase_add_test(tc, change_add_test_p);
}
