#ifndef UI_LIVEVIEW_HEADER_H
#define UI_LIVEVIEW_HEADER_H

/**
 * @defgroup Live_View Live_View
 *
 * A Live_View is responsible for showing widgets.
 * After user manipulations with parts, states or programs and saving
 * into a file, this space will show the resulted widget
 * by using user's theme (already saved one).
 */

#include "widget_manager.h"
#include "project_manager.h"
#include "common_macro.h"
#include <Evas.h>
#include <Elementary.h>

/**
 * @typedef Live_View
 *
 * Contain objects to show demo of widget (live view).
 */
struct _Live_View
{
   Evas_Object *live_view; /**< live_view object that contain widget for showing,
                                and spinner for scaling */
   Evas_Object *layout; /**< the layout of the whole live view widget */
   Evas_Object *object; /**< current widget for being showed in live view */
   Evas_Object *scale_spinner; /**< spinner that changes scale
                                    of the showed widget */
   double current_scale; /**< current scale value */
};
typedef struct _Live_View Live_View;

/**
 * Add new live view object.
 *
 * This function will create "live view" object for showing widget.
 * It uses layout that contain spinner for scaling widget,
 * and also it contains groupspace (for showing widget example).
 *
 * @param parent The parent Evas_Object.
 * @return Pointer to live_view structure.
 *
 * @ingroup Live_View
 */
Live_View *
live_view_add(Evas_Object *parent);

/**
 * Set and show given widget and apply the style to it for showing the view of
 * of this widget.
 *
 * This function will create widget/widgets for showing loaded group,
 * and create new theme for applying changed style to widget.
 * This function also will hide the scaling spinner.
 *
 * @param live live_view structure.
 * @param project project structure that contain path to swap file.
 * @param style style that contain current style name is being showed.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_widget_style_set(Live_View *live, Project *project, Style *style);

/**
 * Unset widget from live view and hide it aswell.
 *
 * This function will delete and unset all previously created widgets from live
 view and then hide live view and scaling spinner.
 *
 * @param live live_view structure.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_widget_style_unset(Live_View *live);

/**
 * Update changed and loaded project theme.
 *
 * This function will update the widget inside of the live view.
 * If previously loaded theme was changed it will update the widget aswell.
 *
 * @param live live_view structure.
 * @param project project structure that contain path to swap file.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Live_View
 */
Eina_Bool
live_view_theme_update(Live_View *live, Project *project);

/**
 * Freeing the live_view structure.
 *
 * This function will free and delete the structure that contain all
 * information about live view.
 *
 * @param live live_view structure.
 *
 * @ingroup Live_View
 */
void
live_view_free(Live_View *live);

#endif /* UI_LIVEVIEW_HEADER_H */
