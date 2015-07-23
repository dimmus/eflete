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
 * @addtogroup highlight_object_unfollow
 * @{
 * highlight
 * <TABLE>
 * @}
 */

/**
 * @addtogroup highlight_object_unfollow
 * @{
 * <tr>
 * <td>highlight_object_unfollow</td>
 * <td>highlight_object_unfollow_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 *
 * @procedure
 * @step 1 Call function highlight_object_unfollow(highlight).
 * @step 2 Check returned value.
 * </td>
 * <td>Evas_Object *highlight</td>
 * <td>EVAS_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_object_unfollow_test_p1)
{
   Evas_Object *parent, *highlight;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);

   ck_assert_msg(highlight_object_unfollow(highlight), "Not successfull unfollow.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_object_unfollow
 * @{
 * <tr>
 * <td>highlight_object_unfollow</td>
 * <td>highlight_object_unfollow_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 main window created.
 * @step 3 highlight created.
 * @step 4 Evas Object that is going to be followed created.
 * @step 5 highlight already following an object.
 *
 * @procedure
 * @step 1 Call function highlight_object_unfollow(highlight).
 * @step 2 Check returned value.
 * @step 3 Move and resize previously followed object (to 1, 1, 1, 1).
 * @step 4 Check that highlight's size and position wasn't changed.
 * </td>
 * <td>Evas_Object *highlight</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFLETE_TEST (highlight_object_unfollow_test_p2)
{
   Evas_Object *parent, *highlight, *object;
   Evas_Coord x, y, w, h;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   highlight = highlight_add(parent);
   evas_object_resize(highlight, 200, 200);
   evas_object_show(highlight);
   object = elm_button_add(parent);
   evas_object_move(object, 432, 166);
   evas_object_resize(object, 1000, 5);
   highlight_object_follow(highlight, object);

   ck_assert_msg(highlight_object_unfollow(highlight), "Not successfull unfollow.");
   evas_object_move(object, 1, 1);
   evas_object_resize(object, 1, 1);
   evas_object_geometry_get(highlight, &x, &y, &w, &h);
   ck_assert_msg(((x == 432) && (y == 166) && (w == 1000) && (h == 5)), "Highlight still following an object.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup highlight_object_unfollow
 * @{
 * </TABLE>
 * @}
 * @}
 */
