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
 * @addtogroup history_module_depth_get
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_module_depth_get
 * @{
 * <tr>
 * <td>history_module_depth_get</td>
 * <td>history_module_depth_get_test_p1</td>
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
 * @step 1 Call history_module_depth_get
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td>0 returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_get_test_p1)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   int result = -1;
   const char *path;

   path = "./edj_build/history_module_depth_get.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_module_depth_get(source);
   ck_assert_msg(result == 0, "Failed get history depth for module");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_get
 * @{
 * <tr>
 * <td>history_module_depth_get</td>
 * <td>history_module_depth_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Set maximum depth 2
 *
 * @procedure
 * @step 1 Call history_module_depth_get
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td> 2 returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_get_test_p2)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   int result = -1;
   const char *path;

   path = "./edj_build/history_module_depth_get.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   history_module_depth_set(source, 2);

   result = history_module_depth_get(source);
   ck_assert_msg(result == 2, "Failed get history depth for module");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_get
 * @{
 * <tr>
 * <td>history_module_depth_get</td>
 * <td>history_module_depth_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 *
 * @procedure
 * @step 1 Call history_module_depth_get
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td>-1 returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_get_test_n1)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   int result = 0;
   const char *path;

   path = "./edj_build/history_module_depth_get.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");

   result = history_module_depth_get(source);
   ck_assert_msg(result < 0, "Getted history depth from object, that not registered"
                 " as module");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_get
 * @{
 * <tr>
 * <td>history_module_depth_get</td>
 * <td>history_module_depth_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call history_module_depth_get
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_get_test_n2)
{
   int result = 0;

   elm_init(0, 0);

   result = history_module_depth_get(NULL);
   ck_assert_msg(result < 0, "Getted history depth from NULL object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
