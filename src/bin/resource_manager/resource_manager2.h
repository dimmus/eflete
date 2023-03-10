#ifndef RESOURCE_MANAGER2_H
#define RESOURCE_MANAGER2_H

#include "eflete.h"
#include "resource_manager_signals.h"

enum _Resource2_Type
{
   RESOURCE2_TYPE_GROUP = 0,
   RESOURCE2_TYPE_PART,
   RESOURCE2_TYPE_STATE,
   RESOURCE2_TYPE_ITEM,
   RESOURCE2_TYPE_PROGRAM,
   RESOURCE2_TYPE_SCRIPT,
   RESOURCE2_TYPE_LIMIT, /* not yet */
   RESOURCE2_TYPE_DATA_GLOBAL,
   RESOURCE2_TYPE_DATA_GROUP,

   /* other managers */
   RESOURCE2_TYPE_VECTOR,
   RESOURCE2_TYPE_IMAGE,
   RESOURCE2_TYPE_IMAGE_SET,
   RESOURCE2_TYPE_SAMPLE,
   RESOURCE2_TYPE_TONE,
   RESOURCE2_TYPE_FONT,
   RESOURCE2_TYPE_COLORCLASS,
   RESOURCE2_TYPE_STYLE,
   RESOURCE2_TYPE_STYLE_TAG,
   RESOURCE2_TYPE_SIZECLASS,
   RESOURCE2_TYPE_TEXTCLASS,
   RESOURCE2_TYPE_VIBRO, /* something for now? */
   RESOURCE2_TYPE_LAST
};
typedef enum _Resource2_Type Resource2_Type;

struct _Resource2_Internal
{
   unsigned int id;
   Eina_Stringshare *name;
   Resource2_Type type;
   Eina_List *used_in; /* where is this resource actually used in? */
   Eina_List *uses___; /* what kind of reources this res is actually usiung now? */
};
typedef struct _Resource2_Internal Resource2_Internal;

/*********************************************/
/*********************************************/
/******* SOME COMMON RESOURCE2 STRUCTS ********/
/*********************************************/
/*********************************************/
typedef struct _Resource2 Resource2;
typedef struct _Group2 Group2;
typedef struct _Part2 Part2;
typedef struct _Part_Item2 Part_Item2;
typedef struct _State2 State2;
typedef struct _Program2 Program2;
typedef struct _Style2 Style2;
typedef struct _Image2 Image2;
typedef struct _Image_Set2 Image_Set2;
typedef struct _Tone2 Tone2;
typedef struct _Style_Tag2 Style_Tag2;
typedef struct _Colorclass2 Colorclass2;

typedef struct _Sound2 Vector2;
typedef struct _Sound2 Sound2;
typedef struct _Sound2 Font2;
typedef struct _Sound2 Global_Data2;
typedef struct _Sound2 Group_Data2;
typedef struct _Script2 Script2;

struct _Resource2
{
   Resource2_Internal common;
};

struct _Group2
{
   Resource2_Internal common;
   Eina_List *parts;
   Eina_List *programs;
   Eina_List *data_items;
   Eina_List *limits;
   Script2 *script;
   Group2 *main_group;         /**< pointer to main group. NULL if group is not an alias */
   Eina_List *aliases;        /**< list of pointers to aliases. NULL if group is an alias */

   Eina_Stringshare *widget;  /**< parsed widget name */
   Eina_Stringshare *class;   /**< parsed class name */
   Eina_Stringshare *style;   /**< parsed style name */

   Evas_Object *edit_object;  /**< object needed to access group with edje_edit functions. Should be NULL if group is not open */
   Resource2 *current_selected;
   History *history;          /**< history of changes in the group */
   const char *display_name;  /**< display name for project navi. @note don't
                                   free this member, because it's a slice of
                                   full group name (common.name) **/
};

struct _Part2
{
   Resource2_Internal common;
   Edje_Part_Type type;       /**< part type */
   Eina_List *states;         /**< list of states */
   State2 *current_state;      /**< pointer to selected state */
   Eina_List *items;          /**< list of item names. Used only for BOX and TABLE parts */
   Eina_Stringshare * current_item_name; /**< name of selected item */
   Group2 *group;              /**< pointer to group */
   Eina_Bool visible;         /**< is part visible on workspace*/
};

struct _State2
{
   Resource2_Internal common;
   double val;                /**< parsed state value */

   Eina_Stringshare *normal;   /* normal image being used (could be NONE) */
   Eina_List *tweens;          /* list of tweens in correct order */
   Part2 *part;                /**< pointer to part */
};

struct _Part_Item2
{
   Resource2_Internal common;
   Eina_Stringshare *source;
   Part2 *part;
};

struct _Program2
{
   Resource2_Internal common;
   Edje_Action_Type type;
   Eina_List *targets;
   Eina_List *afters;
   Eina_Stringshare *filter_part;
   Group2 *group;
};


struct _Script2
{
   Resource2_Internal common;
   Eina_Strbuf *code;          /**< pointer to script code */
};

struct _Style2
{
   Resource2_Internal common;
   Eina_Stringshare *raw_style; //for set to textblock and to edje_edit
   Eina_List *tags;
};

struct _Style_Tag2
{
   Resource2_Internal common;
   Eina_Stringshare *font;
   Style2 *style;
};

struct _Image2
{
   Resource2_Internal common;
   const char *source;
   Edje_Edit_Image_Comp comp_type;
   int quality;
   Eina_Bool is_used;
   int height, width;
};

struct _Image_Set2
{
   Resource2_Internal common;
   Eina_Bool is_used;
};

struct _Tone2
{
   Resource2_Internal common;
   int freq;
};

struct _Sound2
{
   Resource2_Internal common;
   const char *source;
};

struct _Colorclass2
{
   Resource2_Internal common;
   struct {
      int r,g,b,a;
   } color1;
   struct {
      int r,g,b,a;
   } color2;
   struct {
      int r,g,b,a;
   } color3;
   Eina_Stringshare *description;
};

Eina_Bool
resource_manager_init(Project *project);

Eina_Bool
resource_manager_shutdown(Project *project);

Resource2 *
resource_manager_find(const Eina_List *list, Eina_Stringshare *name);

Resource2 *
resource_manager_v_find(const Eina_List *list, Eina_Stringshare *name, double value);

Resource2 *
resource_manager_id_find(const Eina_List *list, unsigned int id);

void
resource_group_edit_object_load(Project *pro, Group2 *group, Evas *e);

void
resource_group_edit_object_unload(Group2 *group);

void
resource_group_edit_object_reload(Project *pro, Group2 *group);

Eina_Bool
resource_manager_groups_circular_are(Part2 *current, Group2 *source);

#endif /* RESOURCE_MANAGER2_H */
