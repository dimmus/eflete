#include "main_window.h"
#include "syntax_color.h"


#ifndef PROPERTY_PRIVATE_H
#define PROPERTY_PRIVATE_H

#define PROPERTY_DATA "prop_data"
#define PROPERTY_DATA_GET(OBJ) \
   Property_Data *pd = evas_object_data_get(OBJ, PROPERTY_DATA); \
   assert(pd != NULL);

typedef struct
{
   int index;
   Eina_Stringshare *data;
} Combobox_Item;

typedef struct
{
   int index;
   Eina_Stringshare *data;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
} Combobox_Cc_Item;

enum _Property_Control {
   PROPERTY_CONTROL_NONE,

   PROPERTY_CONTROL_CHECK,
   PROPERTY_CONTROL_BUTTON,
   PROPERTY_CONTROL_COMBOBOX,
   PROPERTY_CONTROL_COMBOBOX_CC, /* special combobox for color_classes */
   PROPERTY_CONTROL_SPINNER,
   PROPERTY_CONTROL_ENTRY,
   PROPERTY_CONTROL_ENTRY_SCRIPT,
   PROPERTY_CONTROL_COLOR,
   PROPERTY_CONTROL_COLORSEL,
   PROPERTY_CONTROL_LABEL,
   PROPERTY_CONTROL_VECTOR_NORMAL,
   PROPERTY_CONTROL_IMAGE_NORMAL,
   PROPERTY_CONTROL_IMAGE_TWEEN,
   PROPERTY_CONTROL_IMAGE_SELECTOR,

   PROPERTY_CONTROL_LAST
};
typedef enum _Property_Control Property_Control;
typedef struct _Property_Attribute Property_Attribute;
typedef struct _Property_Action Property_Action;
typedef struct {
   Evas_Object *layout;
   Evas_Object *genlist;
   Property_Mode mode;
   Property_Action *started_action;
   Property_Attribute *started_pa;

   Elm_Genlist_Item_Class *itc_caption;
   Elm_Genlist_Item_Class *itc_1swallow;
   Elm_Genlist_Item_Class *itc_1swallow_wide;
   Elm_Genlist_Item_Class *itc_2swallow;

   /* Stores what item class to use with each pair of controls */
   Elm_Genlist_Item_Class *item_classes[PROPERTY_CONTROL_LAST][PROPERTY_CONTROL_LAST];
} Property_Data;

typedef void (* Property_Cb) (Property_Attribute *, Property_Action *);
typedef Eina_Bool (* Property_Update_Cb) (Property_Attribute *, Property_Action *);
typedef Eina_Bool (* Property_Attribute_Filter_Cb) (Property_Attribute *);
typedef void (* Property_Attribute_Del_Cb) (Property_Attribute *);
typedef Eina_List * (* Property_Expand_Cb) (Property_Attribute *);

/* filter needs to be set to any non-zero value to be applied. Not in all cases
   useful key can be given so we ignore its value and use 1 instead. */
#define GENLIST_FILTER_APPLY(GEN) elm_genlist_filter_set(GEN, (void *)1);
#pragma GCC poison elm_genlist_filter_set

typedef enum {
   ATTRIBUTE_IMAGE_ITEM_NAME,
   ATTRIBUTE_IMAGE_ITEM_TYPE,

   ATTRIBUTE_IMAGE_ITEM_LOCATION,
   ATTRIBUTE_IMAGE_ITEM_COMPRESSION,
   ATTRIBUTE_IMAGE_ITEM_QUALITY,
   ATTRIBUTE_IMAGE_ITEM_WIDTH,
   ATTRIBUTE_IMAGE_ITEM_HEIGHT,

   ATTRIBUTE_IMAGE_ITEM_BORDER_L,
   ATTRIBUTE_IMAGE_ITEM_BORDER_R,
   ATTRIBUTE_IMAGE_ITEM_BORDER_T,
   ATTRIBUTE_IMAGE_ITEM_BORDER_B,
   ATTRIBUTE_IMAGE_ITEM_MIN_W,
   ATTRIBUTE_IMAGE_ITEM_MIN_H,
   ATTRIBUTE_IMAGE_ITEM_MAX_W,
   ATTRIBUTE_IMAGE_ITEM_MAX_H,
   ATTRIBUTE_IMAGE_ITEM_BORDER_SCALE,

   ATTRIBUTE_IMAGE_ITEM_LAST
} Attribute_Image_Item;

