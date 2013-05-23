#ifndef UI_WIDGET_LIST_H
#define UI_WIDGET_LIST_H

#include "efl_tet.h"
#include "pm.h"
#include "ui_main_window.h"
#include "ui_property.h"
#include <Elementary.h>

/**
 *
 */
Evas_Object *
ui_widget_list_add(App_Data *ap, Eina_Inlist *widget_list);

#endif /* UI_WIDGET_LIST_H */
