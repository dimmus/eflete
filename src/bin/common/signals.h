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

typedef struct {
   const struct {
      const char *del;
      const char *help;
      const char *quit;
      const char *save;
      const struct {
         const char *data_item;
         const char *group;
         const char *item;
         const char *part;
         const char *program;
         const char *state;
      } add;
      const struct {
         const char *redo;
         const char *undo;
      } history;
      const struct {
         const char *color_class;
         const char *image;
         const char *sound;
         const char *style;
      } manager;
      const struct {
         const char *cancel;
         const char *done;
      } popup;
      const struct {
         const char *close;
         const char *next;
         const char *prev;
         const char *num; /* event_info - tab number */
      } tab;
      const struct {
         const char *fill;
         const struct {
            const char *code;
            const char *demo;
            const char *normal;
         } mode;
         const struct {
            const char *object_area;
            const char *part;
            const char *all_parts;
            const char *rulers;
         } show_hide;
         const struct {
            const char *part_next;
            const char *part_prev;
            const char *state_next;
         } select;
         const struct {
            const char *in;
            const char *out;
            const char *reset;
            const char *fit;
         } zoom;
      } workspace;
   } shortcut;
} Signals;

extern const Signals signals;
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
 * emited when new group is added.
 * eventinfo - pointer to Group object
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_GROUP_ADDED "SIGNAL_EDITOR_GROUP_ADDED"

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
 * emited when group is deleted.
 * eventinfo - pointer to group name (stringshare)
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_GROUP_DELETED "SIGNAL_EDITOR_GROUP_DELETED"

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

/**
 * emited when any attribute of top level resource (image, colorclass etc) is changed in editor.
 * eventinfo - Pointer to Attribute
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_RESOURCE_ATTRIBUTE_CHANGED "SIGNAL_EDITOR_RESOURCE_ATTRIBUTE_CHANGED"

typedef struct {
   const char *part_name;
   const char *state_name;
   double state_value;
   Change *change;
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
   const char *source;
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
typedef struct {
   const char *part_name;
   Change *change;
} Editor_Part;
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
 * emited when colorclass is added.
 * eventinfo - colorclass'es name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_COLORCLASS_ADDED "SIGNAL_EDITOR_COLORCLASS_ADDED"

/**
 * emited when colorclass is deleted.
 * eventinfo - colorclass'es name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_COLORCLASS_DELETED "SIGNAL_EDITOR_COLORCLASS_DELETED"

/**
 * emited when sound is added.
 * eventinfo - sound's name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_SOUND_ADDED "SIGNAL_EDITOR_SOUND_ADDED"

/**
 * emited when sound is deleted.
 * eventinfo - sound's name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_SOUND_DELETED "SIGNAL_EDITOR_SOUND_DELETED"

/**
 * emited when tone is added.
 * eventinfo - tone's name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_TONE_ADDED "SIGNAL_EDITOR_TONE_ADDED"

/**
 * emited when sound is deleted.
 * eventinfo - tone's name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_TONE_DELETED "SIGNAL_EDITOR_TONE_DELETED"

/**
 * emited when tone is added.
 * eventinfo - image's name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_IMAGE_ADDED "SIGNAL_EDITOR_IMAGE_ADDED"

/**
 * emited when sound is deleted.
 * eventinfo - image's name
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_IMAGE_DELETED "SIGNAL_EDITOR_IMAGE_DELETED"

/**
 * emited when tone is added.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_STYLE_ADDED "SIGNAL_EDITOR_STYLE_ADDED"

/**
 * emited when sound is deleted.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_EDITOR_STYLE_DELETED "SIGNAL_EDITOR_STYLE_DELETED"

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
 * emited when workspace is changed.
 * event_info - pointer to mode (Workspace_Mode *)
 *
 * @ingroup Window
 */
#define SIGNAL_WORKSPACE_MODE_CHANGED "SIGNAL_WORKSPACE_MODE_CHANGED"
#endif /* SIGNALS_H*/
