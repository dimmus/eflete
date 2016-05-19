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

#ifndef PROPERTY_PRIVATE_H
#define PROPERTY_PRIVATE_H

#define PROPERTY_DATA "prop_data"
#define PROPERTY_DATA_GET(OBJ) \
   Property_Data *pd = evas_object_data_get(OBJ, PROPERTY_DATA); \
   assert(pd != NULL);

enum _Property_Control {
   PROPERTY_CONTROL_NONE,

   PROPERTY_CONTROL_CHECK,
   PROPERTY_CONTROL_BUTTON,
   PROPERTY_CONTROL_COMBOBOX,
   PROPERTY_CONTROL_SPINNER,
   PROPERTY_CONTROL_ENTRY,
   PROPERTY_CONTROL_COLOR,
   PROPERTY_CONTROL_COLORSEL,
   PROPERTY_CONTROL_LABEL,
   PROPERTY_CONTROL_IMAGE_PREVIEW,
   PROPERTY_CONTROL_IMAGE_NORMAL,
   PROPERTY_CONTROL_IMAGE_TWEEN,
   PROPERTY_CONTROL_SOUND_PLAYER,

   PROPERTY_CONTROL_LAST
};
typedef enum _Property_Control Property_Control;
typedef struct _Property_Attribute Property_Attribute;
typedef struct _Property_Action Property_Action;
typedef struct {
   Evas_Object *genlist;
   Property_Mode mode;
   Property_Action *started_action;
   Property_Attribute *started_pa;

   Elm_Genlist_Item_Class *itc_caption;
   Elm_Genlist_Item_Class *itc_1swallow;
   Elm_Genlist_Item_Class *itc_2swallow;

   /* Stores what item class to use with each pair of controls */
   Elm_Genlist_Item_Class *item_classes[PROPERTY_CONTROL_LAST][PROPERTY_CONTROL_LAST];
} Property_Data;

typedef void (* Property_Cb) (Property_Attribute *, Property_Action *);
typedef Eina_Bool (* Property_Attribute_Filter_Cb) (Property_Attribute *);
typedef void (* Property_Attribute_Del_Cb) (Property_Attribute *);
typedef Eina_List * (* Property_Expand_Cb) (Property_Attribute *);

/* filter needs to be set to any non-zero value to be applied. Not in all cases
   useful key can be given so we ignore its value and use 1 instead. */
#define GENLIST_FILTER_APPLY(GEN) elm_genlist_filter_set(GEN, (void *)1);
#pragma GCC poison elm_genlist_filter_set

typedef enum {
   PROPERTY_IMAGE_CONTROL_PREVIEW,
   PROPERTY_IMAGE_CONTROL_NAME,
   PROPERTY_IMAGE_CONTROL_LOCATION,
   PROPERTY_IMAGE_CONTROL_TYPE,
   PROPERTY_IMAGE_CONTROL_COMPRESSION,
   PROPERTY_IMAGE_CONTROL_QUALITY,
   PROPERTY_IMAGE_CONTROL_WIDTH,
   PROPERTY_IMAGE_CONTROL_HEIGHT,
   PROPERTY_IMAGE_CONTROL_LAST
} Attribute_Image;

struct _Property_Action {
   Property_Control control_type;
   Eina_Stringshare *name;
   Eina_Stringshare *units;
   union {
      Attribute attribute; /**< type for group properties */
      Attribute_Image attribute_image; /**< type for group properties */
   } type; /**< submodule-specific enums */
   Evas_Object *control; /**< pointer to widget */
   Property_Cb init_cb;
   Property_Cb start_cb;
   Property_Cb change_cb;
   Property_Cb stop_cb;
   Property_Cb update_cb; /**< called when item is realized and control's values should be updated */
};

/* Enum of property_group items. Can't use Attribute here because items can have
   0-2 attributes associated with them */
