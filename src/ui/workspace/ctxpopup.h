/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#ifndef CTXPOPUP_H
#define CTXPOPUP_H

/**
 * @defgroup UI_Ctxpopup UI Ctxpopup
 * @ingroup Workspace
 *
 * Ctxpopup module provide some method to manipulate popup object.
 */
#include "Elementary.h"
#include "ui_workspace.h"

/**
 * Show ctxpopup object on parent. Ctxpopup will be deleted automatically
 * after the object be hide.
 *
 * @param obj A parent object.
 *
 * @ingroup UI_Ctxpopup
 */
Evas_Object *
ui_popup_add(Evas_Object *obj);

#endif /* CTXPOPUP_H */
