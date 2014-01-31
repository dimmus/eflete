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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "eflete.h"
#include "ui_main_window.h"

#define CHECK_AP(RET) \
if (!ap) \
  { \
     ERR("Application data structure does'nt exist"); \
     return RET; \
  }

App_Data *ap = NULL;

Evas_Object *
win_layout_get(void)
{
   if ((ap) && (ap->win_layout))
     return ap->win_layout;
   else
     return NULL;
}

App_Data *
app_create(void)
{
   if (!ap)
     ap = mem_calloc(1, sizeof (App_Data));
   return ap;
}

Evas_Object *
main_window_get(void)
{
   CHECK_AP(NULL)
   return ap->win;
}

Evas_Object *
colorselector_get(void)
{
   CHECK_AP(NULL)
   if (!ap->colorsel) ap->colorsel = colorselector_add(ap->win);
   return ap->colorsel;
}

void
app_free(App_Data *ap)
{
   /*TODO: here need delete all created objects from ap! */
   if (ap) free(ap);
}

Eina_Bool
app_init()
{
   if (!eina_init())
     {
        CRIT("Can't initialize the Eina library");
        return false;
     }

   if (!efreet_init())
     {
        CRIT("Can't initialize the Efreet system");
        return false;
     }

   if (!ecore_init())
     {
        CRIT("Can't initialize the Ecore library");
        return false;
     }

   if (!edje_init())
     {
        CRIT("Can't initialize the Edje Library");
        return false;
     }

   if (!logger_init())
     {
        CRIT("Can't initialize the logger library");
        return false;
     }

   if (!ecore_evas_init())
     {
        CRIT("Can't initialize the Ecore_Evas system");
        return false;
     }

   elm_theme_extension_add(NULL, TET_THEME);
   return true;
}

void
app_shutdown()
{
   app_free(ap);
   elm_theme_extension_del(NULL, TET_THEME);
   eina_shutdown();
   efreet_shutdown();
   ecore_shutdown();
   edje_shutdown();
   logger_shutdown();
   ecore_evas_shutdown();
}

#undef CHECK_AP
