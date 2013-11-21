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
* along with this program; If not, see .
*/

#ifndef UI_POPUP_HEADER_H
#define UI_POPUP_HEADER_H

/**
 * @defgroup Popup Popup
 * @ingroup Workspace
 *
 * Popup module provide some method to manipulate popup object.
 */
#include "logger.h"
#include "Elementary.h"
#include "ui_workspace.h"
#include "ui_groupspace.h"
#include "efl_ete.h"
#include "ui_highlight.h"


/**
 * Show popup object on workspace.
 *
 * @param ws  An workspace structure pointer. Needed to create popup menu on it
 *
 * @ingroup Popup
 */
void
ui_popup_show (Workspace *ws);
#endif /* UI_POPUP_HEADER_H */
