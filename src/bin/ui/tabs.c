/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "tabs_private.h"
#include "workspace.h"
#include "tabs.h"
#include "history.h"
#include "project_manager.h"
#include "main_window.h"
#include "change.h"

struct _Tabs_Item {
   Group2 *group;
   Elm_Object_Item *toolbar_item;
   Evas_Object *content;
   Eina_Bool need_recalc : 1;
};

typedef struct _Tabs_Item Tabs_Item;

struct _Tab_Home {
   Evas_Object *content;
   Evas_Object *tabs;
   Elm_Object_Item *tab_project_info;
   Evas_Object *content_project_info;
   Elm_Object_Item *tab_open_project;
   Evas_Object *content_open_project;
   Elm_Object_Item *tab_new_project;
   Evas_Object *content_new_project;
   Elm_Object_Item *tab_import_edj;
   Evas_Object *content_import_edj;
   Elm_Object_Item *tab_import_edc;
   Evas_Object *content_import_edc;
};

typedef struct _Tab_Home Tab_Home;

struct _Tabs {
   Evas_Object *layout;
   Evas_Object *toolbar;
   Evas_Object *box;
   Elm_Object_Item *selected;
   Eina_List *items;
   Evas_Object *current_workspace;
   Group2 *current_group;
   Tab_Home home;
};

typedef struct _Tabs Tabs;

static Tabs tabs;

static void
_content_unset(void)
{
   Evas_Object *content;

   assert(tabs.layout != NULL);
   tabs.current_workspace = NULL;
   tabs.current_group = NULL;
   content = elm_layout_content_unset(tabs.layout, NULL);
   evas_object_hide(content);
   content = elm_layout_content_unset(ap.panes.left_ver, "right");
   evas_object_hide(content);
}

static void
_content_set(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Tabs_Item *item = (Tabs_Item *)data;
   Elm_Object_Item *toolbar_item = event_info;

   assert(tabs.layout != NULL);
   if (tabs.selected == toolbar_item) return;
   elm_toolbar_item_selected_set(tabs.selected, false);
   tabs.selected = toolbar_item;

   _content_unset();
   if (item)
     {
        elm_layout_content_set(tabs.layout, NULL, item->content);
        if (!item->group)
          {
             elm_object_part_content_set(ap.panes.left_ver, "right", workspace_group_navigator_get(NULL));
             elm_object_part_content_unset(ap.panes.right, "right");
             evas_object_hide(ap.property.group);
             evas_object_hide(ap.property.demo);
             return;
          }
        else
          elm_object_part_content_set(ap.panes.left_ver, "right", workspace_group_navigator_get(item->content));
        tabs.current_workspace = item->content;
        tabs.current_group = item->group;
        if (ap.project)
          ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, false);
        if (item->need_recalc)
          {
             workspace_groupview_hard_update(tabs.current_workspace);
             item->need_recalc = false;
          }

        /* notify that group is changed */
        evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_CHANGED, item->group);

        /* and update property */
        if (workspace_active_demo_mode_get(tabs.current_workspace))
          {
             elm_object_part_content_unset(ap.panes.right, "right");
             evas_object_hide(ap.property.group);
             elm_object_part_content_set(ap.panes.right, "right", ap.property.demo);
             evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_MODE_DEMO, NULL);
          }
        else
          {
             elm_object_part_content_unset(ap.panes.right, "right");
             evas_object_hide(ap.property.demo);
             elm_object_part_content_set(ap.panes.right, "right", ap.property.group);
             evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_MODE_GROUP, NULL);
          }
     }
}

static void
_mode_changed(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info)
{
   Evas_Object *content;

   content = elm_layout_content_unset(ap.panes.left_ver, "right");
   evas_object_hide(content);

   elm_layout_content_set(ap.panes.left_ver, "right", workspace_group_navigator_get(tabs.current_workspace));
   Workspace_Mode mode = *((Workspace_Mode *)event_info);

   if (mode == MODE_DEMO)
     {
        elm_object_part_content_unset(ap.panes.right, "right");
        evas_object_hide(ap.property.group);
        elm_object_part_content_set(ap.panes.right, "right", ap.property.demo);
     }
   else
     {
        elm_object_part_content_unset(ap.panes.right, "right");
        evas_object_hide(ap.property.demo);
        elm_object_part_content_set(ap.panes.right, "right", ap.property.group);
     }
}

