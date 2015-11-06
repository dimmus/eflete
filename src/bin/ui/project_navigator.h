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
 * @li "project_navigator,group,selected": the user double clicked on the group name;
 */
#include "eflete.h"
#include "group_manager.h"

/**
 * Add project_navigator object. This object is a layout with two buttons: "plus" and
 * "minus", for add and delete layout from project_navigator. Also layout contaions a
 * genlist for view and navigate by layouts in given project.
 *
 * @return The project_navigator object
 * @ingroup Navigator
 */
Evas_Object *
project_navigator_add(void);

/**
 * Set the project to project_navigator. By project data project_navigator build a layouts
 * (styles) tree. Navigator used the global data (App_Data) for take a opened
 * project.
 *
 * @warning If project not avaliable assert is work.
 *
 * @ingroup Navigator
 */
void
project_navigator_project_set(void);

/**
 * Unset the given project. Navigator delete all data from genlist and disable
 * manipulation buttons ("plus" and "delete").
 *
 * @ingroup Navigator
 */
void
project_navigator_project_unset(void);

#endif /* NAVIGATOR_H */
