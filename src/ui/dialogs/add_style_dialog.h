#ifndef ADD_STYLE_DIALOG_H
#define ADD_STYLE_DIALOG_H

/**
 * @defgroup New_Style_Dialog New_Style_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for create new style and/or class.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
#include "ui_groupspace.h"
#include "modal_window.h"
#include "widget_define.h"
#include "ui_block.h"
#include "ui_states_list.h"
#include "ui_groupspace.h"
#include "notify.h"
#include "ui_widget_list.h"

/**
 * Add new dialog for input params style and/or class, like names of style and class.
 *
 * @param ap The App_Data struct pointer.
 *
 * @ingroup New_Style_Dialog
 */
void
new_style_dialog_add(App_Data *ap);

#endif /* ADD_STYLE_DIALOG_H */
