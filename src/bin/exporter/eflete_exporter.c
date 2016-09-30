#define EDJE_EDIT_IS_UNSTABLE_AND_I_KNOW_ABOUT_IT

#include <stdlib.h>
#include <stdio.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Ecore_File.h>
#include <Ecore_Getopt.h>
#include <Edje_Edit.h>
#include <Eet.h>
#include "group_manager_resources.h"
#include "project_manager2_data.h"
#include "string_common.h"

#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif /* include eflete_config.h */
#include "common_macro.h"
#include "logger.h"

#define IMAGES "images"
#define SOUNDS "sounds"
#define FONTS  "fonts"

static char *edj = NULL;
static char *path = NULL;
static Eina_List *groups = NULL;
static Eina_Bool source = false;
static Eina_Bool info_only = false;
static Eina_Bool ignore = false;
static Eina_Bool deps = false;

static Ecore_Evas *ee;
static Evas_Object *obj;
static char *spath;
static char *sedj;
static Eina_List *images, *sounds, *fonts;

static Ecore_Job *images_job = NULL;
static Ecore_Job *sounds_job = NULL;
static Ecore_Job *fonts_job = NULL;
static Ecore_Job *source_job = NULL;

static PM_Project_Result exit_status = PM_PROJECT_SUCCESS;

static const Ecore_Getopt options = {
   PACKAGE_NAME,
   "%prog [OPTION]... FILE"
   ,
   VERSION,
   "(C) 2013-2016 Samsung Electronics.",
   "GNU Library General Public License version 2",
   "This application was written for Enlightenment, to use EFL\n"
   "and design as Eflete support tool.\n",
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_STR(0, "edj", "Path to edj file"),
      ECORE_GETOPT_STORE_STR(0, "path", "Path to export dirrectory"),
      ECORE_GETOPT_APPEND_METAVAR('g', "group", "Group for export", "GROUP_NAME", ECORE_GETOPT_TYPE_STR),
      ECORE_GETOPT_STORE_TRUE('s', "source", "Export surce code for given group(s)"),
      ECORE_GETOPT_STORE_TRUE('i', "ignore", "Ignore missing groups"),
      ECORE_GETOPT_STORE_TRUE('d', "dependencies", "Export group with dependencies"),
      ECORE_GETOPT_VERSION  ('v', "version"),
      ECORE_GETOPT_COPYRIGHT('c', "copyright"),
      ECORE_GETOPT_LICENSE  ('l', "license"),
      ECORE_GETOPT_HELP     ('h', "help"),
      ECORE_GETOPT_SENTINEL
   }
};

static int
_digist_get(int v)
{
   int dig = 0;

   while (v != 0)
     {
        v /= 10;
        dig++;
     }
   return dig;
}

static void
_terminate(PM_Project_Result error)
{
   char buf[256];

   ecore_job_del(images_job);
   snprintf(buf, strlen(spath) + strlen("/"IMAGES) + 1,
           "%s/"IMAGES, spath);
   if (!ecore_file_exists(buf))
     ecore_file_recursive_rm(buf);

   ecore_job_del(sounds_job);
   snprintf(buf, strlen(spath) + strlen("/"SOUNDS) + 1,
            "%s/"SOUNDS, spath);
   if (!ecore_file_exists(buf))
     ecore_file_recursive_rm(buf);

   ecore_job_del(fonts_job);
   snprintf(buf, strlen(spath) + strlen("/"FONTS) + 1,
            "%s/"FONTS, spath);
   if (!ecore_file_exists(buf))
     ecore_file_recursive_rm(buf);

   ecore_job_del(source_job);
   exit_status = error;
   ecore_main_loop_quit();
}

