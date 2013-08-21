#ifndef ADD_STATE_DIALOG_H
#define ADD_STATE_DIALOG_H

/**
 * @defgroup New_State_Dialog New_State_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for choise part type.
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
/**
 * Add new dialog for input params state, like name and value.
 *
 * @param ap The App_Data struct pointer.
 * @return Pointer to inwin object.
 *
 * @ingroup New_Part_Dialog
 */
Evas_Object *
new_state_dialog_add(App_Data *ap);

#endif /* ADD_STATE_DIALOG_H */
