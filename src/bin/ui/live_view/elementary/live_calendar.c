#include "live_elementary_widgets.h"

Evas_Object *
widget_calendar_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_calendar_add(parent);

   /* Structure containing a calendar date and time
      broken down into its components (see "time.h").
      {sec, min, hour, day of the month, month, year since 1900,
      days since Sunday, days since January 1, Daylight Saving Time flag} */
   TODO("fix warnings on windows")
   struct tm saturday = {0, 0, 0, 0, 1, 114, 6, -1, 0, 0, NULL};
   elm_calendar_mark_add(object, "checked", &saturday,
                                ELM_CALENDAR_WEEKLY);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
