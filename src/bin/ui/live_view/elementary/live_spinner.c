#include "live_elementary_widgets.h"

Evas_Object *
widget_spinner_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_spinner_add(parent);
   elm_spinner_label_format_set(object, _("%1.1f units"));
   elm_spinner_step_set(object, 1.5);
   elm_spinner_wrap_set(object, true);
   elm_spinner_min_max_set(object, -50.0, 250.0);
   elm_spinner_editable_set(object, true);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
