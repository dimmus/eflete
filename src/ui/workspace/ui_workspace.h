#ifndef UI_WORKSPACE_HEADER_H
#define UI_WORKSPACE_HEADER_H

#include <Evas.h>
#include <Elementary.h>
#include "ui_ruler.h"
#include "ui_popup.h"


struct _Workspace
{
	Evas *canvas;
	Evas_Object *bg;
	int zoom_step;
	Evas_Object *ruler_hor;
	Evas_Object *ruler_ver;
	Evas_Object *popup;
	Evas_Object *button_zoom_out;
	Evas_Object *button_zoom_in;
	Evas_Object *button_separate;
};
typedef struct _Workspace Workspace;

/**
 * Set zoom step for workspace.
 * in: step Step size.
 * in: ws Workspace structure.
 */
void
ws_zoom_step_set (unsigned int step, Workspace *ws);

/**
 * Get zoom step for workspace.
 * in: ws Workspace structure.
 * out: integer value of step size.
 */
int
ws_zoom_step_get (Workspace *ws);

/**
 * Add new workspace object to the given parent Elemntary (container) object
 * in: parent The parent object.
 * out: pointer to an Workspace structure in wich store workspace elements.
 */
Workspace *
ws_add (Evas_Object *parent);

/**
 * Add new context popup to the given parent Elemntary (container) object.
 * in: parent The parent object.
 * in: ws Pointer to an Workspace structure. (need for use in callbacks)
 */
Evas_Object *
ui_popup_add (Evas_Object *parent, Workspace *ws);
#endif /* UI_WORKSPACE_HEADER_H */
