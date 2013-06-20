#ifndef UI_WORKSPACE_HEADER_H
#define UI_WORKSPACE_HEADER_H

/**
 * @defgroup Workspace Workspace
 * @ingroup EFL_TET
 *
 * A Workspace module create and control workspace area. It's contain buttons
 * for zoom operation with workspace, button to split part of showed group.
 * In workspace providet highlight element for show current part.
 * Also Workspace contain two rulers vertical and horizontal with functional
 * like most graphics editors. A popup menu can show by click right button on
 * workspace.
 */

#include <Evas.h>
#include <Elementary.h>
#include "ui_ruler.h"
#include "ui_popup.h"

/**
 * @typedef  Workspace
 *
 * Contain helper objects for manipulate workspace.
 * A canvas object get handle to an Evas canvas.
 */
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
	Evas_Object *groupspace;
	struct {
		Evas_Object *highlight;
		Evas_Object *obj; /* highlighting object */
	} highlight;
};
typedef struct _Workspace Workspace;

/**
 * Set zoom step for workspace.
 *
 * @param step Step size.
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ws_zoom_step_set (unsigned int step, Workspace *ws);

/**
 * Get zoom step for workspace.
 *
 * @param ws Workspace structure.
 * @return integer value of step size.
 *
 * @ingroup Workspace
 */
int
ws_zoom_step_get (Workspace *ws);

/**
 * Add new workspace object to the given parent Elemntary (container) object
 *
 * @param parent The parent object.
 * @return pointer to an Workspace structure in wich store workspace elements.
 *
 * @ingroup Workspace
 */
Workspace *
ws_add (Evas_Object *parent);

/**
 *
 */
void
ws_free(Workspace *ws);

/**
 * Add new context popup to the given parent Elemntary (container) object.
 *
 * @param parent The parent object.
 * @param ws Pointer to an Workspace structure. (need for use in callbacks)
 *
 * @ingroup Workspace
 */
Evas_Object *
ui_popup_add (Evas_Object *parent, Workspace *ws);

/**
 * Set highlight to selected part.
 *
 * @param ws Workspace structure.
 * @param part Evas_Object pointer to selected part object.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_set(Workspace *ws, Evas_Object *part);

/**
 * Move highlight object on workspace.
 *
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_move(Workspace *ws);

/**
 * Hide highlight object.
 *
 * @param ws Workspace structure.
 *
 * @ingroup Workspace
 */
void
ui_object_highlight_hide(Workspace *ws);
#endif /* UI_WORKSPACE_HEADER_H */
