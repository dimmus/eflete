
#include "test_diff.h"

/**
 * @addtogroup diff_test
 * @{
 * @addtogroup diff_update
 * @{
 * Diff
 * <TABLE>
 * @}
 */

/* test stubs that return true and never will be actual called */
static Eina_Bool
_function_type_int_undo_return_true(Evas_Object *obj EINA_UNUSED, int val EINA_UNUSED)
{
   return true;
}
static Eina_Bool
_function_type_int_redo_return_true(Evas_Object *obj EINA_UNUSED, int val EINA_UNUSED)
{
   return true;
}
/**
 * @addtogroup diff_update
 * @{
 * <tr>
 * <td>diff_update</td>
 * <td>diff_update_test_p</td>
 * <td>
 * @precondition
 *
 * @procedure
 * @step 1 add two diffs with same undo/redo functions but different arg values
 * @step 2 call diff_update(d1, d2)
 * @step 3 check that d1.undo values are not changed
 * @step 3 check that d1.redo values are changed to d2.redo values
 * </td>
 * <td>Diff *d1, Diff *d2</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFL_START_TEST (diff_update_test_p)
{
   Diff *d1, *d2;

   d1 = mem_calloc(1, sizeof(Diff));
   d1->undo.type = FUNCTION_TYPE_INT;
   d1->undo.function = _function_type_int_undo_return_true;
   d1->undo.args.type_i.i1 = 21;
   d1->redo.type = FUNCTION_TYPE_INT;
   d1->redo.function = _function_type_int_redo_return_true;
   d1->redo.args.type_i.i1 = 38;

   d2 = mem_calloc(1, sizeof(Diff));
   d2->undo.type = FUNCTION_TYPE_INT;
   d2->undo.function = _function_type_int_undo_return_true;
   d2->undo.args.type_i.i1 = 38;
   d2->redo.type = FUNCTION_TYPE_INT;
   d2->redo.function = _function_type_int_redo_return_true;
   d2->redo.args.type_i.i1 = 42;

   diff_update(d1, d2);

   ck_assert(d1->undo.type == FUNCTION_TYPE_INT);
   ck_assert(d1->undo.function == _function_type_int_undo_return_true);
   ck_assert(d1->undo.args.type_i.i1 == 21);
   ck_assert(d1->redo.type == FUNCTION_TYPE_INT);
   ck_assert(d1->redo.function == _function_type_int_redo_return_true);
   ck_assert(d1->redo.args.type_i.i1 == 42);
}
EFL_END_TEST

/**
 * @addtogroup diff_update
 * @{
 * </TABLE>
 * @}
 * @}
 */

void diff_update_test(TCase *tc)
{
   tcase_add_test(tc, diff_update_test_p);
}