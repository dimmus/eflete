/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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

#ifndef EWE_MAIN_H
#define EWE_MAIN_H

/**
 * Initialize Elementary Widget Extensions library
 *
 * @param argc System's argument count value
 * @param argv System's pointer to array of argument strings
 * @return The init counter value.
 *
 * This function initializes Ewe and increments a counter of
 * the number of calls to it. It returns the new counter's value.
 *
 * @see elm_shutdown().
 */
EAPI int       ewe_init(int argc, char **argv);

/**
 * Shut down Elementary Widget Extensions library
 *
 * @return The init counter value.
 *
 * This should be called at the end of your application, just
 * before it ceases to do any more processing. This will clean up
 * any permanent resources your application may have allocated via
 * Ewe that would otherwise persist.
 *
 * @see elm_init()
 */
EAPI int       ewe_shutdown(void);

#endif /* EWE_MAIN_H */
