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
 * @addtogroup history_genlist_get
 * @{
 * history
 * <TABLE>
 * @}
 */

/**
 * @addtogroup history_genlist_get
 * @{
 * <tr>
 * <td>history_genlist_get</td>
 * <td>history_genlist_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize history module.
 * @step 3 Create window, that will be parent for genlist.
 *
 * @procedure
 * @step 1 Call history_genlist_get.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(History *) history, (Evas_Object *) win</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_genlist_get_test_p1)
{
   History *history = NULL;
   Evas_Object *win = NULL;
   const Evas_Object *genlist = NULL;

   elm_init(0, 0);
   history = history_init();
   win = elm_win_add(NULL, "history_genlist_get", ELM_WIN_BASIC);

   genlist = history_genlist_get(history, win);
   ck_assert_msg(genlist != NULL, "Failed to get history genlist.");

   history_term(history);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_genlist_get
 * @{
 * <tr>
 * <td>history_genlist_get</td>
 * <td>history_genlist_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize history module.
 * @step 3 Create window, that will be parent for genlist.
 * @step 4 Call history_genlist_get.
 *
 * @procedure
 * @step 1 Call history_genlist_get. without parent param.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(History *) history, NULL</td>
 * <td>Not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_genlist_get_test_p2)
{
   History *history = NULL;
   Evas_Object *win = NULL;
   const Evas_Object *genlist = NULL;

   elm_init(0, 0);
   history = history_init();
   win = elm_win_add(NULL, "history_genlist_get", ELM_WIN_BASIC);
   genlist = history_genlist_get(history, win);

   genlist = history_genlist_get(history, NULL);
   ck_assert_msg(genlist != NULL, "Failed to get history genlist.");

   history_term(history);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_genlist_get
 * @{
 * <tr>
 * <td>history_genlist_get</td>
 * <td>history_genlist_get_test_n1</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize history module.
 *
 * @procedure
 * @step 1 Call history_genlist_get without parent.
 * @step 2 Check returned pointer.
 * </td>
 * <td>(History *) history, NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_genlist_get_test_n1)
{
   History *history = NULL;
   const Evas_Object *genlist = NULL;

   elm_init(0, 0);
   history = history_init();

   genlist = history_genlist_get(history, NULL);
   ck_assert_msg(genlist == NULL, "Get history genlist without parent.");

   history_term(history);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_genlist_get
 * @{
 * <tr>
 * <td>history_genlist_get</td>
 * <td>history_genlist_get_test_n2</td>
 * <td>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Create window, that will be parent for genlist.
 *
 * @procedure
 * @step 1 Call history_genlist_get. without history.
 * @step 2 Check returned pointer.
 * </td>
 * <td>NULL, (Evas_Object *)win</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(history_genlist_get_test_n2)
{
   Evas_Object *win = NULL;
   const Evas_Object *genlist = NULL;

   elm_init(0, 0);
   win = elm_win_add(NULL, "history_genlist_get", ELM_WIN_BASIC);

   genlist = history_genlist_get(NULL, win);
   ck_assert_msg(genlist == NULL, "Get history genlist without history.");

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup history_genlist_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
