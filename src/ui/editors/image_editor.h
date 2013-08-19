#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

/**
 * @defgroup ImageEditor Image Editor
 *
 * Image editor is subwindow that contain information about
 * all the images used by the project.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "modal_window.h"
#include "widget_manager.h"
#include "image_edit_api.h"

/**
 * Add new image editor inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object, which contain image grid and buttons.
 *
 * @ingroup ImageEditor
 */
Evas_Object *
image_editor_window_add(Evas_Object *parent);

/**
 * Initialize image editor. It's method show images of the project on image
 * editor dialog window.
 *
 * @param img_view Pointer to inwin object, which was created with
 * image_editor_add method.
 * @param project A project that was earlier loaded.
 *
 * @ingroup ImageEditor
 */
void
image_editor_init(Evas_Object *img_edit, Project *project);

#endif /*IMAGE_EDITOR_H*/
