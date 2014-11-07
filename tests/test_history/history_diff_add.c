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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * @step 1 Call history_diff_add with correct data for value type ONE.
 * @step 2 Check returned value.
 * </td>
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, ONE, (int) 0,
 *     (int) 20, (const char *) "elm/radio/base/def",
 *     (void *)edje_edit_group_min_w_set, "Group min w"</td>
 * <td>EINA_TRUE value returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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

   result = history_diff_add(source, PROPERTY, MODIFY, ONE, 0, 20,
                             "elm/radio/base/def",
                             (void *)edje_edit_group_min_w_set,
                             "Group min w");
   ck_assert_msg(result, "Failed to add new diff with type ONE"
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * <td>(Evas_Object *) source, PROPERTY, MODIFY, ONE, (int) 10, (int) 15,
 *     (const char *) "elm/radio/base/def", NULL, "Group min w", "bg",
 *     "default", 0.0 </td>
 * <td>EINA_FALSE value returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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

   result = history_diff_add(source, PROPERTY, MODIFY, ONE, 10, 15,
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
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
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
 * </TABLE>
 * @}
 * @}
 */