static void
_images_resources_export(void *data __UNUSED__)
{
   Eina_List *l;
   int id, total, proc = 0;
   char buf[256];
   char *name, *file_dir;
   Evas_Object *img;

   total = eina_list_count(images);
   if (0 == total) return;
   snprintf(buf, strlen(spath) + strlen("/"IMAGES) + 1,
            "%s/"IMAGES, spath);
   fprintf(stdout, "Export images. Total images for export '%i'\n", total);

   if (!ecore_file_exists(buf))
     {
        if (!ecore_file_mkpath(buf))
          {
             fprintf(stderr, "ERROR: Failed create path %s for export images", buf);
             _terminate(PM_PROJECT_EXPORT_CREATE_PATH_FAILED);
             return;
          }
     }

   img = evas_object_image_add(ecore_evas_get(ee));
   EINA_LIST_FOREACH(images, l, name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!name) continue;

        fprintf(stdout, "(%*i/%i) %s\n", _digist_get(total), ++proc, total, name);
        if (EDJE_EDIT_IMAGE_COMP_USER == edje_edit_image_compression_type_get(obj, name))
          {
             snprintf(buf, strlen(spath) + strlen("/"IMAGES"/") + strlen(name) + 1,
                      "%s/"IMAGES"/%s\n", spath, name);
             if (!eina_file_copy(name, buf, EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR, NULL, NULL))
               {
                  fprintf(stderr, "ERROR: Could not copy image '%s'\n", name);
                  _terminate(PM_PROJECT_EXPORT_COPY_FILE_FAILED);
                  return;
               }
          }
        else
          {
             id = edje_edit_image_id_get(obj, name);
             if (id < 0)
               {
                  fprintf(stderr, "ERROR: Image have wrong id %i\n", id);
                  _terminate(PM_PROJECT_EXPORT_WRONG_IMAGE_ID);
                  return;
               }
             snprintf(buf, strlen("edje/images/") + (_digist_get(id) + 1) + 1,
                     "edje/images/%i", id);
             evas_object_image_file_set(img, sedj, buf);
             snprintf(buf, strlen(spath) + strlen("/"IMAGES"/") + strlen(name) + 1,
                      "%s/"IMAGES"/%s", spath, name);
             file_dir = ecore_file_dir_get(buf);
             if (!ecore_file_exists(file_dir)) ecore_file_mkpath(file_dir);
             free(file_dir);

             if (!evas_object_image_save(img, buf, NULL, NULL))
               {
                  fprintf(stderr, "ERROR: Image does not save, error: %s\n", evas_load_error_str(evas_object_image_load_error_get(img)));
                  _terminate(PM_PROJECT_EXPORT_SAVE_IMAGE_FAILED);
                  return;
               }
          }
     }
   evas_object_del(img);
   return;
}

static void
_sounds_resources_export(void *data __UNUSED__)
{
   Eina_List *l;
   int total, proc = 0;
   char buf[256];
   char *name, *file_dir;
   Eina_Stringshare *sound_file;
   Eina_Binbuf *sound_bin;
   FILE *f;

   total = eina_list_count(sounds);
   if (0 == total) return;
   fprintf(stdout, "Export sounds. Total sounds for export '%i'\n", total);

   snprintf(buf, strlen(spath) + strlen("/"SOUNDS) + 1,
           "%s/"SOUNDS, spath);
   if (!ecore_file_exists(buf))
     {
        if (!ecore_file_mkpath(buf))
          {
             fprintf(stderr, "ERROR: Failed create path %s for export sounds", buf);
             _terminate(PM_PROJECT_EXPORT_CREATE_PATH_FAILED);
             return;
          }
     }

   EINA_LIST_FOREACH(sounds, l, name)
     {
        sound_file = edje_edit_sound_samplesource_get(obj, name);
        fprintf(stdout, "(%*i/%i) %s\n", _digist_get(total), ++proc, total, sound_file);

        snprintf(buf, strlen(spath) + strlen("/"SOUNDS"/") + strlen(sound_file) + 1,
                 "%s/"SOUNDS"/%s", spath, sound_file);
        file_dir = ecore_file_dir_get(buf);
        if (!ecore_file_exists(file_dir)) ecore_file_mkpath(file_dir);
        free(file_dir);
        sound_bin = edje_edit_sound_samplebuffer_get(obj, name);
        if (!(f = fopen(buf, "wb")))
          {
             fprintf(stderr, "ERROR: Could not export sound '%s'. File does not open.\n", sound_file);
             _terminate(PM_PROJECT_EXPORT_SAVE_SAMPLE_FAILED);
             return;
          }
        if (fwrite(eina_binbuf_string_get(sound_bin),
                   eina_binbuf_length_get(sound_bin), 1, f) != 1)
          {
             fprintf(stderr, "ERROR: Could not export sound '%s', Unable to write file.\n", sound_file);
             _terminate(PM_PROJECT_EXPORT_SAVE_SAMPLE_FAILED);
             fclose(f);
             return;
          }
        if (f) fclose(f);
        eina_binbuf_free(sound_bin);
        edje_edit_string_free(sound_file);
     }
   return;
}

