#ifndef DEMO_GROUP_H
#define DEMO_GROUP_H

#include "eflete.h"
#include "resource_manager2.h"

/**
 * @defgroup Partlist Partlist
 *
 * @ingroup EFLETE
 */

/**
 * emited when part is selected in part list while in demo mode.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_DEMO_GROUP_PART_SELECTED "SIGNAL_DEMO_GROUP_PART_SELECTED"

typedef enum _Swallow_Content_Type
{
   CONTENT_NONE = 0 ,  /**< None type value */
   CONTENT_RECTANGLE,  /**< Rectangle type value */
   CONTENT_IMAGE,  /**< Image type value */
   CONTENT_WIDGET  /**< Widget type value */
} Swallow_Content_Type;

typedef enum _Swallow_Content_Widget_Type
{
   WIDGET_LAYOUT = 0,   /**< layout widget */
   WIDGET_BUTTON,  /**< button widget */
   WIDGET_CHECK,  /**< check widget */
   WIDGET_SLIDER,  /**< slider widget */
   WIDGET_ENTRY,  /**< entry widget */
   WIDGET_PROGRESSBAR,  /**< progressbar widget */
   WIDGET_RADIO, /**< radio widget */
   WIDGET_SPINNER, /**< spinner widget */
   WIDGET_LABEL, /**< label widget */
   WIDGET_CALENDAR, /**< calendar widget */
   WIDGET_ICON, /**<icon widget */
   WIDGET_COLORSELECTOR, /**<colorselector widget */
   WIDGET_FILESELECTOR, /**<fileselector widget */
   WIDGET_DAYSELECTOR, /**<dayselector widget */
   WIDGET_DATETIME, /**<datetime widget */
   WIDGET_FRAME, /**<frame widget */
   WIDGET_PANEL, /**<panel widget */
   WIDGET_SEGMENT_CONTROL, /**<segment_control widget */
   WIDGET_CLOCK, /**<clock widget */
   WIDGET_PANES, /**<panes widget */
   WIDGET_LIST, /**< list widget */
   WIDGET_GRID, /**< grid widget */
   WIDGET_TABLE, /**< table widget */
   WIDGET_BOX, /**< box widget */
   WIDGET_NAVIFRAME, /**< box widget */
   WIDGET_GENLIST, /**<genlist widget */
   WIDGET_GENGRID, /**<gengrid widget */
   WIDGET_SCROLLER, /**<scroller widget */
   WIDGET_TOOLBAR, /**<toolbar widget */
   WIDGET_MAP, /**< map widget */
} Swallow_Content_Widget_Type;

/**
 * @typedef Demo_Part_
 * @ingroup DemoGroup
 */
typedef struct Demo_Part_ Demo_Part;

/**
 * @typedef Demo_Signal_
 * @ingroup DemoGroup
 */
typedef struct Demo_Signal_ Demo_Signal;

struct Demo_Part_
{
   Eina_Stringshare *name;    /**< part name */
   Edje_Part_Type type;       /**< part type */

   Eina_Stringshare *text_content; /**< text in demo (for TEXT parts) */
   Evas_Object *object; /**< object content in demo (for SWALLOW parts) */
   Eina_List *objects; /**< for demos containing several widgets to show (like radio) */
   int swallow_content; /**< type of content (picture, rectangle or widget) */
   int r, g, b, a; /**< color of content */
   int widget; /**< number of widget to be set up */
   Eina_Stringshare *content_style; /**< number of widget to be set up */
   Eina_Stringshare *image_path; /**< path to image to set up */
   int max_w, max_h, min_w, min_h; /**< object's size */
   Eina_Bool change; /**< in case when content type is changed,
                          demo need to create new one */
};

struct Demo_Signal_
{
   Eina_Stringshare *prog_name;
   Eina_Stringshare *sig_name;
   Eina_Stringshare *source_name;
   Eina_Stringshare *emit_signal;
   Eina_Stringshare *emitter;
   Edje_Action_Type action;
};

/**
 * @ingroup DemoGroup
 */
Evas_Object *
demo_group_add(Group2 *group);

/**
 * @ingroup DemoGroup
 */
void
demo_group_del(Evas_Object *demo_group);

/**
 * @ingroup DemoGroup
 */
void
demo_group_part_add(Evas_Object *demo, Part2 *part);

/**
 * @ingroup DemoGroup
 */
void
demo_group_part_del(Evas_Object *demo, Part2 *part);

/**
 * @ingroup DemoGroup
 */
void
demo_group_program_del(Evas_Object *demo, Eina_Stringshare *program_name);

/**
 * @ingroup DemoGroup
 */
void
demo_group_program_add(Evas_Object *demo, Eina_Stringshare *program_name);

/**
 * @ingroup DemoGroup
 */
void
demo_group_demo_update(Evas_Object *demo);

/**
 * @ingroup DemoGroup
 */
void
demo_group_property_update(Evas_Object *demo);

#endif /* DEMO_GROUP_H */