static Tabs_Item *
_find_tab(Group2 *group)
{
   Eina_List *l;
   Tabs_Item *item;

   EINA_LIST_FOREACH(tabs.items, l, item)
     {
        if (item->group == group)
          return item;
     }
   return NULL;
}

static void
_del_tab(Tabs_Item *item)
{
   elm_object_item_del(item->toolbar_item);
   /* delete pans with workspace and liveview */
   evas_object_del(item->content);
   if (item->group)
     {
        history_del(item->group->history);
        item->group->history = NULL;
        item->group->current_selected = NULL;
        item->group = NULL;
     }
   free(item);
}

static void
_home_tab_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   evas_object_hide(elm_layout_content_unset(tabs.home.content, NULL));
   elm_layout_content_set(tabs.home.content, NULL, data);
   _tab_project_update();
}

static void
_property_attribute_changed(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Attribute *attr = event_info;

   /* for example when image is deleted from image manager */
   if (!tabs.current_workspace)
     return;

   switch ((int)*attr)
     {
      case ATTRIBUTE_GROUP_MIN_W:
      case ATTRIBUTE_GROUP_MIN_H:
      case ATTRIBUTE_GROUP_MAX_W:
      case ATTRIBUTE_GROUP_MAX_H:
      case ATTRIBUTE_STATE_MIN_W:
      case ATTRIBUTE_STATE_MIN_H:
      case ATTRIBUTE_STATE_MAX_W:
      case ATTRIBUTE_STATE_MAX_H:
      case ATTRIBUTE_STATE_ALIGN_X:
      case ATTRIBUTE_STATE_ALIGN_Y:
      case ATTRIBUTE_STATE_REL1_RELATIVE_X:
      case ATTRIBUTE_STATE_REL1_RELATIVE_Y:
      case ATTRIBUTE_STATE_REL2_RELATIVE_X:
      case ATTRIBUTE_STATE_REL2_RELATIVE_Y:
      case ATTRIBUTE_STATE_REL1_OFFSET_X:
      case ATTRIBUTE_STATE_REL1_OFFSET_Y:
      case ATTRIBUTE_STATE_REL2_OFFSET_X:
      case ATTRIBUTE_STATE_REL2_OFFSET_Y:
      case ATTRIBUTE_STATE_ASPECT_MIN:
      case ATTRIBUTE_STATE_ASPECT_MAX:
      case ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_X:
      case ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_Y:
      case ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_X:
      case ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_Y:
      case ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_X:
      case ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_Y:
      case ATTRIBUTE_STATE_FILL_SIZE_OFFSET_X:
      case ATTRIBUTE_STATE_FILL_SIZE_OFFSET_Y:
      case ATTRIBUTE_STATE_TEXT_ALIGN_X:
      case ATTRIBUTE_STATE_TEXT_ALIGN_Y:
      case ATTRIBUTE_STATE_TEXT_ELIPSIS:
      case ATTRIBUTE_STATE_TEXT_SIZE:
      case ATTRIBUTE_STATE_TEXT_FIT_X:
      case ATTRIBUTE_STATE_TEXT_FIT_Y:
      case ATTRIBUTE_STATE_TEXT_MAX_X:
      case ATTRIBUTE_STATE_TEXT_MAX_Y:
      case ATTRIBUTE_STATE_TEXT_MIN_X:
      case ATTRIBUTE_STATE_TEXT_MIN_Y:
      case ATTRIBUTE_STATE_FIXED_H:
      case ATTRIBUTE_STATE_FIXED_W:
      case ATTRIBUTE_STATE_IMAGE:
      case ATTRIBUTE_STATE_IMAGE_TWEEN:
      case ATTRIBUTE_STATE_REL1_TO_X:
      case ATTRIBUTE_STATE_REL1_TO_Y:
      case ATTRIBUTE_STATE_REL2_TO_X:
      case ATTRIBUTE_STATE_REL2_TO_Y:
      case ATTRIBUTE_STATE_TEXT_SOURCE:
      case ATTRIBUTE_STATE_TEXT_TEXT_SOURCE:
      case ATTRIBUTE_STATE_TEXT:
      case ATTRIBUTE_STATE_FONT:
      case ATTRIBUTE_STATE_TEXT_STYLE:
      case ATTRIBUTE_STATE_ASPECT_PREF:
      case ATTRIBUTE_PART_TEXT_EFFECT:
      case ATTRIBUTE_PART_TEXT_SHADOW_DIRECTION:
      case ATTRIBUTE_PART_CLIP_TO:
      case ATTRIBUTE_PART_DRAG_CONFINE:
      case ATTRIBUTE_PART_GROUP_SOURCE:
      case ATTRIBUTE_PART_TEXTBLOCK_SELECTION_UNDER:
      case ATTRIBUTE_PART_TEXTBLOCK_SELECTION_OVER:
      case ATTRIBUTE_PART_TEXTBLOCK_CURSOR_UNDER:
      case ATTRIBUTE_PART_TEXTBLOCK_CURSOR_OVER:
      case ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_UNDER:
      case ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_OVER:
      case ATTRIBUTE_STATE_CONTAINER_ALIGN_X:
      case ATTRIBUTE_STATE_CONTAINER_ALIGN_Y:
      case ATTRIBUTE_STATE_CONTAINER_MIN_H:
      case ATTRIBUTE_STATE_CONTAINER_MIN_V:
      case ATTRIBUTE_STATE_TABLE_HOMOGENEOUS:
      case ATTRIBUTE_STATE_CONTAINER_PADING_X:
      case ATTRIBUTE_STATE_CONTAINER_PADING_Y:
      case ATTRIBUTE_STATE_MINMUL_H:
      case ATTRIBUTE_STATE_MINMUL_W:
      case ATTRIBUTE_PART_MULTILINE:
      case ATTRIBUTE_PART_ENTRY_MODE:
      case ATTRIBUTE_STATE_FILL_TYPE:
      case ATTRIBUTE_STATE_COLOR:
      case ATTRIBUTE_STATE_OUTLINE_COLOR:
      case ATTRIBUTE_STATE_SHADOW_COLOR:
      case ATTRIBUTE_STATE_MAP_ROTATION_CENTER:
      case ATTRIBUTE_STATE_MAP_ROTATION_X:
      case ATTRIBUTE_STATE_MAP_ROTATION_Y:
      case ATTRIBUTE_STATE_MAP_ROTATION_Z:
      case ATTRIBUTE_STATE_MAP_POINT_COLOR_1:
      case ATTRIBUTE_STATE_MAP_POINT_COLOR_2:
      case ATTRIBUTE_STATE_MAP_POINT_COLOR_3:
      case ATTRIBUTE_STATE_MAP_POINT_COLOR_4:
         workspace_groupview_hard_update(tabs.current_workspace);
         break;
      case ATTRIBUTE_STATE_FILL_SMOOTH:
      case ATTRIBUTE_STATE_VISIBLE:
      case ATTRIBUTE_STATE_IMAGE_BORDER_TOP:
      case ATTRIBUTE_STATE_IMAGE_BORDER_BOTTOM:
      case ATTRIBUTE_STATE_IMAGE_BORDER_LEFT:
      case ATTRIBUTE_STATE_IMAGE_BORDER_RIGHT:
      case ATTRIBUTE_STATE_IMAGE_BORDER_FILL:
      case ATTRIBUTE_STATE_COLOR_CLASS:
      case ATTRIBUTE_STATE_MAP_ON:
      case ATTRIBUTE_STATE_MAP_PERSPECTIVE_ON:
      case ATTRIBUTE_STATE_MAP_PERSPECTIVE:
      case ATTRIBUTE_STATE_MAP_LIGHT:
      case ATTRIBUTE_STATE_MAP_SMOOTH:
      case ATTRIBUTE_STATE_MAP_ALPHA:
      case ATTRIBUTE_STATE_MAP_BACKFACE_CULL:
      case ATTRIBUTE_STATE_MAP_PERSPECTIVE_FOCAL:
      case ATTRIBUTE_STATE_MAP_PERSPECTIVE_ZPLANE:
      case ATTRIBUTE_PART_ITEM_ASPECT_MODE:
      case ATTRIBUTE_PART_ITEM_ALIGN_X:
      case ATTRIBUTE_PART_ITEM_ALIGN_Y:
      case ATTRIBUTE_PART_ITEM_WEIGHT_X:
      case ATTRIBUTE_PART_ITEM_WEIGHT_Y:
      case ATTRIBUTE_PART_ITEM_ASPECT_H:
      case ATTRIBUTE_PART_ITEM_ASPECT_W:
      case ATTRIBUTE_PART_ITEM_MAX_H:
      case ATTRIBUTE_PART_ITEM_MAX_W:
      case ATTRIBUTE_PART_ITEM_MIN_H:
      case ATTRIBUTE_PART_ITEM_MIN_W:
      case ATTRIBUTE_PART_ITEM_PREFER_H:
      case ATTRIBUTE_PART_ITEM_PREFER_W:
      case ATTRIBUTE_PART_ITEM_SPREAD_H:
      case ATTRIBUTE_PART_ITEM_SPREAD_W:
      case ATTRIBUTE_PART_ITEM_SPAN_COL:
      case ATTRIBUTE_PART_ITEM_SPAN_ROW:
      case ATTRIBUTE_PART_ITEM_POSITION_COL:
      case ATTRIBUTE_PART_ITEM_POSITION_ROW:
      case ATTRIBUTE_PART_ITEM_SOURCE:
      case ATTRIBUTE_PART_ITEM_PADDING_RIGHT:
      case ATTRIBUTE_PART_ITEM_PADDING_LEFT:
      case ATTRIBUTE_PART_ITEM_PADDING_TOP:
      case ATTRIBUTE_PART_ITEM_PADDING_BOTTOM:
         workspace_groupview_soft_update(tabs.current_workspace);
         break;
      default:
         break;
     }
}

