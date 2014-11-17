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

#include "config.h"
#include "shortcuts.h"

#define CONFIG_FILE        EFLETE_SETT_PATH"eflete.cfg"
#define CONFIG_FILE_TMP    CONFIG_FILE".tmp"
#define CONFIG_FILE_KEY    "config"

#define PROFILE_FILE_EXT   ".prf"
#define PROFILE_FILE_KEY   "profile"
#define PROFILE_VERSION    1

Config *config;
Profile *profile;

static Eet_Data_Descriptor *edd_base = NULL;
static Eet_Data_Descriptor *edd_profile = NULL;
static Eet_Data_Descriptor *edd_keys = NULL;
static Eet_Data_Descriptor *edd_color = NULL;

static void
_config_free(void)
{
   free(config);
   config = NULL;
}

static void
_profile_free(void)
{
   Shortcuts *sc;

   free((char *)profile->general.home_folder);
   free((char *)profile->general.swap_folder);
   free((char *)profile->workspace.bg_image);
   free((char *)profile->liveview.bg_image);

   EINA_LIST_FREE(profile->shortcuts, sc)
     {
        eina_stringshare_del(sc->description);
        free(sc);
     }
   eina_list_free(profile->shortcuts);

   free(profile);
   profile = NULL;
}

Eina_Bool
config_init(App_Data *ap)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor_Class eddkc;

   if (!ap) return false;

   /* Config descriptor */
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

   /* Profile descriptor */
   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Profile", sizeof(Profile));
   edd_profile = eet_data_descriptor_stream_new(&eddc);

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Profile_Color", sizeof(Profile_Color));
   edd_color = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_color, Profile_Color, "r", r, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_color, Profile_Color, "g", g, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_color, Profile_Color, "b", b, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_color, Profile_Color, "a", a, EET_T_UCHAR);

   if (!edd_color) return false;

   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "version",  version, EET_T_INT);

   /* general */
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "general.home_folder",               general.home_folder, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "general.swap_folder",               general.swap_folder, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "general.save_ui",                   general.save_ui, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "general.save_win_pos",              general.save_win_pos, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "general.autosave.autosave",         general.autosave.autosave, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "general.autosave.period",           general.autosave.period, EET_T_INT);

   /* workspce */
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "workspace.bg_image",                workspace.bg_image, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "workspace.groupedit.handler_size",  workspace.groupedit_handler_size, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "workspace.rulers.visible",          workspace.rulers.visible, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "workspace.rulers.mode",             workspace.rulers.mode, EET_T_UCHAR);

   /* liveview */
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "liveview.bg_image",                 liveview.bg_image, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "liveview.auto_fill_text",           liveview.auto_fill_text, EET_T_UCHAR);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "liveview.auto_fill_content",        liveview.auto_fill_content, EET_T_UCHAR);

   /* colors */
   EET_DATA_DESCRIPTOR_ADD_ARRAY
      (edd_profile, Profile, "colors",                            colors, edd_color);

   /* shortcuts */
   eet_eina_stream_data_descriptor_class_set(&eddkc, sizeof(eddkc), "Shortcuts", sizeof(Shortcuts));
   edd_keys = eet_data_descriptor_stream_new(&eddkc);

   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcuts, "keyname",       keyname, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcuts, "keycode",       keycode, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcuts, "modifiers",     modifiers, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcuts, "description",   description, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcuts, "holdable",      holdable, EET_T_UCHAR);

   EET_DATA_DESCRIPTOR_ADD_LIST
     (edd_profile, Profile, "shortcuts",    shortcuts, edd_keys);

   if (!edd_profile) return false;
   if (!shortcuts_init(ap))
     {
        CRIT("Can't initialize the shortcut module");
        return false;
     }

   config = NULL;
   profile = NULL;

   return true;
}

Eina_Bool
config_shutdown(App_Data *ap)
{
   if (!ap) return false;

   if (edd_base)
     {
        eet_data_descriptor_free(edd_base);
        edd_base = NULL;
     }
   if (edd_color)
     {
        eet_data_descriptor_free(edd_color);
        edd_color = NULL;
     }
   if (edd_profile)
     {
        eet_data_descriptor_free(edd_profile);
        edd_profile = NULL;
     }
   if (edd_keys)
     {
        eet_data_descriptor_free(edd_keys);
        edd_keys = NULL;
     }
   if (config) _config_free();
   shortcuts_shutdown(ap);

   return true;
}

