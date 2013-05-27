#ifndef UI_POPUP_HEADER_H
#define UI_POPUP_HEADER_H

#include "logger.h"
#include "Elementary.h"

/*Evas_Object *
ui_popup_add (Evas_Object *parent, Workspace *ws);
*/
void
ui_popup_show (Evas_Object *parent, Evas_Object *popup);

void
ui_popup_hide (Evas_Object *obj);

#endif /* UI_POPUP_HEADER_H */
