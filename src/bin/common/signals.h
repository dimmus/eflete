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

#ifndef SIGNALS_H
#define SIGNALS_H

/**
 * emited when active group changed.
 * eventinfo - pointer to Group structure.
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_CHANGED "SIGNAL_GROUP_CHANGED"

/**
 * emited when property mode should be changed to group.
 *
 * @ingroup Window
 */
#define SIGNAL_PROPERTY_MODE_GROUP "SIGNAL_PROPERTY_MODE_GROUP"

/**
 * emited when property mode should be changed to demo.
 *
 * @ingroup Window
 */
#define SIGNAL_PROPERTY_MODE_DEMO "SIGNAL_PROPERTY_MODE_DEMO"

/**
 * emited while user trying to close tab.
 * eventinfo - pointer to given Group object.
 *
 * @ingroup Window
 */
#define SIGNAL_TAB_CLOSE "SIGNAL_TAB_CLOSE"

/**
 * emited when new group is added.
 * eventinfo - pointer to Group object
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_ADDED "SIGNAL_GROUP_ADDED"

/**
 * emited when group is opened.
 * eventinfo - pointer to Group object
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_OPEN "SIGNAL_GROUP_OPEN"

/**
 * emited when group is deleted.
 * eventinfo - pointer to group name (stringshare)
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_DELETED "SIGNAL_GROUP_DELETED"

/**
 * emited when part is selected.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_SELECTED "SIGNAL_PART_SELECTED"

/**
 * emited when program is selected.
 * eventinfo - pointer to Resource structure
 *
 * @ingroup Window
 */
#define SIGNAL_PROGRAM_SELECTED "SIGNAL_PROGRAM_SELECTED"

/**
 * emited when program is unselected.
 * eventinfo - pointer to Resource structure
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_NAVIGATOR_UNSELECTED "SIGNAL_GROUP_NAVIGATOR_UNSELECTED"

typedef struct {
   const char *old_name;
   const char *new_name;
} Rename;
/**
 * emited when part is renamed.
 * eventinfo - pointer to Rename structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_RENAMED "SIGNAL_PART_RENAMED"

/**
 * emited when group_data is renamed.
 * eventinfo - pointer to Rename structure
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_DATA_RENAMED "SIGNAL_GROUP_DATA_RENAMED"

/**
 * emited when program is renamed.
 * eventinfo - pointer to Rename structure
 *
 * @ingroup Window
 */
#define SIGNAL_PROGRAM_RENAMED "SIGNAL_PROGRAM_RENAMED"

/**
 * emited when new state is added.
 * eventinfo - pointer to State structure
 *
 * @ingroup Window
 */
#define SIGNAL_STATE_ADDED "SIGNAL_STATE_ADDED"

/**
 * emited when part state changed.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_STATE_SELECTED "SIGNAL_PART_STATE_SELECTED"

/**
 * emited when part item is selected.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_ITEM_SELECTED "SIGNAL_PART_ITEM_SELECTED"

/**
 * emited when part item is unselected.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_ITEM_UNSELECTED "SIGNAL_PART_ITEM_UNSELECTED"

/**
 * emited when any attribute is changed in properties.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_PROPERTY_ATTRIBUTE_CHANGED "SIGNAL_PROPERTY_ATTRIBUTE_CHANGED"

/**
 * emited when any attribute is changed in editor.
 * eventinfo - Pointer to Attribute
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_ATTRIBUTE_CHANGED "SIGNAL_EDITOR_ATTRIBUTE_CHANGED"

typedef struct {
   const char *part_name;
   const char *state_name;
} Editor_State;
/**
 * emited when state is added in editor.
 * eventinfo - Editor_State struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_STATE_ADDED "SIGNAL_EDITOR_STATE_ADDED"

/**
 * emited *before* state will be deleted in editor.
 * eventinfo - Editor_State struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_STATE_DELETED "SIGNAL_EDITOR_STATE_DELETED"

typedef struct {
   const char *part_name;
   const char *item_name;
} Editor_Item;
/**
 * emited when item is added in editor.
 * eventinfo - Editor_Item struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_ITEM_ADDED "SIGNAL_EDITOR_PART_ITEM_ADDED"

/**
 * emited *before* item will be deleted in editor.
 * eventinfo - Editor_Item struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_ITEM_DELETED "SIGNAL_EDITOR_PART_ITEM_DELETED"

/**
 * emited when part is added in editor.
 * eventinfo - stringshare with parts name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_ADDED "SIGNAL_EDITOR_PART_ADDED"

/**
 * emited *before* part will be deleted in editor.
 * eventinfo - stringshare with parts name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_DELETED "SIGNAL_EDITOR_PART_DELETED"

/**
 * emited when program is added in editor.
 * eventinfo - stringshare with programs name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PROGRAM_ADDED "SIGNAL_EDITOR_PROGRAM_ADDED"

/**
 * emited when program is being changed from property.
 * eventinfo - stringshare with programs name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PROGRAM_UPDATE "SIGNAL_EDITOR_PROGRAM_UPDATE"

/**
 * emited *before* program will be deleted in editor.
 * eventinfo - stringshare with programs name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PROGRAM_DELETED "SIGNAL_EDITOR_PROGRAM_DELETED"

/**
 * emited when group_data is added in editor.
 * eventinfo - stringshare with group_datas name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_GROUP_DATA_ADDED "SIGNAL_EDITOR_GROUP_DATA_ADDED"

/**
 * emited *before* group_data will be deleted in editor.
 * eventinfo - stringshare with group_datas name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_GROUP_DATA_DELETED "SIGNAL_EDITOR_GROUP_DATA_DELETED"

typedef struct {
   const char *part_name;
   const char *relative_part_name;
} Editor_Part_Restack;
/**
 * emited when is restacked.
 * eventinfo - Editor_Part_Restack struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_RESTACKED "SIGNAL_EDITOR_PART_RESTACKED"

typedef struct {
   const char *part_name;
   const char *part_item;
   const char *relative_part_item;
} Editor_Part_Item_Restack;
/**
 * emited when part item is restacked.
 * eventinfo - Editor_Part_Item_Restack struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_ITEM_RESTACKED "SIGNAL_EDITOR_PART_ITEM_RESTACKED"

/**
 * emited when part state is selected.
 * eventinfo - EEditor_State struct
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_PART_STATE_SELECTED "SIGNAL_EDITOR_PART_STATE_SELECTED"


/**
 * emited when project is changed in any way (through attribute, .
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_PROJECT_CHANGED "SIGNAL_PROJECT_CHANGED"

/**
 * emited when change is added to history.
 * eventinfo - Pointer to added Change
 *
 * @ingroup Window
 */
