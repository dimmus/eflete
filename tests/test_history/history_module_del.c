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
 * @addtogroup history_module_del
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_module_del
 * @{
 * <tr>
 * <td>history_module_del</td>
 * <td>history_module_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create Evas_Object, that will be source of changes.
 * @step 6 Initialize module with object from step 5.
 *
 * @procedure
 * @step 1 Call history_module_del.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_del_test_p1)
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
   history_module_add(source);

   result = history_module_del(source);
   ck_assert_msg(result, "Failed to del module from history.");

   history_term(app->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
