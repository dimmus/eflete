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
   Group *group;
   Elm_Object_Item *toolbar_item;
   Evas_Object *content;
   Eina_Bool need_recalc : 1;
};

typedef struct _Tabs_Item Tabs_Item;

struct _Tabs {
   Evas_Object *layout;
   Evas_Object *toolbar;
   Evas_Object *toolbar_editors;
   Evas_Object *box;
   Elm_Object_Item *selected;
   Eina_List *items;
   Evas_Object *current_workspace;
   Group *current_group;
   struct {
      Elm_Object_Item *item_home;
      Elm_Object_Item *item_sound;
      Elm_Object_Item *item_text;
      Elm_Object_Item *item_image;
      Elm_Object_Item *item_colorclass;
      /* those are used for showing home params */
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
      /* editors */
      Evas_Object *content_image_editor;
      Evas_Object *content_sound_editor;
      Evas_Object *content_text_editor;
      Evas_Object *content_colorclass_editor;
   } menu;
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

   if (item)
     {
        _content_unset();
        elm_layout_content_set(tabs.layout, NULL, item->content);
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

        if (workspace_active_demo_mode_get(tabs.current_workspace))
          evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_MODE_DEMO, NULL);
        else
          evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_MODE_GROUP, NULL);
     }
   else
     {
        tabs.current_workspace = NULL;
        tabs.current_group = NULL;
        if (ap.project)
          ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

        if (toolbar_item == tabs.menu.item_sound)
          {
             tabs_menu_tab_open(TAB_SOUND_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_SOUND_EDITOR_TAB_CLICKED, NULL);
          }
        else if (toolbar_item == tabs.menu.item_text)
          {
             tabs_menu_tab_open(TAB_STYLE_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_STYLE_EDITOR_TAB_CLICKED, NULL);
          }
        else if (toolbar_item == tabs.menu.item_image)
          {
             tabs_menu_tab_open(TAB_IMAGE_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_EDITOR_TAB_CLICKED, NULL);
          }
        else if (toolbar_item == tabs.menu.item_colorclass)
          {
             tabs_menu_tab_open(TAB_COLORCLASS_EDITOR);
             evas_object_smart_callback_call(ap.win, SIGNAL_COLOR_EDITOR_TAB_CLICKED, NULL);
          }
        else
          {
             tabs_menu_tab_open(TAB_LAST);
             evas_object_smart_callback_call(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, NULL);
          }
     }
}

static Tabs_Item *
_find_tab(Group *group)
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
   history_del(item->group->history);
   item->group->history = NULL;
   if (item->group->current_part)
     item->group->current_part->current_item_name = NULL;
   item->group->current_part = NULL;
   if (item->group->current_program)
     {
        eina_stringshare_del(item->group->current_program);
        item->group->current_program = NULL;
     }
   evas_object_smart_callback_call(ap.win, SIGNAL_TAB_CLOSE, item->group);
   free(item);
}

static void
_home_tab_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   evas_object_hide(elm_layout_content_unset(tabs.menu.content, NULL));
   elm_layout_content_set(tabs.menu.content, NULL, data);
   _tab_project_update();
}

static void
_property_attribute_changed(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Attribute *attr = event_info;

   assert(tabs.current_workspace != NULL);

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
      case ATTRIBUTE_PART_ITEM_PADDING:
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
         workspace_groupview_hard_update(tabs.current_workspace);
         break;
      case ATTRIBUTE_STATE_FILL_SMOOTH:
      case ATTRIBUTE_STATE_VISIBLE:
      case ATTRIBUTE_STATE_COLOR_CLASS:
      case ATTRIBUTE_STATE_COLOR:
      case ATTRIBUTE_STATE_OUTLINE_COLOR:
      case ATTRIBUTE_STATE_SHADOW_COLOR:
      case ATTRIBUTE_STATE_IMAGE_BORDER:
      case ATTRIBUTE_STATE_IMAGE_BORDER_FILL:
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
   Part *part;

   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);
   assert(ren != NULL);

   part = pm_resource_unsorted_get(tabs.current_group->parts, ren->old_name);
   gm_part_rename(part, ren->new_name);
   workspace_group_navigator_update_part(tabs.current_workspace, part);
}

