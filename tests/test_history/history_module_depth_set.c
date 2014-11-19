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

#include "test_history.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_module_depth_set
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_module_depth_set
 * @{
 * <tr>
 * <td>history_module_depth_set</td>
 * <td>history_module_depth_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 *
 * @procedure
 * @step 1 Call history_module_depth_set with value depth equal 2
 * @step 2 Check returned value.
 * @step 3 Check returned value from history_module_get, it should be equal 2
 * </td>
 * <td>(Evas_Object *) source, (int) 2 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_set_test_p)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;
   int depth = 0;

   path = "./edj_build/history_module_depth_set.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_module_depth_set(source, 2);
   ck_assert_msg(result, "Failed to set new history depth for module");
   depth = history_module_depth_get(source);
   ck_assert_msg(depth == 2, "Getted depth value is not equal to setted");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_set
 * @{
 * <tr>
 * <td>history_module_depth_set</td>
 * <td>history_module_depth_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 *
 * @procedure
 * @step 1 Call history_module_depth_set with value depth equal 2
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, (int) 2 </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_set_test_n1)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_module_depth_set.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");

   result = history_module_depth_set(source, 2);
   ck_assert_msg(!result, "Setted new history depth for object, that not registered"
                 " as module");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_set
 * @{
 * <tr>
 * <td>history_module_depth_set</td>
 * <td>history_module_depth_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call history_module_depth_set with value depth equal 2
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (int) 2 </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_set_test_n2)
{
   Eina_Bool result = true;

   elm_init(0, 0);

   result = history_module_depth_set(NULL, 2);
   ck_assert_msg(!result, "Seted new history depth for NULL object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
