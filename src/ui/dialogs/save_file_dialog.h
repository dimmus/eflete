#ifndef SAVE_FILE_DIALOG_H
#define SAVE_FILE_DIALOG_H


#include "efl_tet.h"
#include "alloc.h"

/**
 * @defgroup SaveFileDialog Save file dialog
 * The widget provide a save project functionality.
 */

/**
 * Create and show a dialog for save project into a edj file.
 *
 * This function will ask user about the path where he would like
 * to save his file. If the file has wrong extension then it will be error noti
 * will be appeared (@see Notify). If the file is Exist then function will also
 * ask the user if he want to replace the existed file.
 *
 * @param ap The Application data.
 * @return EINA_TRUE if a project saved, else EINA_FALSE.
 *
 * @ingroup SaveFileDialog
 */
Eina_Bool
save_as_edj_file(App_Data *ap);

/**
 * Create and show a dialog for save project into a edc file.
 *
 * @param ap The Apllication data.
 * @return EINA_TRUE if a project saved, else EINA_FALSE.
 *
 * @ingroup SaveFileDialog
 */
Eina_Bool
save_as_edc_file(App_Data *ap);

#endif /* SAVE_FILE_DIALOG_H */