static void
_project_changed(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   /* project may be changed in editors for example */
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
        gm_group_edit_object_reload(ap.project, item->group);
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
   tabs.menu.content_image_editor = image_manager_add();
   tabs.menu.content_sound_editor = sound_manager_add();
   tabs.menu.content_text_editor = style_manager_add();
   tabs.menu.content_colorclass_editor = colorclass_manager_add();

   elm_object_item_disabled_set(tabs.menu.item_image, false);
   elm_object_item_disabled_set(tabs.menu.item_sound, false);
   elm_object_item_disabled_set(tabs.menu.item_text, false);
   elm_object_item_disabled_set(tabs.menu.item_colorclass, false);
}

static void
_project_closed(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{
   evas_object_del(tabs.menu.content_image_editor);
   evas_object_del(tabs.menu.content_sound_editor);
   evas_object_del(tabs.menu.content_text_editor);
   evas_object_del(tabs.menu.content_colorclass_editor);

   tabs.menu.content_image_editor = NULL;
   tabs.menu.content_sound_editor = NULL;
   tabs.menu.content_text_editor = NULL;
   tabs.menu.content_colorclass_editor = NULL;

   elm_object_item_disabled_set(tabs.menu.item_image, true);
   elm_object_item_disabled_set(tabs.menu.item_sound, true);
   elm_object_item_disabled_set(tabs.menu.item_text, true);
   elm_object_item_disabled_set(tabs.menu.item_colorclass, true);

   tabs_menu_tab_open(TAB_LAST);
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

   workspace_part_state_select(tabs.current_workspace, editor_state->part_name, editor_state->state_name);
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
   Eina_Stringshare *part_name = event_info;

   assert(part_name != NULL);
   assert(tabs.current_group != NULL);
   assert(tabs.current_workspace != NULL);

   workspace_part_del(tabs.current_workspace, part_name);
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

   workspace_part_state_add(tabs.current_workspace, editor_state->part_name, editor_state->state_name);
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

   workspace_part_state_del(tabs.current_workspace, editor_state->part_name, editor_state->state_name);
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
_shortcut_tab_image_manager_cb(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   if (!elm_object_item_disabled_get(tabs.menu.item_image))
     elm_toolbar_item_selected_set(tabs.menu.item_image, true);
}

static void
_shortcut_tab_sound_manager_cb(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   if (!elm_object_item_disabled_get(tabs.menu.item_sound))
     elm_toolbar_item_selected_set(tabs.menu.item_sound, true);
}

static void
_shortcut_tab_color_class_manager_cb(void *data __UNUSED__,
                                     Evas_Object *obj __UNUSED__,
                                     void *event_info __UNUSED__)
{
   if (!elm_object_item_disabled_get(tabs.menu.item_colorclass))
     elm_toolbar_item_selected_set(tabs.menu.item_colorclass, true);
}

static void
_shortcut_tab_style_manager_cb(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   if (!elm_object_item_disabled_get(tabs.menu.item_text))
     elm_toolbar_item_selected_set(tabs.menu.item_text, true);
}

static void
_shortcut_mode_normal_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_mode_set(tabs.current_workspace, MODE_NORMAL);
}

static void
_shortcut_mode_code_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if (tabs.current_workspace)
     workspace_mode_set(tabs.current_workspace, MODE_CODE);
}

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
_shortcut_rulers_visible_cb(void *data __UNUSED__,
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

   /* adding toolbar for editors */
   tabs.toolbar_editors = elm_toolbar_add(tabs.layout);
   elm_object_style_set(tabs.toolbar_editors, "editor_tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar_editors, ELM_TOOLBAR_SHRINK_NONE);
   elm_toolbar_select_mode_set(tabs.toolbar_editors, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar_editors, 0.0, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar_editors, 0.0, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar_editors, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar_editors);
   evas_object_show(tabs.toolbar_editors);
   /* addind two different toolbars */
   tabs.toolbar = elm_toolbar_add(tabs.layout);
   elm_object_style_set(tabs.toolbar, "tabs_horizontal");
   elm_toolbar_shrink_mode_set(tabs.toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.toolbar, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(tabs.toolbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(tabs.toolbar, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_toolbar_align_set(tabs.toolbar, 0.0);
   elm_box_pack_end(tabs.box, tabs.toolbar);
   evas_object_show(tabs.toolbar);

   tabs.menu.content = elm_layout_add(ap.win);
   elm_layout_theme_set(tabs.menu.content, "layout", "tab_home", "default");
   tabs.menu.tabs = elm_toolbar_add(tabs.menu.content);
   elm_layout_content_set(tabs.menu.content, "elm.swallow.toolbar", tabs.menu.tabs);
   elm_toolbar_horizontal_set(tabs.menu.tabs, false);
   elm_object_style_set(tabs.menu.tabs, "tabs_vertical");
   elm_toolbar_shrink_mode_set(tabs.menu.tabs, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(tabs.menu.tabs, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_toolbar_align_set(tabs.menu.tabs, 0.0);

   tabs.menu.content_open_project = _tab_open_project_add();
   tabs.menu.content_new_project = _tab_new_project_add();
   tabs.menu.content_import_edj = _tab_import_edj_add();
   tabs.menu.content_import_edc = _tab_import_edc_add();
   tabs.menu.content_project_info = _tab_project_info_add();

   tabs.menu.tab_open_project =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Open project"), _home_tab_change, tabs.menu.content_open_project);
   tabs.menu.tab_new_project =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("New project"), _home_tab_change, tabs.menu.content_new_project);
   tabs.menu.tab_import_edj =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Import edj-file"), _home_tab_change, tabs.menu.content_import_edj);
   tabs.menu.tab_import_edc =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Import edc-file"), _home_tab_change, tabs.menu.content_import_edc);
   tabs.menu.tab_project_info =
      elm_toolbar_item_append(tabs.menu.tabs, NULL, _("Project info"), _home_tab_change, tabs.menu.content_project_info);

   tabs.menu.item_home = elm_toolbar_item_append(tabs.toolbar_editors, "go-home", NULL,
                                                 _content_set, NULL);
   tabs.menu.item_image = elm_toolbar_item_append(tabs.toolbar_editors, "image2", NULL,
                                                  _content_set, NULL);
   tabs.menu.item_sound = elm_toolbar_item_append(tabs.toolbar_editors, "sound2", NULL,
                                                  _content_set, NULL);
   tabs.menu.item_text = elm_toolbar_item_append(tabs.toolbar_editors, "text2", NULL,
                                                 _content_set, NULL);
   tabs.menu.item_colorclass = elm_toolbar_item_append(tabs.toolbar_editors, "color", NULL,
                                                       _content_set, NULL);

   elm_toolbar_item_selected_set(tabs.menu.item_home, true);
   elm_layout_content_set(tabs.layout, NULL, tabs.menu.content);

   elm_object_item_disabled_set(tabs.menu.item_image, true);
   elm_object_item_disabled_set(tabs.menu.item_sound, true);
   elm_object_item_disabled_set(tabs.menu.item_text, true);
   elm_object_item_disabled_set(tabs.menu.item_colorclass, true);

   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_SAVED, _on_save, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _on_project_changed, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _demo_swallow_set, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, _demo_text_set, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _demo_send_signal, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_PART_RENAMED, _part_renamed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _project_changed, NULL);
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

   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ADD_PART, _shortcut_add_part_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ADD_ITEM, _shortcut_add_part_item_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ADD_STATE, _shortcut_add_state_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ADD_PROGRAM, _shortcut_add_program_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_DEL, _shortcut_del_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_STATE_NEXT, _shortcut_state_next_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_PART_NEXT, _shortcut_part_next_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_PART_PREV, _shortcut_part_prev_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_PART_SHOWHIDE, _shortcut_part_showhide_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_PART_UNSELECT, _shortcut_part_unselect_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ALL_PARTS_SHOWHIDE, _shortcut_all_parts_showhide_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_NEXT, _shortcut_tab_next_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_PREV, _shortcut_tab_prev_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_NUM, _shortcut_tab_num_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_CLOSE, _shortcut_tab_close_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_IMAGE_MANAGER, _shortcut_tab_image_manager_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_SOUND_MANAGER, _shortcut_tab_sound_manager_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_COLOR_CLASS_MANAGER, _shortcut_tab_color_class_manager_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_TAB_STYLE_MANAGER, _shortcut_tab_style_manager_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_MODE_NORMAL, _shortcut_mode_normal_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_MODE_CODE, _shortcut_mode_code_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_MODE_DEMO, _shortcut_mode_demo_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ZOOM_IN, _shortcut_zoom_in_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ZOOM_OUT, _shortcut_zoom_out_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ZOOM_RESET, _shortcut_zoom_reset_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_FIT, _shortcut_fit_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_FILL, _shortcut_fill_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_OBJECT_AREA, _shortcut_object_area_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_RULERS_VISIBLED, _shortcut_rulers_visible_cb, NULL);
   return tabs.layout;
}

void
tabs_menu_tab_open(Tabs_Menu view)
{
   assert(tabs.layout != NULL);

   _content_unset();


#define _TAB_HOME_CASE(HOME_TAB) \
   elm_toolbar_item_selected_set(tabs.menu.HOME_TAB, true); \
   elm_toolbar_item_selected_set(tabs.menu.item_home, true); \
   elm_layout_content_set(tabs.layout, NULL, tabs.menu.content);
   switch(view)
     {
        /* home case */
      case TAB_HOME_PROJECT_INFO:
         _TAB_HOME_CASE(tab_project_info);
         break;
      case TAB_HOME_OPEN_PROJECT:
         _TAB_HOME_CASE(tab_open_project);
         _tab_open_project_recents_update();
         break;
      case TAB_HOME_NEW_PROJECT:
         _TAB_HOME_CASE(tab_new_project);
         break;
      case TAB_HOME_IMPORT_EDJ:
         _TAB_HOME_CASE(tab_import_edj);
         break;
      case TAB_HOME_IMPORT_EDC:
         _TAB_HOME_CASE(tab_import_edc);
         break;

      /* editor cases */
      case TAB_IMAGE_EDITOR:
         if (ap.project)
           elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_image_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_image, true);
         break;
      case TAB_SOUND_EDITOR:
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_sound_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_sound, true);
         break;
      case TAB_COLORCLASS_EDITOR:
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_colorclass_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_colorclass, true);
         break;
      case TAB_STYLE_EDITOR:
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content_text_editor);
         elm_toolbar_item_selected_set(tabs.menu.item_text, true);
         break;
      case TAB_LAST:
         elm_toolbar_item_selected_set(tabs.menu.item_home, true);
         elm_layout_content_set(tabs.layout, NULL, tabs.menu.content);
         _tab_project_update();
      default:
         break;
     }
