/**
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

#ifndef STYLE_DIALOG_H
#define STYLE_DIALOG_H

/**
 * @defgroup New_Style_Dialog New_Style_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for create new style and/or class.
 */

#include "eflete.h"
#include "widget_manager.h"
#include "widget_macro.h"
#include "notify.h"
#include "ui_widget_list.h"

/**
 * Add new dialog for input params style and/or class, like names of style and class.
 *
 * @param ap The App_Data struct pointer.
 * @return EINA_TRUE if a dialog was added, else EINA_FALSE.
 *
 * @ingroup New_Style_Dialog
 */
Eina_Bool
style_dialog_add(App_Data *ap);

#endif /* STYLE_DIALOG_H */
