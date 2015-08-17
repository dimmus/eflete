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

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

/**
 * @defgroup Navigator
 * @ingroup Block
 *
 * This widget is used to navigate in groups tree.
 *
 * This widget emits the following signals:
 * @li "navigator,group,selected": the user double clicked on the group name;
 */
#include "eflete.h"
#include "group_manager.h"

/**
 * Add navigator object. This object is a layout with two buttons: "plus" and
 * "minus", for add and delete layout from navigator. Also layout contaions a
 * genlist for view and navigate by layouts in given project.
 *
 * @return The navigator object
 * @ingroup Navigator
 */
Evas_Object *
navigator_add(void);

#endif /* NAVIGATOR_H */
