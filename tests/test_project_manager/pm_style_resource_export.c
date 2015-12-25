/*
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

#include "test_project_manager.h"
#include "main_window.h"
#include "test_common.h"

/**
 * @addtogroup project_manager_test
 * @{
 * @addtogroup pm_style_resource_export
 * @{
 * project_manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup pm_style_resource_export
 * @{
 * <tr>
 * <td>pm_style_resource_export</td>
 * <td>pm_style_resource_export_test_p</td>
 * <td>
 * This test check the export style resources  procedure.<br>
 * @precondition
 * @step 1 Initialize elementary library.
 * @step 2 Initialize Application Data structure.
 * @step 3 Create directory for export resources.
 * @step 4 Initialize main window.
 * @step 5 Open project.
 * @step 6 Fill widget inlist with data.
 * @step 7 Find style that represent the group "elm/radio/base/def"
 * @step 8 Load style into project.
 *
 * @procedure
 * @step 1 Call pm_style_resource_export;
 * @step 2 Check returned value.
 * @step 3 Check exists image file in given path.
 * @step 4 Check exists sound file in given path.
 * @step 5 Check exists font file in given path.
 * </td>
 * <td>(Project *)project, (Style *)style, (Eina_Stringshare *)path</td>
 * <td>All check passed </td>
 * </tr>
 * @}
 */

EFLETE_TEST(pm_style_resource_export_test_p)
{
   Eina_Bool result = EINA_FALSE;
   //Style *style = NULL;
   Eina_Stringshare *path = NULL;
   Eina_Stringshare *check_resource = NULL;
ck_assert(false);
   elm_init(0,0);
   app_init();
   ecore_file_recursive_rm("./UTC");
   path = eina_stringshare_add("./pm_style_resource_export_test_p/export");
   check_resource = eina_stringshare_printf("%s/images/radio.png", path);
   ecore_file_mkdir(path);

   ui_main_window_add();
   ap.project = setup("pm_style_resource_export_test_p");
  /* wm_widgets_list_objects_load(ap.project->widgets,
                                evas_object_evas_get(ap.win),
                                ap.project->mmap_file);*/
   //style = wm_style_object_find(ap.project->widgets, "elm/radio/base/def");
   //ui_style_clicked(style);

   //result = pm_style_resource_export(ap.project, style, path);
   ck_assert_msg(result, "Failed export resources of group.");
   result = ecore_file_exists(check_resource);
   ck_assert_msg(result, "Exported image does not exist");
   eina_stringshare_del(check_resource);
   check_resource = eina_stringshare_printf("%s/sounds/kbd-tap.wav", path);
   result = ecore_file_exists(check_resource);
   ck_assert_msg(result, "Exported sound does not exist");
   eina_stringshare_del(check_resource);
   check_resource = eina_stringshare_printf("%s/fonts/PTS55F.ttf", path);
   result = ecore_file_exists(check_resource);
   ck_assert_msg(result, "Exported font does not exist");

   pm_project_close(ap.project);
   ecore_file_recursive_rm("./UTC");
   ap.project = NULL;
   ui_main_window_del();
   app_shutdown();
   teardown("./pm_style_resource_export_test_p");
   eina_stringshare_del(path);
   eina_stringshare_del(check_resource);
   elm_shutdown();
}
END_TEST

/**
 * @addtogroup pm_style_resource_export
 * @{
 * </TABLE>
 * @}
 * @}
 */
