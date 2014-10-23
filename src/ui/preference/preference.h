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

/**
 * @defgroup Preference Preference
 * @ingroup EFLETE
 *
 * View of Eflete config.
 */

#include "eflete.h"

enum _Preference_Item
{
   PREFERENCE_GENERAL = 0,
   PREFERENCE_WORKSPACE,
   PREFERENCE_LIVEVIEW,
   PREFERENCE_NONE
};
typedef enum _Preference_Item Preference_Item;

/**
 * Create a preference window. It window consist UI view of Eflete config.
 * Gives the opportunity to change the Eflete config.
 *
 * @param item The Preference_Item, it means what the frame need to show. To show
 * default view set the PREFERENCE_NONE.
 *
 * @return The Preference window object, or NULL overwise.
 *
 * @ingroup Preference
 */
Evas_Object *
preference_add(Preference_Item item);
