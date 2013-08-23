#ifndef ABOUT_DIALOG_HEADER_H
#define ABOUT_DIALOG_HEADER_H

/**
 * @defgroup TextStyle Text Style
 *
 * This widget emits the following signals:
 *
 * TextStyle is a Inwin that contain information
 * of the widgets text styles.
 */

#include "efl_tet.h"
#include "modal_window.h"

/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @param parent - the object that will be the parent of this dialog.
 *
 * @return the inwin object that contain style viewer
 *
 * @ingroup TextStyle
 */
Evas_Object *
about_window_add(Evas_Object *parent);

#endif  /* ABOUT_DIALOG_HEADER_H */
