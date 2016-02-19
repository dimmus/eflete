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

#ifndef DEMO_GROUP_H
#define DEMO_GROUP_H

#include "eflete.h"

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
   WIDGET_BUTTON = 0 ,  /**< button widget */
   WIDGET_CHECK,  /**< check widget */
   WIDGET_SLIDER,  /**< slider widget */
   WIDGET_ENTRY,  /**< entry widget */
   WIDGET_PROGRESSBAR /**< progressbar widget */
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
};

/**
 * @ingroup DemoGroup
 */
Evas_Object *
demo_group_add(Group *group);

/**
 * @ingroup DemoGroup
 */
void
demo_group_del(Evas_Object *demo_group);

/**
 * @ingroup DemoGroup
 */
void
demo_group_part_add(Evas_Object *demo, Part *part);

/**
 * @ingroup DemoGroup
 */
void
demo_group_part_del(Evas_Object *demo, Part *part);

/**
 * @ingroup DemoGroup
 */
void
demo_group_demo_update(Evas_Object *demo);

#endif /* DEMO_GROUP_H */
