#ifndef _EFL_TET_
#define _EFL_TET_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Eina.h>
#include <Ecore.h>
#include <Evas.h>
#include <Eet.h>
#include <Efreet.h>
#include <Elementary.h>
#include "logger.h"
#include "pm.h"
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

typedef struct _App_Data App_Data;

Eina_Bool
efl_tet_init (void);

void
efl_tet_shutdown (void);

App_Data *
app_create (void);

#endif /* _EFL_TET_ */