typedef enum {
   ATTRIBUTE_DEMO_ITEM_TEXT_NAME,
   ATTRIBUTE_DEMO_ITEM_TEXT_CONTENT,

   ATTRIBUTE_DEMO_ITEM_SWALLOW_NAME,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_CONTENT,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_PICTURE,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_WIDGET,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_STYLE,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_LAYOUT_GROUP,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_COLOR,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_W,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_H,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_W,
   ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_H,

   ATTRIBUTE_DEMO_ITEM_PROGRAM_SIGNAL,
   ATTRIBUTE_DEMO_ITEM_PROGRAM_SOURCE,
   ATTRIBUTE_DEMO_ITEM_PROGRAM_ACTION,
   ATTRIBUTE_DEMO_ITEM_PROGRAM_EMIT,
   ATTRIBUTE_DEMO_ITEM_PROGRAM_EMITTER,

   ATTRIBUTE_DEMO_ITEM_LAST
} Attribute_Demo_Item;

typedef enum {
   ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME,
   ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT,
   ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH,
   ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR,
   ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE,

   ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR,
   ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_ALIGN_VER,
   ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_LEFT,
   ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_MARGIN_RIGHT,
   ATTRIBUTE_TEXTBLOCK_ITEM_POSITION_WRAP,

   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_TABSTOPS,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_CHECK,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_BG_COLOR_COLOR,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_PASSWORD,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_CHECK,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS_VALUE,
   ATTRIBUTE_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE,

   ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE,
   ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR,
   ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION,
   ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR,
   ATTRIBUTE_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR,

   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_CHECK,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH_COLOR,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_WIDTH,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_GAP,
   ATTRIBUTE_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR,

   ATTRIBUTE_TEXTBLOCK_ITEM_LAST
} Attribute_Textblock_Item;

struct _Property_Action {
   Property_Control control_type;
   Eina_Stringshare *name;
   Eina_Stringshare *units;
   Eina_Stringshare *tooltip;
   union {
      Attribute attribute; /**< type for group properties */
      Attribute_Image_Item attribute_image; /**< type for group properties */
      Attribute_Demo_Item attribute_demo; /**< type for demo properties */
      Attribute_Textblock_Item attribute_textblock; /**< type for textblock properties */
   } type; /**< submodule-specific enums */
   Evas_Object *control; /**< pointer to widget */
   Property_Cb init_cb;
   Property_Cb start_cb;
   Property_Cb change_cb;
   Property_Cb stop_cb;
   Property_Update_Cb update_cb; /**< called when item is realized and control's values should be updated */
};

/* Enum of property_group items. Can't use Attribute here because items can have
   0-2 attributes associated with them */
typedef enum {
   PROPERTY_GROUP_ITEM_GROUP_TITLE,
   PROPERTY_GROUP_ITEM_GROUP_NAME,
   PROPERTY_GROUP_ITEM_GROUP_MIN,
   PROPERTY_GROUP_ITEM_GROUP_MAX,
   PROPERTY_GROUP_ITEM_GROUP_SCRIPT,

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

   PROPERTY_GROUP_ITEM_STATE_VECTOR_TITLE,
   PROPERTY_GROUP_ITEM_STATE_VECTOR_NORMAL,

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

   PROPERTY_GROUP_ITEM_STATE_MAP_TITLE,
   PROPERTY_GROUP_ITEM_STATE_MAP_ON,
   PROPERTY_GROUP_ITEM_STATE_MAP_PERSPECTIVE_ON,
   PROPERTY_GROUP_ITEM_STATE_MAP_PERSPECTIVE,
   PROPERTY_GROUP_ITEM_STATE_MAP_LIGHT,
   PROPERTY_GROUP_ITEM_STATE_MAP_SMOOTH,
   PROPERTY_GROUP_ITEM_STATE_MAP_ALPHA,
   PROPERTY_GROUP_ITEM_STATE_MAP_BACKFACE_CULL,
   PROPERTY_GROUP_ITEM_STATE_MAP_PERSPECTIVE_FOCAL,
   PROPERTY_GROUP_ITEM_STATE_MAP_PERSPECTIVE_ZPLANE,
   PROPERTY_GROUP_ITEM_STATE_MAP_ROTATION_CENTER,
   PROPERTY_GROUP_ITEM_STATE_MAP_ROTATION_X,
   PROPERTY_GROUP_ITEM_STATE_MAP_ROTATION_Y,
   PROPERTY_GROUP_ITEM_STATE_MAP_ROTATION_Z,
   PROPERTY_GROUP_ITEM_STATE_MAP_POINT_COLOR_12,
   PROPERTY_GROUP_ITEM_STATE_MAP_POINT_COLOR_34,

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
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_FROM_CURRENT,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_FACTOR,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_GRADIENT,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_DECAY,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_BOUNCES,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_SWINGS,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_BEZIER_POINT1,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TRANSITION_BEZIER_POINT2,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_SCRIPT,
   PROPERTY_GROUP_ITEM_PROGRAM_ACTION_TARGET,
   PROPERTY_GROUP_ITEM_PROGRAM_AFTER,
   PROPERTY_GROUP_ITEM_PROGRAM_CHANNEL,
   PROPERTY_GROUP_ITEM_PROGRAM_TONE_DURATION,
   PROPERTY_GROUP_ITEM_PROGRAM_SAMPLE_SPEED,
   PROPERTY_GROUP_ITEM_PROGRAM_SAMPLE_NAME,
   PROPERTY_GROUP_ITEM_PROGRAM_TONE_NAME,

   PROPERTY_GROUP_ITEM_GROUP_DATA_TITLE,
   PROPERTY_GROUP_ITEM_GROUP_DATA_NAME,
   PROPERTY_GROUP_ITEM_GROUP_DATA_VALUE,

   PROPERTY_GROUP_ITEM_LAST
} Property_Group_Item;

