/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
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
* along with this program; If not, see .
*/

#define GET_NAME_FROM_PATH(name, path) \
   char **arr; \
   int i; \
   arr = eina_str_split(path, "/", 0); \
   for (i = 0; arr[i]; i++) ; \
   name = strdup(arr[i-1]); \
   free(arr[0]); \
   free(arr);
