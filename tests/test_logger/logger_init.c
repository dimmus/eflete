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
 * @addtogroup logger_init
 * @{
 * logger
 * <TABLE>
 * @}
 */

/**
 * @addtogroup logger_init
 * @{
 * <tr>
 * <td>logger_init</td>
 * <td>logger_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 NONE
 *
 * @procedure
 * @step 1 Call function logger_init
 * @step 2 Check returned value.
 * </td>
 * <td>void</td>
 * <td>Returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(logger_init_test_p)
{
   Eina_Bool ret = EINA_FALSE;

   ret = logger_init();
   ck_assert_msg(ret == EINA_TRUE, "Failed initialize logger");

}
END_TEST

/**
 * @addtogroup logger_init
 * @{
 * </TABLE>
 * @}
 * @}
 */
