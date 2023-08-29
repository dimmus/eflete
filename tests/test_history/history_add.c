
#include "test_history.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_add
 * @{
 * History
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_add
 * @{
 * <tr>
 * <td>history_add</td>
 * <td>history_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 create pseudo_group
 *
 * @procedure
 * @step 1 call history_add
  * </td>
 * <td></td>
 * <td>Not NULL returned</td>
 * </tr>
 * @}
 */
EFL_START_TEST (history_add_test_p)
{
   // eina_init();
   int sense = 42;
   Group2 *group = mem_calloc(1, sizeof(Group2));
   group->edit_object = (Evas_Object *) &sense;

   ck_assert(history_add(group) != NULL);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup history_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

void history_add_test(TCase *tc)
{
   tcase_add_test(tc, history_add_test_p);
}