static Eina_List *
_default_shortcuts_get()
{
   Eina_List *shortcuts = NULL;
   Shortcuts *shortcut;

#define ADD_SHORTCUT(Name, Keycode, Modifiers, Descr, Holdable)              \
   shortcut = calloc(1, sizeof(Shortcuts));                                  \
   if (!shortcut) return shortcuts;                                          \
   shortcut->keyname = eina_stringshare_add_length(Name, strlen(Name));      \
   shortcut->keycode = Keycode;                                              \
   shortcut->modifiers = Modifiers;                                          \
   shortcut->description = eina_stringshare_add_length(Descr, strlen(Descr));\
   shortcut->holdable = Holdable;                                             \
   shortcuts = eina_list_append(shortcuts, shortcut);

   /* No modifiers */
   ADD_SHORTCUT("Delete", 119, 0, "item.delete", false);

   /* Ctrl- */
   ADD_SHORTCUT("slash", 61, CTRL, "separate_mode", false);
   ADD_SHORTCUT("n", 57, CTRL, "new_theme", false);
   ADD_SHORTCUT("o", 32, CTRL, "open_edj", false);
   ADD_SHORTCUT("s", 39, CTRL, "save", false);
   ADD_SHORTCUT("e", 26, CTRL, "export", false);
   ADD_SHORTCUT("q", 24, CTRL, "quit", false);

   ADD_SHORTCUT("1", 10, CTRL, "style_editor", false);
   ADD_SHORTCUT("2", 11, CTRL, "image_editor", false);
   ADD_SHORTCUT("3", 12, CTRL, "sound_editor", false);
   ADD_SHORTCUT("4", 13, CTRL, "colorclass_viewer", false);
   ADD_SHORTCUT("5", 14, CTRL, "program_editor", false);

   ADD_SHORTCUT("Left", 113, CTRL, "widget_manager.style", false);
   ADD_SHORTCUT("Right", 114, CTRL, "widget_manager.layout", false);

   ADD_SHORTCUT("equal", 21, CTRL, "zoom.in", false);
   ADD_SHORTCUT("minus", 20, CTRL, "zoom.out", false);

   /* Alt- */
   ADD_SHORTCUT("w", 25, ALT, "part.add.swallow", false);
   ADD_SHORTCUT("b", 56, ALT, "part.add.textblock", false);
   ADD_SHORTCUT("t", 28, ALT, "part.add.text", false);
   ADD_SHORTCUT("r", 27, ALT, "part.add.rectangle", false);
   ADD_SHORTCUT("i", 31, ALT, "part.add.image", false);
   ADD_SHORTCUT("p", 33, ALT, "part.add.proxy", false);
   ADD_SHORTCUT("s", 39, ALT, "part.add.spacer", false);

   /* Ctrl-Shift- */
   ADD_SHORTCUT("o", 32, CTRL + SHIFT, "open_edc", false);
   ADD_SHORTCUT("s", 39, CTRL + SHIFT, "save_as", false);
   ADD_SHORTCUT("v", 55, CTRL + SHIFT, "property.visual_tab", false);
   ADD_SHORTCUT("c", 54, CTRL + SHIFT, "property.code_tab", false);
   ADD_SHORTCUT("l", 46, CTRL + SHIFT, "style.create", false);

   /* Holdable keys. */
   ADD_SHORTCUT("Alt_L", 64, CTRL, "highlight.align.show", true);
   ADD_SHORTCUT("Alt_L", 64, 0, "object_area.show", true);

#undef ADD_SHORTCUT
   return shortcuts;
}

static Profile *
_profile_default_new(void)
{
   Profile *prof = NULL;

   prof = mem_malloc(sizeof(Profile));
   prof->version                             = PROFILE_VERSION;
   prof->general.home_folder                 = strdup("HOME");
   prof->general.swap_folder                 = strdup(EFLETE_SWAP_PATH);
   prof->general.save_ui                     = true;
   prof->general.save_win_pos                = true;
   prof->general.autosave.autosave           = false;
   prof->general.autosave.period             = 300; /* 5 minutes */
   prof->workspace.bg_image                  = NULL;
   prof->workspace.groupedit_handler_size    = 7;
   prof->workspace.rulers.visible            = true;
   prof->workspace.rulers.mode               = ABS_REL_SCALE;
   prof->liveview.bg_image                   = NULL;
   prof->liveview.auto_fill_text             = false;
   prof->liveview.auto_fill_content          = false;

   prof->colors[WORKSPACE].r           = 255;
   prof->colors[WORKSPACE].g           = 255;
   prof->colors[WORKSPACE].b           = 255;
   prof->colors[WORKSPACE].a           = 255;

   prof->colors[GROUPEDIT].r           = 0;
   prof->colors[GROUPEDIT].g           = 0;
   prof->colors[GROUPEDIT].b           = 0;
   prof->colors[GROUPEDIT].a           = 255;

   prof->colors[LIVEVIEW].r            = 255;
   prof->colors[LIVEVIEW].g            = 255;
   prof->colors[LIVEVIEW].b            = 255;
   prof->colors[LIVEVIEW].a            = 255;

   prof->colors[HIGHLIGHT].r           = 58;
   prof->colors[HIGHLIGHT].g           = 100;
   prof->colors[HIGHLIGHT].b           = 155;
   prof->colors[HIGHLIGHT].a           = 255;

   prof->shortcuts = NULL;

   return prof;
}

