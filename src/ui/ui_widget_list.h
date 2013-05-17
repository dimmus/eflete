#ifndef UI_WIDGET_LIST
#define UI_WIDGET_LIST

#include "efl_tet.h"
#include "pm.h"
#include "ui_main_window.h"
#include <Elementary.h>

/**
 *
 */
Evas_Object *
ui_widget_list_add(App_Data *ap, Eina_Inlist *widget_list);

#endif /* UI_WIDGET_LIST */