typedef enum {
   PROPERTY_GROUP_ITEM_GROUP_TITLE,
   PROPERTY_GROUP_ITEM_GROUP_NAME,
   PROPERTY_GROUP_ITEM_GROUP_MIN,
   PROPERTY_GROUP_ITEM_GROUP_MAX,

   PROPERTY_GROUP_ITEM_PART_TITLE,
   PROPERTY_GROUP_ITEM_PART_NAME,
   PROPERTY_GROUP_ITEM_PART_TYPE,
   PROPERTY_GROUP_ITEM_PART_SCALE,
   PROPERTY_GROUP_ITEM_PART_MOUSE_EVENTS,
   PROPERTY_GROUP_ITEM_PART_REPEAT_EVENTS,
   PROPERTY_GROUP_ITEM_PART_CLIP_TO,
   PROPERTY_GROUP_ITEM_PART_IGNORE_FLAGS,
   PROPERTY_GROUP_ITEM_PART_POINTER_MODE,

   PROPERTY_GROUP_ITEM_PART_DRAGABLE_TITLE,
   PROPERTY_GROUP_ITEM_PART_DRAGABLE_ENABLE,
   PROPERTY_GROUP_ITEM_PART_DRAGABLE_STEP,
   PROPERTY_GROUP_ITEM_PART_DRAGABLE_COUNT,
   PROPERTY_GROUP_ITEM_PART_DRAGABLE_CONFINE,
   PROPERTY_GROUP_ITEM_PART_DRAGABLE_THRESHOLD,
   PROPERTY_GROUP_ITEM_PART_DRAGABLE_EVENTS,

   PROPERTY_GROUP_ITEM_PART_GROUP_SOURCE,

   PROPERTY_GROUP_ITEM_PART_TEXT_EFFECT,
   PROPERTY_GROUP_ITEM_PART_TEXT_SHADOW_DIRECTION,

   PROPERTY_GROUP_ITEM_STATE_TITLE,
   PROPERTY_GROUP_ITEM_STATE_NAME,
   PROPERTY_GROUP_ITEM_STATE_VISIBLE,

   PROPERTY_GROUP_ITEM_STATE_IMAGE_TITLE,
   PROPERTY_GROUP_ITEM_STATE_IMAGE_NORMAL,
   PROPERTY_GROUP_ITEM_STATE_IMAGE_TWEEN,
   PROPERTY_GROUP_ITEM_STATE_IMAGE_BORDER_V,
   PROPERTY_GROUP_ITEM_STATE_IMAGE_BORDER_H,
   PROPERTY_GROUP_ITEM_STATE_IMAGE_MIDDLE,

   PROPERTY_GROUP_ITEM_STATE_SIZE_TITLE,
   PROPERTY_GROUP_ITEM_STATE_SIZE_MIN,
   PROPERTY_GROUP_ITEM_STATE_SIZE_MAX,
   PROPERTY_GROUP_ITEM_STATE_SIZE_MINMUL,
   PROPERTY_GROUP_ITEM_STATE_SIZE_FIXED,
   PROPERTY_GROUP_ITEM_STATE_SIZE_ASPECT_PREF,
   PROPERTY_GROUP_ITEM_STATE_SIZE_ASPECT,

   PROPERTY_GROUP_ITEM_STATE_POSITION_TITLE,
   PROPERTY_GROUP_ITEM_STATE_POSITION_ALIGN,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL1_TITLE,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL1_TO_X,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL1_TO_Y,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL1_RELATIVE,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL1_OFFSET,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL2_TITLE,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL2_TO_X,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL2_TO_Y,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL2_RELATIVE,
   PROPERTY_GROUP_ITEM_STATE_POSITION_REL2_OFFSET,

   PROPERTY_GROUP_ITEM_STATE_COLORS_TITLE,
   PROPERTY_GROUP_ITEM_STATE_COLORS_COLOR_CLASS,
   PROPERTY_GROUP_ITEM_STATE_COLORS_COLOR,
   PROPERTY_GROUP_ITEM_STATE_COLORS_OUTLINE_COLOR,
   PROPERTY_GROUP_ITEM_STATE_COLORS_SHADOWCOLOR,

   PROPERTY_GROUP_ITEM_STATE_TEXT_COMMON_TITLE,
   PROPERTY_GROUP_ITEM_STATE_TEXT_COMMON_TEXT,
   PROPERTY_GROUP_ITEM_STATE_TEXT_COMMON_ALIGN,
   PROPERTY_GROUP_ITEM_STATE_TEXT_COMMON_MIN,
   PROPERTY_GROUP_ITEM_STATE_TEXT_COMMON_MAX,
   PROPERTY_GROUP_ITEM_STATE_TEXT_FONT,
   PROPERTY_GROUP_ITEM_STATE_TEXT_SIZE,
   PROPERTY_GROUP_ITEM_STATE_TEXT_FIT,
   PROPERTY_GROUP_ITEM_STATE_TEXT_SOURCE,
   PROPERTY_GROUP_ITEM_STATE_TEXT_TEXT_SOURCE,
   PROPERTY_GROUP_ITEM_STATE_TEXT_ELIPSIS,

   PROPERTY_GROUP_ITEM_STATE_TEXT_STYLE,
   PROPERTY_GROUP_ITEM_PART_SELECT_MODE,
   PROPERTY_GROUP_ITEM_PART_ENTRY_MODE,
   PROPERTY_GROUP_ITEM_PART_CURSOR_MODE,
   PROPERTY_GROUP_ITEM_PART_MULTILINE,
   PROPERTY_GROUP_ITEM_PART_TEXTBLOCK_SELECTION_UNDER,
   PROPERTY_GROUP_ITEM_PART_TEXTBLOCK_SELECTION_OVER,
   PROPERTY_GROUP_ITEM_PART_TEXTBLOCK_CURSOR_UNDER,
   PROPERTY_GROUP_ITEM_PART_TEXTBLOCK_CURSOR_OVER,
   PROPERTY_GROUP_ITEM_PART_TEXTBLOCK_ANCHORS_UNDER,
   PROPERTY_GROUP_ITEM_PART_TEXTBLOCK_ANCHORS_OVER,

   PROPERTY_GROUP_ITEM_STATE_PROXY_SOURCE,

   PROPERTY_GROUP_ITEM_STATE_FILL_TITLE,
   PROPERTY_GROUP_ITEM_STATE_FILL_TYPE,
   PROPERTY_GROUP_ITEM_STATE_FILL_SMOOTH,
   PROPERTY_GROUP_ITEM_STATE_FILL_ORIGIN_TITLE,
   PROPERTY_GROUP_ITEM_STATE_FILL_ORIGIN_RELATIVE,
   PROPERTY_GROUP_ITEM_STATE_FILL_ORIGIN_OFFSET,
   PROPERTY_GROUP_ITEM_STATE_FILL_SIZE_TITLE,
   PROPERTY_GROUP_ITEM_STATE_FILL_SIZE_RELATIVE,
   PROPERTY_GROUP_ITEM_STATE_FILL_SIZE_OFFSET,

   PROPERTY_GROUP_ITEM_STATE_CONTAINER_TITLE,
   PROPERTY_GROUP_ITEM_STATE_CONTAINER_ALIGN,
   PROPERTY_GROUP_ITEM_STATE_CONTAINER_PADDING,
   PROPERTY_GROUP_ITEM_STATE_CONTAINER_MIN,
   PROPERTY_GROUP_ITEM_STATE_CONTAINER_HOMOGENEOUS,

   PROPERTY_GROUP_ITEM_PART_ITEM_TITLE,
   PROPERTY_GROUP_ITEM_PART_ITEM_NAME,
   PROPERTY_GROUP_ITEM_PART_ITEM_SOURCE,
   PROPERTY_GROUP_ITEM_PART_ITEM_MIN,
   PROPERTY_GROUP_ITEM_PART_ITEM_MAX,
   PROPERTY_GROUP_ITEM_PART_ITEM_PREFER,
   PROPERTY_GROUP_ITEM_PART_ITEM_ALIGN,
   PROPERTY_GROUP_ITEM_PART_ITEM_WEIGHT,
   PROPERTY_GROUP_ITEM_PART_ITEM_ASPECT_MODE,
   PROPERTY_GROUP_ITEM_PART_ITEM_ASPECT,
   PROPERTY_GROUP_ITEM_PART_ITEM_SPREAD,
   PROPERTY_GROUP_ITEM_PART_ITEM_PADDING_H,/* horizontal (left and right) controls of padding */
   PROPERTY_GROUP_ITEM_PART_ITEM_PADDING_V,/* vertical (top and bottom) controls of padding */
   PROPERTY_GROUP_ITEM_PART_ITEM_SPAN,
   PROPERTY_GROUP_ITEM_PART_ITEM_POSITION,

   PROPERTY_GROUP_ITEM_PROGRAM_TITLE,
   PROPERTY_GROUP_ITEM_PROGRAM_NAME,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION,
   PROPERTY_GROUP_ITEM_PROGRAM_SIGNAL,
   PROPERTY_GROUP_ITEM_PROGRAM_SOURCE,
   PROPERTY_GROUP_ITEM_PROGRAM_IN,
   PROPERTY_GROUP_ITEM_PROGRAM_FILTER_PART,
   PROPERTY_GROUP_ITEM_PROGRAM_FILTER_STATE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TITLE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_STATE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_VALUE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_EMIT_SIGNAL,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_EMIT_SOURCE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_DRAG_VALUE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_TITLE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_TYPE,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_TIME,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_FACTOR,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_GRADIENT,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_DECAY,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_BOUNCES,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_SWINGS,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_BEZIER_POINT1,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_BEZIER_POINT2,

   PROPERTY_GROUP_ITEM_LAST
} Property_Group_Item;

