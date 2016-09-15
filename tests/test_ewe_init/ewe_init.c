/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#include "test_ewe_init.h"

/**
 * @addtogroup ewe_test
 * @{
 * @addtogroup ewe_init
 * @{
 * ewe
 * <TABLE>
 * @}
 */

 /**
  * @addtogroup ewe_init
  * @{
  * <tr>
  * <td>ewe_init</td>
  * <td>TEST_CASE_NAME</td>
  * <td>
  * @procedure
  * @step 1 init ewe
  * @step 2 check that init counts >0
  * @step 3 shutdown ewe
  * @step 4 check that init counts decreased by 1
  * </td>
  * <td>0, 0</td>
  * <td>All checks passed</td>
  * <td>_REAL_RESULT_</td>
  * <td>_PASSED_</td>
  * </tr>
  * @}
  */
EFLETE_TEST(ewe_init_test_p)
{
M_ logger_init();
M_ int elm_inits = ewe_init(0, 0);
M_ ck_assert_msg(elm_inits > 0, "Init failure");
M_ ck_assert_msg(ewe_shutdown() == elm_inits - 1, "Shutdown failure");
}
END_TEST

/**
 * @addtogroup ewe_init
 * @{
 * </TABLE>
 * @}
 * @}
 */
