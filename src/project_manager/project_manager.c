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
pm_project_add(const char *path,
				const char *id, /* image directory */
				const char *fd, /* font directory */
				const char *sd  /* sound directory */)
{
	Project *pro;
	char *tmp = NULL;
	char *tmp_path = NULL;
	char **arr;
	int i, j;

	pro = calloc(1, sizeof(*pro));

	arr = eina_str_split(path, "/", 0);
	i = sizeof(arr);

	/* 6 -- 5 chars in a longest word "sound" + null-charecter */
	tmp_path = malloc(sizeof(path) + 6);
	strcpy(tmp_path, arr[0]);
	for(j = 1; j < i; i++)
	{
		strcat(tmp_path, "/");
		strcat(tmp_path, arr[j]);
	}

	/* set project name */
	pro->name = strdup(arr[i - 1]);

	/* set path to edc */
	pro->edc = strdup(path);

	/* FIXME: A substirng '.edj' can meet in the middle of the path */
	tmp = strstr(pro->edc, ".edj");
	if(tmp != NULL)
		strncpy(tmp, ".edc", 4);

	/* set path to edj */
	pro->edj = strdup(path);

	/* FIXME: A substring '.edc' can meet in the middle of the path */
	tmp = strstr(pro->edj, ".edc");
	if(tmp != NULL)
		strncpy(tmp, ".edj", 4);

	/* set path to swap file */
	pro->swapfile = strdup(tmp_path);
	strcat(pro->swapfile, ".swapfile");

	/* set path to image directory */
	if(!id)
		pro->image_directory = strdup(id);
	else /* set default path to image directory */
	{
		pro->image_directory = strdup(tmp_path);
		strcat(pro->image_directory, "images/");
	}

	/* set path to font directory */
	if(!fd)
		pro->font_directory = strdup(fd);
	else /* set default path to font direcotry */
	{
		pro->font_directory = strdup(tmp_path);
		strcat(pro->font_directory, "fonts/");
	}

	/* set default path to sound directory */
	if(!sd)
		pro->sound_directory = strdup(tmp_path);
		strcat(pro->sound_directory, "sounds/");

	/* free a temp strings */
	free(arr[0]);
	free(arr);
	free(tmp_path);

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
	if(project->compiler)
		if(!ecore_file_cp(project->edj, project->swapfile))
			free(project->swapfile);

	return project;
}

Project *
pm_open_project_edj(const char *path)
{
	Project *project;

	if(!path)
	{
		ERR("Can not open project path is NULL!");
		return NULL;
	}

	project = pm_project_add(path, NULL, NULL, NULL);

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
