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
#include "test_common.h"

/**
 * @addtogroup preference_test
 * @{
 * @addtogroup preferences_project_autosave_update
 * @{
 * preferences
 * <TABLE>
 * @}
 */

/**
 * @addtogroup preferences_project_autosave_update
 * @{
 * <tr>
 * <td>preferences_project_autosave_update</td>
 * <td>preferences_project_autosave_update_test_p</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 init app
 * @step 3 load profile
 * @step 4 load project
 *
 * @procedure
 * @step 1 call preferences_project_autosave_update
 * @step 2 check returned value
 *
 * </td>
 * <td>Project *project</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFLETE_TEST (preferences_project_autosave_update_test_p)
{
   elm_init(0,0);
   app_init();
   setup("preferences_project_autosave_update_test_p");

   Eina_Bool result;
   App_Data *app;
   app = app_data_get();
   profile_load("default");
   app->project = pm_project_open("./preferences_project_autosave_update_test_p/preferences_project_autosave_update_test_p.pro");

   result = preferences_project_autosave_update(app->project);
   ck_assert_msg(result == EINA_TRUE, "Can not register autosave callback");

   app_shutdown();
   teardown("./preferences_project_autosave_update_test_p");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preferences_project_autosave_update
 * @{
 * <tr>
 * <td>preferences_project_autosave_update</td>
 * <td>preferences_project_autosave_update_test_n1</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 load profile
 *
 * @procedure
 * @step 1 call preferences_project_autosave_update(NULL)
 * @step 2 check returned value
 *
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */

EFLETE_TEST (preferences_project_autosave_update_test_n1)
{
   elm_init(0,0);
   Eina_Bool result;
   app_init();
   profile_load("default");

   result = preferences_project_autosave_update(NULL);
   ck_assert_msg(result == EINA_FALSE, "The autosave should not be registered"
                                       "if project is not loaded");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preferences_project_autosave_update
 * @{
 * <tr>
 * <td>preferences_project_autosave_update</td>
 * <td>preferences_project_autosave_update_test_n2</td>
 * <td>
 * @precondition
 * @step 1 init elemantary
 * @step 2 open project
 *
 * @procedure
 * @step 1 call preferences_project_autosave_update(project)
 * @step 2 check returned value
 *
 * </td>
 * <td>Project *project</td>
 * <td>EINA_FALSE</td>
 * </tr>
 * @}
 */

EFLETE_TEST (preferences_project_autosave_update_test_n2)
{
   elm_init(0,0);
   app_init();
   setup("preferences_project_autosave_update_test_n2");

   Eina_Bool result;
   Project *project;

   project= pm_project_open("./preferences_project_autosave_update_test_n2/preferences_project_autosave_update_test_n2.pro");
   result = preferences_project_autosave_update(project);
   ck_assert_msg(result == EINA_FALSE, "The autosave should not be registered"
                                       "if profile is not loaded");

   app_shutdown();
   teardown("./preferences_project_autosave_update_test_n2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup preferences_project_autosave_update
 * @{
 * </TABLE>
 * @}
 * @}
 */
