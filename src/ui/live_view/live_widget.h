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

#ifndef LIVE_WIDGET_H
#define LIVE_WIDGET_H

/**
 * @defgroup Live_Widget Live_Widget
 * @ingroup Live_View
 *
 * SubModule used to create widgets with all information they need for
 * showing them in live view and in live view properties.
 */

#include "live_view.h"
#include <Evas.h>
#include <Elementary.h>

Evas_Object *
live_widget_create(const char *widget, const char *class, const char *style, Evas_Object *parent);

Eina_Bool
live_widget_del(Evas_Object *widget);


#endif /* LIVE_WIDGET_H */
