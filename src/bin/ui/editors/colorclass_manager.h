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

#ifndef COLORCLASS_MANAGER_DIALOG_H
#define COLORCLASS_MANAGER_DIALOG_H

/**
 * @defgroup Colorclass_Manager Colorclass_Manager
 * @ingroup Dialogs
 *
 * Show color classes, which uses in project.
 */

#include "eflete.h"

/**
 * Add new colorclass viewer inwin object.
 *
 * @return Pointer to inwin object.
 *
 * @ingroup Colorclass_Manager
 */
Evas_Object *
colorclass_manager_add();

#endif /* COLORCLASS_MANAGER_DIALOG_H */
