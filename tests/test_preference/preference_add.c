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

/**
 * @addtogroup preference_test
 * @{
 * @addtogroup preferences_window_add
 * @{
 * preferences
 * <TABLE>
 * @}
 */

/**
 * @addtogroup preferences_window_add
 * @{
 * <tr>
 * <td>preferences_window_add</td>
 * <td>preferences_window_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 * @step 4 load project
 *
 * @procedure
 * @step 1 call preferences_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>Project *project</td>
 * <td>Not NULL Pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (preferences_window_add_test_p1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *preferences;
   app = app_data_get();
   ui_main_window_add(app);
   app->project = pm_open_project_edj("./edj_build/preferences_window_add.edj");

   preferences = preferences_window_add(app->project);
   ck_assert_msg(preferences != NULL,
                 "Cannot create prefernces window");

   evas_object_del(preferences);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preferences_window_add
 * @{
 * <tr>
 * <td>preferences_window_add</td>
 * <td>preferences_window_add_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 create main window
 *
 * @procedure
 * @step 1 call preferences_window_add
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL</td>
 * <td>NULL Pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (preferences_window_add_test_p2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   Evas_Object *preferences;
   app = app_data_get();
   ui_main_window_add(app);

   preferences = preferences_window_add(NULL);
   ck_assert_msg(preferences != NULL,
                 "Cannot create preferences window for NULL project");

   evas_object_del(preferences);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preferences_window_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
