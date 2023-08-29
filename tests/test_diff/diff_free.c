
#include "test_diff.h"

/**
 * @addtogroup diff_test
 * @{
 * @addtogroup diff_free
 * @{
 * Diff
 * <TABLE>
 * @}
 */

/* test stubs that return true and never will be actual called */
static Eina_Bool
_function_type_int_undo_return_true(Evas_Object *obj __UNUSED__, int val __UNUSED__)
{
   return true;
}
static Eina_Bool
_function_type_int_redo_return_true(Evas_Object *obj __UNUSED__, int val __UNUSED__)
{
   return true;
}
/**
 * @addtogroup diff_free
 * @{
 * <tr>
 * <td>diff_free</td>
 * <td>diff_free_test_p</td>
 * <td>
 * @precondition
 *
 * @procedure
 * @step 1 add one diff
 * @step 2 call diff_free
 * </td>
 * <td>Diff *diff</td>
 * <td>Not aborted</td>
 * </tr>
 * @}
 */
EFL_START_TEST (diff_free_test_p)
{
   Diff *d1;

   d1 = mem_calloc(1, sizeof(Diff));
   d1->undo.type = FUNCTION_TYPE_INT;
   d1->undo.function = _function_type_int_undo_return_true;
   d1->undo.args.type_i.i1 = 21;
   d1->redo.type = FUNCTION_TYPE_INT;
   d1->redo.function = _function_type_int_redo_return_true;
   d1->redo.args.type_i.i1 = 38;

   diff_free(d1);
}
EFL_END_TEST

/**
 * @addtogroup diff_free
 * @{
 * </TABLE>
 * @}
 * @}
 */

void diff_free_test(TCase *tc)
{
   tcase_add_test(tc, diff_free_test_p);
}