/* Enum of property_sound items. */
typedef enum {
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

/* Enum of property_image items. */
typedef enum {
     PROPERTY_IMAGE_ITEM_INFO_TITLE,
     PROPERTY_IMAGE_ITEM_NAME,
     PROPERTY_IMAGE_ITEM_TYPE,

     PROPERTY_IMAGE_ITEM_IMAGE_PROPERTY_TITLE,
     PROPERTY_IMAGE_ITEM_LOCATION,
     PROPERTY_IMAGE_ITEM_COMPRESSION,
     PROPERTY_IMAGE_ITEM_QUALITY,
     PROPERTY_IMAGE_ITEM_WIDTH,
     PROPERTY_IMAGE_ITEM_HEIGHT,

     PROPERTY_IMAGE_ITEM_SET_IMAGE_PROPERTY_TITLE,
     PROPERTY_IMAGE_ITEM_BORDER_H,
     PROPERTY_IMAGE_ITEM_BORDER_V,
     PROPERTY_IMAGE_ITEM_BORDER_SCALE,
     PROPERTY_IMAGE_ITEM_MIN,
     PROPERTY_IMAGE_ITEM_MAX,

     PROPERTY_IMAGE_ITEM_LAST
} Property_Image_Item;

/* Enum of property_demo items. */
typedef enum {
   PROPERTY_DEMO_ITEM_TEXT_TITLE,
   PROPERTY_DEMO_ITEM_TEXT_NAME,
   PROPERTY_DEMO_ITEM_TEXT_CONTENT,

   PROPERTY_DEMO_ITEM_SWALLOW_TITLE,
   PROPERTY_DEMO_ITEM_SWALLOW_NAME,
   PROPERTY_DEMO_ITEM_SWALLOW_CONTENT,
   PROPERTY_DEMO_ITEM_SWALLOW_PICTURE,
   PROPERTY_DEMO_ITEM_SWALLOW_WIDGET,
   PROPERTY_DEMO_ITEM_SWALLOW_STYLE,
   PROPERTY_DEMO_ITEM_SWALLOW_LAYOUT_GROUP,
   PROPERTY_DEMO_ITEM_SWALLOW_COLOR,
   PROPERTY_DEMO_ITEM_SWALLOW_MIN,
   PROPERTY_DEMO_ITEM_SWALLOW_MAX,

   PROPERTY_DEMO_ITEM_PROGRAM_TITLE,
   PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL,
   PROPERTY_DEMO_ITEM_PROGRAM_SOURCE,
   PROPERTY_DEMO_ITEM_PROGRAM_ACTION,
   PROPERTY_DEMO_ITEM_PROGRAM_EMIT,
   PROPERTY_DEMO_ITEM_PROGRAM_EMITTER,

   PROPERTY_DEMO_ITEM_LAST
} Property_Demo_Item;

/* Enum of property_textblock items. */
typedef enum {
   PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE,
   PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME,
   PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT,
   PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH,
   PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR,
   PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE,

   PROPERTY_TEXTBLOCK_ITEM_POSITION_TITLE,
   PROPERTY_TEXTBLOCK_ITEM_POSITION_ALIGN_HOR,
   PROPERTY_TEXTBLOCK_ITEM_POSITION_ALIGN_VER,
   PROPERTY_TEXTBLOCK_ITEM_POSITION_MARGIN,
   PROPERTY_TEXTBLOCK_ITEM_POSITION_WRAP,

   PROPERTY_TEXTBLOCK_ITEM_FORMAT_TITLE,
   PROPERTY_TEXTBLOCK_ITEM_FORMAT_TABSTOPS,
   PROPERTY_TEXTBLOCK_ITEM_FORMAT_LINE_SIZE,
   PROPERTY_TEXTBLOCK_ITEM_FORMAT_BG_COLOR,
   PROPERTY_TEXTBLOCK_ITEM_FORMAT_PASSWORD,
   PROPERTY_TEXTBLOCK_ITEM_FORMAT_ELLIPSIS,
   PROPERTY_TEXTBLOCK_ITEM_FORMAT_LINE_RELATED_SIZE,

   PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_TITLE,
   PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_STYLE,
   PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_COLOR,
   PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_DIRECTION,
   PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_OUTER_COLOR,
   PROPERTY_TEXTBLOCK_ITEM_GLOW_SHADOW_INNER_COLOR,

   PROPERTY_TEXTBLOCK_ITEM_LINES_TITLE,
   PROPERTY_TEXTBLOCK_ITEM_LINES_STRIKETHROUGH,
   PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE,
   PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_ONE,
   PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_COLOR_TWO,
   PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH,
   PROPERTY_TEXTBLOCK_ITEM_LINES_UNDERLINE_DASH_COLOR,

   PROPERTY_TEXTBLOCK_ITEM_LAST
} Property_Textblock_Item;

struct _Property_Attribute {
   Eina_Stringshare *name;
   union {
      Property_Group_Item group_item;
      Property_Sound_Item sound_item;
      Property_Image_Item image_item;
      Property_Demo_Item demo_item;
      Property_Textblock_Item textblock_item;
   } type;
   Eina_Stringshare *icon_name;
   void *data;
   int int_data;
   Elm_Object_Item *glit; /**< reference to gennlist item iteself */
   Property_Action action1; /**< first control */
   Property_Action action2; /**< second control */
   Property_Expand_Cb expand_cb; /**< This cb will be called to get subitems */
   Property_Attribute_Filter_Cb filter_cb; /**< called when item is checked for being filtered */
   union {
      unsigned int part_types;
      unsigned int sound_types;
      unsigned int image_types;
      unsigned int action_types;
      unsigned int demo_types;
      unsigned int textblock_types;
   } filter_data;
   Property_Attribute_Del_Cb del_cb; /**< called when item is deleted from genlist */
   Eina_Bool expandable : 1; /**< if true  item is tree node */
   Eina_Bool expanded : 1; /**< if true initial state will be expanded. */
   Eina_Bool realized : 1; /**< if item is not realized update callback will not be called */
   Eina_Bool default_is : 1; /**< if attribute is in default state */
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

/* demo image path selector */
Evas_Object *
property_image_selector_get(Evas_Object *control);

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

/* group vectornormal control */
Evas_Object *
property_vector_normal_control_add(Evas_Object *parent);

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

void
property_player_sound_set(Sound_Data *sound);

/* color class manager submodule */
void
property_color_class_manager_init(Property_Data *pd);

Eina_List *
property_color_class_manager_items_get(void);

/* image manager submodule */
void
property_image_manager_init(Property_Data *pd);

Eina_List *
property_image_manager_items_get(void);

/* sound manager submodule */
void
property_sound_manager_init(Property_Data *pd);

Eina_List *
property_sound_manager_items_get(void);

/* textblock manager submodule */
void
property_textblock_manager_init(Property_Data *pd);

Eina_List *
property_textblock_manager_items_get(void);

Evas_Object *
property_entry_script_control_add(Evas_Object *parent);

/* dummy submodule. for implementation tests. will be deleted later */
void
property_dummy_init(void);

Eina_List *
property_dummy_items_get(void);

/* property group */
void
property_group_del(void);

void
property_group_init(Property_Data *pd);

Eina_List *
property_group_items_get(void);

/* property demo */
void
property_demo_init(Property_Data *pd);

Eina_List *
property_demo_items_get(void);

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
        if ((elm_entry_entry_get(entry)) &&
            (strcmp(markup, elm_entry_entry_get(entry)) != 0))
          elm_entry_entry_set(entry, markup);
        free(markup);
     }
   else
     elm_entry_entry_set(entry, "");
}

static inline void
property_color_entry_set(Evas_Object *entry, const char *text, color_data *c_data)
{
   char *markup;
   char *colored;

   assert(entry != NULL);

   if (text != NULL)
     {
        markup = elm_entry_utf8_to_markup(text);
        colored = color_apply(c_data, markup, strlen_safe(markup), NULL, NULL);

        if (colored == NULL) return;

        if ((elm_entry_entry_get(entry)) &&
            (strcmp(colored, elm_entry_entry_get(entry)) != 0))
          elm_entry_entry_set(entry, colored);
        free(markup);
        free(colored);
     }
   else
     elm_entry_entry_set(entry, "");
}

#pragma GCC poison elm_entry_entry_get
#pragma GCC poison elm_entry_entry_set

/* string constants */

#define STR_NONE "None"

#endif /* PROPERTY_PRIVATE_H */
