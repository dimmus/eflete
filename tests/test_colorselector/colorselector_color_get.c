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

#include "test_colorselector.h"

/**
 * @addtogroup colorselector_test
 * @{
 * @addtogroup colorselector_color_get
 * @{
 * colorselector
 * <TABLE>
 * @}
 */

/**
 * @addtogroup colorselector_color_get
 * @{
 * <tr>
 * <td>colorselector_color_get</td>
 * <td>colorselector_color_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 create colorselector
 *
 * @procedure
 * @step 1 Call colorselector_color_get()
 * @step 2 Check default value
 * @step 3 Call colorselector_color_set()
 * @step 4 Call colorselector_color_get()
 * @step 5 Check returned value
 *
 * </td>
 * <td>Evas_Object *, int *, int *, int *</td>
 * <td>values set changed successfully</td>
 * </tr>
 * @}
 */
EFLETE_TEST (colorselector_color_get_test_p)
{
   elm_init(0,0);
   App_Data *app;
   int r, g, b, a;

   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   Evas_Object *colsel = colorselector_add(app->win);

   colorselector_color_get(colsel, &r, &g, &b, &a);
   ck_assert_msg( (!r || !g || !b || !a ), "Wrong default value returned");

   colorselector_color_set(colsel, 11, 22, 33, 44);
   colorselector_color_get(colsel, &r, &g, &b, &a);
   ck_assert_msg(((r == 11) || (g == 22) || (b == 33) || (a == 44) ), "Color is not set correctly");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup colorselector_color_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
