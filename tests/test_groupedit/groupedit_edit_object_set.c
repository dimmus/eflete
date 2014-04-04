/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "test_groupedit.h"

/**
 * @addtogroup groupedit_test
 * @{
 * @addtogroup groupedit_edit_object_set
 * @{
 * Groupedit
 * <TABLE>
 * @}
 */

/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * <tr>
 * <td>groupedit_edit_object_set</td>
 * <td>groupedit_edit_object_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 * @step 4 create a edje edit obejct
 * @step 5 load some group from file to edje edit object
 *
 * @procedure
 * @step 1 set to the groupedit editable object
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, (Evas_Object) *edje_edit, (const char *) file</td>
 * <td>EINA_TRUE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_edit_object_set_test_p)
{
   Evas_Object *parent, *groupedit, *edje_edit;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   groupedit = groupedit_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "edj_build/radio.edj", "elm/radio/base/def");

   ck_assert_msg(groupedit_edit_object_set(groupedit, edje_edit, "./edj_build/radio.edj"),
                 "The edje edit object not setted to the groupedit.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * <tr>
 * <td>groupedit_edit_object_set</td>
 * <td>groupedit_edit_object_set_test_n1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a edje edit obejct
 * @step 4 load some group from file to edje edit object
 *
 * @procedure
 * @step 1 set to the groupedit editable object
 * @step 2 check returned value
 * </td>
 * <td>NULL, (Evas_Object) *edje_edit, (const char *) file</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_edit_object_set_test_n1)
{
   Evas_Object *parent, *edje_edit;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "./edj_build/radio.edj", "elm/radio/base/def");

   ck_assert_msg(!groupedit_edit_object_set(NULL, edje_edit, "./edj_build/radio.edj"),
                 "The edje edit object setted to the NULL pointer.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * <tr>
 * <td>groupedit_edit_object_set</td>
 * <td>groupedit_edit_object_set_test_n2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 *
 * @procedure
 * @step 1 set to the groupedit editable object
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, NULL, (const char *) file</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_edit_object_set_test_n2)
{
   Evas_Object *parent, *groupedit;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   groupedit = groupedit_add(parent);

   ck_assert_msg(!groupedit_edit_object_set(groupedit, NULL, "./edj_build/radio.edj"),
                 "The NULL edje edit object setted to the groupedit.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST

/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * <tr>
 * <td>groupedit_edit_object_set</td>
 * <td>groupedit_edit_object_set_test_n3</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create parent window
 * @step 3 create a groupedit
 * @step 4 create a edje edit obejct
 * @step 5 load some group from file to edje edit object
 *
 * @procedure
 * @step 1 set to the groupedit editable object
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) groupedit, (Evas_Object) *edje_edit, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFLETE_TEST(groupedit_edit_object_set_test_n3)
{
   Evas_Object *parent, *groupedit, *edje_edit;

   elm_init(0, 0);
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   groupedit = groupedit_add(parent);
   edje_edit = edje_edit_object_add(evas_object_evas_get(parent));
   edje_object_file_set(edje_edit, "./edj_build/radio.edj", "elm/radio/base/def");

   ck_assert_msg(!groupedit_edit_object_set(groupedit, edje_edit, NULL),
                 "The edje edit object with not valid file setted to the groupedit.");

   evas_object_del(parent);

   elm_shutdown();
}
END_TEST


/**
 * @addtogroup groupedit_edit_object_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
