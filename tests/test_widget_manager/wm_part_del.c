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

#include "test_widget_manager.h"

/**
 * @addtogroup widget_manager_test
 * @{
 * @addtogroup wm_part_del
 * @{
 * widget_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup wm_part_del
 * @{
 * <tr>
 * <td>wm_part_del</td>
 * <td>wm_part_del_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialized elm
 * @step 2 Evas canvas created
 * @step 3 Create Style structure
 * @step 4 Edje Edit object of the tested existing group
 * @step 5 Create Part with a name of existing part.
 *
 * @procedure
 * @step 1 Call function wm_part_del(style, "elm.swallow.content").
 * @step 2 Check returned value.
 * </td>
 * <td>Style *style, "elm.swallow.content"</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */

EFLETE_TEST (wm_part_del_test_p1)
{
   elm_init(0,0);
   Evas_Object *win = NULL;
   Evas *e = NULL;
   Part *part = NULL;
   Style *style = NULL;
   const char *name = "elm.swallow.content";
   const char *style_name = "def";
   const char *full_style_name = "elm/radio/base/def";

   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   e = evas_object_evas_get(win);
   style = wm_style_add(style_name, full_style_name, STYLE, NULL);
   style->obj = edje_edit_object_add(e);
   edje_object_file_set(style->obj, "./edj_build/wm_part_del.edj", "elm/radio/base/def");
   part = wm_part_add(style, name);
   ck_assert_msg(wm_part_del(style, part) == EINA_TRUE, "cannot delete Part object");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup wm_part_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
