
#include "test_history.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_del
 * @{
 * History
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_del
 * @{
 * <tr>
 * <td>history_del</td>
 * <td>history_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 create pseudo_group
 * @step 3 add history
 *
 * @procedure
 * @step 1 call history_del
  * </td>
 * <td></td>
 * <td>Not aborted</td>
 * </tr>
 * @}
 */
EFL_START_TEST (history_del_test_p1)
{
   // eina_init();
   int sense = 42;
   Group2 *group = mem_calloc(1, sizeof(Group2));
   group->edit_object = (Evas_Object *) &sense;

   History *history = history_add(group);
   history_del(history);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup history_del
 * @{
 * <tr>
 * <td>history_del</td>
 * <td>history_del_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 create pseudo_group
 * @step 3 add history
 * @step 4 add change to history
 *
 * @procedure
 * @step 1 call history_del
  * </td>
 * <td></td>
 * <td>Not aborted</td>
 * </tr>
 * @}
 */
EFL_START_TEST (history_del_test_p2)
{
   Change *change;
   // eina_init();
   int sense = 42;
   Group2 *group = mem_calloc(1, sizeof(Group2));
   group->edit_object = (Evas_Object *) &sense;
   change = change_add("test");

   History *history = history_add(group);
   history_change_add(history, change);
   history_del(history);

   // eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup history_del
 * @{
 * </TABLE>
 * @}
 * @}
 */

void history_del_test(TCase *tc)
{
   tcase_add_test(tc, history_del_test_p1);
   tcase_add_test(tc, history_del_test_p2);
}
