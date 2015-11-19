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
 * emited when tab changed.
 * eventinfo - pointer to Group structure. Can be NULL
 *
 * @ingroup Window
 */
#define SIGNAL_TAB_CHANGED "SIGNAL_TAB_CHANGED"

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
 * emited when group is deleted.
 * eventinfo - pointer to group name (stringshare)
 *
 * @ingroup Window
 */
#define SIGNAL_GROUP_DELETED "SIGNAL_GROUP_DELETED"

/**
 * emited when part is selected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_SELECTED "SIGNAL_PART_SELECTED"

/**
 * emited when part is unselected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_UNSELECTED "SIGNAL_PART_UNSELECTED"

/**
 * emited when new part is added.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_ADDED "SIGNAL_PART_ADDED"

/**
 * emited when part is deleted.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_DELETED "SIGNAL_PART_DELETED"

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
 * emited when new state is added.
 * eventinfo - pointer to State structure
 *
 * @ingroup Window
 */
#define SIGNAL_STATE_ADDED "SIGNAL_STATE_ADDED"

/**
 * emited when part state changed.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_STATE_SELECTED "SIGNAL_PART_STATE_SELECTED"

/**
 * emited when part item is selected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_ITEM_SELECTED "SIGNAL_PART_ITEM_SELECTED"

/**
 * emited when part item is unselected.
 * eventinfo - pointer to Part_ structure
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
 * emited when sound/sounds in sound editor is/are selected.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_SOUND_SELECTED "SIGNAL_SOUND_SELECTED"

 /**
 * emited when style editor tab is clicked.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_STYLE_EDITOR_TAB_CLICKED "SIGNAL_STYLE_EDITOR_TAB_CLICKED"


#endif /* SIGNALS_H*/
