/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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

#include <check.h>
#include "ui_main_window.h"
#include "eflete.h"

/**
 * @addtogroup ui_main_window_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_main_window_add(app)
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_main_window_add_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(ui_main_window_add(app) == true, "cannot create window");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_main_window_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call ui_main_window_add(app)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_main_window_add_test_n)
{
   elm_init(0,0);
   ck_assert_msg(ui_main_window_add(NULL) == false,
               "Trying to create window when app == NULL");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_main_window_del
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_main_window_del(app)
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_main_window_del_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_main_window_del(app), "Cannot delete main window");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_main_window_del
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_main_window_del(NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_main_window_del_test_n)
{
   elm_init(0,0);
   app_init();
   ck_assert_msg(ui_main_window_del(NULL) == false,
                 "Trying to delete window when app == NULL");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_panes_add(app)
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_panes_add_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_panes_add(app), "cannot create panes");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call ui_panes_add(NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_add_test_n)
{
   elm_init(0,0);
   ck_assert_msg(ui_panes_add(NULL) == false,
                 "true returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_settings_load
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_panes_settings_load()
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_panes_settings_load_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_panes_settings_load(), "Cannot load panes settings");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_settings_load
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_panes_settings_load()
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_settings_load_test_n)
{
   elm_init(0,0);
   app_init();
   ck_assert_msg(ui_panes_settings_load() == false,
                 "Trying to load panes settings without window created");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_settings_save
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_panes_settings_save()
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_panes_settings_save_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_panes_settings_save(), "Cannot save panes settings");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_settings_save
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_panes_settings_save()
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_settings_save_test_n)
{
   elm_init(0,0);
   app_init();
   ck_assert_msg(ui_panes_settings_save() == false,
                 "Trying to save panes settings without window created");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_add_test
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_menu_add(app)
 * @step 2 Check returned value
 *
 * @passcondition Not NULL pointer returned
 * @}
 */
START_TEST (ui_menu_add_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_menu_add(app) != NULL, "NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_menu_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_menu_add(NULL)
 * @step 2 Check returned value
 *
 * @passcondition NULL pointer returned
 * @}
 */
START_TEST (ui_menu_add_test_n)
{
   elm_init(0,0);
   app_init();
   ck_assert_msg(ui_menu_add(NULL) == NULL, "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_resize_panes
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_resize_panes(100, 100)
 * @step 2 Call ui_resize_panes(0, 80)
 * @step 3 Call ui_resize_panes(45, 0)
 * @step 4 Call ui_resize_panes(0, 0)
 * @step 5 Check returned values
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_resize_panes_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_resize_panes(100, 100), "Can't resize panes to 100, 100");
   ck_assert_msg(ui_resize_panes(0, 80), "Can't resize panes to 0, 80");
   ck_assert_msg(ui_resize_panes(45, 0), "Can't resize panes to 45, 0");
   ck_assert_msg(ui_resize_panes(0, 0), "Can't resize panes to 0, 0");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_resize_panes(-10, -10)
 * @step 2 Call ui_resize_panes(23, -42)
 * @step 3 Call ui_resize_panes(-65, 1)
 * @step 4 Check returned values
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_resize_panes_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_resize_panes(-10, -10) == false, "Panes were resized to -10, -10");
   ck_assert_msg(ui_resize_panes(23, -42) == false, "Panes were resized to 23, -42");
   ck_assert_msg(ui_resize_panes(-65, 1) == false, "Panes were resized to -65, 1");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_add
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_resize_panes(3, 5)
 * @step 2 Check returned values
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_resize_panes_test_n2)
{
   elm_init(0,0);
   app_init();
   ck_assert_msg(ui_resize_panes(3, 5) == false, "Non existing Panes were resized");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_show
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_panes_show(app)
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_panes_show_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_panes_show(app), "Can't show panes");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_show
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_panes_show(app)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_show_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(ui_panes_show(app) == false, "Panes shown without window created");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_show
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call ui_panes_show(NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_show_test_n2)
{
   elm_init(0,0);
   ck_assert_msg(ui_panes_show(NULL) == false, "Panes shown with NULL app");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_hide
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_panes_hide(app)
 * @step 2 Check returned value
 *
 * @passcondition true returned
 * @}
 */
START_TEST (ui_panes_hide_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_panes_hide(app), "Can't hide panes");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_hide
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_panes_hide(app)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_hide_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(ui_panes_hide(app) == false, "Panes hidden without window created");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_panes_hide
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call ui_panes_hide(NULL)
 * @step 2 Check returned value
 *
 * @passcondition false returned
 * @}
 */
START_TEST (ui_panes_hide_test_n2)
{
   elm_init(0,0);
   ck_assert_msg(ui_panes_hide(NULL) == false, "Panes hidden with NULL app");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_edc_load_done with existing edc file
 * @step 2 Check returned value
 *
 * @passcondition Not NULL returned
 * @}
 */
