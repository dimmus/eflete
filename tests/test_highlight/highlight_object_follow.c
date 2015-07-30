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

#include "test_highlight.h"

/**
 * @addtogroup highlight_test
 * @{
 * @addtogroup highlight_object_follow
 * @{
 * highlight
 * <TABLE>
 * @}
 */

/**
 * @addtogroup highlight_object_follow
 * @{
 * <tr>
 * <td>highlight_object_follow</td>
 * <td>highlight_object_follow_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 * @step 4 resized highlight to 200, 200.
 * @step 5 Evas_Object that is going to be followed is created.
 *
 * @procedure
 * @step 1 Check highlight's size and position (should be 0, 0, 200, 200).
 * @step 2 Call function highlight_object_follow(highlight, object).
 * @step 3 Check function's result (returned EINA_TRUE).
 * @step 4 Check highlight's size and position (should be same as the object size).
 * @step 5 Change object's position (x = 120, y = 135, w = 100, h = 1400).
 * @step 6 Check highlight's size and position (should be changed to 120, 135, 100, 1400).
 * </td>
 * <td>Evas_Object *highlight, Evas_Object *object</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_object_follow_test_p1)
{
   Evas_Object *parent, *highlight, *object;
   Evas_Coord x, y, w, h;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);
   object = elm_button_add(parent);
   evas_object_resize(highlight, 200, 200);
   evas_object_move(object, 600, 35);
   evas_object_resize(object, 400, 400);
   evas_object_show(highlight);

   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 0) && (y == 0) && (w = 200) && (h = 200)), "Wrong position of highlight.");

   ck_assert_msg(highlight_object_follow(highlight, object), "Can't follow the object");
   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 600) && (y == 35) && (w = 400) && (h = 400)), "Wrong position of highlight.");

   evas_object_move(object, 120, 135);
   evas_object_resize(object, 100, 1400);
   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 120) && (y == 135) && (w = 100) && (h = 1400)), "Wrong position of highlight.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_object_follow
 * @{
 * <tr>
 * <td>highlight_object_follow</td>
 * <td>highlight_object_follow_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 * @step 4 Evas_Object that is going to be followed is created.
 * @step 5 Another Evas_Object that is going to be followed is created.
 *
 * @procedure
 * @step 1 Call function highlight_object_follow(highlight, object).
 * @step 2 Check function's result (returned EINA_TRUE).
 * @step 3 Call function highlight_object_follow(highlight, new_obj).
 * @step 4 Check function's result (returned EINA_TRUE).
 * @step 5 Check highlight's size and position (should be changed to 100, 100, 100, 100).
 * @step 6 Change object's position (x = 120, y = 135, w = 100, h = 1400).
 * @step 7 Check highlight's size and position (shouldn't be changed, still 100, 100, 100, 100).
 * @step 8 Change new_object's position (x = 40, y = 40, w = 40, h = 40).
 * @step 9 Check highlight's size and position (should be changed to 40, 40, 40, 40).
 * </td>
 * <td>Evas_Object *highlight, Evas_Object *object</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_object_follow_test_p2)
{
   Evas_Object *parent, *highlight, *object, *new_obj;
   Evas_Coord x, y, w, h;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);
   object = elm_button_add(parent);
   evas_object_move(object, 600, 35);
   evas_object_resize(object, 400, 400);
   new_obj = elm_button_add(parent);
   evas_object_move(new_obj, 100, 100);
   evas_object_resize(new_obj, 100, 100);
   evas_object_show(highlight);

   ck_assert_msg(highlight_object_follow(highlight, object), "Can't follow first object");
   ck_assert_msg(highlight_object_follow(highlight, new_obj), "Can't follow second object");
   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 100) && (y == 100) && (w = 100) && (h = 100)), "Last object that was followed changed highlight's position and size.");

   evas_object_move(object, 120, 135);
   evas_object_resize(object, 100, 1400);
   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 100) && (y == 100) && (w = 100) && (h = 100)), "Last object that was followed changed highlight's position and size.");

   evas_object_move(new_obj, 40, 40);
   evas_object_resize(new_obj, 40, 40);
   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 40) && (y == 40) && (w = 40) && (h = 40)), "New followed object can't change highlight's size and position.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_object_follow
 * @{
 * </TABLE>
 * @}
 * @}
 */