void
_on_save(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_code_reload(tabs.current_workspace);
}

void
_on_project_changed(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Eina_List *l;
   Tabs_Item *item;

   EINA_LIST_FOREACH(tabs.items, l, item)
     {
        if (!item->group) continue; /* skip home tab */
        workspace_code_changed(item->content);
     }
}

static void
_demo_swallow_set(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *ei)
{
   assert(tabs.current_workspace != NULL);

   workspace_demo_swallow_set(tabs.current_workspace, (Demo_Part *)ei);
}

static void
_demo_text_set(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *ei)
{
   assert(tabs.current_workspace != NULL);

   workspace_demo_text_set(tabs.current_workspace, (Demo_Part *)ei);
}

static void
_demo_send_signal(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *ei)
{
   assert(tabs.current_workspace != NULL);

   workspace_demo_signal_set(tabs.current_workspace, (Demo_Signal *)ei);
}

static void
_part_renamed(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Rename *ren = ei;
   Part2 *part;

   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);
   assert(ren != NULL);

   part = (Part2 *)resource_manager_find(tabs.current_group->parts, ren->old_name);
   workspace_group_navigator_update_part(tabs.current_workspace, part);
}

static void
_group_data_renamed(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Rename *ren = ei;
   Resource2 *group_data;

   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);
   assert(ren != NULL);

   group_data = resource_manager_find(tabs.current_group->data_items, ren->old_name);
   workspace_group_navigator_update_group_data(tabs.current_workspace, group_data);
}

