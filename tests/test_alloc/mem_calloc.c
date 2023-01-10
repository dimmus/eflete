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

#include "test_alloc.h"

/**
 * @addtogroup alloc_test
 * @{
 * @addtogroup mem_calloc
 * @{
 * Alloc
 * <TABLE>
 * @}
 */

/**
 * @addtogroup mem_calloc
 * @{
 * <tr>
 * <td>mem_calloc</td>
 * <td>mem_calloc_test_p</td>
 * <td>
 * @precondition
 * @step 1 nothing
 *
 * @procedure
 * @step 1 call mem_calloc for 1024 ints
 * @step 2 check returned pointer
 * @step 3 check for segfault by setting 1023th int into 42.
 * </td>
 * <td>1024, sizeof(int)</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFL_START_TEST (mem_calloc_test_p)
{
   int *mc = mem_calloc(1024, sizeof(int));
   int i;
   if (mc == NULL)
      ck_abort_msg("failure: cannot return pointer to the memory block allocated");
   for (i = 0; i < 1024; i++)
      if (mc[i] != 0)
         ck_abort_msg("failure: not all elements == 0");
   free(mc);
}
EFL_END_TEST

/**
 * @addtogroup mem_calloc
 * @{
 * </TABLE>
 * @}
 * @}
 */

void mem_calloc_test(TCase *tc)
{
   tcase_add_test(tc, mem_calloc_test_p);
}
