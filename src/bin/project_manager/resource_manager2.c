/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "resource_manager2.h"

Eina_Bool
resource_manager_init(Project *project __UNUSED__)
{
   return false;
}

Eina_Bool
resource_manager_shutdown(Project *project __UNUSED__)
{
   return false;
}

Resource *
resource_manager_find(const Eina_List *list __UNUSED__, Eina_Stringshare *name __UNUSED__)
{
   return NULL;
}

Resource *
resource_manager_v_find(const Eina_List *list __UNUSED__, Eina_Stringshare *name __UNUSED__, double value __UNUSED__)
{
   return NULL;
}
