/**
 * @defgroup ELM_MAIN ELM_MAIN
 * @ingroup EFL_TET
 *
 * The begging point of the programm
 * (contain starting function like main and
 *some functions that free/init base required components)
 */

#ifndef EFL_TET_H
#define EFL_TET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Eina.h>
#include <Ecore.h>
#include <Evas.h>
#include <Eet.h>
#include <Efreet.h>
#include <Elementary.h>
#include "alloc.h"
#include "logger.h"
#include "project_manager.h"
#include "ui_workspace.h"
#include "notify.h"

struct _App_Data
{
	Evas_Object *win;
	Evas_Object *win_layout;
	Evas_Object *main_menu;
    Evas_Object *inwin;
	struct {
		Evas_Object *left_top;
		Evas_Object *left_bottom;
		Evas_Object *bottom_left;
		Evas_Object *bottom_right;
		Evas_Object *right_top;
		Evas_Object *right_bottom;
		Evas_Object *canvas;
	} block;

	Workspace *ws;
	Project *project;
};

/**
 *
 */
typedef struct _App_Data App_Data;
EAPI_MAIN int elm_main();

/**
 * This function inits all libraries required by this programm.
 * Also it will add extension to all edc files required by this programm.
 *
 * @return EINA_TRUE all libraries initialized succesfully.
 * EINA_FALSE one of the libraries was failed to inti.
 *
 * @ingroup EFL_TET
 */
Eina_Bool
app_init(void);

/**
 * Shutdown of the program. It'll free App_Data, remove extension theme
 * (custom EDC needed for this program) and shutdown all launched
 * and inited libraries.
 *
 * @ingroup EFL_TET
 */
void
app_shutdown(void);

/**
 * This function will return win_layout (if App_Data and win_layout exists).
 * If they are not exist, then this function will return NULL.
 * This function is usable for notification.
 *
 * @return win_layout if exist, NULL if not exist.
 *
 * @ingroup EFL_TET
 */
Evas_Object *
win_layout_get(void);

/**
 * Allocate memory for creating App_Data.
 *
 * @return link to created empty App_Data.
 *
 * @ingroup EFL_TET
 */
App_Data *
app_create(void);

/**
 * This function will free allocated memory used for App_Data.
 *
 * @param ap - App_Data of this program for removing before program shutdown.
 *
 * @ingroup EFL_TET
 */
void
app_free(App_Data *ap);

#endif /* EFL_TET_H */
