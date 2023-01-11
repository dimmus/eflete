#include "project_manager2.h"
#include "enventor_module.h"

#ifdef HAVE_ENVENTOR

#define MAX_SCALE 3.0
#define MIN_SCALE 0.5
#define STEP_SCALE 0.1f

static void
_on_enventor_mouse_wheel(void *data __UNUSED__,
                         Evas *evas __UNUSED__,
                         Evas_Object *enventor,
                         void *event_info)
{
   Evas_Event_Mouse_Wheel *event = (Evas_Event_Mouse_Wheel *)event_info ;

   if (!evas_key_modifier_is_set(event->modifiers, "Control"))  return;

   double scale = enventor_object_font_scale_get(enventor);
   scale += event->z >= 0 ? -STEP_SCALE: +STEP_SCALE;

   if ((scale <= MIN_SCALE) || (scale >= MAX_SCALE)) return;

   profile_get()->enventor.scale = scale;

   enventor_object_font_scale_set(enventor, scale);
}


Evas_Object *
enventor_object_init(Evas_Object *parent)
{
  Evas_Object *enventor = NULL;
  enventor = enventor_object_add(parent);

  enventor_object_font_scale_set(enventor, profile_get()->enventor.scale);
  evas_object_event_callback_add(enventor, EVAS_CALLBACK_MOUSE_WHEEL,
                                 _on_enventor_mouse_wheel, NULL);
  enventor_object_auto_complete_set(enventor, true);
  enventor_object_auto_indent_set(enventor, true);

  return enventor;
}

Eina_Bool
enventor_object_project_load(Evas_Object *enventor, Project *project)
{
  Eina_Stringshare *file = NULL;
  Eina_Tmpstr *tmpstr = NULL;
  char **tmp;
  unsigned int i = 0, tokens_count = 0;
  Style *style = NULL;
  Eina_Stringshare *path = NULL;

  if ((!enventor) || (!project) || (!project->current_style)) return false;
  style = project->current_style;

  if (!project->enventor)
    project->enventor = (Enventor_Data *)mem_calloc(1, sizeof(Enventor_Data));

  if (!project->enventor->file)
    {
       /* Prepare edc file and resources for using in enventor mode.
        * Project will created in temporary directory (linux: "/tmp";
        * Windows: path of enviroment variables TEMP|TMP|USERPROFILE|WINDIR).
        * Name of project directory compose by next rule: all terminants "/"
        * will replaced with "_" and generate unieque id suffix.
        * For example:
        * elm/bubble/base/default -> /tmp/elm_bubble_base_default_fGhds1/
        */

       tmp = eina_str_split_full(style->full_group_name, "/", 0, &tokens_count);
       if (!tmp[0]) return false;
       file = eina_stringshare_add(tmp[0]);
       for (i = 1; tokens_count - 1 > 0; i++, tokens_count--)
        file = eina_stringshare_printf("%s_%s", file, tmp[i]);
       free(tmp[0]);
       free(tmp);
       path = eina_stringshare_printf("%s_XXXXXX", file);
       eina_file_mkdtemp(path, &tmpstr);
       eina_stringshare_del(path);

       project->enventor->file = eina_stringshare_printf("%s/%s.edc", tmpstr, file);
       project->enventor->path = eina_stringshare_add(tmpstr);

       eina_tmpstr_del(tmpstr);
    }

  if (!pm_project_style_source_code_export(project, style, project->enventor->file))
    ERR("Source code of the current style was not written to the file %s", project->enventor->file);

  enventor_object_file_set(enventor, project->enventor->file);

  enventor_object_path_set(enventor, ENVENTOR_PATH_TYPE_FONT, project->res.fonts);
  enventor_object_path_set(enventor, ENVENTOR_PATH_TYPE_IMAGE, project->res.images);
  enventor_object_path_set(enventor, ENVENTOR_PATH_TYPE_SOUND, project->res.sounds);
  return true;
}

Eina_Bool
enventor_object_project_unload(Project *project)
{
   if ((!project) || (!project->enventor)) return false;

   if (project->enventor->path)
     {
        ecore_file_recursive_rm(project->enventor->path);
        eina_stringshare_del(project->enventor->path);
     }

   eina_stringshare_del(project->enventor->file);
   project->enventor->file = NULL;

   return true;
}

Eina_Bool
enventor_object_profile_load(Evas_Object *enventor, Profile *profile)
{
   if ((!enventor) || (!profile))
     return false;
   enventor_object_font_scale_set(enventor, profile->enventor.scale);
   return true;
}

Eina_Bool
enventor_object_file_version_update(Evas_Object *enventor, Project *project, const char *key)
{
   char *code, *new_code;
   char *version_ptr, *search_ptr;
   long long f_size;
   size_t code_r;
   FILE *f;
   long int concat_pos = 1;
   Eina_Stringshare *data_str = NULL;
   long int data_len = 0;
   int cursor_pos = 0;

   if ((!enventor) || (!project) || (!key))
     return false;

   cursor_pos = enventor_object_cursor_pos_get(enventor);
   enventor_object_save(enventor, project->enventor->file);

   f_size = ecore_file_size(project->enventor->file);
   f = fopen(project->enventor->file, "r+");
   if (!f)
     {
        ERR("Failed set the Elementary version support to '%s'",
            project->enventor->file);
        return false;
     }
   code = mem_calloc(1, f_size);
   code_r = fread(code, 1, f_size, f);
   if (code_r == 0)
     {
        free(code);
        fclose(f);
        eina_stringshare_del(data_str);
        return false;
     }
   version_ptr = strstr(code, "\"version\"");
   data_str = eina_stringshare_printf("\"version\" \"%s\";", key);
   if (!version_ptr)
     {
        rewind(f);
        fputs("data.item: ", f);
        fputs(data_str, f);
        fputs("\n\n", f);
        fputs(code, f);
     }
   else
     {
        fclose(f);
        f = fopen(project->enventor->file, "w");

        /*Search position where item block ends.*/
        for (search_ptr = version_ptr;
             ((!search_ptr) || (*search_ptr != ';'));
             search_ptr++, concat_pos++);

        /*
         * Position of the source code, where it should be merged
         * into temporary buffer.
         */
         concat_pos += (version_ptr - code);

         /*
         * Copy data into temporary buffer with replacing
         * "version" "value"; to "version" "110";
         */
        if (search_ptr)
          {
             data_len = strlen(data_str);
             /*
              * Calculating new size:
              * size before "version" + size of new string + size of rest code
              */
             new_code = mem_calloc(1, (version_ptr - code) + data_len + strlen(code + concat_pos));

             /* Copying code, that was before "version" */
             memcpy(new_code, code, (version_ptr - code));

             /* Add string: "version" "110"; */
             memcpy(new_code + (version_ptr - code), data_str, data_len);

             /* Add rest of source code */
             memcpy(new_code + data_len + (version_ptr - code), code + concat_pos,
                    strlen(code + concat_pos));

             fputs(new_code, f);
             free(new_code);
          }
     }
   free(code);
   fclose(f);
   enventor_object_file_set(enventor, project->enventor->file);
   enventor_object_cursor_pos_set(enventor, cursor_pos);
   eina_stringshare_del(data_str);
   return true;
}

#endif /* HAVE_ENVENTOR */
