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
 * @addtogroup noti_warning_show
 * @{
 * Notify
 * <TABLE>
 * @}
 */

/**
 * @addtogroup noti_warning_show
 * @{
 * <tr>
 * <td>noti_warning_show</td>
 * <td>noti_warning_show_test_p</td>
 * <td>
 * @precondition
 * @step 1. initialized efl and app
 * @step 2. main window created
 *
 * @procedure
 * @step 1 Call noti_warning_show(win_layout_get(), "text")
 * @step 2 Check returned value
 * @step 3 Call noti_warning_show(win_layout_get(), NULL)
 * @step 4 Check returned value
 * </td>
 * <td>Evas_Object *win_layout, "text"/NULL</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(noti_warning_show_test_p)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_warning_show(win_layout_get(), "text"),
                 "can't show notify with text");
   ck_assert_msg(noti_warning_show(win_layout_get(), NULL),
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_warning_show
 * @{
 * <tr>
 * <td>noti_warning_show</td>
 * <td>noti_warning_show_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call noti_warning_show(NULL, "text")
 * @step 2 Check returned value
 * @step 3 Call noti_warning_show(NULL, NULL)
 * @step 4 Check returned value
 * </td>
 * <td>NULL, "text"/NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(noti_warning_show_test_n)
{
   elm_init(0,0);
   App_Data *app;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(noti_warning_show(NULL, "text") == false,
                 "can't show notify with text");
   ck_assert_msg(noti_warning_show(NULL, NULL) == false,
                 "can't show notify with NULL text");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup noti_warning_show
 * @{
 * </TABLE>
 * @}
 * @}
 */
