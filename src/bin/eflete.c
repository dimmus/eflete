/*
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
#include "config.h"
#include "syntax_color.h"

#define EXT ""
#ifdef _WIN32
#undef EXT
#define EXT ".exe"
#include "win32.h"
#endif

App_Data ap;

Eina_Bool
app_init()
{
   Eina_Stringshare *conf_path;
   char buf[PATH_MAX];
#ifdef ENABLE_NLS
   setlocale(LC_ALL, "");
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

   conf_path = config_dir_get();
   if (!ecore_file_exists(conf_path))
     ecore_file_mkdir(conf_path);

   if (!config_init()) return false;

   elm_app_info_set(app_init, "eflete", "layouts/eflete.edj");

#define PATH_CHECK(LOCALE, CURRENT, DIR_PATH, MESSAGE) \
   snprintf(buf, sizeof(buf), "%s/%s", DIR_PATH, LOCALE); \
   if (!ecore_file_exists(buf)) \
     { \
         snprintf(buf, sizeof(buf), "%s/%s", COMPILE_PATH, CURRENT); \
         if (!ecore_file_exists(buf)) \
           { \
              CRIT(MESSAGE" '%s'", buf); \
              return false; \
           } \
     }

   PATH_CHECK("themes/default/eflete_elm.edj", "data/eflete_elm.edj",
              elm_app_data_dir_get(), "Default theme is missing");
   ap.path.theme_edj = eina_stringshare_add(buf);

   PATH_CHECK("themes/default/ewe.edj", "data/ewe.edj",
              elm_app_data_dir_get(), "Ewe theme is missing");
   ap.path.ewe_edj = eina_stringshare_add(buf);

   PATH_CHECK("layouts/eflete.edj", "data/eflete.edj",
              elm_app_data_dir_get(), "Default layouts is missing");
   ap.path.layout_edj = eina_stringshare_add(buf);

   PATH_CHECK("eflete_exporter"EXT, "src/bin/eflete_exporter"EXT,
              elm_app_bin_dir_get(), "Eflete_exporter is missing");
   ap.path.exporter = eina_stringshare_add(buf);

   ap.path.edj_path = eina_stringshare_printf("%s/themes/default/", elm_app_data_dir_get());
   ap.path.image_path = eina_stringshare_printf("%s/images/", elm_app_data_dir_get());
   ap.path.sound_path = eina_stringshare_printf("%s/sounds/", elm_app_data_dir_get());


   ap.theme = elm_theme_new();

#ifndef _WIN32
   char *theme = strdup(ap.path.theme_edj);
#else
   char *theme = escape_colons(ap.path.theme_edj);
#endif
   elm_theme_set(ap.theme, theme);
   elm_theme_extension_add(ap.theme, ap.path.ewe_edj);
   free(theme);

   elm_need_ethumb();

   ap.last_path = NULL;

   ap.color_data = color_init(eina_strbuf_new());

   eina_stringshare_del(conf_path);
   return true;
}

Eina_Bool
app_shutdown()
{
   if(ap.color_data)
     color_term(ap.color_data);
   if (ap.last_path)
     eina_stringshare_del(ap.last_path);
   if (ap.path.theme_edj)
     eina_stringshare_del(ap.path.theme_edj);
   if (ap.path.layout_edj)
     eina_stringshare_del(ap.path.layout_edj);
   if (ap.path.edj_path)
     eina_stringshare_del(ap.path.edj_path);
   if (ap.path.image_path)
     eina_stringshare_del(ap.path.image_path);
   if (ap.path.export_edj)
     eina_stringshare_del(ap.path.export_edj);
   if (ap.path.export_edc)
     eina_stringshare_del(ap.path.export_edc);

   config_shutdown();
   elm_theme_free(ap.theme);
   eina_shutdown();
   efreet_shutdown();
   ecore_shutdown();
   edje_shutdown();
   logger_shutdown();

   return true;
}

#undef CHECK_AP
