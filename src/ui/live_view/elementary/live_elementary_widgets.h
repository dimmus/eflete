/**
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

#ifndef LIVE_ELEMENTARY_WIDGETS_H
#define LIVE_ELEMENTARY_WIDGETS_H

/**
 * @defgroup Live_Elementary_Widgets Live_Elementary_Widgets
 *
 * Header which contains all private functions required for all widgets in this folder.
 * This folder mostly contains only Elementary Widgets
 */

#include "eflete_config.h"
#include "eflete.h"
#include <Evas.h>
#include <Elementary.h>

/*****************************************************
 ****************** COMMON MACRO *********************
 *****************************************************/

#define ITEM "item"
#define PART_NAME "part_name"
#define SIGNAL_NAME "signal_data_name"
#define SIGNAL_SOURCE "signal_data_source"

#define TEXT_FUNC "text_func"
#define SWALLOW_FUNC "swallow_func"

/*****************************************************
 ***************** BUBBLE WIDGET *********************
 *****************************************************/

/**
 * Create bubble widget.
 *
 * @param parent The parent widget.
 * @param class class of bubble.
 * @return A new 'bubble' widget handler or
 *
 * @ingroup Live_Elementary_Widgets
 */
Evas_Object *
widget_bubble_create(Evas_Object *parent, const char *class);

/*****************************************************
 *****************************************************
 *****************************************************/

#endif /* LIVE_ELEMENTARY_WIDGETS_H */
