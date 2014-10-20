/**
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

#include "utc_common.h"
#include <Elementary.h>
#include "edje_compile.h"

/**
 * @defgroup edje_compile_test
 */

void compile_callback_test_p(int);
void compile_callback_test_n(int);
void compile_test_p(int);
void compile_test_n1(int);
void compile_test_n2(int);
void compile_test_n3(int);

void decompile_callback_test_p(int);
void decompile_test_p(int);
void decompile_test_n1(int);
void decompile_test_n2(int);
