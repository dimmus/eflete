#ifndef FONT_VIEWER_DIALOG_H
#define FONT_VIEWER_DIALOG_H

/**
 * @defgroup Font_Viewer Font_Viewer
 * @ingroup Dialogs
 *
 * Font viewer its subwindow displays all the fonts used by the project.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
/**
 * Add new font viewer inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object.
 *
 * @ingroup Font_Viewer
 */
Evas_Object *
font_viewer_add(Evas_Object *parent);

/**
 * Initialize font viewer. It's method show fonts of the project in font
 * viewer dialog window.
 *
 * @param font_view Pointer to inwin object, which was created with
 * font_viewer_add method.
 * @param project A project that was early loaded.
 *
 * @ingroup Font_Viewer
 */
void
font_viewer_init(Evas_Object *font_view, Project *project);

#endif /*FONT_VIEWER_DIALOG_H*/
