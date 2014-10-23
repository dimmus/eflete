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

#ifndef COMPILE_DIALOG_H
#define COMPILE_DIALOG_H

#include "eflete.h"
#include "main_window.h"

/**
 * Create and show a dialog for compiling an edc file.
 *
 * @param ap The Apllication data.
 * @return EINA_TRUE if dialog executed, else EINA_FALSE.
 *
 * @ingroup OpenFileDialog
 */
Eina_Bool
compile_dialog(App_Data *ap);

#endif /* COMPILE_DIALOG_H */
