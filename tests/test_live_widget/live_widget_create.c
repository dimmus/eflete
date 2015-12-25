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

#include "test_live_widget.h"

/**
 * @addtogroup live_widget_test
 * @{
 * @addtogroup live_widget_create
 * @{
 * live_widget
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_widget_create
 * @{
 * <tr>
 * <td>live_widget_create</td>
 * <td>live_widget_create_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create style object as Style.
 * @step 3 Create parent window.
 *
 * @procedure
 * @step 1 Call live_widget_create.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(const char *)widget = "radio", (Style *) style, Evas_Object *parent</td>
 * <td>Not NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_create_test_p)
{
ck_assert(false);
   elm_init(0, 0);

//   Style *style = NULL;
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;

//   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
//   live = live_widget_create("radio", style, parent);
   ck_assert_msg(live != NULL, "Failed to create live object.");

//   wm_style_free(style);
   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_widget_create
 * @{
 * <tr>
 * <td>live_widget_create</td>
 * <td>live_widget_create_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create style object as Style with unexisted name.
 * @step 3 Create parent window.
 *
 * @procedure
 * @step 1 Call live_widget_create (call for unexisted widget).
 * @step 2 Check returned pointer.
 * </td>
 * <td>(const char *)widget = "fhtagn", (Style *) style, Evas_Object *parent</td>
 * <td>NULL pointer returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_create_test_n1)
{
   elm_init(0, 0);

   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;
//   Style *style = NULL;

//   style = wm_style_add("game_with_you", "lets_play/game_with_you", STYLE, NULL);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
//   live = live_widget_create("fhtagn", style, parent);
   ck_assert_msg(live == NULL, "Something was created o.o");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_widget_create
 * @{
 * </TABLE>
 * @}
 * @}
 */
