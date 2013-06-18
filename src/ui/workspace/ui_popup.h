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


/**
 * Show popup object on workspace.
 *
 * @param parent An Evas_Object that pointed to workspace object.
 * @param popup A popup object, which must be showed.
 *
 * @ingroup Popup
 */
void
ui_popup_show (Evas_Object *parent, Evas_Object *popup);

/**
 * Hide popup object.
 *
 * @param obj A popup object, which must be hided.
 *
 * @ingroup Popup
 */
void
ui_popup_hide (Evas_Object *obj);

#endif /* UI_POPUP_HEADER_H */
