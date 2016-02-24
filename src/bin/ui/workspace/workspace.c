/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#define WORKSPACE_DATA "workspace_data"

#define PANES_RIGHT_SIZE_MIN 225

typedef enum
{
   MODE_NORMAL = 1,
   /* MODE_SEPARATE, */
   /* MODE_ANIMATOR, */
   /* MODE_CODE, */
   MODE_DEMO,
   MODE_LAST
} Workspace_Mode;

struct _Workspace_Data
{
   Evas_Object *panes; /* equal to all workspace, this object returned in workspace_add */
   Evas_Object *group_navi;
   Evas_Object *demo_navi;
   Evas_Object *toolbar;
   Evas_Object *panes_h; /* for set subobject like code, sequance etc */
   Evas_Object *workspace_layout; /* layout for rulers and scroller */
   Evas_Object *ruler_v, *ruler_h;
   struct {
      Evas_Object *scroller;
      Evas_Object *container;
      Evas_Object *bg;
      Evas_Object *content; /* for normal mode - groupview, for demo - elm widget */
   } normal, demo;
   struct {
      Evas_Object *obj;
      double size;
   };
   Workspace_Mode mode;
   Group *group;
};

typedef struct _Workspace_Data Workspace_Data;

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

static void
_job_workspace_del(void *data)
{
   assert(data != NULL);

   Workspace_Data *wd = data;
   gm_group_edit_object_unload(wd->group);

   free(wd);
}

static void
_workspace_del(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   ecore_job_add(_job_workspace_del, data);
}

Evas_Object *
workspace_add(Evas_Object *parent, Group *group)
{
   Workspace_Data *wd;

   assert(parent != NULL);
   assert(group != NULL);

   /* load the editable object */
   gm_group_edit_object_load(ap.project, group, evas_object_evas_get(ap.win));
   edje_object_animation_set(group->edit_object, false);

   wd = mem_calloc(1, sizeof(Workspace_Data));
   wd->panes = elm_panes_add(parent);
   elm_panes_content_right_min_size_set(wd->panes, PANES_RIGHT_SIZE_MIN);
   elm_panes_content_right_size_set(wd->panes, PANES_RIGHT_SIZE_MIN);
   wd->group = group;

   evas_object_data_set(wd->panes, WORKSPACE_DATA, wd);
   evas_object_event_callback_add(wd->panes, EVAS_CALLBACK_DEL, _workspace_del, wd);

   return wd->panes;
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
