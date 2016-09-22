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

#include "test_history.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_undo_redo
 * @{
 * History
 * <TABLE>
 * @}
 */

static Evas_Object *pseudo_object;

/* test stubs that check args, return specified value, and report what function was called */
static int _function_type_int_undo_return_true_called = 0;
static Eina_Bool
_function_type_int_undo_return_true(Evas_Object *obj, Change *change __UNUSED__,
                                    Eina_Bool merge __UNUSED__, Eina_Bool apply __UNUSED__,
                                    int val)
{
   ck_assert(obj == pseudo_object);
   ck_assert(val == 42);
   _function_type_int_undo_return_true_called++;
   return true;
}

static int _function_type_int_redo_return_true_called = 0;
static Eina_Bool
_function_type_int_redo_return_true(Evas_Object *obj, Change *change __UNUSED__,
                                    Eina_Bool merge __UNUSED__, Eina_Bool apply __UNUSED__,
                                    int val)
{
   ck_assert(obj == pseudo_object);
   ck_assert(val == 24);
   _function_type_int_redo_return_true_called++;
   return true;
}


/**
 * @addtogroup history_undo_redo
 * @{
 * <tr>
 * <td>history_undo_redo</td>
 * <td>history_undo_redo_test_p</td>
 * <td>
 * @precondition
 * @step 1 init eina
 * @step 2 create pseudo_group
 * @step 3 add history
 * @step 4 add change to history
 *
 * @procedure
 * @step 1 call history_undo
 * @step 2 check that undo func was called once and with correct args
 * @step 3 call history_redo
 * @step 4 check that redo func was called once and with correct args
 * @step 5 add change to history
 * @step 6 call history_undo_all
 * @step 7 check that undo func was called twice and with correct args
  * </td>
 * <td></td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (history_undo_redo_test_p)
{
   Change *change;
   Diff *d1;
   eina_init();
   int sense = 42;
   Group *group = mem_calloc(1, sizeof(Group));
   pseudo_object = group->edit_object = (Evas_Object *) &sense;
   change = change_add("test");
   History *history = history_add(group);
   d1 = mem_calloc(1, sizeof(Diff));
   d1->undo.type = FUNCTION_TYPE_INT;
   d1->undo.function = _function_type_int_undo_return_true;
   d1->undo.args.type_i.i1 = 42;
   d1->redo.type = FUNCTION_TYPE_INT;
   d1->redo.function = _function_type_int_redo_return_true;
   d1->redo.args.type_i.i1 = 24;
   change_diff_add(change, d1);
   history_change_add(history, change);
   change = change_add("test");
   change_diff_add(change, d1);
   history_change_add(history, change);

   ck_assert(history_undo(history) == true);
   ck_assert(_function_type_int_undo_return_true_called == 1);
   ck_assert(history_redo(history) == true);
   ck_assert(_function_type_int_redo_return_true_called == 1);
   ck_assert(history_undo_all(history) == true);
   ck_assert(_function_type_int_undo_return_true_called == 3);

   eina_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo_redo
 * @{
 * </TABLE>
 * @}
 * @}
 */
