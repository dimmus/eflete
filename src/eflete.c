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

#include "eflete.h"
#include "main_window.h"
#include "shortcuts.h"

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
app_data_get(void)
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

Eina_Bool
app_free()
{
   CHECK_AP(false)
   /*TODO: here need delete all created objects from ap! */
   free(ap);
   return true;
}

Eina_Bool
app_init()
{
   Eina_Stringshare *config_path, *swap_path;

   setlocale(LC_ALL, "");
#ifdef ENABLE_NLS
   bindtextdomain(PACKAGE, LOCALE_DIR);
   textdomain(PACKAGE);
#endif /* set locale */

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

   config_path = eina_stringshare_add(EFLETE_SETT_PATH);
   if (!ecore_file_exists(config_path)) ecore_file_mkdir(config_path);
   swap_path = eina_stringshare_add(EFLETE_SWAP_PATH);
   if (!ecore_file_exists(swap_path)) ecore_file_mkdir(swap_path);

   if (!config_init()) return false;

   if (!ewe_init(0, 0))
     {
        CRIT("Can't initialize the Ewe library");
        return false;
     }

   elm_theme_extension_add(NULL, EFLETE_THEME);

   eina_stringshare_del(config_path);
   eina_stringshare_del(swap_path);
   return true;
}

Eina_Bool
app_shutdown()
{
   config_shutdown(ap);
   elm_theme_extension_del(NULL, EFLETE_THEME);
   eina_shutdown();
   efreet_shutdown();
   ecore_shutdown();
   edje_shutdown();
   logger_shutdown();
   ewe_shutdown();
   if (!app_free())
     {
        CRIT("Can't free application data.");
        return false;
     }
   return true;
}

#undef CHECK_AP
