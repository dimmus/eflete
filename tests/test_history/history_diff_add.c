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
 * @step 1 Call history_diff_add  with correct data for value type RENAME.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, RENAME, (const char*) "bg",
 *     (const char *) "new_bg", (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_part_name_set, "Rename", "bg", NULL, 0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p1)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, RENAME, "bg", "new_bg",
                             "elm/radio/base/def", (void *)edje_edit_part_name_set,
                             "Rename", "bg", NULL, 0 );
   ck_assert_msg(result, "Failed to add new diff with RENAME value type in the"
                         " history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for value type STRING.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, STRING, NULL, (const char *) "radio",
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_rel1_to_x_set,
 *     "Rel1 to x", "bg", (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p2)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, STRING, NULL, "radio",
                             "elm/radio/base/def",
                             (void *)edje_edit_state_rel1_to_x_set,
                             "Rel1 to x", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type STRING"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for value type DOUBLE.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, DOUBLE, (double) 0.0,
 *     (double) 0.5, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_align_x_set, "align x", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p3)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, DOUBLE, 0.0, 0.5,
                             "elm/radio/base/def",
                             (void *)edje_edit_state_align_x_set,
                             "align x", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type DOUBLE"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for value type INT.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, INT, (int) 0,
 *     (int) -50, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_rel1_offset_x_set, "Rel1 offset x", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p4)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, INT, 0, -50,
                             "elm/radio/base/def",
                             (void *)edje_edit_state_rel1_offset_x_set,
                             "Rel1 offset x", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type INT"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add with correct data for value type GROUP.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, GROUP, (int) 0,
 *     (int) 20, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_group_min_w_set, "Group min w"</td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p5)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, GROUP, 0, 20,
                             "elm/radio/base/def",
                             (void *)edje_edit_group_min_w_set,
                             "Group min w");
   ck_assert_msg(result, "Failed to add new diff with type GROUP"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add with correct data for value type FOUR.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, FOUR, (int) 255,  (int) 255,
 *     (int) 255, (int) 255, (int) 0, (int) 20, (int)50, (int) 180,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_color_set,
 *     "Color", "bg", (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p6)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, FOUR, 255, 255, 255, 255,
                             0, 20, 50, 180, "elm/radio/base/def",
                             (void *)edje_edit_state_color_set, "Color", "bg",
                             "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff with type FOUR"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add with correct data for value type FOUR.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, FOUR, (int) 255,  (int) 255,
 *     (int) 255, (int) 255, (int) 0, (int) 20, (int)50, (int) 180,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_color_set,
 *     "Color", "bg", (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p7)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Evas_Object *win = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   win = elm_win_add(NULL, "history_diff_add", ELM_WIN_BASIC);
   history_genlist_get(app->history, win);

   result = history_diff_add(source, PROPERTY, MODIFY, FOUR, 255, 255, 255, 255,
                             0, 20, 50, 180, "elm/radio/base/def",
                             (void *)edje_edit_state_color_set, "Color", "bg",
                             "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff with type FOUR"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for value type STRING
 * with action type ADD
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, ADD, STRING, (const char *) "radio.png",
 *     (void *)edje_edit_state_tween_del, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_tween_add, "tween add", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p8)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, ADD, STRING, "radio.png",
                             (void *)edje_edit_state_tween_del, "elm/radio/base/def",
                             (void *)edje_edit_state_tween_add,
                             "tween add", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type STRING for action ADD"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for value type STRING
 * with action type DEL
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, DEL, STRING, (const char *) "radio.png",
 *     (void *)edje_edit_state_tween_add, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_tween_del, "tween del", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p9)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, DEL, STRING, "radio.png",
                             (void *)edje_edit_state_tween_add, "elm/radio/base/def",
                             (void *)edje_edit_state_tween_del,
                             "tween add", "bg", "default", 0.0 );
   ck_assert_msg(result, "Failed to add new diff with type STRING for action DEL"
                         " in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for action CONTAINER with type INT.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, CONTAINER, INT, (int) 5, (int) 10,
 *     (int) 20, (int) 8, (void *)edje_edit_state_max_w_set,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_max_h_set,
 *     "max size", "bg", "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p10)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, CONTAINER, INT, 5, 10, 20, 8,
                            (void *)edje_edit_state_max_w_set, "elm/radio/base/def",
                            (void *)edje_edit_state_max_h_set, "max size", "bg",
                            "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff from highlight with INT value"
                         " type in the history of module.");

   history_term(app->history);
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
 * @step 1 Call history_diff_add  with correct data for action CONTAINER with type INT.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, CONTAINER, DOUBLE, (double) 0.2, (double) 1.0,
 *     (double) 0.7, (double) 0.0, (void *)edje_edit_state_align_x_set,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_align_y_set,
 *     "align", "bg", "default", 0.0 </td>
 * <td>EINA_TRUE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_p11)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, CONTAINER, DOUBLE, 0.2, 1.0, 0.7, 0.0,
                            (void *)edje_edit_state_align_x_set, "elm/radio/base/def",
                            (void *)edje_edit_state_align_y_set, "align", "bg",
                            "default", 0.0);
   ck_assert_msg(result, "Failed to add new diff from highlight with DOUBLE value"
                         " type in the history of module.");

   history_term(app->history);
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
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   edje_edit_state_add(source, "bg", "new_state", 0.0);

   result = history_diff_add(source, STATE_TARGET, ADD, "elm/radio/base/def",
                             "bg", "new_state", 0.0, "create state");
   ck_assert_msg(result, "Failed to add new diff from STATE_TARGET module with"
                         " action type ADD in the history of module.");

   history_term(app->history);
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
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, STATE_TARGET, DEL, "elm/radio/base/def",
                             "elm.swallow.content", "disabled", 0.0, "delete state");
   ck_assert_msg(result, "Failed to add new diff from STATE_TARGET module with"
                         " action type DEL in the history of module.");

   history_term(app->history);
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
   App_Data *app = NULL;
   Ecore_Evas *ee = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   edje_edit_part_add(source, "new_part", EDJE_PART_TYPE_RECTANGLE);
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, ADD, "new_part");
   ck_assert_msg(result, "Failed to add new diff from PART_TARGET module with"
                         " action type ADD in the history of module.");

   history_term(app->history);
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
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, DEL, "bg");
   ck_assert_msg(result, "Failed to add new diff from PART_TARGET module with"
                         " action type DEL in the history of module.");

   history_term(app->history);
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
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, RESTACK, "bg");
   ck_assert_msg(result, "Failed to add new diff from PART_TARGET module with"
                         " action type RESTACK in the history of module.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 *
 * @procedure
 * @step 1 Call history_diff_add without module object.
 * @step 2 Check returned value.
 * </td>
 * <td>NULL, PROPERTY</td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n1)
{
   App_Data *app = NULL;
   Eina_Bool result = EINA_FALSE;

   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();

   result = history_diff_add(NULL, PROPERTY);
   ck_assert_msg(!result, "Add new diff without module object.");

   history_term(app->history);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_diff_add
 * @{
 * <tr>
 * <td>history_diff_add</td>
 * <td>history_diff_add_test_n2</td>
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
 * @step 1 Call history_diff_add with wrong Target parametr.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, 25, MODIFY, RENAME, (int) 10, (int) 15,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_part_name_set,
 *     "Rename", "bg", NULL, 0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n2)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, 25, MODIFY, RENAME, 10, 15,
                             "elm/radio/base/def", (void *)edje_edit_part_name_set,
                             "Rename", "bg", NULL, 0 );
   ck_assert_msg(!result, "New diff added with wrong Target parametr.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n3</td>
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
 * @step 1 Call history_diff_add with wrong value type parametr.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, 110, (int) 10, (int) 15,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_part_name_set,
 *     "Rename", "bg", NULL, 0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n3)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, 110, 10, 15,
                             "elm/radio/base/def", (void *)edje_edit_part_name_set,
                             "Rename", "bg", NULL, 0 );
   ck_assert_msg(!result, "New diff added with wrong value type parametr.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n4</td>
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
 * @step 1 Call history_diff_add with wrong count of arguments.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, FOUR, (int) 10, (int) 15,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_color_set,
 *     "Color", "bg", "default", 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n4)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, FOUR, 10, 15,
                             "elm/radio/base/def",
                             (void *)edje_edit_state_color_set,
                             "Color", "bg", "default", 0.0 );
   ck_assert_msg(!result, "New diff added with wrong count of arguments.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n5</td>
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
 * @step 1 Call history_diff_add without function pointer.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, GROUP, (int) 10, (int) 15,
 *     (const char *) "elm/radio/base/def", NULL, "Group min w", "bg",
 *     "default", 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n5)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, GROUP, 10, 15,
                             "elm/radio/base/def", NULL, "Group min w", "bg",
                             "default", 0.0 );
   ck_assert_msg(!result, "New diff added without function pointer.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n6</td>
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
 * @step 1 Call history_diff_add with random arguments.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, INT,
 *     (const char *) "elm/radio/base/def", (int) 10, NULL, 0.0,
 *     (const char *) "elm/radio/base/def", NULL, 11, "bg",
 *     555, 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n6)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, MODIFY, INT,
                             (const char *) "elm/radio/base/def", (int) 10, NULL,
                             0.0, (const char *) "elm/radio/base/def", NULL, 11,
                             "bg", 555, 0.0, source, 0.0 );
   ck_assert_msg(!result, "New diff added with random params.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n7</td>
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
 * @step 1 Call history_diff_add  with incorrect data for value type STRING
 * with action type ADD. Wihout revert function.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, ADD, STRING, (const char *) "radio.png",
 *      NULL, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_tween_add, "tween add", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n7)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, ADD, STRING, "radio.png",
                              NULL, "elm/radio/base/def",
                             (void *)edje_edit_state_tween_add,
                             "tween add", "bg", "default", 0.0 );
   ck_assert_msg(!result, "Add new diff with type STRING for action ADD"
                          "without revert function.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n8</td>
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
 * @step 1 Call history_diff_add  with incorrect data for value type STRING
 * with action type DEL. Wihout revert function.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, DEL, STRING, (const char *) "radio.png",
 *      NULL, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_state_tween_del, "tween add", "bg",
 *     (const char *) "default", 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n8)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, DEL, STRING, "radio.png",
                              NULL, "elm/radio/base/def",
                             (void *)edje_edit_state_tween_del,
                             "tween add", "bg", "default", 0.0 );
   ck_assert_msg(!result, "Add new diff with type STRING for action DEL"
                          "without revert function.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n9</td>
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
 * @step 1 Call history_diff_add  with first function pointer.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, CONTAINER, INT, (int) 5, (int) 10,
 *     (int) 20, (int) 8, NULL,
 *     (const char *) "elm/radio/base/def", (void *)edje_edit_state_max_h_set,
 *     "max size", "bg", "default", 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n9)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_TRUE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PROPERTY, CONTAINER, INT, 5, 10, 20, 8,
                             NULL, "elm/radio/base/def",
                            (void *)edje_edit_state_max_h_set, "max size", "bg",
                            "default", 0.0);
   ck_assert_msg(!result, "Added new diff from highlight with INT value type, but"
                          " without the pointer of function, that change first param");

   history_term(app->history);
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
 * <td>history_diff_add_test_n10</td>
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
 * @step 1 Call history_diff_add with wrong action type (RENAME) for target type
 *         STATE_TARGET.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, STATE_TARGET, RENAME, (const char *) "elm/radio/base/def",
 *     (const char *)"bg", (const char *)"new_state", (double) 0.0,
 *     (const char *) "create state"</td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n10)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, STATE_TARGET, RENAME, "elm/radio/base/def",
                             "bg", "new_state", 0.0, "create state");
   ck_assert_msg(!result, "Added new diff from STATE_TARGET module with"
                         " action type RENAME in the history of module.");

   history_term(app->history);
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
 * <td>history_diff_add_test_n11</td>
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
 * @step 1 Call history_diff_add with wrong action type (MODIFY) for
 *         target type PART_TARGET.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PART_TARGET, MODIFY, (const char *)"bg"</td>
 * <td>EINA_FALSE value returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_diff_add_test_n11)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;

   path = "./edj_build/history_diff_add.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_diff_add(source, PART_TARGET, MODIFY, "bg");
   ck_assert_msg(!result, "Add new diff for PART_TARGET module with"
                         " action type MODIFY in the history of module.");

   history_term(app->history);
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