static void
_fonts_resources_export(void *data __UNUSED__)
{
   Eina_List *l;
   int size, total, proc = 0;
   char buf[256];
   char *name;
   Eina_Stringshare *font_file;
   void *font;
   FILE *f;
   Eet_File *ef;

   total = eina_list_count(fonts);
   if (0 == total) return;
   fprintf(stdout, "Export fonts. Total fonts for export '%i'.\n", total);

   snprintf(buf, strlen(spath) + strlen("/"FONTS) + 1,
            "%s/"FONTS, spath);
   if (!ecore_file_exists(buf))
     {
        if (!ecore_file_mkpath(buf))
          {
             fprintf(stderr, "ERROR: Failed create path %s for export fonts.\n", buf);
             _terminate(PM_PROJECT_EXPORT_CREATE_PATH_FAILED);
             return;
          }
     }

   ef = eet_open(sedj, EET_FILE_MODE_READ);
   EINA_LIST_FOREACH(fonts, l, name)
     {
        font_file = edje_edit_font_path_get(obj, name);
        fprintf(stdout, "(%*i/%i) %s\n", _digist_get(total), ++proc, total, name);

        snprintf(buf, strlen("edje/fonts/") + strlen(name) + 1,
                 "edje/fonts/%s", name);
        font = eet_read(ef, buf, &size);
        if (!font)
          {
             fprintf(stderr, "ERROR: Unable to read font '%s' from '%s'.\n", buf, sedj);
             _terminate(PM_PROJECT_EXPORT_READ_EDJ_FONT_FAILED);
             goto exit;
          }
        snprintf(buf, strlen(spath) + strlen("/"FONTS"/") + strlen(font_file) + 1,
                "%s/"FONTS"/%s", spath, font_file);
        if (!(f = fopen(buf, "wb")))
          {
             fprintf(stderr, "ERROR: Could not export font '%s'. File does not open.\n", font_file);
             _terminate(PM_PROJECT_EXPORT_SAVE_FONT_FAILED);
             goto exit;
          }
        if (fwrite(font, size, 1, f) != 1)
          {
             fprintf(stderr, "ERROR: Could not export font '%s', Unable to write file.\n", font_file);
             _terminate(PM_PROJECT_EXPORT_SAVE_FONT_FAILED);
             fclose(f);
             goto exit;
          }
        if (f) fclose(f);
        free(font);

        edje_edit_string_free(font_file);
     }
exit:
   eet_close(ef);
}

static char *
_group_source_code_export(const char *group)
{
   char *name;
   char buf[256];
   FILE *f;
   Eina_Stringshare *code;
   Evas_Object *edje_obj;
   Edje_Load_Error edje_error;

   name = strdup(group);
   string_char_replace(name, '/', '_');
   snprintf(buf, strlen(spath) + strlen("/") + strlen(name) + strlen(".edc") + 1,
            "%s/%s.edc", spath, name);

   f = fopen(buf, "w+");
   if (!f)
     {
        //ERR("Could't open file '%s'", eina_strbuf_string_get(buf));
        _terminate(PM_PROJECT_EXPORT_CREATE_FILE_FAILED);
        return NULL;
     }

   edje_obj = edje_edit_object_add(ecore_evas_get(ee));
   edje_object_file_set(edje_obj, sedj, group);
   edje_error = edje_object_load_error_get(edje_obj);
   if (EDJE_LOAD_ERROR_NONE != edje_error)
     {
        fprintf(stderr, "ERROR: Edje object load error: %s\n", edje_load_error_str(edje_error));
        fclose(f);
        return NULL;
     }
   code = edje_edit_source_generate(edje_obj);
   fputs(code, f);
   edje_edit_string_free(code);
   fclose(f);

   snprintf(buf, sizeof(buf), "%s.edc", name);
   return strdup(buf);
}

static void
_source_code_export(void *data __UNUSED__)
{
   char buf[256];
   FILE *f;
   Eina_List *l;
   const char *g;

   fprintf(stdout, "Generate source code");
   snprintf(buf, strlen(spath) + strlen("/generated.edc") + 1,
            "%s/generated.edc", spath);
   f = fopen(buf, "w+");
   if (!f)
     {
        fprintf(stderr, "ERROR: Could not open file '%s'", buf);
        _terminate(PM_PROJECT_EXPORT_CREATE_FILE_FAILED);
        return;
     }

   if (!groups)
     {
        char *code;
        code = edje_edit_full_source_generate(obj);
        fputs(code, f);
        free(code);
     }
   else
     {
        EINA_LIST_FOREACH(groups, l, g)
          {
             fputs("#include \"", f);
             char *name = _group_source_code_export(g);
             fputs(name, f);
             free(name);
             fputs("\"\n", f);
          }
     }
   fclose(f);
}

static void
_loop_end(void *data __UNUSED__)
{
   ecore_main_loop_quit();
}

