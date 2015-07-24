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

#define TEST_UI_BLOCK_CONTENT_TC(_func_,bl) \
EFLETE_TEST (_func_##_test_p) \
{ \
   elm_init(0, 0); \
   Evas_Object *parent, *content; \
   App_Data *apd; \
   app_init(); \
   apd = app_data_get(); \
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC); \
   elm_object_theme_set(parent, apd->theme); \
   content = ui_block_add(parent); \
   apd->block.bl = ui_block_add(parent); \
   ck_assert_msg(_func_##_set(apd, content) == EINA_TRUE, "Function is not working right"); \
   ck_assert_msg(_func_##_get(apd) == content,"Content isn't set or returned properly"); \
   elm_shutdown(); \
} \
END_TEST

#define TEST_UI_BLOCK_CONTENT(_func_, _block_) \
/** \
 * @addtogroup _func_\
 * @{ \
 * <tr> \
 * <td> ##_func_## </td> \
 * <td> ##_func_##_test_p</td> \
 * <td> \
 * @precondition \
 * @step 1 initialized elm \
 * @step 2 Created window. \
 * @step 3 Created block object. \
 * \
 * @procedure \
 * @step 1 Create content to set. \
 * @step 2 Call _func_##_set(app_data, content) \
 * @step 3 Call _func_##_get(app_data) \
 * </td> \
 * <td>App_Data *app_data, Evas_Object *content</td> \
 * <td>all checks passed</td> \
 * </tr> \
 * @} \
 */ \
TEST_UI_BLOCK_CONTENT_TC(_func_,_block_)
