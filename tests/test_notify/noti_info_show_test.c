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

#include "test_notify.h"

/**
 * @addtogroup notify_test
 * @{
 * @addtogroup noti_info_show
 * @{
 * Notify
 * <TABLE>
 * @}
 */

/**
 * @addtogroup noti_info_show
 * @{
 * <tr>
 * <td>noti_info_show</td>
 * <td>noti_info_show_test_p</td>
 * <td>
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_info_show(win_layout_get(), "text", 1)
 * @step 2 Check returned value
 * @step 3 Call noti_info_show(win_layout_get(), NULL, 1)
 * @step 4 Check returned value
 * </td>
 * <td>Evas_Object *win_layout, "text"/NULL, 1</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(noti_info_show_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   ck_assert_msg(noti_info_show(win_layout_get(), "text", 1),
                 "can't show notify with text");
   ck_assert_msg(noti_info_show(win_layout_get(), NULL, 1),
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info_show
 * @{
 * <tr>
 * <td>noti_info_show</td>
 * <td>noti_info_show_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call noti_info_show(NULL, "text", 1)
 * @step 2 Check returned value
 * @step 3 Call noti_info_show(NULL, NULL, 1)
 * @step 4 Check returned value
 * </td>
 * <td>NULL, "text"/NULL, 1</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(noti_info_show_test_n1)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   ck_assert_msg(noti_info_show(NULL, "text", 1) == false,
                 "can't show notify with text");
   ck_assert_msg(noti_info_show(NULL, NULL, 1) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info_show
 * @{
 * <tr>
 * <td>noti_info_show</td>
 * <td>noti_info_show_test_n2</td>
 * <td>
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_info_show(win_layout_get(), "text", -1)
 * @step 2 Check returned value
 * @step 3 Call noti_info_show(win_layout_get(), NULL, -1)
 * @step 4 Check returned value
 * </td>
 * <td>Evas_Object *win_layout, "text"/NULL, -1</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(noti_info_show_test_n2)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_data_get();
   ui_main_window_add(app);
   ck_assert_msg(noti_info_show(win_layout_get(), "text", -1) == false,
                 "can't show notify with text");
   ck_assert_msg(noti_info_show(win_layout_get(), NULL, -1) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_info_show
 * @{
 * </TABLE>
 * @}
 * @}
 */
