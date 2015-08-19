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
 * @addtogroup history_diff_add
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p1</td>
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
 * @step 1 Call history_diff_add  with correct data for value type VAL_RENAME.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, VAL_RENAME, (const char*) "bg",
 *     (const char *) "new_bg", (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_part_name_set, "Rename", "bg", NULL, 0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p1)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, VAL_RENAME, "bg", "new_bg",
                             "elm/radio/base/def", (void *)edje_edit_part_name_set,
                             "Rename", "bg", NULL, 0 );
   ck_assert_msg(result, "Failed to add new diff with VAL_RENAME value type in the"
                         " history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p2</td>
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
 * @step 1 Call history_diff_add  with correct data for value type VAL_STRING.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, VAL_STRING, NULL, (const char *) "radio",
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_rel1_to_x_set,
 *     "Rel1 to x", "bg", (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p2)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, VAL_STRING, NULL, "radio",
                             "elm/radio/base/def",
                             (void *)edje_edit_state_rel1_to_x_set,
                             "Rel1 to x", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type VAL_STRING"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p3</td>
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
 * @step 1 Call history_diff_add  with correct data for value type VAL_DOUBLE.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, VAL_DOUBLE, (double) 0.0,
 *     (double) 0.5, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_align_x_set, "align x", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p3)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, VAL_DOUBLE, 0.0, 0.5,
                             "elm/radio/base/def",
                             (void *)edje_edit_state_align_x_set,
                             "align x", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type VAL_DOUBLE"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p4</td>
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
 * @step 1 Call history_diff_add  with correct data for value type VAL_INT.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, VAL_INT, (int) 0,
 *     (int) -50, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_rel1_offset_x_set, "Rel1 offset x", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p4)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, VAL_INT, 0, -50,
                             "elm/radio/base/def",
                             (void *)edje_edit_state_rel1_offset_x_set,
                             "Rel1 offset x", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type VAL_INT"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p5</td>
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
 * @step 1 Call history_diff_add with correct data for value type VAL_GROUP.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, CONTAINER, VAL_GROUP, (int) 0,
 *     (int) 20, (int) 0, (int)30, (void *)edje_edit_group_min_w_set,
 *     (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_group_max_set, "Group min weight", NULL, NULL, (int) 0</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p5)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, CONTAINER, VAL_GROUP, 0, 20, 0, 30,
                             (void *)edje_edit_group_min_w_set,
                             "elm/radio/base/def",
                             (void *)edje_edit_group_max_w_set,
                             "Group weight", NULL, NULL, 0);
   ck_assert_msg(result, "Failed to add new diff with type VAL_GROUP"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p6</td>
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
 * @step 1 Call history_diff_add with correct data for value type VAL_FOUR.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, VAL_FOUR, (int) 255,  (int) 255,
 *     (int) 255, (int) 255, (int) 0, (int) 20, (int)50, (int) 180,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_color_set,
 *     "Color", "bg", (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p6)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, VAL_FOUR, 255, 255, 255, 255,
                             0, 20, 50, 180, "elm/radio/base/def",
                             (void *)edje_edit_state_color_set, "Color", "bg",
                             "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff with type VAL_FOUR"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p7</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Create win, that will be parent for history genlist.
 * @step 8 Create history genlist with using history_genlist_get.
 *
 * @procedure
 * @step 1 Call history_diff_add with correct data for value type VAL_FOUR.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, VAL_FOUR, (int) 255,  (int) 255,
 *     (int) 255, (int) 255, (int) 0, (int) 20, (int)50, (int) 180,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_color_set,
 *     "Color", "bg", (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p7)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Evas_Object *win = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   win = elm_win_add(NULL, "history_diff_add", ELM_WIN_BASIC);
   history_genlist_get(ap.history, win);

   result = history_diff_add(source, PROPERTY, MODIFY, VAL_FOUR, 255, 255, 255, 255,
                             0, 20, 50, 180, "elm/radio/base/def",
                             (void *)edje_edit_state_color_set, "Color", "bg",
                             "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff with type VAL_FOUR"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p8</td>
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
 * @step 1 Call history_diff_add  with correct data for value type VAL_STRING
 * with action type ADD
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, ADD, VAL_STRING, (const char *) "radio.png",
 *     (void *)edje_edit_state_tween_del, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_tween_add, "tween add", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p8)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, ADD, VAL_STRING, "radio.png",
                             (void *)edje_edit_state_tween_del, "elm/radio/base/def",
                             (void *)edje_edit_state_tween_add,
                             "tween add", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type VAL_STRING for action ADD"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p9</td>
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
 * @step 1 Call history_diff_add  with correct data for value type VAL_STRING
 * with action type DEL
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, DEL, VAL_STRING, (const char *) "radio.png",
 *     (void *)edje_edit_state_tween_add, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_tween_del, "tween del", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p9)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, DEL, VAL_STRING, "radio.png",
                             (void *)edje_edit_state_tween_add, "elm/radio/base/def",
                             (void *)edje_edit_state_tween_del,
                             "tween add", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type VAL_STRING for action DEL"
                         " in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p10</td>
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
 * @step 1 Call history_diff_add  with correct data for action CONTAINER with type VAL_INT.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, CONTAINER, VAL_INT, (int) 5, (int) 10,
 *     (int) 20, (int) 8, (void *)edje_edit_state_max_w_set,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_max_h_set,
 *     "max size", "bg", "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p10)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, CONTAINER, VAL_INT, 5, 10, 20, 8,
                            (void *)edje_edit_state_max_w_set, "elm/radio/base/def",
                            (void *)edje_edit_state_max_h_set, "max size", "bg",
                            "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff from highlight with VAL_INT value"
                         " type in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p11</td>
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
 * @step 1 Call history_diff_add  with correct data for action CONTAINER with type VAL_INT.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, CONTAINER, VAL_DOUBLE, (double) 0.2, (double) 1.0,
 *     (double) 0.7, (double) 0.0, (void *)edje_edit_state_align_x_set,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_align_y_set,
 *     "align", "bg", "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p11)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, CONTAINER, VAL_DOUBLE, 0.2, 1.0, 0.7, 0.0,
                            (void *)edje_edit_state_align_x_set, "elm/radio/base/def",
                            (void *)edje_edit_state_align_y_set, "align", "bg",
                            "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff from highlight with VAL_DOUBLE value"
                         " type in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p12</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Add new state with name "new_state" and value 0.0 to part "bg";
 *
 * @procedure
 * @step 1 Call history_diff_add with correct data for target type STATE_TARGET and
 *         action type ADD.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, STATE_TARGET, ADD, (const char *) "elm/radio/base/def",
 *     (const char *)"bg", (const char *)"new_state", (double) 0.0,
 *     (const char *) "create state"</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p12)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   edje_edit_state_add(source, "bg", "new_state", 0.0);

   result = history_diff_add(source, STATE_TARGET, ADD, "elm/radio/base/def",
                             "bg", "new_state", 0.0, "create state");
   ck_assert_msg(result, "Failed to add new diff from STATE_TARGET module with"
                         " action type ADD in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p13</td>
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
 * @step 1 Call history_diff_add with correct data for target type STATE_TARGET and
 *         action type DEL.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, STATE_TARGET, DEL, (const char *) "elm/radio/base/def",
 *     (const char *)"elm.swallow.content", (const char *)"disable", (double) 0.0,
 *     (const char *) "delete state"</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p13)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, STATE_TARGET, DEL, "elm/radio/base/def",
                             "elm.swallow.content", "disabled", 0.0, "delete state");
   ck_assert_msg(result, "Failed to add new diff from STATE_TARGET module with"
                         " action type DEL in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p14</td>
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
 * @step 1 Call history_diff_add with correct data for target type PART_TARGET and
 *         action type ADD.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PART_TARGET, ADD, (const char *)"new_part"</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p14)
{
   Ecore_Evas *ee = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   edje_edit_part_add(source, "new_part", EDJE_PART_TYPE_RECTANGLE);
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, ADD, "new_part");
   ck_assert_msg(result, "Failed to add new diff from PART_TARGET module with"
                         " action type ADD in the history of module.");

   history_term(ap.history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p15</td>
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
 * @step 1 Call history_diff_add with correct data for target type PART_TARGET and
 *         action type DEL.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PART_TARGET, DEL, (const char *)"bg"</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p15)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, DEL, "bg");
   ck_assert_msg(result, "Failed to add new diff from PART_TARGET module with"
                         " action type DEL in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_p16</td>
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
 * @step 1 Call history_diff_add with correct data for target type PART_TARGET and
 *         action type RESTACK.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PART_TARGET, RESTACK, (const char *)"bg"</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p16)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   ap.history = history_init();
   ui_main_window_add();
   history_genlist_get(ap.history, ap.win);
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, RESTACK, "bg");
   ck_assert_msg(result, "Failed to add new diff from PART_TARGET module with"
                         " action type RESTACK in the history of module.");

   history_term(ap.history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
