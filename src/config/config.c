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

#include "config.h"

#define CONFIG_FILE        EFLETE_SETT_PATH"eflete.cfg"
#define CONFIG_FILE_TMP    CONFIG_FILE".tmp"
#define CONFIG_FILE_KEY    "config"
Config *config;

static Eet_Data_Descriptor *edd_base = NULL;

static void
_config_free(void)
{
   free(config);
   config = NULL;
}

Eina_Bool
config_init(void)
{
   Eet_Data_Descriptor_Class eddc;

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Config", sizeof(Config));
   edd_base = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "x", window.x, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "y", window.y, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "w", window.w, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "h", window.h, EET_T_INT);

   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.left",        panes.left, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.right",       panes.right, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.left_hor",    panes.left_hor, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.right_hor",   panes.right_hor, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.center",      panes.center, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.center_down", panes.center_down, EET_T_DOUBLE);

   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "profile", profile, EET_T_STRING);

   if (!edd_base) return false;

   config = NULL;

   return true;
}

void
config_shutdown(void)
{
   if (edd_base)
     {
        eet_data_descriptor_free(edd_base);
        edd_base = NULL;
     }
   if (config) _config_free();
}

static Config *
_config_default_new(void)
{
   Config *conf = NULL;

   conf = mem_malloc(sizeof(Config));
   conf->window.x =           0;
   conf->window.y =           0;
   conf->window.w =           800;
   conf->window.h =           600;
   conf->panes.left =         0.2;
   conf->panes.right =        0.75;
   conf->panes.left_hor =     0.7;
   conf->panes.right_hor =    0.3;
   conf->panes.center =       0.65;
   conf->panes.center_down =  0.4;
   conf->profile = strdup("default");

   return conf;
}

void
config_load(void)
{
   Eet_File *ef;

   if (config) _config_free();
   ef = eet_open(CONFIG_FILE, EET_FILE_MODE_READ);
   if (ef)
     {
        config = eet_data_read(ef, edd_base, CONFIG_FILE_KEY);
        if (!config) config = _config_default_new();
        eet_close(ef);
     }
   else
     config = _config_default_new();
}

Eina_Bool
config_save(App_Data *ap)
{
   int x, y, w, h;
   Eet_File *ef;
   Eina_Bool ok;

   if (!ap) return false;

   ecore_file_mkdir(EFLETE_SETT_PATH);
   evas_object_geometry_get(ap->win, NULL, NULL, &w, &h);
   elm_win_screen_position_get(ap->win, &x, &y);
   config->window.x =            x;
   config->window.y =            y;
   config->window.w =            w;
   config->window.h =            h;
   config->panes.left =          elm_panes_content_left_size_get(ap->panes.left);
   config->panes.right =         elm_panes_content_left_size_get(ap->panes.right);
   config->panes.left_hor =      elm_panes_content_left_size_get(ap->panes.left_hor);
   config->panes.right_hor =     elm_panes_content_left_size_get(ap->panes.right_hor);
   config->panes.center =        elm_panes_content_left_size_get(ap->panes.center);
   config->panes.center_down =   elm_panes_content_left_size_get(ap->panes.center_down);

   ef = eet_open(CONFIG_FILE_TMP, EET_FILE_MODE_WRITE);
   if (!ef) return false;
   ok = eet_data_write(ef, edd_base, CONFIG_FILE_KEY, config, 1);
   eet_close(ef);
   if (!ok) return false;
   ecore_file_mv(CONFIG_FILE_TMP, CONFIG_FILE);

   return true;
}

Config *
config_get(void)
{
   return config;
}
