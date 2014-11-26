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

#ifndef HISTORY_PRIVATE_H
#define HISTORY_PRIVATE_H

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
 * @typedef Attribute_Diff
 * @ingroup History
 */
typedef struct _Attribute_Diff Attribute_Diff;

/**
 * @typedef State_Diff
 * @ingroup History
 */
typedef struct _State_Diff State_Diff;

/**
 * @typedef Part_Diff
 * @ingroup History
 */
typedef struct _Part_Diff Part_Diff;

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
   Elm_Object_Item *ui_item; /**< Genlist item, that provide discard all changes*/
   unsigned int depth; /**< Maximum count of stored changes */
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
   Eina_Stringshare *description; /**<Description, that will be shown on UI */
   Eina_Stringshare *new; /**< New value, that will be shown on UI */
   Eina_Stringshare *old; /**< Old value, that will be shown on UI */
   Eina_Stringshare *source; /**< Source name, that will be shown on UI */
   Eina_Stringshare *state; /**< State name, that will be shown on UI */

   Elm_Object_Item *ui_item; /**< This item uses for showing description of the change on ui */
   unsigned int index; /**< Position in changes, this field needed for
                            calculate delta beetwen changes*/
};

/* INTERNAL FUNCTIONS FOR HISTORY_ATTRIBUTE SUBMODULE */
/**
 * @defgroup History_Attribute History attributes
 * @ingroup History
 *
 * This submodule manage changes of the attributes. Parse changes with module
 * type PROPERTY. Manage undo/redo actions with that changes.
 *
 * @note This is private submodule for history module.
 *
 */
/**
 * Create right diff, from input data list. Parse input list of parameters and
 * fill internal structure Attribute_Diff.
 *
 * @param list The list of params. For detail description of this params
 *             please see history_diff_add.
 *
 * @return Pointer to filled structure, that casted to the Diff in successful
 *         case or NULL if something went wrong.
 *
 * @ingroup History_Attribute
 */
Diff *
_attribute_change_new(va_list list);

/**
 * Freed memory, that was allocated in attribute_change_new.
 *
 * @param change The diff, that was created with using _attribute_change_new.
 *
 * @ingroup History_Attribute
 */
void
_attribute_change_free(Attribute_Diff *change);

/**
 * This function provide functionality for merging given change with previous
 * diff. In case when the same attribute changed twice in successions this
 * diffs can be merged in single diff.
 * For example: renaming part 'elm' to 'efl'
 * <pre>
 * default flow:
 * |Action     | Current name | Diffs list                                     |
 * |---------------------------------------------------------------------------|
 * |delete 'm' |    "el"      | [RENAME 'el']                                  |
 * |delete 'l' |    "e"       | [RENAME 'el']->[RENAME 'e']                    |
 * |add    'f' |    "ef"      | [RENAME 'el']->[RENAME 'e']->[RENAME 'ef']     |
 * |add    'l' |    "efl"     | [RENAME 'el']->[RENAME 'e']->[RENAME 'ef']->   |
 * |           |              | ->[RENAME 'efl']                               |
 * |---------------------------------------------------------------------------|
 * For undo renaming this part need to call _attribute_undo 5 times.
 *
 * merge diffs flow:
 * |Action     | Current name | Diffs list                                     |
 * |---------------------------------------------------------------------------|
 * |delete 'm' |    "el"      | [RENAME 'el']                                  |
 * |delete 'l' |    "e"       | [RENAME 'e']                                   |
 * |add    'f' |    "ef"      | [RENAME 'ef']                                  |
 * |add    'l' |    "efl"     | [RENAME 'efl']                                 |
 * |---------------------------------------------------------------------------|
 * For undo renaming part with using merge need to call _attribute_undo one time.
 * </pre>
 * In case when previous diff contain info about change another attribute,
 * given attribute will not merge.
 * <pre>
 * |Action            |  Diffs list                                            |
 * |---------------------------------------------------------------------------|
 * |delete 'm'        |  [RENAME 'el']                                         |
 * |delete 'l'        |  [RENAME 'e']                                          |
 * |change 'visible'  |  [RENAME 'e']->[MODIFY 'visible']                      |
 * |add    'f'        |  [RENAME 'e']->[MODIFY 'visible']->[RENAME 'ef']       |
 * |---------------------------------------------------------------------------|
 * </pre>
 *
 * @param change The diff, that was created with using _attribute_change_new.
 * @param module The module, that contain given diff.
 *
 * @return If change can be merged with previous diff will returned NULL, because
 * given change is merged into previous, if merge is impossible will return pointer
 * to the given change.
 *
 * @note this function support merging only for diffs with action_type MODIFY.
 *
 * @ingroup History_Attribute
 */
Diff *
_attribute_change_merge(Attribute_Diff *change, Module *module);

/**
 * This function cancel given diff.
 *
 * @source The object, that present module and can be changed with functions,
 *  that saved in a @c Attribute_Diff structure.
 * @param change The diff, that was created with using _attribute_change_new.
 *
 * @return EINA_TRUE if diff canceled successful or EINA_FALSE in otherwise.
 *
 * @ingroup History_Attribute
 */
Eina_Bool
_attribute_undo(Evas_Object *source, Attribute_Diff *change);

/**
 * This function restore given diff.
 *
 * @param source The object, that present module and can be changed with
 * functions, that saved in a @c Attribute_Diff structure.
 * @param change The diff, that was created with using _attribute_change_new.
 *
 * @return EINA_TRUE if diff restored successful or EINA_FALSE in otherwise.
 *
 * @ingroup History_Attribute
 */
Eina_Bool
_attribute_redo(Evas_Object *source, Attribute_Diff *change);