#undef _TAB_HOME_CASE
}

void
tabs_menu_import_edj_data_set(const char *name, const char *path, const char *edj)
{
   _tab_import_edj_data_set(name, path, edj);
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
           Elm_Object_Item *it __UNUSED__,
           const char *emission __UNUSED__,
           const char *source __UNUSED__)
{
   Tabs_Item *item = (Tabs_Item *)data;
   tabs.items = eina_list_remove(tabs.items, item);
   _del_tab(item);
   if (!tabs.items)
     {
        tabs.selected = NULL;
        tabs_menu_tab_open(TAB_HOME_PROJECT_INFO);
     }
}
void
tabs_tab_add(Group *group)
{
   Tabs_Item *item;
   Eina_Stringshare *msg;
   Change *change;

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

   item->toolbar_item = elm_toolbar_item_append(tabs.toolbar, NULL, group->name,
                                               _content_set, (void *)item);
   elm_toolbar_item_selected_set(item->toolbar_item, true);
   elm_object_item_signal_callback_add(item->toolbar_item, "tab,close", "eflete", _tab_close, (void *)item);
   tabs.items = eina_list_append(tabs.items, item);

   msg = eina_stringshare_printf(_("open group \"%s\""), group->name);
   change = change_add(msg);
   history_change_add(group->history, change);
   eina_stringshare_del(msg);
}

void
tabs_current_tab_close(void)
{
   Tabs_Item *item;

   item = elm_object_item_data_get(tabs.selected);
   if (!item) return;
   tabs.items = eina_list_remove(tabs.items, item);
   _del_tab(item);
   if (!tabs.items) tabs_menu_tab_open(TAB_HOME_PROJECT_INFO);
}

void
tabs_clean(void)
{
   Tabs_Item *item;

   EINA_LIST_FREE(tabs.items, item)
     {
        _del_tab(item);
     }
   tabs.selected = NULL;
   tabs.items = NULL;
}

Evas_Object *
tabs_current_workspace_get(void)
{
   return tabs.current_workspace;
}

Group *
tabs_current_group_get(void)
{
   return tabs.current_group;
}
