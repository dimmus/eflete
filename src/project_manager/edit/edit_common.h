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

/**
 *  Getting first object from project. Needed to access top-level blocks
 */
#define GET_OBJ(project, edje_object) \
   Eina_Inlist *__styles, *__groups, *__widgets = NULL; \
   Widget *__widget; \
   Style *__style; \
   Group *__group; \
   __widgets = project->widgets; \
   if (!__widgets) edje_object = NULL; \
   else\
     { \
         __widget = EINA_INLIST_CONTAINER_GET(__widgets, Widget); \
         __styles = __widget->styles; \
         if (!__styles) edje_object = NULL; \
         else \
           { \
               __style = EINA_INLIST_CONTAINER_GET(__styles, Style); \
               __groups = __style->groups; \
               if (!__groups) edje_object = NULL; \
               else\
                 { \
                     __group = EINA_INLIST_CONTAINER_GET(__groups, Group); \
                     edje_object = __group->obj; \
                 } \
           } \
     }
