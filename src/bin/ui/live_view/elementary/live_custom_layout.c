#include "live_elementary_widgets.h"

Evas_Object *
layout_custom_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);

   Evas_Object *obj = elm_layout_add(parent);
   if (!elm_layout_file_set(obj, ap.project->dev, group->common.name))
     {
        ERR(N_("Could not load group '%s' from mapped file '%s'."), group->common.name, ap.project->dev);
        evas_object_del(obj);
        obj = NULL;
        TODO("Add frame to container with info that need this state is unstable"
             "and need to restart Eflete.");
        return NULL;
     }

   evas_object_smart_callback_add(obj, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(obj, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(obj, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);
   evas_object_clip_set(obj, evas_object_clip_get(parent));

   return obj;
}
