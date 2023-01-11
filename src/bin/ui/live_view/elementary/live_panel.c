#include "live_elementary_widgets.h"

static Elm_Panel_Orient
_panel_orient_get(const char *class)
{
   assert(class != NULL);

   Elm_Panel_Orient orient = ELM_PANEL_ORIENT_LEFT;

   if (strcmp(class, "top") == 0)
     orient = ELM_PANEL_ORIENT_TOP;
   else if (strcmp(class, "bottom") == 0)
     orient = ELM_PANEL_ORIENT_BOTTOM;
   else if (strcmp(class, "left") == 0)
     orient = ELM_PANEL_ORIENT_LEFT;
   else if (strcmp(class, "right") == 0)
     orient = ELM_PANEL_ORIENT_RIGHT;

   return orient;
}

Evas_Object *
widget_panel_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_panel_add(parent);
   elm_panel_orient_set(object, _panel_orient_get(group->class));

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