START_TEST (ui_edc_load_done_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_edc_load_done(app, "radio", "./edje_build/radio.edc",
                 "", "", "") != NULL, "NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_edc_load_done with existing edc file
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edc_load_done_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(ui_edc_load_done(app, "radio", "./edje_build/radio.edc",
                 "", "", "") == NULL, "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_edc_load_done with non-existing edc file
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edc_load_done_test_n2)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_edc_load_done(app, "radio", "./edje_build/radio_123.edc",
                 "", "", "") == NULL, "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_edc_load_done with NULL edc file
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edc_load_done_test_n3)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_edc_load_done(app, "radio", NULL,
                 "", "", "") == NULL, "NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edc_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call ui_edc_load_done with existing edc file and app==NULL
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edc_load_done_test_n4)
{
   elm_init(0,0);
   ck_assert_msg(ui_edc_load_done(NULL, "radio", "./edje_build/radio.edc",
                 "", "", "") == NULL, "Not NULL pointer returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Positive test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_edj_load_done with existing edj file
 * @step 2 Check returned value
 *
 * @passcondition Not NULL returned
 * @}
 */
START_TEST (ui_edj_load_done_test_p)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_edj_load_done(app, NULL, "./edje_build/radio.edj") != NULL,
                                  "NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 *
 * @procedure
 * @step 1 Call ui_edj_load_done with existing edj file
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edj_load_done_test_n1)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ck_assert_msg(ui_edj_load_done(app, NULL, "./edje_build/radio.edj") == NULL,
                                  "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_edj_load_done with non-existing edj file
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edj_load_done_test_n2)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_edj_load_done(app, NULL, "./edje_build/radio_123.edj") == NULL,
                 "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl and app
 * @step 2 main window created
 *
 * @procedure
 * @step 1 Call ui_edj_load_done with NULL edj file
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edj_load_done_test_n3)
{
   elm_init(0,0);
   App_Data *app = NULL;
   app_init();
   app = app_create();
   ui_main_window_add(app);
   ck_assert_msg(ui_edj_load_done(app, NULL, NULL) == NULL,
                 "Not NULL pointer returned");
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup ui_edj_load_done
 * @{
 * @objective Negative test case
 *
 * @precondition
 * @step 1 initialized efl
 *
 * @procedure
 * @step 1 Call ui_edj_load_done with existing edj file and app==NULL
 * @step 2 Check returned value
 *
 * @passcondition NULL returned
 * @}
 */
START_TEST (ui_edj_load_done_test_n4)
{
   elm_init(0,0);
   ck_assert_msg(ui_edj_load_done(NULL, NULL, "./edje_build/radio.edj") == NULL,
                 "Not NULL pointer returned");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup test_suite
 * @{
 * @objective Creating above to the test case
 *
 * @procedure
 * @step 1 Create suite
 * @step 2 Create test case
 * @step 3 Add unit tests to the test case
 * @step 4 Add test case to the suite
 *
 * @passcondition Return a Suite object.
 * @}
 */
Suite* test_suite (void) {
   Suite *suite = suite_create("ui_main_window_test");
   TCase *tcase = tcase_create("TCase");
   tcase_add_test(tcase, ui_main_window_add_test_p);
   tcase_add_test(tcase, ui_main_window_add_test_n);
   tcase_add_test(tcase, ui_main_window_del_test_p);
   tcase_add_test(tcase, ui_main_window_del_test_n);
   tcase_add_test(tcase, ui_panes_add_test_p);
   tcase_add_test(tcase, ui_panes_add_test_n);
   tcase_add_test(tcase, ui_panes_settings_load_test_p);
   tcase_add_test(tcase, ui_panes_settings_load_test_n);
   tcase_add_test(tcase, ui_panes_settings_save_test_p);
   tcase_add_test(tcase, ui_panes_settings_save_test_n);
   tcase_add_test(tcase, ui_menu_add_test_p);
   tcase_add_test(tcase, ui_menu_add_test_n);
   tcase_add_test(tcase, ui_resize_panes_test_p);
   tcase_add_test(tcase, ui_resize_panes_test_n1);
   tcase_add_test(tcase, ui_resize_panes_test_n2);
   tcase_add_test(tcase, ui_panes_show_test_p);
   tcase_add_test(tcase, ui_panes_show_test_n1);
   tcase_add_test(tcase, ui_panes_show_test_n2);
   tcase_add_test(tcase, ui_panes_hide_test_p);
   tcase_add_test(tcase, ui_panes_hide_test_n1);
   tcase_add_test(tcase, ui_panes_hide_test_n2);
   tcase_add_test(tcase, ui_edc_load_done_test_p);
   tcase_add_test(tcase, ui_edc_load_done_test_n1);
   tcase_add_test(tcase, ui_edc_load_done_test_n2);
   tcase_add_test(tcase, ui_edc_load_done_test_n3);
   tcase_add_test(tcase, ui_edc_load_done_test_n4);
   tcase_add_test(tcase, ui_edj_load_done_test_p);
   tcase_add_test(tcase, ui_edj_load_done_test_n1);
   tcase_add_test(tcase, ui_edj_load_done_test_n2);
   tcase_add_test(tcase, ui_edj_load_done_test_n3);
   tcase_add_test(tcase, ui_edj_load_done_test_n4);

   suite_add_tcase(suite, tcase);
   return suite;
}

/**
 * @addtogroup main
 * @{
 * @objective Run a Check Unit Test
 *
 * @procedure
 * @step 1 Create a suite
 * @step 2 Create a suite runner object of type SRunner
 * @step 3 Add report of Check tests to the xml format
 * @step 4 Run the suite, using the CK_VERBOSE flag
 * @step 5 Create int object for list of the failures
 *
 * @passcondition Print a summary of the run unit tests.
 * @}
 */
int main(void) {
   int number_failed;
   Suite *suite = test_suite();
   SRunner *runner = srunner_create(suite);
   srunner_set_xml (runner, "test_ui_main_window.xml");
   srunner_run_all(runner, CK_VERBOSE);
   number_failed = srunner_ntests_failed(runner);
   srunner_free(runner);
   return number_failed;
}
