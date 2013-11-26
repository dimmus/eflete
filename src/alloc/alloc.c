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

#include "alloc.h"

#define TERM_MESSAGE "Could not allocate memory! Maybe not enough a memory. Application will be terminated!"

void *
mem_malloc(size_t size)
{
   void *mem_block;
   mem_block = malloc(size);
   if (!mem_block)
     {
        CRIT(TERM_MESSAGE);
        app_shutdown();
     }
   return mem_block;
}

void *
mem_calloc(size_t num, size_t size)
{
   void *mem_block;
   mem_block = calloc(num, size);
   if (!mem_block)
     {
        CRIT(TERM_MESSAGE);
        app_shutdown();
     }
   return mem_block;
}
