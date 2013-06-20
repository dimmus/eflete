#ifndef COLORCLASS_VIEWER_DIALOG_H
#define COLORCLASS_VIEWER_DIALOG_H

/**
 * @defgroup Colorclass_Viewer Colorclass_Viewer
 * @ingrup Dialogs
 *
 * Show color classes, which uses in project.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
/**
 * Add new colorclass viewer inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object.
 *
 * @ingroup Colorclass_Viewer
 */
Evas_Object *
colorclass_viewer_add(Evas_Object *parent);

/**
 * Initialize colorclass viewer. It's method show color classes of project.
 *
 * @param ccl_view Pointer to inwin object, which was created with
 * colorclass_viewer_add method.
 * @param project A project that was early loaded.
 *
 * @ingroup Colorclass_Viewer
 */
void
colorclass_viewer_init(Evas_Object *ccl_view, Project *project);

#endif /*COLORCLASS_VIEWER_DIALOG_H*/