static void
_editor_saved(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *ei __UNUSED__)
{
   Eina_List *l;
   Tabs_Item *item;

   pm_dev_file_reload(ap.project);
   EINA_LIST_FOREACH(tabs.items, l, item)
     {
        if (!item->group) continue; /* skip home tab */
        resource_group_edit_object_reload(ap.project, item->group);
        if (item->content == tabs.current_workspace)
          workspace_groupview_hard_update(tabs.current_workspace);
        else
          item->need_recalc = true;
     }
}

static void
_project_opened(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{

   tabs_home_tab_add(TAB_HOME_PROJECT_INFO);
}

static void
_project_closed(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{
   tabs_home_tab_add(TAB_HOME_OPEN_PROJECT);
}

static void
_editor_part_state_selected_cb(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info)
{
   const Editor_State *editor_state = event_info;

   assert(editor_state != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_state_select(tabs.current_workspace, editor_state->part_name,
                               editor_state->state_name, editor_state->state_value);
}

static void
_editor_part_added_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Eina_Stringshare *part_name = event_info;

   assert(part_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_add(tabs.current_workspace, part_name);
}

static void
_editor_part_deleted_cb(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Editor_Part *part = (Editor_Part *)event_info;

   assert(part != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_del(tabs.current_workspace, part->part_name);
}

static void
_editor_program_added_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Eina_Stringshare *program_name = event_info;

   assert(program_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_program_add(tabs.current_workspace, program_name);
}

static void
_editor_program_deleted_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Eina_Stringshare *program_name = event_info;

   assert(program_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_program_del(tabs.current_workspace, program_name);
}

static void
_editor_group_data_added_cb(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Eina_Stringshare *group_data_name = event_info;

   assert(group_data_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_group_data_add(tabs.current_workspace, group_data_name);
}

static void
_editor_group_data_deleted_cb(void *data __UNUSED__,
                              Evas_Object *obj __UNUSED__,
                              void *event_info)
{
   Eina_Stringshare *group_data_name = event_info;

   assert(group_data_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_group_data_del(tabs.current_workspace, group_data_name);
}

static void
_editor_part_item_added_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   const Editor_Item *editor_item = event_info;

   assert(editor_item != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_item_add(tabs.current_workspace, editor_item->part_name, editor_item->item_name);
}

static void
_editor_part_item_deleted_cb(void *data __UNUSED__,
                             Evas_Object *obj __UNUSED__,
                             void *event_info)
{
   const Editor_Item *editor_item = event_info;

   assert(editor_item != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_item_del(tabs.current_workspace, editor_item->part_name, editor_item->item_name);
}

static void
_editor_state_added_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   const Editor_State *editor_state = event_info;

   assert(editor_state != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_state_add(tabs.current_workspace, editor_state->part_name,
                               editor_state->state_name, editor_state->state_value);
}

static void
_editor_state_deleted_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   const Editor_State *editor_state = event_info;

   assert(editor_state != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_state_del(tabs.current_workspace, editor_state->part_name,
                               editor_state->state_name, editor_state->state_value);
}

static void
_editor_part_restacked_cb(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   const Editor_Part_Restack *editor_part_restack = event_info;

   assert(editor_part_restack != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_restack(tabs.current_workspace,
                          editor_part_restack->part_name,
                          editor_part_restack->relative_part_name);
}

static void
_editor_part_item_restacked_cb(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info)
{
   const Editor_Part_Item_Restack *editor_part_item_restack = event_info;

   assert(editor_part_item_restack != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_item_restack(tabs.current_workspace,
                               editor_part_item_restack->part_name,
                               editor_part_item_restack->part_item,
                               editor_part_item_restack->relative_part_item);
}

static void
_shortcut_add_part_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_add_part_request(tabs.current_workspace);
}

static void
_shortcut_add_part_item_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_add_part_item_request(tabs.current_workspace);
}

static void
_shortcut_add_state_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_add_state_request(tabs.current_workspace);
}

static void
_shortcut_add_program_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_add_program_request(tabs.current_workspace);
}

static void
_shortcut_add_data_item_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_add_group_data_request(tabs.current_workspace);
}

static void
_shortcut_del_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_delete_request(tabs.current_workspace);
}

static void
_shortcut_state_next_cb(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_state_next_request(tabs.current_workspace);
}

static void
_shortcut_part_next_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_part_next_request(tabs.current_workspace);
}

static void
_shortcut_part_prev_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_part_prev_request(tabs.current_workspace);
}

static void
_shortcut_part_showhide_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_part_showhide_request(tabs.current_workspace);
}

