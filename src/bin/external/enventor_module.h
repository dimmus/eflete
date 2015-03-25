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

#ifndef ENVENTOR_MODULE_H
#define ENVENTOR_MODULE_H

#include "eflete.h"

/**
 * @defgroup Enventor Enventor library support
 *
 * Function of this submodule provide ability to use enventor object
 * in eflete.
 */

/**
 * Function initialize enventor object. Register callbacks for
 * manipulate with enventor object.
 *
 * @param parent The evas object.
 *
 * @return Pointer to created enventor object if successful, or NULL
 * in otherwise.
 *
 * @insgroup Enventor
 */
Evas_Object *
enventor_object_init(Evas_Object *parent);

#endif /* ENVENTOR_MODULE_H */