/* INTERNAL FUNCTIONS FOR HISTORY_STATE SUBMODULE */
/**
 * @defgroup History_State History states
 * @ingroup History
 *
 * This submodule manage changes of the states of parts. Parse changes with module
 * type STATE_TARGET. Manage undo/redo actions with that changes.
 *
 * @note This is private submodule for history module.
 *
 */

/**
 * Create right diff, from input data list. Parse input list of parameters and
 * fill internal structure State_Diff.
 *
 * @param list The list of params. For detail description of this params
 *             please see history_diff_add.
 * @source The object, that represent module.
 *
 * @return Pointer to filled structure, that casted to the Diff in successful
 *         case or NULL if something went wrong.
 *
 * @ingroup History_State
 */
Diff *
_state_change_new(va_list list, Evas_Object *source);

/**
 * Freed memory, that was allocated in _state_change_new.
 *
 * @param change The diff, that was created with using _state_change_new.
 *
 * @ingroup History_State
 */
void
_state_change_free(State_Diff *change);

/**
 * This function cancel given diff.
 *
 * @source The object, that represent module.
 * @param change The diff, that was created with using _state_change_new.
 *
 * @return EINA_TRUE if diff canceled successful or EINA_FALSE in otherwise.
 *
 * @ingroup History_State
 */
Eina_Bool
_state_undo(Evas_Object *source, State_Diff *change);

/**
 * This function restore given diff.
 *
 * @source The object, that represent module.
 * @param change The diff, that was created with using _state_change_new.
 *
 * @return EINA_TRUE if diff canceled successful or EINA_FALSE in otherwise.
 *
 * @ingroup History_State
 */
Eina_Bool
_state_redo(Evas_Object *source, State_Diff *change);

/* INTERNAL FUNCTIONS FOR HISTORY_PART SUBMODULE */
/**
 * @defgroup History_Part History part
 * @ingroup History
 *
 * This submodule manage changes of the parts. Parse changes with module
 * type PART_TARGET. Manage undo/redo actions with that changes.
 *
 * @note This is private submodule for history module.
 *
 */

/**
 * Create right diff, from input data list. Parse input list of parameters and
 * fill internal structure Part_Diff.
 *
 * @param list The list of params. For detail description of this params
 *             please see history_diff_add.
 * @source The object, that represent module.
 *
 * @return Pointer to filled structure, that casted to the Diff in successful
 *         case or NULL if something went wrong.
 *
 * @ingroup History_Part
 */
Diff *
_part_change_new(va_list list, Evas_Object *source);

/**
 * Freed memory, that was allocated in _part_change_new.
 *
 * @param change The diff, that was created with using _part_change_new.
 *
 * @ingroup History_Part
 */
void
_part_change_free(Part_Diff *change);

/**
 * This function cancel given diff.
 *
 * @source The object, that represent module.
 * @param change The diff, that was created with using _part_change_new.
 *
 * @return EINA_TRUE if diff canceled successful or EINA_FALSE in otherwise.
 *
 * @ingroup History_Part
 */
Eina_Bool
_part_undo(Evas_Object *source, Part_Diff *change);

/**
 * This function restore given diff.
 *
 * @source The object, that represent module.
 * @param change The diff, that was created with using _part_change_new.
 *
 * @return EINA_TRUE if diff canceled successful or EINA_FALSE in otherwise.
 *
 * @ingroup History_Part
 */
Eina_Bool
_part_redo(Evas_Object *source, Part_Diff *change);

/* INTERNAL FUNCTIONS FOR HISTORY_UI SUBMODULE */
/**
 * @defgroup History_UI History ui module
 * @ingroup History
 *
 * This submodule manage UI view of history changes. Create main genlist,
 * manage styles for items (select item in "inactive mode", show color marks
 * for different types of actions). Manage user input on history genlist.
 *
 * @note This is private submodule for history module.
 *
 */

/**
 * Create main genlist and item classes, that needed for view changes on UI.
 *
 * @param parent The Evas_Object, that will be parent for genlis.
 *
 * @return Evas_Object, that represent genlist widget.
 *
 * @ingroup History_UI
 */
Evas_Object *
_history_ui_add(Evas_Object *parent);

/**
 * Create new genlist item, that showing description of the change in the history
 * genlist. This item provide functionality to manage changes with using ui.
 *
 * @param change The Diff that was created with using history_diff_add.
 * @param module The Module, that contain diff.
 *
 * @ingroup History_UI
 */
void
_history_ui_item_add(Diff *change, Module *module);

/**
 * Update current view of ui item, accordinly to the current state of change.
 * Is change active, or already canceled. What type of the change action this
 * diff present ADD, DEL or MODIFY.
 *
 * @param change The Diff that was created with using history_diff_add.
 * @param active If this diff already canceled EINA_FALSE, or EINA_TRUE in otherwise.
 * @param current If this diff shoud be selected EINA_TRUE, or EINA_FALSE in therwise.
 *
 * @ingroup History_UI
 */
void
_history_ui_item_update(Diff *change, Eina_Bool active, Eina_Bool current);

/**
 * Reload all items in the history genlist, accordingly to the given module.
 *
 * @param history A History object, that was created wirt using @c history_init.
 * @param module The Module, that needed to reload diffs view.
 *
 * @ingroup History_UI
 */
void
_history_ui_list_reload(History *history, Module *module);

/**
 * Create new genlist item, that provide discard all changes from history
 * of the given module. This item always first in genlist.
 *
 * @param module The module, that will manage by this item.
 * @param module The History structure.
 *
 * @ingroup History_UI
 */
void
_history_module_ui_item_add(History *history, Module *module);
#endif /* HISTORY_PRIVATE_H */
