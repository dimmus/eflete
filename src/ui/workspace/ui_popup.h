#ifndef UI_POPUP_HEADER_H
#define UI_POPUP_HEADER_H

/**
 * @defgroup Popup Popup
 * @ingroup Workspace
 *
 * Popup module provide some method to manipulate popup object.
 */
#include "logger.h"
#include "Elementary.h"
#include "ui_workspace.h"


/**
 * Show popup object on workspace.
 *
 * @param ws  An workspace structure pointer. Needed to create popup menu on it
 *
 * @ingroup Popup
 */
void
ui_popup_show (Workspace *ws);
#endif /* UI_POPUP_HEADER_H */
