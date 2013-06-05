#include "project_manager.h"
#include "widget_manager.h"

Eina_Bool
pm_free(Project *project)
{
   if(!project) return EINA_FALSE;

   INFO ("Closed project: %s", project->name);

   free(project->name);
   free(project->edc);
   free(project->edj);
   free(project->swapfile);
   free(project->image_directory);
   free(project->font_directory);
   free(project->sound_directory);
   if(project->compiler)
        compiler_free(project->compiler);
   if(project->decompiler)
        decompiler_free(project->decompiler);

   //wm_widget_list_free(project->widgets);

   free(project);
   DBG ("Project data is released.");

   return EINA_TRUE;
}

Project *
pm_project_add(const char *path,
               const char *id, /* image directory */
               const char *fd, /* font directory */
               const char *sd  /* sound directory */)
{
   Project *pro;
   char *tmp = NULL;
   char tmp_path[PATH_MAX];
   Eina_Array *array;
   size_t siz, siz_tmp_path;
   int i, j;

   pro = calloc(1, sizeof(*pro));

   array = eina_file_split(strdup(path));
   i = eina_array_count(array);
   eina_strlcpy(tmp_path, "/", sizeof("/"));
   for (j = 0; j < i - 1; j++)
     {
        eina_strlcat(tmp_path, eina_array_data_get(array, j), PATH_MAX);
        eina_strlcat(tmp_path, "/", PATH_MAX);
     }
   DBG("%s", tmp_path);
   siz_tmp_path = strlen(tmp_path);

   /* set project name */
   pro->name = strdup(eina_array_data_get(array, i - 1));
   eina_array_free(array);
   DBG ("Project name: '%s'", pro->name);

   /* set path to edc */
   pro->edc = strdup(path);

   /* FIXME: A substirng '.edj' can meet in the middle of the path */
   tmp = strstr(pro->edc, ".edj");
   if (tmp != NULL)
     {
        strncpy(tmp, ".edc", 4);
     }
   tmp = NULL;

   /* set path to edj */
   pro->edj = strdup(path);

   /* FIXME: A substring '.edc' can meet in the middle of the path */
   tmp = strstr(pro->edj, ".edc");
   if (tmp != NULL)
     {
        strncpy(tmp, ".edj", 4);
     }
   tmp = NULL;
   DBG ("Path to edc-file: '%s'", pro->edc);
   DBG ("Path to edj-file: '%s'", pro->edj);

   /* set path to swap file */
   siz = siz_tmp_path + strlen(".swapfile_") + strlen(pro->name);
   pro->swapfile = calloc(siz, sizeof(char));
   eina_strlcat(pro->swapfile, tmp_path, PATH_MAX);
   eina_strlcat(pro->swapfile, ".swapfile_", PATH_MAX);
   eina_strlcat(pro->swapfile, pro->name, PATH_MAX);
   DBG ("Path to swap file: '%s'", pro->swapfile);

   /* set path to image directory */
   if (id)
     {
        pro->image_directory = strdup(id);
     }
   /* set default path to image directory */
   else
     {
        siz = siz_tmp_path + strlen("images/") + 1;
        pro->image_directory = calloc(siz, sizeof(char));
        eina_strlcat(pro->image_directory, tmp_path, PATH_MAX);
        eina_strlcat(pro->image_directory, "images/", PATH_MAX);
     }
   DBG ("Path to image direcotory: '%s'", pro->image_directory);

   /* set path to font directory */
   if (fd)
     {
        pro->font_directory = strdup(fd);
     }
   /* set default path to font direcotry */
   else
     {
        siz = siz_tmp_path + strlen("fonts/") + 1;
        pro->font_directory = calloc(siz, sizeof(char));
        eina_strlcat(pro->font_directory, tmp_path, PATH_MAX);
        eina_strlcat(pro->font_directory, "fonts/", PATH_MAX);
     }
   DBG("Path to font direcotory: '%s'", pro->font_directory);

   /* set default path to sound directory */
   if (sd)
     {
        pro->sound_directory = strdup(sd);
     }
   else
     {
        siz = siz_tmp_path + strlen("sounds/") + 1;
        pro->sound_directory = calloc(siz, sizeof(char));
        eina_strlcat(pro->sound_directory, tmp_path, PATH_MAX);
        eina_strlcat(pro->sound_directory, "sounds/", PATH_MAX);
     }
   DBG ("Path to sound direcotory: '%s'", pro->sound_directory);

   /* without this like widgets link is set into NULL.
      Maybe this code (or something that looks like this one)
      is existing somewhere else, but I didn't find it */
   pro->widgets = wm_widget_list_new(pro->edj);

   return pro;
}

Project *
pm_open_project_edc(const char *path,
                    const char *image_directory,
                    const char *font_directory,
                    const char *sound_directory)
{
   Project *project;

   if(!path)
     {
        ERR("Can not open project path is NULL!");
        return NULL;
     }

   project = pm_project_add(path,
                            image_directory,
                            font_directory,
                            sound_directory);

   /* compile project and create swapfile */
   project->compiler = compile(project->edc,
                               project->edj,
                               project->image_directory,
                               project->font_directory,
                               project->sound_directory);
   if (project->compiler)
     {
        ecore_file_cp(project->edj, project->swapfile);
     }

   return project;
}

Project *
pm_open_project_edj(const char *path)
{
   Project *project;

   if (!path)
     {
        ERR("Can not open project path is NULL!");
        return NULL;
     }

   INFO("Open project! Path to project: '%s'.", path);
   project = pm_project_add(path, NULL, NULL, NULL);
   ecore_file_cp(project->edj, project->swapfile);
   INFO("Project '%s' is open!", project->name);

   return project;
}

Eina_Bool
pm_save_project_edj(Project *pm)
{
   if (!pm) return EINA_FALSE;

   ecore_file_cp(pm->swapfile, pm->edj);
   return EINA_TRUE;
}