static void
_shortcut_part_unselect_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_part_unselect_request(tabs.current_workspace);
}

static void
_shortcut_all_parts_showhide_cb(void *data __UNUSED__,
                                Evas_Object *obj __UNUSED__,
                                void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_all_parts_showhide_request(tabs.current_workspace);
}

static void
_shortcut_tab_next_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Eina_List *l;
   Tabs_Item *item;

   l = eina_list_next(eina_list_data_find_list(tabs.items, elm_object_item_data_get(tabs.selected)));
   item = eina_list_data_get(l);
   if (item)
     elm_toolbar_item_selected_set(item->toolbar_item, true);
}

static void
_shortcut_tab_prev_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Eina_List *l;
   Tabs_Item *item;

   l = eina_list_prev(eina_list_data_find_list(tabs.items, elm_object_item_data_get(tabs.selected)));
   item = eina_list_data_get(l);
   if (item)
     elm_toolbar_item_selected_set(item->toolbar_item, true);
}

static void
_shortcut_tab_num_cb(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   int num = *((int *)event_info);
   Tabs_Item *item;

   item = eina_list_nth(tabs.items, num - 1);
   if (item)
     elm_toolbar_item_selected_set(item->toolbar_item, true);
}

static void
_shortcut_tab_close_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   /* trigger focus out callback on entries before closing tab */
   elm_object_focus_set(tabs.toolbar, true);
   tabs_current_tab_close();
}