int main(int argc, char **argv)
{
   Eina_List *collections, *l, *l1, *deps_g = NULL;
   int pos;
   Edje_Load_Error edje_error;
   const char *g, *g1;
   Evas_Object *edje_obj;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR(edj),
     ECORE_GETOPT_VALUE_STR(path),
     ECORE_GETOPT_VALUE_LIST(groups),
     ECORE_GETOPT_VALUE_BOOL(source),
     ECORE_GETOPT_VALUE_BOOL(ignore),
     ECORE_GETOPT_VALUE_BOOL(deps),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_NONE
   };

   setbuf(stdout, NULL);
   setbuf(stderr, NULL);

   eina_init();
   ecore_init();
   ecore_evas_init();
   edje_init();

   ecore_app_no_system_modules();

   pos = ecore_getopt_parse(&options, values, argc, argv);
   if ((pos == 1) || (pos < argc - 1))
     {
        ecore_getopt_help(stderr, &options);
        return EXIT_FAILURE;
     }
   spath = eina_file_path_sanitize(path);
   sedj = eina_file_path_sanitize(edj);
   if (!edj)
     {
        fprintf(stderr, "ERROR: Missing edj-file\n");
        ecore_getopt_help(stderr, &options);
        return EXIT_FAILURE;
     }
   if (!ecore_file_exists(sedj))
     {
        fprintf(stderr, "ERROR: File does not exist\n");
        ecore_getopt_help(stderr, &options);
        return EXIT_FAILURE;
     }

   collections = edje_file_collection_list(sedj);
   ee = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ee));
   edje_object_file_set(obj, sedj, eina_list_data_get(collections));
   edje_error = edje_object_load_error_get(obj);
   if (EDJE_LOAD_ERROR_NONE != edje_error)
     {
        fprintf(stderr, "ERROR: Edje object load error: %s\n", edje_load_error_str(edje_error));
        goto exit;
     }

   if (!groups)
     {
        images = edje_edit_images_list_get(obj);
        sounds = edje_edit_sound_samples_list_get(obj);
        fonts = edje_edit_fonts_list_get(obj);
     }
   else
     {
        edje_obj = edje_edit_object_add(ecore_evas_get(ee));
        if (deps)
          {
             EINA_LIST_FOREACH(groups, l, g)
               {
             fprintf(stdout, "- %s\n", g);
                  edje_object_file_set(edje_obj, sedj, g);
   edje_error = edje_object_load_error_get(edje_obj);
   if (EDJE_LOAD_ERROR_NONE != edje_error)
     {
        fprintf(stderr, "ERROR: Edje object load error: %s\n", edje_load_error_str(edje_error));
        goto exit;
     }


                  if (!deps_g)
                    deps_g = _object_groups_get(edje_obj);
                  else
                    deps_g = eina_list_merge(deps_g, _object_groups_get(edje_obj));
                  if (!deps_g) continue;
                  fprintf(stdout, "Groups '%s' has dependencies:\n", g);
                  EINA_LIST_FOREACH(deps_g, l1, g1)
                     fprintf(stdout, "\t'%s'\n", g1);
               }
             groups = eina_list_merge(groups, deps_g);
             groups = _strings_list_duplicates_del(groups);
          }
        EINA_LIST_FOREACH(groups, l, g)
          {
             if (!edje_file_group_exists(sedj, g))
               {
                  if (ignore)
                    {
                       fprintf(stdout, "Group '%s' not found. Ignoring...\n", g);
                       continue;
                    }
                  else
                    {
                       fprintf(stdout, "Group '%s' not found. Terminate\n", g);
                       _terminate(PM_PROJECT_EXPORT_CREATE_OBJECT_FAILED);
                       goto exit;
                    }
               }
             edje_object_file_set(edje_obj, sedj, g);

             if (!images)
               images = _object_images_get(edje_obj);
             else
               images = eina_list_merge(images, _object_images_get(edje_obj));

             if (!sounds)
               sounds = _object_samples_get(edje_obj);
             else
               sounds = eina_list_merge(sounds, _object_samples_get(edje_obj));

             if (!fonts)
               fonts = _object_fonts_get(edje_obj);
             else
               fonts = eina_list_merge(fonts, _object_fonts_get(edje_obj));
          }
        evas_object_del(edje_obj);
        if (images) images = _strings_list_duplicates_del(images);
        if (sounds) sounds = _strings_list_duplicates_del(sounds);
        if (fonts) fonts = _strings_list_duplicates_del(fonts);
     }
   images_job = ecore_job_add(_images_resources_export, NULL);
   sounds_job = ecore_job_add(_sounds_resources_export, NULL);
   fonts_job = ecore_job_add(_fonts_resources_export, NULL);
   if (source) source_job = ecore_job_add(_source_code_export, NULL);
   ecore_job_add(_loop_end, NULL);
   ecore_main_loop_begin();

exit:
   edje_edit_string_list_free(collections);
   edje_edit_string_list_free(images);
   edje_edit_string_list_free(sounds);
   edje_edit_string_list_free(fonts);
   ecore_evas_free(ee);
   free(spath);
   free(sedj);

   edje_shutdown();
   ecore_evas_shutdown();
   ecore_shutdown();
   eina_shutdown();

   return exit_status;
}
