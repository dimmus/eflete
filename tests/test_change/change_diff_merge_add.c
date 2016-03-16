/**
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "test_change.h"

/**
 * @addtogroup change_test
 * @{
 * @addtogroup change_diff_merge_add
 * @{
 * Alloc
 * <TABLE>
 * @}
 */

static Evas_Object *pseudo_object;

/* test stubs that check args, return specified value, and report what function was called */
static int _function_type_int_undo_return_true_called = 0;
static Eina_Bool
_function_type_int_undo_return_true(Evas_Object *obj, Change *change __UNUSED__, Eina_Bool merge __UNUSED__, int val)
{
   ck_assert(obj == pseudo_object);
   ck_assert(val == 42);
   _function_type_int_undo_return_true_called++;
   return true;
}

static int _function_type_int_redo_return_true_called = 0;
static Eina_Bool
_function_type_int_redo_return_true(Evas_Object *obj, Change *change __UNUSED__, Eina_Bool merge __UNUSED__, int val)
{
   ck_assert(obj == pseudo_object);
   ck_assert(val == 24);
   _function_type_int_redo_return_true_called++;
   return true;
}
/**
 * @addtogroup change_diff_merge_add
 * @{
 * <tr>
 * <td>change_diff_merge_add</td>
 * <td>change_diff_merge_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 add change
 * @step 3 create two diffs with same functions but different redo arg values
 *
 * @procedure
 * @step 1 add two diff with change_diff_merge_add
 * @step 2 change_undo should return true
 * @step 3 check that undo func was called once
 * @step 4 change_redo should return true
 * @step 5 check that redo func was called once
 * @step 6 check that redo func was called with value from second diff
 * </td>
 * <td>change</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (change_diff_merge_add_test_p)
{
   eina_init();
   int sense = 42;
   Diff *d1, *d2;
   Change *change;
   /* We don't need to create actual object. Only pointer to it is needed for checks */
   pseudo_object = (Evas_Object *) &sense;
   change = change_add("test");

   d1 = mem_calloc(1, sizeof(Diff));
   d1->undo.type = FUNCTION_TYPE_INT;
   d1->undo.function = _function_type_int_undo_return_true;
   d1->undo.args.type_i.i1 = 42;
   d1->redo.type = FUNCTION_TYPE_INT;
   d1->redo.function = _function_type_int_redo_return_true;
   d1->redo.args.type_i.i1 = 42;

   d2 = mem_calloc(1, sizeof(Diff));
   d2->undo.type = FUNCTION_TYPE_INT;
   d2->undo.function = _function_type_int_undo_return_true;
   d2->undo.args.type_i.i1 = 42;
   d2->redo.type = FUNCTION_TYPE_INT;
   d2->redo.function = _function_type_int_redo_return_true;
   d2->redo.args.type_i.i1 = 24;

   change_diff_merge_add(change, d1);
   change_diff_merge_add(change, d2);
   ck_assert(change_undo(pseudo_object, change) == true);
   ck_assert(_function_type_int_undo_return_true_called == 1);
   ck_assert(change_redo(pseudo_object, change) == true);
   ck_assert(_function_type_int_redo_return_true_called == 1);
   eina_shutdown();
}
END_TEST

/**
 * @addtogroup change_diff_merge_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
