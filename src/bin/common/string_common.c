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

#include <string.h>
#include <Eina.h>
#include <assert.h>

int
sort_cb(const void *data1, const void *data2)
{
   const char *str1 = data1;
   const char *str2 = data2;

   if (!str1) return 1;
   if (!str2) return -1;

   return strcmp(str1, str2);
}

void
state_name_split(Eina_Stringshare *name, Eina_Stringshare **name_out, double *val_out)
{
   char **state_split;

   assert(name != NULL);
   assert(name_out != NULL || val_out != NULL);

   state_split = eina_str_split(name, " ", 2);
   if (name_out) *name_out = eina_stringshare_add(state_split[0]);
   if (val_out) *val_out = atof(state_split[1]);
   free(state_split[0]);
   free(state_split);
}

void
string_char_replace(char *str, char src, char rep)
{
   assert(str != NULL);

   while (*str != '\0')
     {
        if (*str == src) *str = rep;
        str++;
     }
}

const char *
widget_name_get(const Eina_Stringshare *group_name)
{
    int len = strlen(group_name);
    int i;
    char str[32];

    if (group_name[0] != 'e') return NULL;
    if (group_name[1] != 'l') return NULL;
    if (group_name[2] != 'm') return NULL;
    if (group_name[3] != '/') return NULL;

    for (i = 4; i < len; i++)
    {
        if (group_name[i] == '/') break;
        str[i - 4] = group_name[i];
    }
    str[i - 4] = '\0';

    return strdup(str);
}
