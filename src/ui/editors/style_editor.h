#ifndef STYLE_VIEWER_DIALOG_HEADER_H
#define STYLE_VIEWER_DIALOG_HEADER_H

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
#include "style_edit.h"

/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @param parent - the object that will be the parent of this dialog.
 * @param project -currently opened project
 *
 * @return the inwin object that contain style viewer
 *
 * @ingroup TextStyle
 */
Evas_Object *
style_editor_window_add(Evas_Object *parent, Project *project);


#endif  /* STYLE_VIEWER_DIALOG_HEADER_H */
