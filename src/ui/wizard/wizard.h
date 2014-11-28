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

#ifndef WIZARD_H
#define WIZARD_H

/**
 * @defgroup Wzard Wizard
 * @ingroup EFLETE
 *
 * This module provide the wizards for a create new project, import edj or edc
 * file to Eflete as new project.
 *
 */

#include "eflete.h"
#include "modal_window.h"

/**
 * Create a wizard for import edj file to Eflete as new project.
 *
 * @param ap The App_Data of Eflete.
 *
 * @return If successful the wizard object, overwise NULL;
 *
 * @ingroup Wizard
 */
Evas_Object *
wizard_import_edj_add(App_Data *ap) EINA_ARG_NONNULL(1);

/**
 * Create a wizard for import edc file to Eflete as new project.
 *
 * @param ap The App_Data of Eflete.
 *
 * @return If successful the wizard object, overwise NULL;
 *
 * @ingroup Wizard
 */
Evas_Object *
wizard_import_edc_add(App_Data *ap) EINA_ARG_NONNULL(1);

#endif /* WIZARD_H */
