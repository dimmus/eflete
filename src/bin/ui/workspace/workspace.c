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

#include "workspace.h"
#include "main_window.h"
#include "highlight.h"
#include "groupedit.h"
#include "container.h"
#include "eflete.h"
#include "group_navigator.h"
#include "history.h"
#include "demo.h"
#include "demo_group.h"
#include "project_manager.h"
#include "change.h"

typedef enum
{
   MODE_NORMAL = 1,
   /* MODE_SEPARATE, */
   /* MODE_ANIMATOR, */
   /* MODE_CODE, */
   MODE_DEMO,
   MODE_LAST
} Workspace_Mode;

Eina_Bool
workspace_zoom_factor_set(Evas_Object *obj __UNUSED__, double factor __UNUSED__)
{
   return false;
}

double
workspace_zoom_factor_get(Evas_Object *obj __UNUSED__)
{
   return -1.0;
}

Eina_Bool
workspace_highlight_unset(Evas_Object *obj __UNUSED__)
{
   return false;
}

TODO("remove after property refactor!!! HIGH LEVEL");
Eina_Bool
workspace_active_demo_mode_get(Evas_Object *obj __UNUSED__)
{
   return false;
}

Evas_Object *
workspace_add(Evas_Object *parent __UNUSED__, Group *group __UNUSED__)
{
   return NULL;
}

void
workspace_group_navigator_update_part(Evas_Object *obj __UNUSED__, Part *part __UNUSED__)
{
}

Eina_Bool
workspace_edit_object_recalc(Evas_Object *obj __UNUSED__)
{
   return false;
}

void
workspace_groupview_hard_update(Evas_Object *obj __UNUSED__)
{
}

void
workspace_groupview_soft_update(Evas_Object *obj __UNUSED__)
{
}

Eina_Bool
workspace_highlight_align_visible_set(Evas_Object *obj __UNUSED__, Eina_Bool flag __UNUSED__)
{
   return false;
}

Eina_Bool
workspace_highlight_align_visible_get(Evas_Object *obj __UNUSED__)
{
   return false;
}

Eina_Bool
workspace_object_area_visible_set(Evas_Object *obj __UNUSED__, Eina_Bool flag __UNUSED__)
{
   return false;
}

Eina_Bool
workspace_object_area_visible_get(Evas_Object *obj __UNUSED__)
{
   return false;
}

void
workspace_part_add(Evas_Object *obj __UNUSED__, Eina_Stringshare *part_name __UNUSED__)
{
}

void
workspace_part_del(Evas_Object *obj __UNUSED__, Eina_Stringshare *part_name __UNUSED__)
{
}

void
workspace_part_item_add(Evas_Object *obj __UNUSED__,
                        Eina_Stringshare *part_name __UNUSED__,
                        Eina_Stringshare *item_name __UNUSED__)
{
}

void
workspace_part_item_del(Evas_Object *obj __UNUSED__,
                        Eina_Stringshare *part_name __UNUSED__,
                        Eina_Stringshare *item_name __UNUSED__)
{
}

void
workspace_part_state_add(Evas_Object *obj __UNUSED__,
                         Eina_Stringshare *part_name __UNUSED__,
                         Eina_Stringshare *state_name __UNUSED__)
{
}

void
workspace_part_state_select(Evas_Object *obj __UNUSED__,
                            Eina_Stringshare *part_name __UNUSED__,
                            Eina_Stringshare *state_name __UNUSED__)
{
}

void
workspace_part_state_del(Evas_Object *obj __UNUSED__,
                         Eina_Stringshare *part_name __UNUSED__,
                         Eina_Stringshare *state_name __UNUSED__)
{
}

void
workspace_part_restack(Evas_Object *obj __UNUSED__,
                       Eina_Stringshare *part_name __UNUSED__,
                       Eina_Stringshare *relative_part_name __UNUSED__)
{
}

void
workspace_part_item_restack(Evas_Object *obj __UNUSED__,
                            Eina_Stringshare *part_name __UNUSED__,
                            Eina_Stringshare *part_item_name __UNUSED__,
                            Eina_Stringshare *relative_part_item_name __UNUSED__)
{
}

void
workspace_program_add(Evas_Object *obj __UNUSED__, Eina_Stringshare *program_name __UNUSED__)
{
}

void
workspace_program_del(Evas_Object *obj __UNUSED__, Eina_Stringshare *program_name __UNUSED__)
{
}
