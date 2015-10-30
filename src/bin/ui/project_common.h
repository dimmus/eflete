/*
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

#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H

Eina_Bool
exist_permission_check(const char *path, const char *name, const char *title, const char *msg);

Eina_Bool
progress_print(void *data, Eina_Stringshare *progress_string);

void
progress_end(void *data, PM_Project_Result result);

#endif /* PROJECT_COMMON_H */
