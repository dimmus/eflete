/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "test_modal_window.h"

/**
 * @addtogroup modal_window_test
 * @{
 * @addtogroup mw_info_text_set
 * @{
 * modal_window
 * <TABLE>
 * @}
 */

/**
 * @addtogroup mw_info_text_set
 * @{
 * <tr>
 * <td>mw_info_text_set</td>
 * <td>mw_info_text_set_p</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_info_text_set(modal_window, "test text")
 * @step 2 Check returned value
 * </td>
 * <td>Evas_Object *modal_window, const char *text = "test text"</td>
 * <td>EINA_TRUE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (mw_info_text_set_test_p)
{
   Evas_Object *mw;
   App_Data *app;

   elm_init(0,0);
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL, NULL);

   ck_assert_msg(mw_info_text_set(mw, "test text"), "Can't set title");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * <tr>
 * <td>mw_info_text_set</td>
 * <td>mw_info_text_set_n1</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call mw_info_text_set(NULL, "new_modal_window")
 * @step 2 Check returned value
 * </td>
 * <td>NULL, const char *text = "new_modal_window"</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (mw_info_text_set_test_n1)
{
   elm_init(0,0);
   app_init();

   ck_assert_msg(mw_info_text_set(NULL, "new_modal_window") == EINA_FALSE, "Trying to set"
                  " title to NULL object");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * <tr>
 * <td>mw_info_text_set</td>
 * <td>mw_info_text_set_n2</td>
 * <td>
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main_window created
 * @step 3 modal window created
 *
 * @procedure
 * @step 1 Call mw_info_text_set(modal_window, NULL)
 * @step 2 Check returned value
 * </td>
 * <td>Evas_Object *modal_window, NULL</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST (mw_info_text_set_test_n2)
{
   Evas_Object *mw;
   App_Data *app;

   elm_init(0,0);
   app_init();
   app = app_create();
   ui_main_window_add(app);
   mw = mw_add(NULL, NULL);

   ck_assert_msg(mw_info_text_set(mw, NULL) == EINA_FALSE, "Trying to set NULL title");

   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup mw_info_text_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