/* Enum of property_sound items. */
typedef enum {
   PROPERTY_SOUND_ITEM_PREVIEW_TITLE,
   PROPERTY_SOUND_ITEM_PLAYER,

   PROPERTY_SOUND_ITEM_INFO_TITLE,
   PROPERTY_SOUND_ITEM_NAME,
   PROPERTY_SOUND_ITEM_FILE_NAME,
#ifdef HAVE_AUDIO
   PROPERTY_SOUND_ITEM_DURATION,
#endif
   PROPERTY_SOUND_ITEM_TYPE,
   PROPERTY_SOUND_ITEM_SIZE,
   PROPERTY_SOUND_ITEM_COMPRESSION_TYPE,
   PROPERTY_SOUND_ITEM_COMPRESSION_QUALITY,
   PROPERTY_SOUND_ITEM_FREQ,

   PROPERTY_SOUND_ITEM_LAST
} Property_Sound_Item;

struct _Property_Attribute {
   Eina_Stringshare *name;
   union {
      Property_Group_Item group_item;
      Property_Sound_Item sound_item;
   } type;
   Eina_Stringshare *icon_name;
   void *data;
   Elm_Object_Item *glit; /**< reference to gennlist item iteself */
   Property_Action action1; /**< first control */
   Property_Action action2; /**< second control */
   Property_Expand_Cb expand_cb; /**< This cb will be called to get subitems */
   Property_Attribute_Filter_Cb filter_cb; /**< called when item is checked for being filtered */
   union {
      unsigned int part_types;
      unsigned int sound_types;
      unsigned int action_types;
   } filter_data;
   Property_Attribute_Del_Cb del_cb; /**< called when item is deleted from genlist */
   Eina_Bool expandable : 1; /**< if true  item is tree node */
   Eina_Bool expanded : 1; /**< if true initial state will be expanded. */
   Eina_Bool realized : 1; /**< if item is not realized update callback will not be called */
};

