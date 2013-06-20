#ifndef IMAGE_VIEWER_DIALOG_H
#define IMAGE_VIEWER_DIALOG_H

/**
 * @defgroup Image_Viewer Images_Viewer
 * @ingroup Dialogs
 *
 * Image viewer its subwindow displays all the images used by the project.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
/**
 * Add new image viewer inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object, which contain image grid and buttons.
 *
 * @ingroup Image_Viewer
 */
Evas_Object *
image_viewer_add(Evas_Object *parent);

/**
 * Initialize image viewer. It's method show images of the project on image
 * viewer dialog window.
 *
 * @param img_view Pointer to inwin object, which was created with
 * image_viewer_add method.
 * @param project A project that was early loaded.
 *
 * @ingroup Image_Viewer
 */
void
image_viewer_init(Evas_Object *img_view, Project *project);

#endif /*IMAGE_VIEWER_DIALOG_H*/
