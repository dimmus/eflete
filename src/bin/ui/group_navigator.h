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

#ifndef GROUP_NAVIGATOR_H
#define GROUP_NAVIGATOR_H

#include "eflete.h"
#include "group_manager.h"

/**
 * @defgroup Partlist Partlist
 *
 * @ingroup EFLETE
 */

/**
 * emited when part is selected in part list.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_GROUP_NAVIGATOR_PART_SELECTED "SIGNAL_GROUP_NAVIGATOR_PART_SELECTED"


/**
 * emited when part is restacked in part list.
 * eventinfo - pointer to Editor_Part_Restack structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_GROUP_NAVIGATOR_PART_RESTACKED "SIGNAL_GROUP_NAVIGATOR_PART_RESTACKED"

/**
 * emited when part_item is restacked in part list.
 * eventinfo - pointer to Editor_Part_Item_Restack structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_GROUP_NAVIGATOR_PART_ITEM_RESTACKED "SIGNAL_GROUP_NAVIGATOR_PART_ITEM_RESTACKED"

/**
 * emited when part state is activated in part list.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_GROUP_NAVIGATOR_PART_STATE_SELECTED "SIGNAL_GROUP_NAVIGATOR_PART_STATE_SELECTED"

/**
 * emited when part visibility is changed.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_GROUP_NAVIGATOR_PART_VISIBLE_CHANGED "SIGNAL_GROUP_NAVIGATOR_PART_VISIBLE_CHANGE"

/**
 * @ingroup Partlist
 */
Evas_Object *
group_navigator_add(Group *group);

/**
 * @ingroup Partlist
 */
void
group_navigator_part_select(Evas_Object *obj, Part_ *part);

/**
 * @ingroup Partlist
 */
void
group_navigator_part_update(Evas_Object *obj, Part_ *part);
#endif /* GROUP_NAVIGATOR_H */
