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

#include "ewe_private.h"
#include "ewe_main.h"

EAPI int
ewe_init(int argc, char **argv)
{
   elm_theme_extension_add(NULL, EWE_THEME);
   return elm_init(argc, argv);
}

EAPI int
ewe_shutdown(void)
{
   elm_theme_extension_del(NULL, EWE_THEME);
   return elm_shutdown();
}
