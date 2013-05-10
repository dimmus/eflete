#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include "edje_compile.h"
#include "logger.h"
#include <Edje.h>
#include <Ecore.h>
#include <string.h>

struct _Project
{
	char *name;
	char *edc;				// path to edc file(open/save)
	char *edj;				// path to edj file(open/save)
	char *swapfile;			// path to swap(work) file, need for undo and redo
	Edje_CC *compiler;
	Edje_DeCC *decompiler;
	char *image_directory;	// path to image directory, for compile
	char *font_directory;	// path to font directory, for compile
	char *sound_directory;	// path to sound direcory, for compile

	Eina_List widgets;	// list of widgets and they styles in that theme
};

typedef struct _Project Project;

/**
 *
 */
Project *
pm_open_project_edc(const char *path,
					const char *image_directory,
					const char *font_directory,
					const char *sound_direcotory);

/**
 *
 */
Project *
pm_open_project_edj(const char *path);

/**
 *
 */
//Eira_Bool
//pm_save_project_edc(Project *pm);

/**
 *
 */
Eina_Bool
pm_save_project_edj(Project *pm);

/**
 *
 */
//Project *
//pm_project_new(void);

/**
 *
 */
Eina_Bool
pm_free(Project *project);

#endif /* PROJECT_MANAGER_H */