void
property_item_add(Evas_Object *property, Property_Attribute *pa, Elm_Object_Item *parent);

void
property_item_update_recursively(Property_Attribute *pa);

void
property_item_update(Property_Attribute *pa);

/* delete item from property only if it was added */
static inline void
property_item_del(Property_Attribute *pa)
{
   if (pa->glit) elm_object_item_del(pa->glit);
}

/* color_control */
Evas_Object *
property_color_control_add(Evas_Object *parent);

void
property_color_control_color_set(Evas_Object *control, int r, int g, int b, int a);

void
property_color_control_color_get(Evas_Object *control, int *r, int *g, int *b, int *a);

/* group image normal control */
Evas_Object *
property_image_normal_control_add(Evas_Object *parent);

/* tween images control */
Evas_Object *
property_image_tween_control_add(Evas_Object *parent);

void
property_image_tween_append(Evas_Object *control, Eina_Stringshare *image);

Eina_List *
property_image_tween_added_list_get(void);

Eina_List *
property_image_tween_deleted_list_get(void);

void
property_image_tween_lists_free(void);

void
property_image_tween_cleanup(Evas_Object *control);

/* player control */
Evas_Object *
property_player_control_add(Evas_Object *parent);

/* color class manager submodule */
void
property_color_class_manager_init(void);

Eina_List *
property_color_class_manager_items_get(void);

/* image manager submodule */
void
property_image_manager_init(void);

Eina_List *
property_image_manager_items_get(void);

/* sound manager submodule */
void
property_sound_manager_init(Property_Data *pd);

Eina_List *
property_sound_manager_items_get(void);

/* dummy submodule. for implementation tests. will be deleted later */
void
property_dummy_init(void);

Eina_List *
property_dummy_items_get(void);

/* property group */
void
property_group_init(Property_Data *pd);

Eina_List *
property_group_items_get(void);

/* entry functions */
/* we need additional processing before elm_entry_entry_set and after
   elm_entry_entry_get. So we shouldn't use them directly */
static inline Eina_Stringshare *
property_entry_get(Evas_Object *entry)
{
   Eina_Stringshare *text;
   const char *markup;
   char *utf8;

   assert(entry != NULL);

   markup = elm_entry_entry_get(entry);
   utf8 = elm_entry_markup_to_utf8(markup);
   text = eina_stringshare_add(utf8);
   free(utf8);

   return text;
}

static inline void
property_entry_set(Evas_Object *entry, const char *text)
{
   char *markup;

   assert(entry != NULL);

   if (text != NULL)
     {
        markup = elm_entry_utf8_to_markup(text);
        if (strcmp(markup, elm_entry_entry_get(entry)) != 0)
          elm_entry_entry_set(entry, markup);
        free(markup);
     }
   else
     elm_entry_entry_set(entry, "");
}

#pragma GCC poison elm_entry_entry_get
#pragma GCC poison elm_entry_entry_set

/* string constants */

#define STR_NONE "None"

#endif /* PROPERTY_PRIVATE_H */
