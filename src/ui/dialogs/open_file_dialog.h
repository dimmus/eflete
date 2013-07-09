#ifndef OPEN_FILE_DIALOG_H
#define OPEN_FILE_DIALOG_H

#include "efl_tet.h"
#include "alloc.h"
#include "ui_block.h"
#include "ui_widget_list.h"

/**
 * @defgroup OpenFileDialog Open file dialog
 * The widget provide a open project functionality.
 */

/**
 * Create and show a dialog for open project from a edj file.
 *
 * @param ap The Application data.
 * @return EINA_TRUE if a project opened, else EIN_FALSE.
 *
 * @ingroup OpenFileDialog
 */
Eina_Bool
open_edj_file(App_Data *ap);

/**
 * Create and show a dialog for open project from a edc file.
 *
 * @param ap The Apllication data.
 * @return EINA_TRUE if a project opened, else EIN_FALSE.
 *
 * @ingroup OpenFileDialog
 */
Eina_Bool
open_edc_file(App_Data *ap);

#endif /* OPEN_FILE_DIALOG_H */
