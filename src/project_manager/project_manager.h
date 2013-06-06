#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

/**
 * @defgroup ProjectManager ProjectManager
 *
 * It is a basic object. Project manager object consist a information
 * about opened project. Project manager object consist the list of widgets
 * styles @ref widget_manger.h
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Eio.h>
#include "edje_compile.h"
#include "logger.h"
#include <string.h>

struct _Project
{
   /**< name of project */
   char *name;
   /**< path to edc file(open/save) */
   char *edc;
   /**< path to edj file(open/save) */
   char *edj;
   /**< path to swap(work) file */
   char *swapfile;
   /**<  */
   Edje_CC *compiler;
   /**<  */
   Edje_DeCC *decompiler;
   /**< path to image directory, for compile */
   char *image_directory;
   /**< path to font directory, for compile */
   char *font_directory;
   /**< path to sound direcory, for compile */
   char *sound_directory;
   /**< list of widgets and they styles in that theme */
   Eina_Inlist *widgets;
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
 * Open project from edj-file
 *
 * @param path path to edj-file
 * @return the Project object
 *
 * @ingroup ProjectManager
 */
Project *
pm_open_project_edj(const char *path);

/**
 *
 */
/*TODO: Add save project pm_save_project_edc(Project *pm) */

/**
 *
 */
/*TODO: Add new project, template project. pm_project_new(void) */

/**
 * Delete the Project object
 *
 * @param project the given Project object
 * @return EINA_TRUE - object deleted, EINA_FALSE - can not delete object
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_free(Project *project);

#endif /* PROJECT_MANAGER_H */
