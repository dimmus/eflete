#ifndef ADD_PART_DIALOG_H
#define ADD_PART_DIALOG_H

/**
 * @defgroup New_Part_Dialog New_Part_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for choise part type.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
#include "ui_groupspace.h"

/**
 * Add new dialog for choise part type add inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object.
 *
 * @ingroup New_Part_Dialog
 */
Evas_Object *
new_part_dialog_add(Evas_Object *parent, Evas_Object *groupspace);

#endif /* ADD_PART_DIALOG_H */
