
#include "test_history.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_change_add
 * @{
 * History
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_change_add
 * @{
 * <tr>
 * <td>history_change_add</td>
 * <td>history_change_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 create pseudo_group
 * @step 3 add history
 * @step 4 create change
 *
 * @procedure
 * @step 1 call history_change_add
  * </td>
 * <td></td>
 * <td>Not aborted</td>
 * </tr>
 * @}
 */
EFL_START_TEST (history_change_add_test_p)
{
   Change *change;
   // eina_init();
   int sense = 42;
   Group2 *group = mem_calloc(1, sizeof(Group2));
   group->edit_object = (Evas_Object *) &sense;
   change = change_add("test");

   History *history = history_add(group);
   history_change_add(history, change);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup history_change_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

void history_change_add_test(TCase *tc)
{
   tcase_add_test(tc, history_change_add_test_p);
}