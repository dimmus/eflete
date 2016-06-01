/**
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "group_manager.h"
#include "resource_manager.h"
#include "utc_common.h"
#include "config.h"
#include "history.h"
#include "change.h"
#include "diff.h"

void history_add_test_p(int);
void history_del_test_p1(int);
void history_del_test_p2(int);
void history_change_add_test_p(int);
void history_undo_redo_test_p(int);
