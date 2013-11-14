#ifndef UI_DEMOSPACE_HEADER_H
#define UI_DEMOSPACE_HEADER_H

/**
 * @defgroup Demospace Demospace
 *
 * A Demospace is responsible for showing widgets.
 * After some user manipulation of parts or states or programs and after
 * saving the result into file, this space will show the resulted widget
 * by using user's theme (already saved one).
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Evas.h>
#include <Elementary.h>
#include "widget_manager.h"
#include "project_manager.h"
#include "common_macro.h"

/**
 * @typedef Demospace
 *
 * Contain objects to show demo of widget (live view).
 * A canvas object get handle to an Evas canvas.
 * layout - Layout of the live view.
 * object - current widget being used for demostration view.
 * And groupspace.
 */
struct _Demospace
{
   Evas_Object *groupspace;
   Evas_Object *layout;
   Evas_Object *object;
   double current_scale;
   Elm_Theme *th;
};
typedef struct _Demospace Demospace;

/**
 * Add new demospace object.
 *
 * This function will create Evas_Object of "live view" of widget.
 * It uses layout that contain buttons for zooming widget (by changing scale),
 * and also it contain groupspace (for showing wiodget example).
 *
 * @param parent The parent Evas_Object.
 * @return Pointer to demospace structure.
 *
 * @ingroup Demospace
 */
Demospace *
ui_demospace_add(Evas_Object *parent);

/**
 * Set and show selected group and apply style to it.
 *
 * This function will apply changed style to the demo widget.
 * It will copy all groups from swap file to demo file, and set the groupname of
 * current group into the name that can be further used for showing on a widget
 * as extension theme.
 *
 * @param demo demospace structure.
 * @param project project structure that contain path to swap, demo and edj files.
 * @param group group that contain current group name is being showed.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Demospace
 */
Eina_Bool
ui_demospace_set(Demospace *demo, Project *project, Group *group);

/**
 * Set and show selected group and apply style to it.
 *
 * This function will update the groupspace view for showing demo widget
 * properly.
 *
 * @param demo demospace structure.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Demospace
 */
Eina_Bool
ui_demospace_unset(Demospace *demo, Project *project);

/**
 * Set and show selected group and apply style to it.
 *
 * This function will update the groupspace view for showing demo widget
 * properly.
 *
 * @param demo demospace structure.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise;
 *
 * @ingroup Demospace
 */
Eina_Bool
ui_demospace_update(Demospace *demo);

#endif /* UI_DEMOSPACE_HEADER_H */