#define SIGNAL_HISTORY_CHANGE_ADDED "SIGNAL_HISTORY_CHANGE_ADDED"

/**
 * emited when editor_save or editor_save_all are called.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_SAVED "SIGNAL_EDITOR_SAVED"

/**
 * emited when project is opened.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_PROJECT_OPENED "SIGNAL_PROJECT_OPENED"

/**
 * emited when project is closed.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_PROJECT_CLOSED "SIGNAL_PROJECT_CLOSED"

/**
 * emited when NOT editor tab and NOT group tab is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_DIFFERENT_TAB_CLICKED "SIGNAL_DIFFERENT_TAB_CLICKED"

/**
 * emited when image editor tab is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_IMAGE_EDITOR_TAB_CLICKED "SIGNAL_IMAGE_EDITOR_TAB_CLICKED"

/**
 * emited when image/images in image editor is/are selected.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_IMAGE_SELECTED "SIGNAL_IMAGE_SELECTED"

  /**
 * emited when sound editor tab is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_SOUND_EDITOR_TAB_CLICKED "SIGNAL_SOUND_EDITOR_TAB_CLICKED"

/**
 * emited when sound/sounds in sound editor is/are added/selected.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_SOUND_SELECT "SIGNAL_SOUND_SELECT"

/**
 * emited when sound in sound editor ise unselected.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_SOUND_UNSELECTED "SIGNAL_SOUND_UNSELECTED"
 /**
 * emited when sound/sounds in sound editor is/are deleted.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_SOUND_DEL "SIGNAL_SOUND_DEL"

 /**
 * emited when style editor tab is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_STYLE_EDITOR_TAB_CLICKED "SIGNAL_STYLE_EDITOR_TAB_CLICKED"

 /**
 * emited when colorclass editor tab is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_COLOR_EDITOR_TAB_CLICKED "SIGNAL_COLOR_EDITOR_TAB_CLICKED"

/**
 * emited when text part in demo is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_DEMO_TEXT_PART_CLICKED "SIGNAL_DEMO_TEXT_PART_CLICKED"

/**
 * emited when swallow part in demo is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_DEMO_SWALLOW_PART_CLICKED "SIGNAL_DEMO_SWALLOW_PART_CLICKED"

/**
 * emited when program in demo is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_DEMO_PROGRAM_PART_CLICKED "SIGNAL_DEMO_PROGRAM_PART_CLICKED"

/**
 * emited when style/styles in style editor is/are selected.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_STYLE_SELECTED "SIGNAL_STYLE_SELECTED"

/**
 * emited when colorstyle/colorstyles in colorclass editor is/are selected.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_COLOR_SELECTED "SIGNAL_COLOR_SELECTED"

/**
 * emited when part text is set with data in demo.
 * eventinfo - part which contain important information like text, name, content.
 *
 * @ingroup Window
 */
#define SIGNAL_DEMO_TEXT_SET "SIGNAL_DEMO_TEXT_SET"

/**
 * emited when part swallow is set with data in demo.
 * eventinfo - part which contain important information like text, name, content.
 *
 * @ingroup Window
 */
#define SIGNAL_DEMO_SWALLOW_SET "SIGNAL_DEMO_SWALLOW_SET"

/**
 * emited when program's signal is sent into demo.
 * eventinfo - NULL.
 *
 * @ingroup Window
 */
#define SIGNAL_DEMO_SIGNAL_SEND "SIGNAL_DEMO_SIGNAL_SEND"

/**
 * emited when shortcut is pressed.
 * eventinfo - NULL.
 *
 * @ingroup Window
 */
