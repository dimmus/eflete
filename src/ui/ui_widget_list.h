#ifndef UI_WIDGET_LIST_H
#define UI_WIDGET_LIST_H

/**
 * @defgroup WidgetList WidjetList
 *
 * A Widget List used for view a list of widgets styles
 */

#include "efl_tet.h"
#include "ui_main_window.h"
#include "ui_property.h"
#include "ui_states_list.h"
#include "ui_groupspace.h"
#include <Elementary.h>

/**
 * Add a new Widget List
 *
 * @param ap application data
 * @param widget_list the Eina_Inlist which consist a data of widgets styles
 * @return the widget list object
 */
Evas_Object *
ui_widget_list_add(App_Data *ap, Eina_Inlist *widget_list);

#endif /* UI_WIDGET_LIST_H */
