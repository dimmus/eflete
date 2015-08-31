/*
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

#ifndef SIGNALS_H
#define SIGNALS_H

/**
 * emited when tab changed.
 * eventinfo - pointer to Group structure. Can be NULL
 *
 * @ingroup Window
 */
#define SIGNAL_TAB_CHANGED "SIGNAL_TAB_CHANGED"

/**
 * emited when part is selected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_SELECTED "SIGNAL_PART_SELECTED"

/**
 * emited when part is unselected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_UNSELECTED "SIGNAL_PART_UNSELECTED"

/**
 * emited when part state changed.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_STATE_SELECTED "SIGNAL_PART_STATE_SELECTED"

/**
 * emited when part item is selected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_ITEM_SELECTED "SIGNAL_PART_ITEM_SELECTED"

/**
 * emited when part item is unselected.
 * eventinfo - pointer to Part_ structure
 *
 * @ingroup Window
 */
#define SIGNAL_PART_ITEM_UNSELECTED "SIGNAL_PART_ITEM_UNSELECTED"

/**
 * emited when any attribute is changed in properties.
 * eventinfo - NULL
 *
 * @ingroup Window
 */
#define SIGNAL_PROPERTY_ATTRIBUTE_CHANGED "SIGNAL_PROPERTY_ATTRIBUTE_CHANGED"

#endif /* SIGNALS_H*/
