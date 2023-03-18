#include "live_widget.h"
#include "project_manager2.h"
#include "live_elementary_widgets.h"

Evas_Object *
demo_add(Evas_Object *parent, Group2 *group)
{
   Evas_Object *obj = NULL;

   if (group->widget)
     {
        obj = live_widget_create(group, parent);
        if (!obj)
           ERR(N_("Widget live view isn't implemented yet. Using fallback to layout"));
        else
        {
           Elm_Theme *theme = elm_theme_new();
           elm_theme_set(theme, ap.project->dev);
           elm_object_theme_set(obj, theme);
           elm_theme_free(theme);
           return obj;
        }
     }
   /* if widget is not created, need use the layout */
   if (!obj)
     {
        ERR(N_("Widget live view isn't implemented yet. Using fallback to layout"));
        obj = layout_custom_create(parent, group);
     }

   assert(obj != NULL);

   return obj;
}
