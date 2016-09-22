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
EFLETE_TEST (history_del_test_p1)
{
   eina_init();
   int sense = 42;
   Group *group = mem_calloc(1, sizeof(Group));
   group->edit_object = (Evas_Object *) &sense;

   History *history = history_add(group);
   history_del(history);

   eina_shutdown();
}
END_TEST

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
EFLETE_TEST (history_del_test_p2)
{
   Change *change;
   eina_init();
   int sense = 42;
   Group *group = mem_calloc(1, sizeof(Group));
   group->edit_object = (Evas_Object *) &sense;
   change = change_add("test");

   History *history = history_add(group);
   history_change_add(history, change);
   history_del(history);

   eina_shutdown();
}
END_TEST

/**
 * @addtogroup history_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
