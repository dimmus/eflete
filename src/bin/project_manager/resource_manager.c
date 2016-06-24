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

Resource *
resource_add(const char *name, Resource_Type type)
{
   Resource *res = NULL;

   assert(name != NULL);

   switch (type)
     {
      case RESOURCE_TYPE_NONE:
         break;
      case RESOURCE_TYPE_GROUP:
         res = mem_calloc(1, sizeof(Group));
         break;
      case RESOURCE_TYPE_PART:
         res = mem_calloc(1, sizeof(Part));
         break;
      case RESOURCE_TYPE_STATE:
         res = mem_calloc(1, sizeof(State));
         break;
      case RESOURCE_TYPE_PROGRAM:
         res = mem_calloc(1, sizeof(Program));
         break;
      case RESOURCE_TYPE_ITEM:
         res = mem_calloc(1, sizeof(Part_Item));
         break;
      case RESOURCE_TYPE_DATA:
         res = mem_calloc(1, sizeof(Resource));
         break;
      case RESOURCE_TYPE_IMAGE:
         res = mem_calloc(1, sizeof(External_Resource));
         break;
      case RESOURCE_TYPE_SOUND:
         res = mem_calloc(1, sizeof(External_Resource));
         break;
      case RESOURCE_TYPE_FONT:
         res = mem_calloc(1, sizeof(External_Resource));
         break;
      case RESOURCE_TYPE_TONE:
         res = mem_calloc(1, sizeof(Tone_Resource));
         break;
      case RESOURCE_TYPE_STYLE:
         res = mem_calloc(1, sizeof(Resource));
         break;
      case RESOURCE_TYPE_TAG:
         res = mem_calloc(1, sizeof(Resource));
         break;
      case RESOURCE_TYPE_COLORCLASS:
         res = mem_calloc(1, sizeof(Colorclass_Resource));
         break;
     }
   assert(res != NULL);

   res->resource_type = type;
   res->name = eina_stringshare_add(name);

   return res;
}

void
resource_free(Resource *res)
{
   assert(res != NULL);

   eina_stringshare_del(res->name);
   free(res);
}

static int
resource_cmp(Resource *res1, Resource *res2)
{
   int cmp;

   if (res1->resource_type != res2->resource_type)
     return res1->resource_type - res2->resource_type;

   /* stringshares */
   if (res1->name == res2->name)
     cmp = 0;
   else
     cmp = strcmp(res1->name, res2->name);

   if (!cmp && (res1->resource_type == RESOURCE_TYPE_STATE))
     {
        if (((State *)res1)->val > ((State *)res2)->val)
          return 1;
        if (((State *)res1)->val < ((State *)res2)->val)
          return -1;
        return 0;
     }

   return cmp;
}

Resource *
resource_get(const Eina_List *list, const Resource *request)
{
   Resource *ret = NULL;

   assert(request != NULL);
   assert(request->resource_type != RESOURCE_TYPE_NONE);

   switch (request->resource_type)
     {
      case RESOURCE_TYPE_GROUP:
      case RESOURCE_TYPE_STATE:
      case RESOURCE_TYPE_PROGRAM:
      case RESOURCE_TYPE_DATA:
      case RESOURCE_TYPE_IMAGE:
      case RESOURCE_TYPE_SOUND:
      case RESOURCE_TYPE_FONT:
      case RESOURCE_TYPE_TONE:
      case RESOURCE_TYPE_STYLE:
      case RESOURCE_TYPE_TAG:
      case RESOURCE_TYPE_COLORCLASS:
         ret = eina_list_search_sorted(list, (Eina_Compare_Cb)resource_cmp, request);
         break;
      case RESOURCE_TYPE_PART:
      case RESOURCE_TYPE_ITEM:
         ret = eina_list_search_unsorted(list, (Eina_Compare_Cb)resource_cmp, request);
         break;
      case RESOURCE_TYPE_NONE:
         break;
     }
   return ret;
}

void
resource_insert(Eina_List **list, const Resource *res)
{
   assert(list != NULL);
   assert(res != NULL);
   assert(res->resource_type != RESOURCE_TYPE_NONE);

   switch (res->resource_type)
   {
      case RESOURCE_TYPE_GROUP:
      case RESOURCE_TYPE_STATE:
      case RESOURCE_TYPE_PROGRAM:
      case RESOURCE_TYPE_DATA:
      case RESOURCE_TYPE_IMAGE:
      case RESOURCE_TYPE_SOUND:
      case RESOURCE_TYPE_FONT:
      case RESOURCE_TYPE_TONE:
      case RESOURCE_TYPE_STYLE:
      case RESOURCE_TYPE_TAG:
      case RESOURCE_TYPE_COLORCLASS:
         *list = eina_list_sorted_insert(*list, (Eina_Compare_Cb)resource_cmp, res);
         break;
      case RESOURCE_TYPE_PART:
      case RESOURCE_TYPE_ITEM:
         *list = eina_list_append(*list, res);
         break;
      case RESOURCE_TYPE_NONE:
         break;
   }
}

Resource *
resource_remove(Eina_List **list, const Resource *res)
{
   Eina_List *l_del = NULL;
   Resource *ret;

   assert(list != NULL);
   assert(res != NULL);
   assert(res->resource_type != RESOURCE_TYPE_NONE);

   switch (res->resource_type)
   {
      case RESOURCE_TYPE_GROUP:
      case RESOURCE_TYPE_STATE:
      case RESOURCE_TYPE_PROGRAM:
      case RESOURCE_TYPE_DATA:
      case RESOURCE_TYPE_IMAGE:
      case RESOURCE_TYPE_SOUND:
      case RESOURCE_TYPE_FONT:
      case RESOURCE_TYPE_TONE:
      case RESOURCE_TYPE_STYLE:
      case RESOURCE_TYPE_TAG:
      case RESOURCE_TYPE_COLORCLASS:
         l_del = eina_list_search_sorted_list(*list, (Eina_Compare_Cb)resource_cmp, res);
         break;
      case RESOURCE_TYPE_PART:
      case RESOURCE_TYPE_ITEM:
         l_del = eina_list_search_unsorted_list(*list, (Eina_Compare_Cb)resource_cmp, res);
         break;
      case RESOURCE_TYPE_NONE:
         break;
   }
   assert(l_del != NULL);

   ret = eina_list_data_get(l_del);
   *list = eina_list_remove_list(*list, l_del);
   return ret;
}

void
resource_insert_after(Eina_List **list, const Resource *after, const Resource *res)
{
   assert(list != NULL);
   assert(res != NULL);
   assert(res->resource_type == RESOURCE_TYPE_PART);

   *list = eina_list_append_relative(*list, res, after);
}

void
resource_used_in_add(Resource *res, const Resource *used_in)
{
   if (!res) return;
   if (!used_in) return;

   res->used_in = eina_list_sorted_insert(res->used_in, (Eina_Compare_Cb)resource_cmp, used_in);
}

void
resource_used_in_del(Resource *res, const Resource *used_in)
{
   Eina_List *l_del;

   assert(res != NULL);
   assert(used_in != NULL);

   l_del = eina_list_search_sorted_list(res->used_in, (Eina_Compare_Cb)resource_cmp, used_in);

   TODO("replace with assert when resource manager will be ready");
   if (l_del == NULL)
     return;

   res->used_in = eina_list_remove_list(res->used_in, l_del);
}

Eina_Bool
resource_used_in_get(Resource *res, const Resource *used_in)
{
   assert(res != NULL);
   assert(used_in != NULL);

   return !!eina_list_search_sorted(res->used_in, (Eina_Compare_Cb)resource_cmp, used_in);
}
