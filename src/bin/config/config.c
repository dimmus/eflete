#include "config.h"
#include "shortcuts.h"
#include "enventor_module.h"

#define CONFIG_FILE        "eflete.cfg"
#define CONFIG_FILE_KEY    "config"

#define PROFILE_FILE_EXT   ".prf"
#define PROFILE_FILE_KEY   "profile"
#define PROFILE_VERSION    1

Config *config;
Profile *profile;

static Eet_Data_Descriptor *edd_base = NULL;
static Eet_Data_Descriptor *edd_recent = NULL;
static Eet_Data_Descriptor *edd_profile = NULL;
static Eet_Data_Descriptor *edd_keys = NULL;
static Eet_Data_Descriptor *edd_color = NULL;

#ifdef __APPLE__
Eina_Stringshare *
config_dir_get(void)
{
   const char *conf_path = NULL;
   Eina_Stringshare *cfg;

   conf_path = "Library/Preferences";
   cfg = eina_stringshare_printf("%s/%s", getenv("HOME"), conf_path);

   return cfg;
}
#else
Eina_Stringshare *
config_dir_get(void)
{
   const char *conf_path = NULL;
   Eina_Stringshare *cfg;

   conf_path = getenv("XDG_CONFIG_HOME");
   if (conf_path)
     cfg = eina_stringshare_printf("%s/eflete", conf_path);
   else
     {
        conf_path = eina_environment_home_get();
        cfg = eina_stringshare_printf("%s/.config/eflete", conf_path);
     }
   return cfg;
}
#endif
static Eina_Stringshare *
_config_file_path_get(void)
{
   Eina_Stringshare *cfg, *conf_path;

   conf_path = config_dir_get();
   cfg = eina_stringshare_printf("%s/"CONFIG_FILE, conf_path);

   eina_stringshare_del(conf_path);
   return cfg;
}

static Eina_Stringshare *
_config_tmp_file_path_get(const char *file)
{
   assert (file != NULL);

   return eina_stringshare_printf("%s.tmp", file);
}

static Eina_Stringshare *
_profile_path_get(const char *name)
{
   Eina_Stringshare *prf, *conf_path;

   assert (name != NULL);

   conf_path = config_dir_get();
   prf = eina_stringshare_printf("%s/%s"PROFILE_FILE_EXT, conf_path, name);

   eina_stringshare_del(conf_path);
   return prf;
}

static void
_config_free(void)
{
   free(config);
   config = NULL;
}

static void
_recent_free(Recent *r)
{
   eina_stringshare_del(r->name);
   eina_stringshare_del(r->path);
   free(r);
}

void
config_recent_list_clear(void)
{
   Recent *r;

   EINA_LIST_FREE(config->recents, r)
     {
       _recent_free(r);
     }
   config->recents = NULL;
}

void
config_recent_add(const char *name, const char *path)
{
   Recent *r;
   Eina_List *l, *l_n;

   assert(name != NULL);
   assert(path != NULL);

   EINA_LIST_FOREACH_SAFE(config->recents, l, l_n, r)
     {
        if (!strcmp(path, r->path))
          config->recents = eina_list_remove_list(config->recents, l);
        if (!ecore_file_exists(r->path))
          config->recents = eina_list_remove(config->recents, r);
     }

   if (eina_list_count(config->recents) > 9)
     config->recents = eina_list_remove_list(config->recents, eina_list_last(config->recents));

   r = mem_malloc(sizeof(Recent));
   r->name = strdup(name);
   r->path = strdup(path);

   config->recents = eina_list_prepend(config->recents, r);
   config_save();
}

static void
_profile_free(void)
{
   free((char *)profile->general.projects_folder);
   free((char *)profile->workspace.bg_image);
   free((char *)profile->liveview.bg_image);

   free(profile);
   profile = NULL;
}

Eina_Bool
config_init(void)
{
   Eet_Data_Descriptor_Class eddc;
   Eet_Data_Descriptor_Class eddkc;
   Eet_Data_Descriptor_Class eddcr;

   /* Config descriptor */
   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Config", sizeof(Config));
   edd_base = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "x", window.x, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "y", window.y, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "w", window.w, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "h", window.h, EET_T_INT);

   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.left",        panes.left, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.right",       panes.right, EET_T_DOUBLE);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Config, "panes.left_ver",    panes.left_ver, EET_T_DOUBLE);

   eet_eina_stream_data_descriptor_class_set(&eddcr, sizeof(eddcr), "Recent", sizeof(Recent));
   edd_recent = eet_data_descriptor_stream_new(&eddcr);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_recent, Recent, "recent.name", name, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_recent, Recent, "recent.path", path, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_LIST(edd_base, Config, "recents",  recents, edd_recent);

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
      (edd_profile, Profile, "general.projects_folder",           general.projects_folder, EET_T_STRING);
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

   /* enventor */
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_profile, Profile, "enventor.scale", enventor.scale, EET_T_DOUBLE);
   /* colors */
   EET_DATA_DESCRIPTOR_ADD_ARRAY
      (edd_profile, Profile, "colors",                            colors, edd_color);

   /* shortcuts */
   eet_eina_stream_data_descriptor_class_set(&eddkc, sizeof(eddkc), "Shortcut", sizeof(Shortcut));
   edd_keys = eet_data_descriptor_stream_new(&eddkc);

   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcut, "keyname",       keyname, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcut, "keycode",       keycode, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcut, "modifiers",     modifiers, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC
     (edd_keys, Shortcut, "combination",   combination, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_keys, Shortcut, "type_press", type_press, EET_T_UINT);
   EET_DATA_DESCRIPTOR_ADD_BASIC
      (edd_keys, Shortcut, "type_unpress", type_unpress, EET_T_UINT);


   EET_DATA_DESCRIPTOR_ADD_LIST
     (edd_profile, Profile, "shortcuts",    shortcuts, edd_keys);

   if (!edd_profile) return false;

   config = NULL;
   profile = NULL;

   return true;
}

