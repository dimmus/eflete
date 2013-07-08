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

/**
 *
 */
Eina_Bool
app_init(void);

/**
 *
 */
void
app_shutdown(void);

/**
 *
 */
Evas_Object *
win_layout_get(void);

/**
 *
 */
App_Data *
app_create(void);

/**
 *
 */
void
app_free(App_Data *ap);

#endif /* EFL_TET_H */
