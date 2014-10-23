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

#ifndef UNDOREDO_PRIVATE_H
#define UNDOREDO_PRIVATE_H

#include "eflete.h"
#include "main_window.h"

#define HISTORY_MODULE_KEY "history_module_key"

/**
 * @typedef Module
 * @ingroup History
 */
typedef struct _Module Module;

/**
 * @typedef Diff
 * @ingroup History
 */
typedef struct _Diff Diff;

/**
 * @struct _History
 *
 * @brief This struct designed to storage list of modules, that can generatre
 * changes.
 * Also here present main UI control for history feature.
 *
 * @ingroup History
 */
struct _History
{
   Eina_List *modules; /**< List of registered modules. */
   Evas_Object *genlist; /**< Genlist, that provide visual navigation in history. */
   Ecore_Timer *timer; /**< Using for merge the changes, folowing one by one
                            and changes the same parameter in the same module. */
};

/**
 * @struct _Module
 *
 * @brief This struct contain list of changes for one module.
 *
 * @ingroup History
 */
struct _Module
{
   Evas_Object *target; /**< Target object. Can be any Evas_Object with lifetime
                             equale to lifetime of module*/
   Eina_List *changes; /**< List of structures, that contain diff info about change */
   Diff *current_change; /**< Current diff for module.*/
};

/**
 * @struct _Diff
 *
 * @brief This structure is common to all types of changes (i.e. Attributes,
 * States, Parts). The information stored in this structure identifies changes
 * to the membership of a particular type of module. Data about the action of a
 * change allow properly handle undo/redo.
 *
 * @ingroup History
 */
struct _Diff
{
   Target module_type; /**< Type of module, that produced this change*/
   Action action_type; /**< Type of change. Adding something new, or deleting, etc.. */
};

#endif /* UNDOREDO_PRIVATE_H */