Eina_Bool
config_shutdown(void)
{

   if (edd_base)
     {
        eet_data_descriptor_free(edd_base);
        edd_base = NULL;
     }
   if (edd_recent)
     {
        eet_data_descriptor_free(edd_recent);
        edd_recent = NULL;
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

   return true;
}


static void
_profile_update(Profile *prof)
{
   assert(prof != NULL);
   if (prof->version > PROFILE_VERSION)
     return;

   char *env_path = getenv("EFLETE_PROJECTS_DIR");
   prof->version                             = PROFILE_VERSION;

   if ((env_path) && (ecore_file_exists(env_path)))
     prof->general.projects_folder = strdup(env_path);
   else if ((!prof->general.projects_folder) ||
       (!ecore_file_exists(prof->general.projects_folder)))
     prof->general.projects_folder = getenv("HOME");

}

static Profile *
_profile_default_new(void)
{
   Profile *prof;

   prof = mem_calloc(1, sizeof(Profile));
   prof->version                             = PROFILE_VERSION;
   prof->general.save_ui                     = true;
   prof->general.save_win_pos                = true;
   prof->general.autosave.period             = 300; /* 5 minutes */
   prof->workspace.groupedit_handler_size    = 7;
   prof->workspace.rulers.visible            = true;
   prof->workspace.rulers.mode               = ABS_REL_SCALE;
   prof->enventor.scale                      = 1;

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

   return prof;
}

static Config *
_config_default_new(void)
{
   Config *conf = NULL;

   conf = mem_calloc(1, sizeof(Config));
   conf->window.x =           0;
   conf->window.y =           0;
   conf->window.w =           1366;
   conf->window.h =           768;
   conf->panes.left =         0.2;
   conf->panes.right =        0.7;
   conf->panes.left_ver =     0.5;
   conf->profile = strdup("default");

   return conf;
}

static void
_update_recents(Config *conf)
{
   Recent *r;
   Eina_List *l, *l_n;

   EINA_LIST_FOREACH_SAFE(conf->recents, l, l_n, r)
     {
        if (!ecore_file_exists(r->path))
          conf->recents = eina_list_remove(conf->recents, r);
     }
}

void
config_load(void)
{
   Eet_File *ef;
   Eina_Stringshare *config_file;

   if (config) _config_free();
   config_file = _config_file_path_get();
   ef = eet_open(config_file, EET_FILE_MODE_READ);
   if (ef)
     {
        config = eet_data_read(ef, edd_base, CONFIG_FILE_KEY);
        if (!config) config = _config_default_new();
        eet_close(ef);
     }
   else
     config = _config_default_new();

   profile_load(config->profile);

   _update_recents(config);

#ifdef HAVE_ENVENTOR
   enventor_object_profile_load(ap.enventor, profile_get());
#endif /* HAVE_ENVENTOR */
   eina_stringshare_del(config_file);
}

Eina_Bool
config_panes_sizes_data_update(void)
{

   if (!config) return false;

   config->panes.left =
     elm_panes_content_left_size_get(ap.panes.left);
   config->panes.right =
     elm_panes_content_left_size_get(ap.panes.right);
   config->panes.left_ver =
     elm_panes_content_left_size_get(ap.panes.left_ver);

   return true;
}


Eina_Bool
config_save(void)
{
   int x, y, w, h;
   Eet_File *ef;
   Eina_Bool ok;
   Eina_Stringshare *cfg, *tmp;

   if (!edd_base)
     {
        CRIT("Nothing to save! Config not loaded.");
        return false;
     }

   evas_object_geometry_get(ap.win, &x, &y, &w, &h);
   TODO("find out why w/h here sometimes become close to 0");
   if ((profile->general.save_win_pos) &&
        (w > 200 && h > 200)) /* don't write too small windwo size to config */
     {
        config->window.x = x;
        config->window.y = y;
        config->window.w = w;
        config->window.h = h;
     }
   if (profile->general.save_ui)
     config_panes_sizes_data_update();

   profile_save(config->profile);
   cfg = _config_file_path_get();
   tmp = _config_tmp_file_path_get(cfg);

   ef = eet_open(tmp, EET_FILE_MODE_WRITE);
   if (!ef) return EINA_FALSE;
   ok = eet_data_write(ef, edd_base, CONFIG_FILE_KEY, config, 1);
   eet_close(ef);
   if (!ok) return EINA_FALSE;
   ecore_file_mv(tmp, cfg);

   eina_stringshare_del(tmp);
   eina_stringshare_del(cfg);
   return EINA_TRUE;
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

   assert(name != NULL);

   path = _profile_path_get(name);

   if (profile) _profile_free();
   ef = eet_open(path, EET_FILE_MODE_READ);
   if (ef)
     {
        profile = eet_data_read(ef, edd_profile, PROFILE_FILE_KEY);
        eet_close(ef);
     }

   if (!profile)
     profile = _profile_default_new();

   _profile_update(profile);

   eina_stringshare_del(path);
}

Eina_Bool
profile_save(const char *name)
{
   Eet_File *ef;
   Eina_Stringshare *path, *tmp;
   Eina_Bool ok;

   assert(name != NULL);

   if (!edd_profile)
     {
        CRIT("Nothing to save! Profile not loaded.");
        return false;
     }

   path = _profile_path_get(name);
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
   Eina_Stringshare *p, *conf_path;

   conf_path = config_dir_get();
   files = ecore_file_ls(conf_path);
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

   eina_stringshare_del(conf_path);
   return profiles;
}
