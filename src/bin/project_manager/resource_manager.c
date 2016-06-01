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

#include "resource_manager.h"

int
resource_cmp(Resource *res1, Resource *res2)
{
   return strcmp(res1->name, res2->name);
}

void *
pm_resource_get(Eina_List *list, Eina_Stringshare *name)
{
   Resource res;
   res.name = name;
   return eina_list_search_sorted(list, (Eina_Compare_Cb)resource_cmp, &res);
}

void *
pm_resource_unsorted_get(Eina_List *list, Eina_Stringshare *name)
{
   Resource res;
   res.name = name;
   return eina_list_search_unsorted(list, (Eina_Compare_Cb)resource_cmp, &res);
}

Eina_Bool
pm_resource_usage_add(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_get(list, name);

   if (!res)
      return false;

   res->used_in = eina_list_sorted_insert(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);
   return true;
}

Eina_Bool
pm_resource_usage_unsorted_add(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_unsorted_get(list, name);

   if (!res)
      return false;

   res->used_in = eina_list_sorted_insert(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);
   return true;
}

Eina_List *
pm_resource_del(Eina_List *list, void *res)
{
   assert(res != NULL);
   assert(list != NULL);
   list = eina_list_remove(list, res);
   return list;
}

void
pm_resource_usage_del(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_get(list, name);
   Eina_List *l_del;

   assert(res != NULL);

   l_del = eina_list_search_sorted_list(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);

   TODO("remove this after fixing resource managment");
   if (!l_del)
     {
        ERR("Can't delete resource \"%s\"", name);
        return;
     }
   assert(l_del);

   res->used_in = eina_list_remove_list(res->used_in, l_del);
}

void
pm_resource_usage_unsorted_del(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_unsorted_get(list, name);
   Eina_List *l_del;

   assert(res != NULL);

   l_del = eina_list_search_sorted_list(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);

   TODO("remove this after fixing resource managment");
   if (!l_del)
     {
        ERR("Can't delete resource \"%s\"", name);
        return;
     }
   assert(l_del);

   res->used_in = eina_list_remove_list(res->used_in, l_del);
}
