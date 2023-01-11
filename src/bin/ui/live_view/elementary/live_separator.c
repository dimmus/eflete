#include "live_elementary_widgets.h"

Evas_Object *
widget_separator_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   Evas_Object *rect, *object, *separator;
   Eina_Bool is_horizontal;

   object = elm_box_add(parent);
   separator = elm_separator_add(parent);

   elm_object_style_set(separator, group->style);

   is_horizontal = !strcmp(group->class, "horizontal");
   elm_box_horizontal_set(object, !is_horizontal);
   elm_separator_horizontal_set(separator, is_horizontal);

#define _ADD_RECT(COLOR_BLUE) \
        rect = evas_object_rectangle_add(evas_object_evas_get(parent)); \
        evas_object_color_set(rect, COLOR_BLUE); \
        evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
        evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL); \
        elm_box_pack_end(object, rect); \
        evas_object_show(rect);

   _ADD_RECT(COLOR_BLUE_LIGHT);
   elm_box_pack_end(object, separator);
   evas_object_show(separator);
   _ADD_RECT(COLOR_BLUE_DARK);

#undef _ADD_RECT

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