static Config *
_config_default_new(void)
{
   Config *conf = NULL;

   conf = mem_malloc(sizeof(Config));
   conf->window.x =           0;
   conf->window.y =           0;
   conf->window.w =           1366;
   conf->window.h =           768;
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
config_load(App_Data *ap)
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

   profile_load(config->profile);

   shortcuts_profile_load(ap, profile_get());
   shortcuts_main_add(ap);
}

Eina_Bool
config_panes_sizes_data_update(App_Data *ap)
{
   if (!config) return false;

   config->panes.left =
     elm_panes_content_left_size_get(ap->panes.left);
   config->panes.right =
     elm_panes_content_left_size_get(ap->panes.right);
   config->panes.left_hor =
     elm_panes_content_left_size_get(ap->panes.left_hor);
   config->panes.right_hor =
     elm_panes_content_left_size_get(ap->panes.right_hor);
   config->panes.center =
     elm_panes_content_left_size_get(ap->panes.center);
   config->panes.center_down =
     elm_panes_content_left_size_get(ap->panes.center_down);

   return true;
}


Eina_Bool
config_save(App_Data *ap)
{
   int x, y, w, h;
   Eet_File *ef;
   Eina_Bool ok;

   if (!edd_base)
     {
        CRIT("Nothing to save! Config not loaded.");
        return false;
     }

   if (!ap) return false;

   evas_object_geometry_get(ap->win, &x, &y, &w, &h);
   if (profile->general.save_win_pos)
     {
        config->window.x =            x;
        config->window.y =            y;
        config->window.w =            w;
        config->window.h =            h;
     }
   if (profile->general.save_ui)
     config_panes_sizes_data_update(ap);

   profile_save(config->profile);

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

void
profile_load(const char *name)
{
   Eet_File *ef;
   Eina_Stringshare *path;

   if (!name) return;
   path = eina_stringshare_printf(EFLETE_SETT_PATH"%s"PROFILE_FILE_EXT, name);

   if (profile) _profile_free();
   ef = eet_open(path, EET_FILE_MODE_READ);
   if (ef)
     {
        profile = eet_data_read(ef, edd_profile, PROFILE_FILE_KEY);
        if (!profile) profile = _profile_default_new();
        eet_close(ef);
     }
   else
     {
        profile = _profile_default_new();
        profile->shortcuts = _default_shortcuts_get();
     }

   eina_stringshare_del(path);
}

Eina_Bool
profile_save(const char *name)
{
   Eet_File *ef;
   Eina_Stringshare *path, *tmp;
   Eina_Bool ok;

   if (!edd_profile)
     {
        CRIT("Nothing to save! Profile not loaded.");
        return false;
     }

   if (!name) return false;
   path = eina_stringshare_printf(EFLETE_SETT_PATH"%s"PROFILE_FILE_EXT, name);
   tmp = eina_stringshare_printf("%s%s", path, ".tmp");

   ef = eet_open(tmp, EET_FILE_MODE_WRITE);
   if (!ef) return false;
   ok = eet_data_write(ef, edd_profile, PROFILE_FILE_KEY, profile, 1);
   eet_close(ef);
   if (!ok) return false;
   ecore_file_mv(tmp, path);

   eina_stringshare_del(tmp);
   eina_stringshare_del(path);
   return true;
}

Profile *
profile_get(void)
{
   return profile;
}

Eina_List *
profiles_get(void)
{
   Eina_List *profiles = NULL;
   Eina_List *files;
   char *f;
   char tmp[BUFF_MAX];
   Eina_Stringshare *p;

   files = ecore_file_ls(EFLETE_SETT_PATH);
   EINA_LIST_FREE(files, f)
     {
        if (eina_str_has_suffix(f, PROFILE_FILE_EXT))
          {
             /* as the eina_strlcpy copied the lenght size - 1, for right string 
                will be copied need to add ' + 1' to lenght size */
             eina_strlcpy(tmp, f, strlen(f) - strlen(PROFILE_FILE_EXT) + 1);
             p = eina_stringshare_add(tmp);
             profiles = eina_list_append(profiles, p);
          }
        free(f);
     }

   return profiles;
}
