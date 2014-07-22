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
 * @step 2 Create parent window.
 *
 * @procedure
 * @step 1 Call live_widget_create.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(const char *)widget = "bubble", (const char *)class = "top_left",
 *     (const char *)style = "default", Evas_Object *parent</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_create_test_p)
{
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;
   const char *widget = "bubble";
   const char *class = "top_left";
   const char *style = "default";

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   live = live_widget_create(widget, class, style, parent);
   ck_assert_msg(live != NULL, "Failed to create live object.");

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
 * @step 2 Create parent window.
 *
 * @procedure
 * @step 1 Call live_widget_create (call for unexisted widget).
 * @step 2 Check returned pointer.
 * </td>
 * <td>(const char *)widget = "fhtagn", (const char *)class = "lets_play",
 *     (const char *)style = "game_with_you", Evas_Object *parent</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_create_test_n1)
{
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;
   const char *widget = "fhtagn";
   const char *class = "lets_play";
   const char *style = "game_with_you";

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   live = live_widget_create(widget, class, style, parent);
   ck_assert_msg(live == NULL, "Something was created o.o");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_widget_create
 * @{
 * <tr>
 * <td>live_widget_create</td>
 * <td>live_widget_create_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 *
 * @procedure
 * @step 1 Call live_widget_create (call for unexisted widget).
 * @step 2 Check returned pointer.
 * </td>
 * <td>NULL, NULL, NULL, NULL</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_create_test_n2)
{
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;
   const char *widget = "bubble";
   const char *class = "top_left";
   const char *style = "default";

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   live = live_widget_create(NULL, class, style, parent);
   ck_assert_msg(live == NULL, "Live object was created with NULL params 1");
   live = live_widget_create(widget, NULL, style, parent);
   ck_assert_msg(live == NULL, "Live object was created with NULL params 2");
   live = live_widget_create(widget, class, NULL, parent);
   ck_assert_msg(live == NULL, "Live object was created with NULL params 3");
   live = live_widget_create(widget, class, style, NULL);
   ck_assert_msg(live == NULL, "Live object was created with NULL params 4");

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
