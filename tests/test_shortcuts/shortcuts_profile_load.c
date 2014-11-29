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

#include "test_shortcuts.h"

/**
 * @addtogroup shortcuts_test
 * @{
 * @addtogroup shortcuts_profile_load
 * @{
 * Shortcuts
 * <TABLE>
 * @}
 */

/**
 * @addtogroup shortcuts_profile_load
 * @{
 * <tr>
 * <td>shortcuts_profile_load</td>
 * <td>shortcuts_profile_load_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize and get application data
 * @step 3 load "default" profile
 * @step 4 get loaded "default" profile
 *
 * @procedure
 * @step 1 call shortcuts_profile_load
 * </td>
 * <td>(App_Data *)app, (Profile *)profile</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_profile_load_test_p)
{
   App_Data *ap;

   elm_init(0,0);
   app_init();
   ap = app_data_get();
   profile_load("default");

   ck_assert_msg(shortcuts_profile_load(ap, profile_get()), "Cant load profile.");

   shortcuts_shutdown(ap);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_profile_load
 * @{
 * <tr>
 * <td>shortcuts_profile_load</td>
 * <td>shortcuts_profile_load_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize and get application data
 * @step 3 create empty "profile" structure
 *
 * @procedure
 * @step 1 call shortcuts_profile_load
 * </td>
 * <td>(App_Data *)app, (Profile *)profile</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_profile_load_test_n1)
{
   App_Data *ap;
   Profile *prof = NULL;

   elm_init(0,0);
   app_init();
   ap = app_data_get();
   prof = mem_calloc(1, sizeof(Profile));

   ck_assert_msg(!shortcuts_profile_load(ap, prof), "Shortcuts were loaded.");

   shortcuts_shutdown(ap);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_profile_load
 * @{
 * <tr>
 * <td>shortcuts_profile_load</td>
 * <td>shortcuts_profile_load_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize and get application data
 * @step 3 create empty "profile" structure
 *
 * @procedure
 * @step 1 call shortcuts_profile_load with first parameter as NULL
 * @step 2 check that function returned EINA_FALSE
 * @step 3 call shortcuts_profile_load with second parameter as NULL
 * @step 4 check that function returned EINA_FALSE
 * </td>
 * <td>NULL, NULL</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(shortcuts_profile_load_test_n2)
{
   App_Data *app;

   elm_init(0,0);
   app_init();
   app = app_data_get();
   profile_load("default");

   ck_assert_msg(!shortcuts_profile_load(NULL, profile_get()), "Shortcuts were loaded.");
   ck_assert_msg(!shortcuts_profile_load(app, NULL), "Shortcuts were loaded.");

   shortcuts_shutdown(app);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup shortcuts_profile_load
 * @{
 * </TABLE>
 * @}
 * @}
 */
