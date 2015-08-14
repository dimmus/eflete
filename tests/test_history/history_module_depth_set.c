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
#include "main_window.h"
#include "test_common.h"

/**
 * @addtogroup history_test
 * @{
 * @addtogroup history_module_depth_set
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_module_depth_set
 * @{
 * <tr>
 * <td>history_module_depth_set</td>
 * <td>history_module_depth_set_test_p1</td>
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
 * @step 1 Call history_module_depth_set with value depth equal 2
 * @step 2 Check returned value.
 * @step 3 Check returned value from history_module_get, it should be equal 2
 * </td>
 * <td>(Evas_Object *) source, (int) 2 </td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_set_test_p1)
{
   Evas *canvas = NULL;
   Evas_Object *source = NULL;
   Eina_Bool result = EINA_FALSE;
   const char *path;
   int depth = 0;

   path = "./edj_build/history_module_depth_set.edj";
   elm_init(0, 0);
   app_init();
   ap->history = history_init();
   canvas = evas_new();
   source = edje_edit_object_add(canvas);
   edje_object_file_set(source, path, "elm/radio/base/def");
   history_module_add(source);

   result = history_module_depth_set(source, 2);
   ck_assert_msg(result, "Failed to set new history depth for module");
   depth = history_module_depth_get(source);
   ck_assert_msg(depth == 2, "Getted depth value is not equal to setted");

   history_term(ap->history);
   evas_free(canvas);
   app_shutdown();
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_set
 * @{
 * <tr>
 * <td>history_module_depth_set</td>
 * <td>history_module_depth_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Initialize main window.
 * @step 4 Open project.
 * @step 5 Fill widget inlist with data.
 * @step 6 Find style that represent the group "elm/radio/base/def"
 * @step 7 Load style into project.
 * @step 8 Register in history style object, that finded at step 6, as module.
 * @step 9 Set module depth equal 3.
 * @step 10 Set new value [10] for max height param of group.
 * @step 11 Store diff with using history_diff_add function.
 * @step 12 Set new value [80;90;100;120] for color param of "bg" part.
 * @step 13 Store diff with using history_diff_add function.
 * @step 14 Set new value [events] for rel1 to x param of "radio" part.
 * @step 15 Store diff with using history_diff_add function.
 * @step 16 Set new value [0.4] for aspect param of "bg" part.
 * @step 17 Store diff with using history_diff_add function.
 * @step 18 Call history_undo with count 4.
 *
 * @procedure
 * @step 1 Compare current value of max height param of group with the
 *         value, that was setted at step 10 of precondition. It should be equal.
 * </td>
 * <td>(Evas_Object *) style->obj, (int) 3 </td>
 * <td>First change does not canceled</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_module_depth_set_test_p2)
{
   Style *style = NULL;
   int new_value = 10;
   int check_value = 0;

   elm_init(0, 0);
   app_init();

   ui_main_window_add();
   ap->project = setup("history_module_depth_set_test_p2");
   wm_widgets_list_objects_load(ap->project->widgets,
                                evas_object_evas_get(ap->win), ap->project->mmap_file);
   blocks_show();
   style = wm_style_object_find(ap->project->widgets, "elm/radio/base/def");
   ui_style_clicked(style);
   history_module_add(style->obj);
   history_module_depth_set(style->obj, 3);
   edje_edit_group_max_h_set(style->obj, new_value);
   history_diff_add(style->obj, PROPERTY, CONTAINER, VAL_GROUP, 0, new_value, 0, new_value,
                    (void *)edje_edit_group_max_h_set, "elm/radio/base/def",
                    (void *)edje_edit_group_max_w_set, "group_h", NULL, NULL, 0.0);
   edje_edit_state_color_set(style->obj, "radio", "default", 0.0, 80, 90, 100, 120);
   history_diff_add(style->obj, PROPERTY, MODIFY, VAL_FOUR, 0, 0, 0, 0,
                    80, 90, 100, 120, "elm/radio/base/def",
                    (void *)edje_edit_state_color_set,
                    "color", "radio", "default", 0.0);
   edje_edit_state_rel1_to_x_set(style->obj, "radio", "default", 0.0, "events");
   history_diff_add(style->obj, PROPERTY, MODIFY, VAL_STRING, "bg", "events",
                    "elm/radio/base/def", (void *)edje_edit_state_rel1_to_x_set,
                    "clip to", "radio", "default", 0.0);
   edje_edit_state_aspect_max_set(style->obj, "bg", "default", 0.0, 0.4);
   history_diff_add(style->obj, PROPERTY, MODIFY, VAL_DOUBLE, 0.0, 0.4,
                    "elm/radio/base/def", (void *)edje_edit_state_aspect_max_set,
                    "aspect max", "bg", "default", 0.0);
   history_undo(style->obj, 4);

   check_value = edje_edit_group_max_h_get(style->obj);
   ck_assert_msg(check_value == new_value, "Cancel diff, that outside of depth");

   pm_project_close(ap->project);
   ap->project = NULL;

   ui_main_window_del();
   app_shutdown();
   teardown("./history_module_depth_set_test_p2");
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_module_depth_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
