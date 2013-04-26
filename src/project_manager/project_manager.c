#include "project_manager.h"

Eina_Bool
pm_free(Project *project)
{
	if(!project)
		return EINA_FALSE;

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

	free(project);

	return EINA_TRUE;
}

Project *
pm_open_project_edc(const char *path,
					const char *image_directory,
					const char *font_directory,
					const char *sound_directory)
{
	Project *project;
	char **arr;
	char *tmp;
	char *tmp_path;
	int i, j;

	if(!path)
		return NULL;

	project = calloc(1, sizeof(*project));

	arr = eina_str_split(path, "/", 0);
	i = sizeof(arr);

	// 6 -- 5 chars in a longest word "sound" + null-charecter
	tmp_path = malloc(sizeof(path) + 6);
	strcpy(tmp_path, arr[0]);
	for(j = 1; j < i; i++)
	{
		strcat(tmp_path, "/");
		strcat(tmp_path, arr[j]);
	}

	// set project name
	project->name = strdup(arr[i - 1]);

	// set path to edc
	project->edc = strdup(path);

	// set path to edj
	project->edj = strdup(path);
	tmp = strstr(project->edj, ".edc");
	strncpy(tmp, ".edj", 4);

	// set path to swap file
	project->swapfile = strdup(tmp_path);
	strcat(project->swapfile, "/.swapfile");

	// set path to image directory
	if(!image_directory)
		project->image_directory = strdup(image_directory);
	else // set default path to image directory
	{
		project->image_directory = strdup(tmp_path);
		strcat(project->image_directory, "/images/");
	}

	// set path to font directory
	if(!font_directory)
		project->font_directory = strdup(font_directory);
	else // set default path to font direcotry
	{
		project->font_directory = strdup(tmp_path);
		strcat(project->font_directory, "/fonts/");
	}

	// set default path to sound directory
	if(!sound_directory)
		project->sound_directory = strdup(tmp_path);
		strcat(project->sound_directory, "/sounds/");

	// compile project and create swapfile
	project->compiler = compile(project->edc,
								project->edj,
								project->image_directory,
								project->font_directory,
								project->sound_directory);
	if(project->compiler)
		if(!ecore_file_cp(project->edj, project->swapfile))
			free(project->swapfile);

	// free a temp strings
	 free(arr[0]);
	 free(arr);
	 free(tmp_path);

	return project;
}

Eina_Bool
pm_save_project_edj(Project *pm)
{
	if(!pm)
		return EINA_FALSE;

	ecore_file_cp(pm->swapfile, pm->edj);
	return EINA_TRUE;
}
