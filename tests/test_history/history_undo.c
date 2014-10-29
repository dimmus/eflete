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
 * @addtogroup history_undo
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  min height param of "bg" part.
 * @step 8 Set new value [10] for min height param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of min height param of part "bg" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p1)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int old_value = -1;
   int new_value = 10;
   int check_value = -15;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value = edje_edit_state_min_h_get(source, "bg", "default", 0.0);
   edje_edit_state_min_h_set(source, "bg", "default", 0.0, new_value);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_state_min_h_set,
                    "Min h", "bg", "default", 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with INT value type.");
   check_value = edje_edit_state_min_h_get(source, "bg", "default", 0.0);
   ck_assert_msg(check_value == old_value, "Canceled action doesn't change value");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  drag x param of "bg" part.
 * @step 8 Set new value [1] for drag x param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of drag x param of part "bg" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p2)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int old_value = 99;
   int new_value = 1;
   int check_value = 55;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value = edje_edit_part_drag_x_get(source, "bg");
   edje_edit_part_drag_x_set(source, "bg", new_value);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_part_drag_x_set,
                    "Drag x", "bg", NULL, 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with INT value type, setted without state");
   check_value = edje_edit_part_drag_x_get(source, "bg");
   ck_assert_msg(check_value == old_value, "Canceled action doesn't change value");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p3</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  drag y param of "bg" part.
 * @step 8 Set new value [1] for drag y param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 * @step 10 Save current value of  min weight param of "bg" part.
 * @step 11 Set new value [10] for min weight param of "bg" part.
 * @step 12 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of min weight param of part "bg" with the
 *         value, that was saved at step 10 of precondition.
 * @step 4 Compare current value of drag y param of part "bg" with the
 *         value, that was setted at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p3)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int old_value_drag_y = 11;
   int new_value_drag_y = 1;
   int check_value_drag_y = 55;
   int old_value_min_w = -1;
   int new_value_min_w = 10;
   int check_value_min_w = -15;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value_drag_y = edje_edit_part_drag_y_get(source, "bg");
   edje_edit_part_drag_y_set(source, "bg", new_value_drag_y);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value_drag_y, new_value_drag_y,
                    "elm/radio/base/def", (void *)edje_edit_part_drag_y_set,
                    "Drag y", "bg", NULL, 0.0);
   old_value_min_w = edje_edit_state_min_w_get(source, "bg", "default", 0.0);
   edje_edit_state_min_w_set(source, "bg", "default", 0.0, new_value_min_w);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value_min_w, new_value_min_w,
                    "elm/radio/base/def", (void *)edje_edit_state_min_w_set,
                    "Min h", "bg", "default", 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with INT value type, setted without state");
   check_value_min_w = edje_edit_state_min_w_get(source, "bg", "default", 0.0);
   ck_assert_msg(check_value_min_w == old_value_min_w, "Cancel last action doesn't change value");
   check_value_drag_y = edje_edit_part_drag_y_get(source, "bg");
   ck_assert_msg(check_value_drag_y == new_value_drag_y, "Canceled all actions");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p4</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  drag x param of "bg" part.
 * @step 8 Set new value [1] for drag x param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 * @step 10 Save current value of  min height param of "bg" part.
 * @step 11 Set new value [10] for min height param of "bg" part.
 * @step 12 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of min height param of part "bg" with the
 *         value, that was saved at step 10 of precondition.
 * @step 4 Compare current value of drag x param of part "bg" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 2 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p4)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int old_value_drag_x = 99;
   int new_value_drag_x = 1;
   int check_value_drag_x = 55;
   int old_value_min_h = -1;
   int new_value_min_h = 10;
   int check_value_min_h = -15;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value_drag_x = edje_edit_part_drag_x_get(source, "bg");
   edje_edit_part_drag_x_set(source, "bg", new_value_drag_x);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value_drag_x, new_value_drag_x,
                    "elm/radio/base/def", (void *)edje_edit_part_drag_x_set,
                    "Drag x", "bg", NULL, 0.0);
   old_value_min_h = edje_edit_state_min_h_get(source, "bg", "default", 0.0);
   edje_edit_state_min_h_set(source, "bg", "default", 0.0, new_value_min_h);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value_min_h, new_value_min_h,
                    "elm/radio/base/def", (void *)edje_edit_state_min_h_set,
                    "Min h", "bg", "default", 0.0);

   result = history_undo(source, 2);
   ck_assert_msg(result, "Failed to undo diff with INT value type, setted without state");
   check_value_min_h = edje_edit_state_min_h_get(source, "bg", "default", 0.0);
   ck_assert_msg(check_value_min_h == old_value_min_h, "Cancel last action doesn't change value");
   check_value_drag_x = edje_edit_part_drag_x_get(source, "bg");
   ck_assert_msg(check_value_drag_x == old_value_drag_x, "Canceled not all actions");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p5</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  aspect max param of "bg" part.
 * @step 8 Set new value [0.4] for aspect param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of aspect max param of part "bg" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p5)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   double old_value = -1;
   double new_value = 0.4;
   double check_value = -5;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value = edje_edit_state_aspect_max_get(source, "bg", "default", 0.0);
   edje_edit_state_aspect_max_set(source, "bg", "default", 0.0, new_value);
   history_diff_add(source, PROPERTY, MODIFY, DOUBLE, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_state_aspect_max_set,
                    "Min h", "bg", "default", 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with DOUBLE value type.");
   check_value = edje_edit_state_aspect_max_get(source, "bg", "default", 0.0);
   ck_assert_msg(check_value == old_value, "Canceled action doesn't change value");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p6</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of clip to param of "bg" part.
 * @step 8 Set new value [events] for clip to param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of clip to param of part "bg" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p6)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   Eina_Stringshare *old_value = NULL;
   Eina_Stringshare *new_value =  NULL;
   Eina_Stringshare *check_value = NULL;
   const char *tmp;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   new_value = eina_stringshare_add("events");
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   tmp = edje_edit_part_clip_to_get(source, "bg");
   old_value = eina_stringshare_add(tmp);
   edje_edit_part_clip_to_set(source, "bg", new_value);
   history_diff_add(source, PROPERTY, MODIFY, STRING, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_part_clip_to_set,
                    "clip to", "bg", NULL, 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with STRING value type.");
   tmp = edje_edit_part_clip_to_get(source, "bg");
   check_value = eina_stringshare_add(tmp);
   ck_assert_msg(check_value == old_value, "Canceled action doesn't change value");

   eina_stringshare_del(new_value);
   eina_stringshare_del(old_value);
   eina_stringshare_del(check_value);
   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p7</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of rel1 to x param of "radio" part.
 * @step 8 Set new value [events] for rel1 to x param of "radio" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of rel1 to x param of part "radio" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p7)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   Eina_Stringshare *old_value = NULL;
   Eina_Stringshare *new_value = NULL;
   Eina_Stringshare *check_value = NULL;
   const char *tmp;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   new_value = eina_stringshare_add("events");
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   tmp = edje_edit_state_rel1_to_x_get(source, "radio", "default", 0.0);
   old_value = eina_stringshare_add(tmp);
   edje_edit_state_rel1_to_x_set(source, "radio", "default", 0.0, new_value);
   history_diff_add(source, PROPERTY, MODIFY, STRING, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_state_rel1_to_x_set,
                    "clip to", "radio", "default", 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with STRING value type.");
   tmp = edje_edit_state_rel1_to_x_get(source, "radio", "default", 0.0);
   check_value = eina_stringshare_add(tmp);
   ck_assert_msg(check_value == old_value, "Canceled action doesn't change value");

   eina_stringshare_del(new_value);
   eina_stringshare_del(old_value);
   eina_stringshare_del(check_value);
   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p8</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of color param of "bg" part.
 * @step 8 Set new value [80;90;100;120] for color param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of color param of part "bg" with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p8)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int oldr, oldg, oldb, olda;
   int newr = 80, newg = 90, newb = 100, newa = 120;
   int checkr, checkg, checkb, checka;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   edje_edit_state_color_get(source, "radio", "default", 0.0, &oldr, &oldg,
                             &oldb, &olda);
   edje_edit_state_color_set(source, "radio", "default", 0.0, newr, newg,
                             newb, newa);
   history_diff_add(source, PROPERTY, MODIFY, FOUR, oldr, oldg, oldb, olda,
                    newr, newg, newb, newa, "elm/radio/base/def",
                    (void *)edje_edit_state_color_set,
                    "clip to", "radio", "default", 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with FOUR value type.");
   edje_edit_state_color_get(source, "radio", "default", 0.0, &checkr, &checkg,
                             &checkb, &checka);
   ck_assert_msg(((checkr == oldr) && (checkg == oldg) && (checkb == oldb) &&
                  (checka == olda )), "Canceled action doesn't change value");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p9</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  max height param of group.
 * @step 8 Set new value [10] for max height param of group.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Compare current value of max height param of group with the
 *         value, that was saved at step 7 of precondition.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p9)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int old_value = -1;
   int new_value = 10;
   int check_value = -15;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value = edje_edit_group_max_h_get(source);
   edje_edit_group_max_h_set(source, new_value);
   history_diff_add(source, PROPERTY, MODIFY, ONE, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_group_max_h_set,
                    "Min h", NULL, NULL, 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with ONE value type.");
   check_value = edje_edit_group_max_h_get(source);
   ck_assert_msg(check_value == old_value, "Canceled action doesn't change value");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_p10</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Set new name [new] for part "bg".
 * @step 8 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 * @step 3 Check is exist part "bg" in group.
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_p10)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   char *old_value = "bg";
   char *new_value = "new";
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   edje_edit_part_name_set(source, old_value, new_value);
   history_diff_add(source, PROPERTY, MODIFY, RENAME, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_part_name_set,
                    "Rename", new_value, NULL, 0.0);

   result = history_undo(source, 1);
   ck_assert_msg(result, "Failed to undo diff with RENAME value type.");
   result = edje_edit_part_exist(source, old_value);
   ck_assert_msg(result, "Canceled action doesn't change value");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Create canvas, that needed for creating source object.
 * @step 4 Create edje edit object, that will be source of changes.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 *
 * </td>
 * <td>(Evas_Object *) source, (int) 1 </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_n1)
{
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");

   result = history_undo(source, 1);
   ck_assert_msg(!result, "Change was cancel with uninitialized history module.");

   ecore_evas_free(ee);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize history module.
 * @step 4 Create canvas, that needed for creating source object.
 * @step 5 Create edje edit object, that will be source of changes.
 * @step 6 Register in history object created at step 5, as module.
 * @step 7 Save current value of  min height param of "bg" part.
 * @step 8 Set new value [10] for min height param of "bg" part.
 * @step 9 Store diff with using history_diff_add function.
 *
 * @procedure
 * @step 1 Call history_undo for object from step 5 of precondition.
 * @step 2 Check returned value.
 *
 * </td>
 * <td>(Evas_Object *) source, (int) -1 </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_n2)
{
   App_Data *app = NULL;
   Evas *canvas = NULL;
   Ecore_Evas *ee = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   int old_value = -1;
   int new_value = 10;
   char *path;

   path = "./edj_build/history_undo.edj";
   elm_init(0, 0);
   app_init();
   app = app_data_get();
   app->history = history_init();
   ee = ecore_evas_new(NULL, 0, 0, 10, 10, NULL);
   canvas = ecore_evas_get(ee);
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);
   old_value = edje_edit_state_min_h_get(source, "bg", "default", 0.0);
   edje_edit_state_min_h_set(source, "bg", "default", 0.0, new_value);
   history_diff_add(source, PROPERTY, MODIFY, INT, old_value, new_value,
                    "elm/radio/base/def", (void *)edje_edit_state_min_h_set,
                    "Min h", "bg", "default", 0.0);

   result = history_undo(source, -1);
   ck_assert_msg(!result, "Undo diff with quantity of changes  '-1' .");

   history_term(app->history);
   ecore_evas_free(ee);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * <tr>
 * <td>history_undo</td>
 * <td>history_undo_test_n3</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call history_undo.
 * @step 2 Check returned value.
 *
 * </td>
 * <td>NULL, (int) 1 </td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_undo_test_n3)
{
   Eina_Bool result = EINA_FALSE;

   elm_init(0, 0);

   result = history_undo(NULL, 1);
   ck_assert_msg(!result, "Change was canceled with NULL source param.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_undo
 * @{
 * </TABLE>
 * @}
 * @}
 */