static void
_shortcut_mode_normal_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_mode_set(tabs.current_workspace, MODE_NORMAL);
}

#if !HAVE_TIZEN
static void
_shortcut_mode_code_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_mode_set(tabs.current_workspace, MODE_CODE);
}
#endif

static void
_shortcut_mode_demo_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_mode_set(tabs.current_workspace, MODE_DEMO);
}

static void
_shortcut_zoom_in_cb(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   double factor;

   if (tabs.current_workspace)
     {
        factor = workspace_zoom_factor_get(tabs.current_workspace);
        workspace_zoom_factor_set(tabs.current_workspace, factor + 0.1);
     }
}

static void
_shortcut_zoom_out_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   double factor;

   if (tabs.current_workspace)
     {
        factor = workspace_zoom_factor_get(tabs.current_workspace);
        workspace_zoom_factor_set(tabs.current_workspace, factor - 0.1);
     }
}

static void
_shortcut_zoom_reset_cb(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_zoom_factor_set(tabs.current_workspace, 1.0);
}

static void
_shortcut_fill_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_container_fill(tabs.current_workspace);
}

static void
_shortcut_fit_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_container_fit(tabs.current_workspace);
}

static void
_shortcut_object_area_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Eina_Bool visible;

   if (tabs.current_workspace)
     {
        visible = workspace_object_area_visible_get(tabs.current_workspace);
        workspace_object_area_visible_set(tabs.current_workspace, !visible);
     }
}

static void
_shortcut_rulers_show_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Eina_Bool visible;

   if (tabs.current_workspace)
     {
        visible = workspace_rulers_visible_get(tabs.current_workspace);
        workspace_rulers_visible_set(tabs.current_workspace, !visible);
     }
}

static void
_shortcut_undo_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_history_undo(tabs.current_workspace);
}

static void
_shortcut_redo_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_history_redo(tabs.current_workspace);
}

static void
_history_update_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_history_update(tabs.current_workspace);
}

static void
_demo_property_update(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_demo_group_property_update(tabs.current_workspace);
}


Evas_Object *
tabs_add(void)
{
   assert (ap.win != NULL);

   tabs.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tabs.layout, "layout", "tabs", "default");
   BOX_ADD(tabs.layout, tabs.box, true, false);
   evas_object_size_hint_weight_set(tabs.box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_align_set(tabs.box, 0.0, 0.5);
   elm_layout_content_set(tabs.layout, "elm.swallow.toolbar", tabs.box);
   evas_object_show(tabs.box);

   tabs.toolbar = elm_toolbar_add(tabs.layout);
   elm_object_style_set(tabs.toolbar, "tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.toolbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar);
   evas_object_show(tabs.toolbar);

   tabs_home_tab_add(TAB_HOME_OPEN_PROJECT);

   evas_object_smart_callback_add(ap.win, SIGNAL_WORKSPACE_MODE_CHANGED, _mode_changed, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_SAVED, _on_save, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _on_project_changed, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _demo_swallow_set, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, _demo_text_set, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _demo_send_signal, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_PART_RENAMED, _part_renamed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_DATA_RENAMED, _group_data_renamed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_SAVED, _editor_saved, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_OPENED, _project_opened, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CLOSED, _project_closed, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, _property_attribute_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_STATE_SELECTED, _editor_part_state_selected_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ADDED, _editor_part_added_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_DELETED, _editor_part_deleted_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_ADDED, _editor_part_item_added_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_DELETED, _editor_part_item_deleted_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_RESTACKED, _editor_part_restacked_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_RESTACKED, _editor_part_item_restacked_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_ADDED, _editor_state_added_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_DELETED, _editor_state_deleted_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_ADDED, _editor_program_added_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_DELETED, _editor_program_deleted_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_GROUP_DATA_ADDED, _editor_group_data_added_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_GROUP_DATA_DELETED, _editor_group_data_deleted_cb, NULL);

   evas_object_smart_callback_add(ap.win, signals.shortcut.add.part, _shortcut_add_part_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.add.item, _shortcut_add_part_item_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.add.state, _shortcut_add_state_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.add.program, _shortcut_add_program_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.add.data_item, _shortcut_add_data_item_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.del, _shortcut_del_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.select.state_next, _shortcut_state_next_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.select.part_next, _shortcut_part_next_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.select.part_prev, _shortcut_part_prev_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.show_hide.part, _shortcut_part_showhide_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.popup.cancel, _shortcut_part_unselect_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.show_hide.all_parts, _shortcut_all_parts_showhide_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.tab.next, _shortcut_tab_next_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.tab.prev, _shortcut_tab_prev_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.tab.num, _shortcut_tab_num_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.tab.close, _shortcut_tab_close_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.mode.normal, _shortcut_mode_normal_cb, NULL);
#if !HAVE_TIZEN
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.mode.code, _shortcut_mode_code_cb, NULL);
#endif
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.mode.demo, _shortcut_mode_demo_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.zoom.in, _shortcut_zoom_in_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.zoom.out, _shortcut_zoom_out_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.zoom.reset, _shortcut_zoom_reset_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.zoom.fit, _shortcut_fit_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.fill, _shortcut_fill_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.show_hide.object_area, _shortcut_object_area_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.workspace.show_hide.rulers, _shortcut_rulers_show_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.history.undo, _shortcut_undo_cb, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.history.redo, _shortcut_redo_cb, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_HISTORY_CHANGE_ADDED, _history_update_cb, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_PROPERTY_MODE_DEMO, _demo_property_update, NULL);
   return tabs.layout;
}

