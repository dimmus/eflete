/**
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "test_ui_item_dialog.h"
#include "test_common.h"

/**
 * @addtogroup ui_item_dialog_test
 * @{
 * @addtogroup item_dialog_add
 * @{
 * ui_item_dialog
 * <TABLE>
 * @}
 */

/**
 * @addtogroup item_dialog_add
 * @{
 * <tr>
 * <td>item_dialog_add</td>
 * <td>item_dialog_add_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 * @step 5 open edj project
 * @step 6 get a canvas form win
 * @step 7 add and load data of style to the project structure
 * @step 8 find part with type TABLE
 *
 * @procedure
 * @step 1 Call function item_dialog_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data, (Part *) part</td>
 * <td>Evas_Object pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (item_dialog_add_test_p1)
{
   elm_init(0, 0);
   setup("item_dialog_add_test_p1");

   App_Data *app_data;
   Evas *canvas;
   Evas_Object *popup;
   Style *style = NULL;
   Part *part = NULL;
   Eina_Stringshare *part_name;


   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   part_name = eina_stringshare_add("table");
   app_data->project = pm_project_open("./item_dialog_add_test_p1/item_dialog_add_test_p1.pro");
   canvas = evas_object_evas_get(app_data->win);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, canvas, app_data->project->mmap_file);
   app_data->project->current_style = style;
   part = wm_part_by_name_find(style, part_name);

   popup = item_dialog_add(app_data, part);
   ck_assert_msg(popup != NULL, "Failed to create item dialog");

   pm_project_close(app_data->project);
   app_data->project = NULL;
   eina_stringshare_del(part_name);
   teardown("./item_dialog_add_test_p1");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup item_dialog_add
 * @{
 * <tr>
 * <td>item_dialog_add</td>
 * <td>item_dialog_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 * @step 5 open edj project
 * @step 6 get a canvas form win
 * @step 7 add and load data of style to the project structure
 * @step 8 find part with type TABLE
 *
 * @procedure
 * @step 1 Call function item_dialog_add.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, (Part *) part</td>
 * <td>returned NULL</td>
 * </tr>
 * @}
 */
EFLETE_TEST (item_dialog_add_test_n1)
{
   elm_init(0, 0);
   setup("item_dialog_add_test_n1");

   App_Data *app_data;
   Evas *canvas;
   Evas_Object *popup;
   Style *style = NULL;
   Part *part = NULL;
   Eina_Stringshare *part_name;


   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   part_name = eina_stringshare_add("table");
   app_data->project = pm_project_open("./item_dialog_add_test_n1/item_dialog_add_test_n1.pro");
   canvas = evas_object_evas_get(app_data->win);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, canvas, app_data->project->mmap_file);
   app_data->project->current_style = style;
   part = wm_part_by_name_find(style, part_name);

   popup = item_dialog_add(NULL, part);
   ck_assert_msg(popup == NULL, "Create item dialog window without application data");

   pm_project_close(app_data->project);
   app_data->project = NULL;
   eina_stringshare_del(part_name);
   teardown("./item_dialog_add_test_n1");
   app_shutdown();
   elm_shutdown();
}
END_TEST


/**
 * @addtogroup item_dialog_add
 * @{
 * <tr>
 * <td>item_dialog_add</td>
 * <td>item_dialog_add_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 initialize application with app_init() function
 * @step 3 create application data
 * @step 4 create main window
 * @step 5 open edj project
 * @step 6 get a canvas form win
 * @step 7 add and load data of style to the project structure
 *
 * @procedure
 * @step 1 Call function item_dialog_add.
 * @step 2 Check returned value.
 * </td>
 * <td>(App_Data *) app_data, NULL</td>
 * <td>NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (item_dialog_add_test_n2)
{
   elm_init(0, 0);
   setup("item_dialog_add_test_n2");

   App_Data *app_data;
   Evas *canvas;
   Evas_Object *popup;
   Style *style = NULL;


   app_init();
   app_data = app_data_get();
   ui_main_window_add(app_data);
   app_data->project = pm_project_open("./item_dialog_add_test_n2/item_dialog_add_test_n2.pro");
   canvas = evas_object_evas_get(app_data->win);
   style = wm_style_add("test", "layout/table", LAYOUT, NULL);
   wm_style_data_load(style, canvas, app_data->project->mmap_file);
   app_data->project->current_style = style;

   popup = item_dialog_add(app_data, NULL);
   ck_assert_msg(popup == NULL, "Create item dialog without part");

   pm_project_close(app_data->project);
   app_data->project = NULL;
   teardown("./item_dialog_add_test_n2");
   app_shutdown();
   elm_shutdown();
}

END_TEST
/**
 * @addtogroup item_dialog_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
