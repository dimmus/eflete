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

#ifndef HISTORY_H
#define HISTORY_H

#include <Eina.h>
#include <Evas.h>

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
 * @c history_diff_add, all changes, that stored after the current will be
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
 * @typedef Action
 * @ingroup History
 */
typedef enum _Action Action;

/**
 * @typedef Target
 * @ingroup History
 */
typedef enum _Target Target;

/**
 * @typedef Value_Type
 * @ingroup History
 */
typedef enum _Value_Type Value_Type;

/**
 * @enum _Action
 *
 * @brief All types of possible actions, that can be generated by changes.
 *
 * @ingroup History
 */
enum _Action
{
   ADD = 0, /**< Adding something new, like adding new state in part. */
   DEL,     /**< Deleting, like delete part from style. */
   MODIFY,  /**< Usaly it indicate that modifing attributes of part or state. */
   CONTAINER, /**< This action mean, that changed two params, whith using differents
                   function. For this case needed special logic to manage changes*/
   RESTACK, /**< This action needed for manage parts restack */
   LAST_ACTION
};

/**
 * @enum _Target
 *
 * @brief All types of modules, changes which can be processed by history module.
 *
 * @ingroup History
 */
enum _Target
{
   PROPERTY = 0, /**< This type of module generate changes, that modify attributes
                      of states, parts or style. For example min/max values for
                      state in part*/
   STATE_TARGET, /**< This type of change happens with state of part.
                      It can be add new state, or delete already exists state.*/
   PART_TARGET, /**< This type of change happens with part. It can be add new
                     part, or delete already exist, or restack.*/
   LAST_TARGET
};

/**
 * @enum _Value_Type
 *
 * @brief Possible types of values, that could be changed. This is say how to
 * processed input parameters into function, that will revert or restore changes.
 *
 * @ingroup History
 */
enum _Value_Type
{
   STRING = 0, /**< string values: char *, const char*, Eina_Stringshare * */
   VAL_DOUBLE, /**< only double values */
   VAL_INT, /**< for int, char, Eina_Bool types*/
   VAL_GROUP, /**< use in functions with int param, that describe change. This
             function usually look like edje_edit_group_max_set(Evas_Object*, int v)*/
   FOUR, /**< for functions like edje_edit_state_color_set. */
   RENAME, /**< Only for function edje_edit_part_name_set. */
   LAST_VALUE_TYPE
};

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

/**
 * Add module for create history of changes. All changes that will generate
 * with this source will stored in separete history. This function creates the
 * history if this source wasn't initiated before. If this module was initiated
 * early will returned EINA_TRUE.
 *
 * @param source The object with lifetime equale to lifetime of module, that
 *                will generate changes.
 *
 * @return EINA_TRUE if new history created and assigned to a given target or if
 *         source already initialised with this function, EINA_FALSE in otherwise.
 * @note Code example
   @verbatim
   Evas_Object *source = evas_object_rectangle_add(canvas);
   History *history = history_init();
   if(!history_module_add(source)) return;
   ... here add changes ...
   history_module_del(source);
   history_term(history);
   @endverbatim
 * @ingroup History
 */
Eina_Bool
history_module_add(Evas_Object *source);

/**
 * Delete history from given source, that was initialized by @c history_module_add.
 * Clear all entries of changes in history of given source. And delete source
 * from list of the watched modules.
 *
 * @param source Smart object, that present module.
 *
 * @return EINA_TRUE if history delete succesfull, or EINA_FALSE in otherwise.
 *
 * @see history_module_add
 *
 * @ingroup History
 */
Eina_Bool
history_module_del(Evas_Object *source);

