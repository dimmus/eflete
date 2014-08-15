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

#include "test_preference.h"
#include "main_window.h"

/**
 * @addtogroup preference_test
 * @{
 * @addtogroup preference_add
 * @{
 * Preference
 * <TABLE>
 * @}
 */

/**
 * @addtogroup preference_add
 * @{
 * <tr>
 * <td>preference_add</td>
 * <td>preference_add_test_p</td>
 * <td>
 * @step 1 initialized efl and app
 * @step 2 main_window created
 *
 * @procedure
 * @step 1 Call preference_add
 * @step 2 Check returned pointer
 * </td>
 * <td>(Preference_Item) PREFERENCE_NONE</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(preference_add_test_p)
{
   Evas_Object *preference;
   App_Data *ap;

   elm_init(0,0);
   app_init();
   ap = app_create();
   ui_main_window_add(ap);

   preference = preference_add(PREFERENCE_NONE);
   ck_assert_msg(preference != NULL, "cannot create new Preference window");

   evas_object_del(preference);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preference_add
 * @{
 * <tr>
 * <td>preference_add</td>
 * <td>preference_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call preference_add
 * @step 2 Check returned pointer
 * </td>
 * <td>(Preference_Item) PREFERENCE_NONE</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(preference_add_test_n)
{
   Evas_Object *preference;
   elm_init(0,0);

   preference = preference_add(PREFERENCE_NONE);
   ck_assert_msg(preference == NULL, "Preference window was successfully created");

   evas_object_del(preference);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preference_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
