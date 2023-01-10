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

#include "test_logger.h"

/**
 * @addtogroup logger_test
 * @{
 * @addtogroup logger_shutdown
 * @{
 * logger
 * <TABLE>
 * @}
 */

/**
 * @addtogroup logger_shutdown
 * @{
 * <tr>
 * <td>logger_shutdown</td>
 * <td>logger_shutdown_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize logger.
 *
 * @procedure
 * @step 1 Call function logger_shutdown
 * @step 2 Check returned value.
 * </td>
 * <td>void</td>
 * <td> -1 </td>
 * </tr>
 * @}
 */
EFL_START_TEST(logger_shutdown_test_p)
{
   int ret = -1;

   logger_init();

   ret = logger_shutdown();
   ck_assert_msg(ret == -1 , "Failed shutdown logger");

   eina_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup logger_shutdown
 * @{
 * </TABLE>
 * @}
 * @}
 */

void logger_shutdown_test(TCase *tc)
{
   tcase_add_test(tc, logger_shutdown_test_p);
}
