#include "live_elementary_widgets.h"

Evas_Object *
widget_actionslider_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_actionslider_add(parent);
   elm_actionslider_magnet_pos_set(object, ELM_ACTIONSLIDER_ALL);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, send_signal, object);

   elm_object_style_set(object, group->style);

   return object;
}
