/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "test_diff.h"

/**
 * @addtogroup diff_test
 * @{
 * @addtogroup diff_undo_redo
 * @{
 * Diff
 * <TABLE>
 * @}
 */

static Evas_Object *pseudo_object;

/**
 * @addtogroup diff_undo_redo
 * @{
 * <tr>
 * <td>diff_undo_redo</td>
 * <td>diff_undo_redo_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init pseudo evas object
 *
 * @procedure
 * @step 1 fill diff with FUNC_TYPE_NONE
 * @step 2 call undo/redo
 * @step 3 check that true returned
 * </td>
 * <td>Evas_Object *obj, Diff *diff</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFL_START_TEST (diff_undo_redo_test_p1)
{
   int sense = 42;
   Diff diff;
   /* We don't need to create actual object. Only pointer to it is needed for checks */
   pseudo_object = (Evas_Object *) &sense;

   diff.undo.type = FUNCTION_TYPE_NONE;
   diff.redo.type = FUNCTION_TYPE_NONE;
   ck_assert(diff_undo(pseudo_object, &diff) == true);
   ck_assert(diff_redo(pseudo_object, &diff) == true);
}
EFL_END_TEST

/* test stubs that check args, return specified value, and report what function was called */
static Eina_Bool _function_type_int_undo_return_true_called = false;
static Eina_Bool
_function_type_int_undo_return_true(Evas_Object *obj, Change *change __UNUSED__,
                                    Eina_Bool merge __UNUSED__, Eina_Bool apply __UNUSED__,
                                    int val)
{
   ck_assert(obj == pseudo_object);
   ck_assert(val == 42);
   _function_type_int_undo_return_true_called = true;
   return true;
}

static Eina_Bool _function_type_int_redo_return_true_called = false;
static Eina_Bool
_function_type_int_redo_return_true(Evas_Object *obj, Change *change __UNUSED__,
                                    Eina_Bool merge __UNUSED__, Eina_Bool apply __UNUSED__,
                                    int val)
{
   ck_assert(obj == pseudo_object);
   ck_assert(val == 42);
   _function_type_int_redo_return_true_called = true;
   return true;
}

/**
 * @addtogroup diff_undo_redo
 * @{
 * <tr>
 * <td>diff_undo_redo</td>
 * <td>diff_undo_redo_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init pseudo evas object
 *
 * @procedure
 * @step 1 fill diff with FUNC_TYPE_INT to return true
 * @step 2 call undo/redo
 * @step 3 check that true returned
 * @step 4 check that correct functions were called
 * @step 5 fill diff with FUNC_TYPE_INT to return false
 * @step 6 call undo/redo
 * @step 7 check that false returned
 * @step 8 check that correct functions were called
 * </td>
 * <td>Evas_Object *obj, Diff *diff</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFL_START_TEST (diff_undo_redo_test_p2)
{
   int sense = 42;
   Diff diff;
   /* We don't need to create actual object. Only pointer to it is needed for checks */
   pseudo_object = (Evas_Object *) &sense;

   diff.undo.type = FUNCTION_TYPE_INT;
   diff.undo.function = _function_type_int_undo_return_true;
   diff.undo.args.type_i.i1 = 42;
   diff.redo.type = FUNCTION_TYPE_INT;
   diff.redo.function = _function_type_int_redo_return_true;
   diff.redo.args.type_i.i1 = 42;
   ck_assert(diff_undo(pseudo_object, &diff) == true);
   ck_assert(_function_type_int_undo_return_true_called);
   ck_assert(diff_redo(pseudo_object, &diff) == true);
   ck_assert(_function_type_int_redo_return_true_called);
}
EFL_END_TEST

/**
 * @addtogroup diff_undo_redo
 * @{
 * </TABLE>
 * @}
 * @}
 */

void diff_undo_redo_test(TCase *tc)
{
   tcase_add_test(tc, diff_undo_redo_test_p1);
   tcase_add_test(tc, diff_undo_redo_test_p2);
}