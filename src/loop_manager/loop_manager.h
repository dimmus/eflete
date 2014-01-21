/* Edje Theme Editor
* Copyright (C) 2014 Samsung Electronics.
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

#ifndef LOOP_MANAGER_H
#define LOOP_MANAGER_H

/**
 * @defgroup LoopManager Loop Manager
 *
 * Aplication loops control module. Wrappers for ecore loops functions with loops
 * stack and destructors.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Ecore.h>
#include "common_macro.h"
#include "alloc.h"

/**
 * @typedef Loop_Exit_Cb
 * @ingroup LoopManager
 * Loop exit callback. Returns true if loop can be quited, false otherwise.
 *
 * First param is "force" flag. If it is set return value will be ignored and loop
 * will be quited anyway.
 * Second param is optional data bypassed from loop_begin
 */
typedef Eina_Bool (*Loop_Exit_Cb) (Eina_Bool, void *);

/**
 * Start a new loop and adds its exit callback to loops stack.
 *
 * @param loop_exit_cb Exit loop callback. NULL value means that loop requires no
 * specific checks or action before exit.
 *
 * @param data Additional context data that will be passed to calback.
 * @ingroup LoopManager
 */
void loop_begin(Loop_Exit_Cb loop_exit_cb, void *data);

/**
 * Calls last exit callback from loops stack. If it returns true, loop will be
 * quited and callback will be deleted from stack.
 *
 * @param force If force flag is set return value will be ignored and loop will be
 * quited anyway.
 *
 * @return true if loop successfully ended, false otherwise.
 * @ingroup LoopManager
 */
Eina_Bool loop_quit(Eina_Bool force);

/**
 * Quits all loops in stack.
 *
 * @param force If force param isn't set and some quit callback
 * returns false function will be stoped.
 *
 * @return true if loop successfully ended, false otherwise.
 * @ingroup LoopManager
 */
Eina_Bool loop_quit_all(Eina_Bool force);

#endif
