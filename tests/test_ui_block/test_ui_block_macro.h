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

#define TEST_UI_BLOCK_CONTENT_TC(_func_,tst,bl,block_obj,cont,result,pass_cond) \
EFLETE_TEST (_func_##_test_##tst) \
{ \
   elm_init(0, 0); \
   Evas_Object *parent, *content; \
   App_Data apd; \
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC); \
   apd.block.bl = block_obj; \
   content = cont; \
   ck_assert_msg(_func_##_set(&apd, content) == pass_cond, "Function is not working right"); \
   ck_assert_msg(_func_##_get(&apd) == result,"Content isn't set or returned properly"); \
   elm_shutdown(); \
} \
END_TEST

#define TEST_UI_BLOCK_CONTENT(_func_,_block_) \
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
TEST_UI_BLOCK_CONTENT_TC(_func_,p,_block_,ui_block_add(parent),ui_block_add(parent),content,EINA_TRUE) \
/** \
 * @addtogroup _func_\
 * @{ \
 * <tr> \
 * <td> ## _func_## </td> \
 * <td> ## _func_##_test_n1</td> \
 * <td> \
 * @precondition \
 * @step 1 initialized elm \
 * @step 2 Created window. \
 * @step 3 Block object is not created. \
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
TEST_UI_BLOCK_CONTENT_TC(_func_,n1,_block_,NULL,ui_block_add(parent),NULL,EINA_FALSE) \
/** \
 * @addtogroup _func_\
 * @{ \
 * <tr> \
 * <td> ##_func_## </td> \
 * <td> ##_func_##_test_n2</td> \
 * <td> \
 * @precondition \
 * @step 1 initialized elm \
 * @step 2 Created window. \
 * @step 3 Created block object. \
 * \
 * @procedure \
 * @step 1 Call _func_##_set(app_data, NULL) \
 * @step 2 Call _func_##_get(app_data) \
 * </td> \
 * <td>App_Data *app_data, NULL</td> \
 * <td>all checks passed</td> \
 * </tr> \
 * @} \
 */ \
TEST_UI_BLOCK_CONTENT_TC(_func_,n2,_block_,ui_block_add(parent),NULL,NULL,EINA_FALSE)
