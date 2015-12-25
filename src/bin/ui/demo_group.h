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
#include "group_manager.h"

/**
 * @defgroup Partlist Partlist
 *
 * @ingroup EFLETE
 */

/**
 * emited when part is selected in part list while in demo mode.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_DEMO_GROUP_PART_SELECTED "SIGNAL_DEMO_GROUP_PART_SELECTED"

/**
 * @ingroup Partlist
 */
Evas_Object *
demo_group_add(Group *group);

#endif /* DEMO_GROUP_H */