void
tabs_menu_import_edj_data_set(const char *name, const char *path, const char *edj, const Eina_List *widgets)
{
   _tab_import_edj_data_set(name, path, edj, widgets);
}

void
tabs_menu_import_edc_data_set(const char *name, const char *path, const char *edc,
                              const Eina_List *img, const Eina_List *snd, const Eina_List *fnt, const Eina_List *dd)
{
   _tab_import_edc_data_set(name, path, edc, img, snd, fnt, dd);
}

void
tabs_menu_new_data_set(const char *name, const char *path, const Eina_List *widgets)
{
   _tab_new_data_set(name, path, widgets);
}

static void
_tab_close(void *data,
           Elm_Object_Item *it,
           const char *emission __UNUSED__,
           const char *source __UNUSED__)
{
   Tabs_Item *item = (Tabs_Item *)data;
   Evas_Object *content;

   if (!item) return;

   tabs.items = eina_list_remove(tabs.items, item);
   tabs.current_workspace = NULL;
   tabs.current_group = NULL;
   _del_tab(item);
   if (tabs.selected == it)
     {
        tabs.selected = NULL;
        content = elm_layout_content_unset(ap.panes.left_ver, "right");
        evas_object_hide(content);
        elm_layout_content_set(ap.panes.left_ver, "right", workspace_group_navigator_get(NULL));
        elm_object_part_content_unset(ap.panes.right, "right");
        evas_object_hide(ap.property.group);
        evas_object_hide(ap.property.demo);
     }
}

void
tabs_tab_add(Group2 *group)
{
   Tabs_Item *item;

   assert(group != NULL);

   /* check group, maybe this group already opend */
   item = _find_tab(group);
   if (item)
     {
        elm_toolbar_item_selected_set(item->toolbar_item, true);
        return;
     }
   /* check the group history if tabs haven't tab with given group history MUST
    * be NULL */
   assert(group->history == NULL);

   group->history = history_add(group);

   item = mem_calloc(1, sizeof(Tabs_Item));
   item->group = group;
   item->content = workspace_add(tabs.layout, group);

   item->toolbar_item = elm_toolbar_item_append(tabs.toolbar, NULL, group->common.name,
                                               _content_set, (void *)item);
   elm_toolbar_item_selected_set(item->toolbar_item, true);
   elm_object_item_signal_callback_add(item->toolbar_item, "tab,close", "eflete", _tab_close, (void *)item);
   tabs.items = eina_list_append(tabs.items, item);
}

static void
_tab_home_del(void *data __UNUSED__,
              Evas *e __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   evas_object_del(tabs.home.content_open_project);
   evas_object_del(tabs.home.content_new_project);
   evas_object_del(tabs.home.content_import_edj);
   evas_object_del(tabs.home.content_import_edc);
   evas_object_del(tabs.home.content_project_info);
}

