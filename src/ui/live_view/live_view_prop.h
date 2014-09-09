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

#ifndef UI_LIVEVIEWPROP_HEADER_H
#define UI_LIVEVIEWPROP_HEADER_H

/**
 * @defgroup Live_View_Property Live_View_Property
 *
 * A Live_View_Property is responsible for setting data into swallows of widget
 * that is being showed in Life View. This property allows user to set rectangles,
 * texts and images into swallow, text and textblock parts. Also it allows to
 * call signals and call widget-related functions to check behaviour and view of
 * the widget.
 */

#include "live_elementary_widgets.h"
#include "widget_macro.h"
#include "notify.h"
#include <Evas.h>
#include <Elementary.h>

/**
 * Add a new Live View Property widget.
 *
 * @param parent The parent widget.
 * @param live_view Live View structure.
 * @param in_prog_edit Is this live_view for program_editor.
 * @return A new 'property' widget handler or NULL, on errors
 *
 * @ingroup Live_View_Property
 */
Evas_Object *
live_view_property_add(Evas_Object *parent, Eina_Bool in_prog_edit);

/**
 * Set new style's property to Live View Property widget.
 *
 * @param property Live View Property widget.
 * @param object Widget that is being shown in Live View.
 * @param Style Style structure of the widget in Live View.
 * @param widget Name of viewed widget.
 * @return @c EINA_TRUE - succesfull, EINA_FALSE - otherwise.
 *
 * @ingroup Live_View_Property
 */
Eina_Bool
live_view_property_style_set(Evas_Object *property, Evas_Object *object, Style *style, const char *widget);

/**
 * Clear all data from to Live View Property widget.
 *
 * @param property Live View Property widget.
 * @return @c EINA_TRUE - succesfull, EINA_FALSE - otherwise.
 *
 * @ingroup Live_View_Property
 */
Eina_Bool
live_view_property_style_unset(Evas_Object *property);

#endif /* UI_LIVEVIEWPROP_HEADER_H */
