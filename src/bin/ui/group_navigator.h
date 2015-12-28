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

/**
 * @defgroup Partlist Partlist
 *
 * @ingroup EFLETE
 */

/**
 * emited when part is selected in part list.
 * eventinfo - pointer to Part structure
 *
 * @ingroup Partlist
 */
#define SIGNAL_GROUP_NAVIGATOR_PART_SELECTED "SIGNAL_GROUP_NAVIGATOR_PART_SELECTED"

/**
 * emited when part visibility is changed.
 * eventinfo - pointer to Part structure
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
group_navigator_part_select(Evas_Object *obj, Part *part);

/**
 * @ingroup Partlist
 */
void
group_navigator_part_update(Evas_Object *obj, Part *part);


void
group_navigator_part_state_select(Evas_Object *obj, State *state);

void
group_navigator_part_add(Evas_Object *obj, Part *part);

void
group_navigator_part_del(Evas_Object *obj, Part *part);

void
group_navigator_part_item_add(Evas_Object *obj, Part *part, Eina_Stringshare * item_name);

void
group_navigator_part_item_del(Evas_Object *obj, Part *part, Eina_Stringshare * item_name);

void
group_navigator_part_state_add(Evas_Object *obj, Part *part, State *state);

void
group_navigator_part_state_del(Evas_Object *obj, Part *part, State *state);

void
group_navigator_part_restack(Evas_Object *obj, Part *part, Part *rel_part);

void
group_navigator_part_item_restack(Evas_Object *obj,
                                  Part *part,
                                  Eina_Stringshare *part_item,
                                  Eina_Stringshare *relative_part_item);
#endif /* GROUP_NAVIGATOR_H */
