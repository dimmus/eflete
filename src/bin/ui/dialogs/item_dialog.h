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

#ifndef ITEM_DIALOG_H
#define ITEM_DIALOG_H

/**
 * @defgroup Item_Dialog Item_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for add new item into container part (TABLE, BOX).
 * This dialog provide ability to set name for new item and choise source group.
 * Choised source load into preview area.
 */

#include "eflete.h"
#include "common_macro.h"
#include "string_common.h"
#include "main_window.h"

/**
 * Add new dialog for input params of creating item: name and value.
 *
 * @param ap The App_Data struct pointer.
 * @return Pointer to popup object.
 *
 * @ingroup Item_Dialog
 */
Evas_Object *
item_dialog_add(App_Data *ap, Part *part);

#endif /* ITEM_DIALOG_H */
