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

#include "test_eflete.h"

/**
 * @addtogroup eflete_test
 * @{
 * @addtogroup app_init
 * @{
 * eflete
 * <TABLE>
 * @}
 */

/**
 * @addtogroup app_init
 * @{
 * <tr>
 * <td>app_init</td>
 * <td>app_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 *
 * @procedure
 * @step 1 call app_init
 * @step 2 check returned pointer
 * </td>
 * <td>void</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST (app_init_test_p)
{
   // elm_init(0,0);

   ck_assert_msg(app_init() == EINA_TRUE, "failure: libraries was failed to init");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup app_init
 * @{
 * </TABLE>
 * @}
 * @}
 */

void app_init_test(TCase *tc)
{
   tcase_add_test(tc, app_init_test_p);
}

