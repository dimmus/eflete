#ifndef LIVE_WIDGET_H
#define LIVE_WIDGET_H

/**
 * @defgroup Live_Widget Live_Widget
 * @ingroup Live_View
 *
 * SubModule used to create widgets with all information they need for
 * showing them in live view and in live view properties.
 */

#include "eflete.h"
#include "resource_manager2.h"

/**
 * Create widget according to it's name, class and style.
 *
 * This function will create widget/widgets for showing loaded group in Live View.
 * Created widget will contain callback functions as a data in it, so they are able
 * to set up in Live View Propery for setting text and swallows of that widget.
 *
 * @param group current loaded widget.
 * @param parent Parent object of created widget.
 *
 * @return Evas_Object of created widget.
 *
 * @ingroup Live_View_Widget
 */
Evas_Object *
live_widget_create(Group2 *group, Evas_Object *parent);

/**
 * Delete previously created widget object.
 *
 * This function delete created widget object and also delete all data (for example
 * callback functions) that are exists in given widget object.
 * if there is no widgets inside of that widget, then it probably wrong object and
 * function will return EINA_FALSE.
 *
 * @param widget created widget object for deleting.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View_Widget
 */
Eina_Bool
live_widget_del(Evas_Object *widget);


#endif /* LIVE_WIDGET_H */
