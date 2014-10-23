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
 * @defgroup History History module
 * @ingroup Eflete
 *
 * This module provides access to the functionality of the control changes.
 * Support for changes in the attributes of individual parts, the creation and
 * Remove the part states, and create history of changes in parts (create /
 * delete / move in the stack).
 *
 * History of changes assigned to individual modules, it allows to store
 * several stories for the various modules.
 * <pre>
 *  -----------   -----------   -----------        -----------
 *  |         |   | change1 |   | change2 |        | changeN |
 *  | Module1 |-> | (attr)  |-> | (part)  | ->..-> | (state) |
 *  |         |   -----------   -----------        -----------
 *  -----------
 *       /\
 *       |
 *       |
 *       |
 *  -----------      -----------    -----------   -----------        -----------
 *  |         |      |         |    | change1 |   | change2 |        | changeN |
 *  | History |-->   | Module2 | -> | (attr)  |-> | (part)  | ->..-> | (state) |
 *  |         |      |         |    -----------   -----------        -----------
 *  -----------      ----------
 *       |
 *       |
 *       \/
 *  -----------   -----------   -----------        -----------
 *  |         |   | change1 |   | change2 |        | changeN |
 *  | ModuleN |-> | (attr)  |-> | (part)  | ->..-> | (state) |
 *  |         |   -----------   -----------        -----------
 *  -----------
 * </pre>
 * The new changes registered in History module using @c history_change_add.
 *
 * All changes to one module register in general stack of events which is,
 * organized within @c Module.
 *
 * Manager of the stack of modules ensures that for each type of change was
 * caused by the corresponding functional. Manager parses the new changes by
 * source type, and then transmits the data on changes in specific submodule
 * (@c рistory_part, @c history_state, @c history_attribute ), where the
 * structuring of the data according to their content.
 *
 * Move to change history is organized with the help of @c undo and @c redo.
 * When you rollback changes with @c undo, history of the canceled changes
 * still stored in module history. Just moves the pointer to the current event
 * in history. In a situation where a pointer to the current change does not
 * indicate the end of the list and there is a request to save the new changes
 * @c history_change_add, all changes, that stored after the current will be
 * lost. And the new change will be added immediately after the current one.
 * A pointer to the current change moves to new change.
 *
 * As an example, it added three different types of changes in the module history.
 * History of changes is as follows:
 * '*' - The current change.
 *
 * <pre>                                  *
 *  -----------   -----------    -----------
 * | change1 |   | change2 |    | change3 |
 * | (attr)  |-> | (part)  | -> | (state) |
 * -----------   -----------    -----------
 * </pre>
 * When changes is canceled with @c undo, history will look like this:
 *
 * <pre>                  *
 * -----------   -----------    -----------
 * | change1 |   | change2 |    | change3 |
 * | (attr)  |-> | (part)  | -> | (state) |
 * -----------   -----------    -----------
 * </pre>
 * Now add a new change to the type of N4 attr:
 *
 * <pre>                              [deleted]      *
 * -----------   -----------    -----------   -----------
 * | change1 |   | change2 |    | change3 |   | change4 |
 * | (attr)  |-> | (part)  |    | (state) |   | (attr)  |
 * -----------   -----------    -----------   -----------
 *                  |                              /\
 *                  |------------------------------|
 * </pre>
 * The diagram illustrates the case when 3rd update is lost and rollback of
 * this change is impossible.
 *
 */

/**
 * @typedef History
 * @ingroup History
 */
typedef struct _History History;

/**
 * Clear all changes from all modules, that was registered in history module.
 * And unregister all modules.
 *
 * @param history The main history object.
 *
 * @return EINA_TRUE if history cleared succesfull, EINA_FALSE in otherwise.
 *
 * @ingroup History
 */
Eina_Bool
history_clear(History *history);

/**
 * Initialize history module. Create general history structure, where can be
 * added histories of different targets.
 *
 * @return A generated History structure if succesful, or NULL in otherwise.
 *
 * @ingroup History
 */
History *
history_init(void);

/**
 * Terminate history module. Stop watching for all modules, that was registered
 * with using @c undoredo_target_add. Clear histories of this targets and
 * free general history structures.
 *
 * @param history The structure, that store all changes.
 *
 * @return EINA_TRUE if module terminated succesfull, EINA_FALSE in otherwise.
 *
 * @ingroup History
 */
Eina_Bool
history_term(History *history);

