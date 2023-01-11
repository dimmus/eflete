#include "live_elementary_widgets.h"

Evas_Object *
widget_label_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = NULL;
   LABEL_ADD(parent, object, _("Some long text for our label, that is"
                               "long but not too long."));
   elm_label_slide_duration_set(object, 0.5);
   elm_label_slide_mode_set(object, ELM_LABEL_SLIDE_MODE_ALWAYS);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
