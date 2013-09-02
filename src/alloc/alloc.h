/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
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
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ALLOC_H
#define ALLOC_H

/**
 * @defgroup Alloc Alloc
 *
 * This header describes functions used to manage dynamic storage in a project
 * PACKAGE
 */

#include "efl_tet.h"

/**
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.
 *
 * @param size Size of the memory block, in bytes. size_t is an unsigned
 * integral type.
 * @return On success, a pointer to the memory block allocated by the function.
 * If the function failed to allocate the requested block of memory, application
 * will be terminated.
 *
 * @ingroup Alloc
 */
void *
mem_malloc(size_t size);

/**
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero.
 *
 * @param num Number of elements to allocate.
 * @param size Size of each element.
 * @return On success, a pointer to the memory block allocated by the function.
 * If the function failed to allocate the requested block of memory, application
 * will be terminated.
 *
 * @ingroup Alloc
 */
void *
mem_calloc(size_t num, size_t size);

#endif /* ALLOC_H */
