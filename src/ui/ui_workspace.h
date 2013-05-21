#ifndef UI_WORKSPACE_HEADER_H
#define UI_WORKSPACE_HEADER_H

#include <Evas.h>
#include <Elementary.h>
#include "ui_ruler.h"


struct _Workspace
{
	Evas *canvas;
	Evas_Object *bg;
	int zoom_step;
	Evas_Object *ruller_hor;
	Evas_Object *ruller_ver;
	Evas_Object *button_zoom_out;
	Evas_Object *button_zoom_in;
	Evas_Object *button_separate;
};
typedef struct _Workspace Workspace;

Eina_Bool
ws_bg_set (Evas_Object *bg);

void
ws_zoom_step_set (unsigned int step, Workspace *ws);

int
ws_zoom_step_get (Workspace *ws);


Workspace *
ws_add (Evas_Object *layout);

Workspace *
ws_init (void);

#endif /* UI_WORKSPACE_HEADER_H */