#define SIGNAL_SHORTCUT_QUIT "SIGNAL_SHORTCUT_QUIT"
#define SIGNAL_SHORTCUT_UNDO "SIGNAL_SHORTCUT_UNDO"
#define SIGNAL_SHORTCUT_REDO "SIGNAL_SHORTCUT_REDO"
#define SIGNAL_SHORTCUT_SAVE "SIGNAL_SHORTCUT_SAVE"
#define SIGNAL_SHORTCUT_ADD_GROUP "SIGNAL_SHORTCUT_ADD_GROUP"
#define SIGNAL_SHORTCUT_ADD_PART "SIGNAL_SHORTCUT_ADD_PART"
#define SIGNAL_SHORTCUT_ADD_STATE "SIGNAL_SHORTCUT_ADD_STATE"
#define SIGNAL_SHORTCUT_ADD_ITEM "SIGNAL_SHORTCUT_ADD_ITEM"
#define SIGNAL_SHORTCUT_ADD_PROGRAM "SIGNAL_SHORTCUT_ADD_PROGRAM"
#define SIGNAL_SHORTCUT_DEL "SIGNAL_SHORTCUT_DEL"
#define SIGNAL_SHORTCUT_TAB_NEXT "SIGNAL_SHORTCUT_TAB_NEXT"
#define SIGNAL_SHORTCUT_TAB_PREV "SIGNAL_SHORTCUT_TAB_PREV"
#define SIGNAL_SHORTCUT_TAB_CLOSE "SIGNAL_SHORTCUT_TAB_CLOSE"
#define SIGNAL_SHORTCUT_TAB_IMAGE_MANAGER "SIGNAL_SHORTCUT_TAB_IMAGE_MANAGER"
#define SIGNAL_SHORTCUT_TAB_SOUND_MANAGER "SIGNAL_SHORTCUT_TAB_SOUND_MANAGER"
#define SIGNAL_SHORTCUT_TAB_COLOR_CLASS_MANAGER "SIGNAL_SHORTCUT_TAB_COLOR_CLASS_MANAGER"
#define SIGNAL_SHORTCUT_TAB_STYLE_MANAGER "SIGNAL_SHORTCUT_TAB_STYLE_MANAGER"
#define SIGNAL_SHORTCUT_MODE_NORMAL "SIGNAL_SHORTCUT_MODE_NORMAL"
#define SIGNAL_SHORTCUT_MODE_CODE "SIGNAL_SHORTCUT_MODE_CODE"
#define SIGNAL_SHORTCUT_MODE_DEMO "SIGNAL_SHORTCUT_MODE_DEMO"
#define SIGNAL_SHORTCUT_STATE_NEXT "SIGNAL_SHORTCUT_STATE_NEXT"
#define SIGNAL_SHORTCUT_PART_NEXT "SIGNAL_SHORTCUT_PART_NEXT"
#define SIGNAL_SHORTCUT_PART_PREV "SIGNAL_SHORTCUT_PART_PREV"
#define SIGNAL_SHORTCUT_PART_SHOWHIDE "SIGNAL_SHORTCUT_PART_SHOWHIDE"
#define SIGNAL_SHORTCUT_ALL_PARTS_SHOWHIDE "SIGNAL_SHORTCUT_ALL_PARTS_SHOWHIDE"
#define SIGNAL_SHORTCUT_PART_UNSELECT "SIGNAL_SHORTCUT_PART_UNSELECT"
#define SIGNAL_SHORTCUT_ZOOM_IN "SIGNAL_SHORTCUT_ZOOM_IN"
#define SIGNAL_SHORTCUT_ZOOM_OUT "SIGNAL_SHORTCUT_ZOOM_OUT"
#define SIGNAL_SHORTCUT_ZOOM_RESET "SIGNAL_SHORTCUT_ZOOM_RESET"
#define SIGNAL_SHORTCUT_FILL "SIGNAL_SHORTCUT_FILL"
#define SIGNAL_SHORTCUT_FIT "SIGNAL_SHORTCUT_FIT"
#define SIGNAL_SHORTCUT_OBJECT_AREA "SIGNAL_SHORTCUT_OBJECT_AREA"
#define SIGNAL_SHORTCUT_RULERS_VISIBLED "SIGNAL_SHORTCUT_RULERS_VISIBLED"
#define SIGNAL_SHORTCUT_POPUP_CANCEL "SIGNAL_SHORTCUT_POPUP_CANCEL"
#define SIGNAL_SHORTCUT_POPUP_DONE "SIGNAL_SHORTCUT_POPUP_DONE"

/**
 * emited when shortcut is pressed.
 * eventinfo - tab number.
 *
 * @ingroup Window
 */
#define SIGNAL_SHORTCUT_TAB_NUM "SIGNAL_SHORTCUT_TAB_NUM"

/**
 * emited when workspace is changed.
 * event_info - pointer to mode (Workspace_Mode *)
 *
 * @ingroup Window
 */
#define SIGNAL_WORKSPACE_MODE_CHANGED "SIGNAL_WORKSPACE_MODE_CHANGED"
#endif /* SIGNALS_H*/
