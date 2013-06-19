#ifndef IMAGE_VIEWER_DIALOG_H
#define IMAGE_VIEWER_DIALOG_H

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
/**
 *
 */
Evas_Object *
image_viewer_add(Evas_Object *parent);

/**
 *
 */
void
image_viewer_init(Evas_Object *img_view, Project *project);

#endif /*IMAGE_VIEWER_DIALOG_H*/