/**
 *  Add new change into history module. Support all modules from @c Target
 *    enum.
 *
 * @param source Smart object, that represent module.
 * @param target Type of module where change happens.
 *
 * @return EINA_TRUE if new diff successfuly added into history, EINA_FALSE in
 *   otherwise.
 *
 * @brief  How to use this function with different types of modules showed below.
 *
 * In case when you want to add change from module with type PROPERTY.
 *
 *   For simple modify attribute:
 *
 *   history_diff_add(source, Target, MODIFY, Value_Type, VALUES([old, new],
 *                      [old_1, old_2, old_3, old_4, new_1, new_2, new_3, new_4]),
 *                      Style name, Func pointer, description, Part name,
 *                      State name, State value);
 *
 *  For example:
 *  <pre>
 *   ...
 *   History *history = history_init();
 *   if (!history_module_add(style->obj)) return;
 *    ...
 *   const char *old_value = part->name;
 *   value = elm_entry_entry_get(obj);
 *   if (!edje_edit_part_name_set(style->obj, part->name, value)) return;
 *   project_changed();
 *   part->name = value;
 *   style->isModify = true;
 *   history_diff_add(style->obj, PROPERTY, MODIFY, RENAME, old_value, value,
 *                    style->full_group_name, (void*)edje_edit_part_name_set,
 *                    "Rename", part->name, NULL, 0);
 *   </pre>
 *
 *   For ADD/DEL actions:
 *
 *   history_diff_add(source, Target, [ADD, DEL], Value_Type, new_value,
 *                    revert function pointer, Style name, Func pointer,
 *                    description, Part name, State name, State value);
 *
 *  Example of code shown case, when add new image into tween list:
 *  <pre>
 *   ...
 *   History *history = history_init();
 *   if (!history_module_add(style->obj)) return;
 *    ...
 *   if (edje_edit_state_tween_add(style->obj, part->name,
 *                                 part->curr_state,
 *                                 part->curr_state_value, name))
 *   {
 *      history_diff_add(style->obj, PROPERTY, ADD, STRING,
 *                       name, edje_edit_state_tween_del,
 *                       style->full_group_name,
 *                       (void*)edje_edit_state_tween_add, "tween image",
 *                       part->name, part->curr_state,
 *                       part->curr_state_value);
 *   }
 *  ...
 *   </pre>
 *
 *   For actions, that change params with highlight from workspace:
 *
 *   history_diff_add(source, Target, CONTAINER, Value_Type, old_value_1,
 *                    new_value_1, old_value_2, new_value_2
 *                    Func_1 pointer, Style name, Func_2 pointer,
 *                    description, Part name, State name, State value);
 *
 *  Example of code shown case, when add new image into tween list:
 *  <pre>
 *   ...
 *   History *history = history_init();
 *   if (!history_module_add(style->obj)) return;
 *    ...
 *   Eina_Bool result = false;
 *   old_value_1 = edje_edit_state_max_w_get(sd->style->obj, part->name,
 *                                           part->curr_state,
 *                                           part->curr_state_value);
 *   old_value_2 = edje_edit_state_max_h_get(sd->style->obj, part->name,
 *                                           part->curr_state,
 *                                           part->curr_state_value);
 *   result = edje_edit_state_max_w_set(sd->style->obj, part->name,
 *                                      part->curr_state, part->curr_state_value,
 *                                      events->w);
 *   result &= edje_edit_state_max_h_set(sd->style->obj, part->name,
 *                                       part->curr_state, part->curr_state_value,
 *                                       events->h);
 *  if (result)
 *    history_diff_add(style->obj, PROPERTY, CONTAINER, VAL_INT, old_value_1, events->w,
 *                     old_value_2, events->h, (void *)edje_edit_state_max_w_set,
 *                     style->full_group_name, (void *)edje_edit_state_max_h_set,
 *                     "max size", part->name, part->curr_state,
 *                     part->curr_state_value);
 *  ...
 *   </pre>
 * --------------------------------------------------------------------------
 * In case when you want to add change for module with type STATE_TARGET.
 *
 *   Case when new state is added:
 *
 *   history_diff_add(source, STATE_TARGET, ADD, full style name, part name,
 *                    state name, state value, description);
 *
 *   Case when state is deleted:
 *
 *   history_diff_add(source, STATE_TARGET, DEL, full style name, part name,
 *                    state name, state value, description);
 *
 * Example of code that shows case of adding new state:
 * <pre>
 *   ...
 *   History *history = history_init();
 *   if (!history_module_add(style->obj)) return;
 *    ...
 *   Eina_Bool ret = EINA_FALSE;
 *   ret = edje_edit_state_add(style->obj, "elm.text", "selected", 0.0);
 *   if (!ret) return;
 *   history_diff_add(style->obj, STATE_TARGET, ADD, "elm/button/base/default",
 *                    "elm.text", "selected", 0.0, "create state");
 *    ...
 * </pre>
 * --------------------------------------------------------------------------
 * In case when you want to add change for module with type PART_TARGET.
 *
 *   Case when new part is added:
 *
 *   history_diff_add(source, PART_TARGET, ADD, part name);
 *
 *   Case when part is deleted:
 *
 *   history_diff_add(source, PART_TARGET, DEL, part name);
 *
 *   Case when part is restacked:
 *
 *   history_diff_add(source, PART_TARGET, RESTACK, part name);
 *
 * @ingroup History
 */
Eina_Bool
history_diff_add(Evas_Object *source, Target target, ...);

/**
 * Make undo n-count of changes for given source. Source should be initialized
 * in history module with using @c history_module_add. In case when amount of
 * the changes that can be canceled is less than inputted value - all changes
 * will  be reverted.
 *
 * @param source The object, that have history of changes.
 * @param count Qantity of changes that will be reverted.
 *
 * @return EINA_TRUE if change(s) undo successfuly, EINA_FALSE in otherwise.
 *
 * @ingroup History
 */
Eina_Bool
history_undo(Evas_Object *source, int count);

/**
 * Make redo n-count of changes for given source. Source should be initialized
 * in history module with using @c history_module_add. In case when amount of
 * the changes that can be restored is less than inputted value - all changes
 * will be recovered.
 *
 * @param source The object, that have history of changes.
 * @param count Qantity of changes that will be restored.
 *
 * @return EINA_TRUE if change(s) undo successfuly, EINA_FALSE in otherwise.
 *
 * @ingroup History
 */
Eina_Bool
history_redo(Evas_Object *source, int count);

/**
 * If genlist for history submodule wasn't created, will be returned newly
 * created genlist. In case when genlist is already created it will be returned.
 *
 * @param history A History object, that was created wirt using @c history_init.
 * @param parent A Evas_Object, that can be a parent. This param can be NULL if
 *               genlist for history is already created.
 *
 * @return A const Evas_Object, that represent genlist from history module.
 *
 * @note returned object shoud be changed only in history module.
 *
 * @ingroup History.
 */
Evas_Object *
history_genlist_get(History *history, Evas_Object *parent);

/**
 * Set maximum count of the changes in the history of given module.
 * By default this value equal 0, it mean that depth of history is unlimited.
 *
 * @param source The object, that have history of changes.
 * @param count Maximum count of stored changes in the history. If 0 - size of
 *        the history is unlimited.
 *
 * @return EINA_TRUE if depth setted succesful , EINA_FALSE in otherwise.
 *
 * @ingroup History
 */
Eina_Bool
history_module_depth_set(Evas_Object *source, unsigned int depth);

/**
 * Get maximum depth value of the changes in the history of given module.
 *
 * @param source The object, that have history of changes.
 *
 * @return Maximum count of stored changes in the history. If 0 - size of
 *        the history is unlimited. If returned negative value something went wrong
 *
 * @ingroup History
 */
int
history_module_depth_get(Evas_Object *source);

#endif /* HISTORY_H */
