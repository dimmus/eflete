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
 * @addtogroup history_module_add
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_module_add
 * @{
 * <tr>
 * <td>history_module_add</td>
 * <td>history_module_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create Evas_Object, that will be source of changes.
 *
 * @procedure
 * @step 1 Call history_module_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_add_test_p1)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = evas_object_rectangle_add(canvas);

   result = history_module_add(source);
   ck_assert_msg(result, "Failed to add new module in history.");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_add
 * @{
 * <tr>
 * <td>history_module_add</td>
 * <td>history_module_add_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create Evas_Object, that will be source of changes.
 *
 * @procedure
 * @step 1 Call history_module_add with source object from step 5.
 * @step 2 Check returned value.
 * @step 3 Call history_module_add again with the same object.
 * @step 4 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_add_test_p2)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = evas_object_rectangle_add(canvas);

   result = history_module_add(source);
   ck_assert_msg(result, "Failed to add new module in history.");
   result = history_module_add(source);
   ck_assert_msg(result, "Already initilized module didn't confirmed.");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_add
 * @{
 * <tr>
 * <td>history_module_add</td>
 * <td>history_module_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create canvas, that needed for creating source object.
 * @step 3 Create Evas_Object, that will be source of changes.
 *
 * @procedure
 * @step 1 Call history_module_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_add_test_n1)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_TRUE;

   elm_init(0, 0);
   canvas = evas_new();
   source = evas_object_rectangle_add(canvas);

   result = history_module_add(source);
   ck_assert_msg(!result, "Add new module in history, without init history module");

   evas_free(canvas);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_add
 * @{
 * <tr>
 * <td>history_module_add</td>
 * <td>history_module_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 *
 * @procedure
 * @step 1 Call history_module_add.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_add_test_n2)
{
   App_Data *app = NULL;
   Eina_Bool result = EINA_TRUE;

   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();

   result = history_module_add(NULL);
   ck_assert_msg(!result, "Add NULL pointer as new module in history.");

   history_term(app->history);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