void
tabs_home_tab_add(Tabs_Menu view)
{
   Tabs_Item *item;

   assert (NULL != tabs.toolbar);

   item = _find_tab(NULL);
   if (item)
     {
        goto subtab_select;
     }

   Evas_Object *scroller = elm_scroller_add(ap.win);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(scroller);

   tabs.home.content = elm_layout_add(ap.win);
   evas_object_event_callback_add(tabs.home.content, EVAS_CALLBACK_DEL, _tab_home_del, NULL);
   elm_layout_theme_set(tabs.home.content, "layout", "tab_home", "default");
   tabs.home.tabs = elm_toolbar_add(tabs.home.content);
   elm_layout_content_set(tabs.home.content, "elm.swallow.toolbar", tabs.home.tabs);
   elm_toolbar_horizontal_set(tabs.home.tabs, false);
   elm_object_style_set(tabs.home.tabs, "tabs_vertical");
   elm_toolbar_shrink_mode_set(tabs.home.tabs, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.home.tabs, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(tabs.home.tabs, 0.0);

   evas_object_size_hint_weight_set(tabs.home.content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.home.content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(scroller, tabs.home.content);

   tabs.home.content_open_project = _tab_open_project_add();
   tabs.home.content_new_project = _tab_new_project_add();
   tabs.home.content_import_edj = _tab_import_edj_add();
   tabs.home.content_import_edc = _tab_import_edc_add();
   tabs.home.content_project_info = _tab_project_info_add();

   tabs.home.tab_open_project =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Open project"), _home_tab_change, tabs.home.content_open_project);
   tabs.home.tab_new_project =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("New project"), _home_tab_change, tabs.home.content_new_project);
   tabs.home.tab_import_edj =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Import edj-file"), _home_tab_change, tabs.home.content_import_edj);
   tabs.home.tab_import_edc =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Import edc-file"), _home_tab_change, tabs.home.content_import_edc);
   tabs.home.tab_project_info =
      elm_toolbar_item_append(tabs.home.tabs, NULL, _("Project info"), _home_tab_change, tabs.home.content_project_info);

   item = mem_calloc(1, sizeof(Tabs_Item));
   item->group = NULL;
   item->content = scroller;
#ifndef HAVE_TIZEN
   item->toolbar_item = elm_toolbar_item_append(tabs.toolbar, "go-home", _("Home"),
                                                _content_set, (void *)item);
#else
   elm_object_part_content_set(ap.panes.left_ver, "right", workspace_group_navigator_get(NULL));
#endif /* HAVE_TIZEN */

   elm_object_item_signal_callback_add(item->toolbar_item, "tab,close", "eflete", _tab_close, (void *)item);
   tabs.items = eina_list_append(tabs.items, item);

subtab_select:
   switch(view)
     {
      case TAB_HOME_PROJECT_INFO:
         elm_toolbar_item_selected_set(tabs.home.tab_project_info, true);
         break;
      case TAB_HOME_OPEN_PROJECT:
         elm_toolbar_item_selected_set(tabs.home.tab_open_project, true);
         _tab_open_project_recents_update();
         break;
      case TAB_HOME_NEW_PROJECT:
         elm_toolbar_item_selected_set(tabs.home.tab_new_project, true);
         break;
      case TAB_HOME_IMPORT_EDJ:
         elm_toolbar_item_selected_set(tabs.home.tab_import_edj, true);
         break;
      case TAB_HOME_IMPORT_EDC:
         elm_toolbar_item_selected_set(tabs.home.tab_import_edc, true);
         break;
      default:
         break;
     }
#ifdef HAVE_TIZEN
   Evas_Object *content, *button;
   content = elm_layout_content_get(tabs.home.content, NULL);
   button = elm_object_part_content_get(content, "elm.swallow.btn_create");
   evas_object_smart_callback_call(button, signals.elm.button.clicked, NULL);
#endif /* HAVE_TIZEN */
   elm_toolbar_item_selected_set(item->toolbar_item, true);
}

void
tabs_current_tab_close(void)
{
   Tabs_Item *item;

   item = elm_object_item_data_get(tabs.selected);
   _tab_close(item, NULL, NULL, NULL);
}

void
tabs_clean(void)
{
   Eina_List *l, *l_n;
   Tabs_Item *item;

   /* delete all tab besides home tab */
   EINA_LIST_FOREACH_SAFE(tabs.items, l, l_n, item)
     {
        if (!item->group) continue;
        _tab_close(item, item->toolbar_item, NULL, NULL);
     }
   tabs.selected = NULL;
}

Evas_Object *
tabs_current_workspace_get(void)
{
   return tabs.current_workspace;
}

Group2 *
tabs_current_group_get(void)
{
   return tabs.current_group;
}
