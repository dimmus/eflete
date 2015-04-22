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
 * @addtogroup live_widget_del
 * @{
 * live_widget
 * <TABLE>
 * @}
 */

/**
 * @addtogroup live_widget_del
 * @{
 * <tr>
 * <td>live_widget_del</td>
 * <td>live_widget_del_test_p</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create style object as Style.
 * @step 3 Create parent window.
 * @step 4 Create radio live_object.
 *
 * @procedure
 * @step 1 Call live_widget_del.
 * @step 2 Check returned pointer.
 * </td>
 * <td>Evas_Object *live_object</td>
 * <td>EINA_TRUE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_del_test_p)
{
   elm_init(0, 0);

   Style *style = NULL;
   Evas_Object *parent = NULL;
   Evas_Object *live = NULL;

   style = wm_style_add("def", "elm/radio/base/def", STYLE, NULL);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   live = live_widget_create("radio", style, parent);
   ck_assert_msg(live_widget_del(live), "it is not able to delete live object, something is wrong.");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_widget_del
 * @{
 * <tr>
 * <td>live_widget_del</td>
 * <td>live_widget_del_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create parent window.
 * @step 3 Create button (not live object).
 *
 * @procedure
 * @step 1 Call live_widget_del.
 * @step 2 Check returned pointer.
 * </td>
 * <td>Evas_Object *button</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_del_test_n1)
{
   elm_init(0, 0);

   Evas_Object *parent = NULL;
   Evas_Object *button = NULL;

   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   button = elm_button_add(parent);
   ck_assert_msg(!live_widget_del(button), "Not live object was deleted as live object.");

   evas_object_del(parent);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_widget_del
 * @{
 * <tr>
 * <td>live_widget_del</td>
 * <td>live_widget_del_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 *
 * @procedure
 * @step 1 Call live_widget_del.
 * @step 2 Check returned pointer.
 * </td>
 * <td>NULL</td>
 * <td>EINA_FALSE returned</td>
 * </tr>
 * @}
 */
EFLETE_TEST(live_widget_del_test_n2)
{
   elm_init(0, 0);

   ck_assert_msg(!live_widget_del(NULL), "NULL was deleted.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup live_widget_del
 * @{
 * </TABLE>
 * @}
 * @}
 */
