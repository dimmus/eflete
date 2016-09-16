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

#include "eflete.h"

/**
 * @defgroup Demo Demo
 *
 * A Demo is responsible for showing widgets with contexts.
 * After user manipulations with parts, states or programs and saving
 * into a file, this space will show the resulted widget
 * by using user's theme (already saved one).
 */

/**
 * Create a object accordingly to given group.
 *
 * @param parent The parent object
 * @param group The group for create widget
 * @return The widget or layout with contexts
 *
 * @ingroup Demo
 */
Evas_Object *
demo_add(Evas_Object *parent, Group2 *group);
