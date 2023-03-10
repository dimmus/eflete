#include "property.h"
#include "property_private.h"
#include "main_window.h"

static void
_btn_script_manager_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *content = data;

   assert(content != NULL);

   evas_object_smart_callback_call(content, signals.eflete.property.script_control.clicked, NULL);
}

Evas_Object *
property_entry_script_control_add(Evas_Object *parent)
{
   Evas_Object *content, *btn;

   assert(parent != NULL);

   ENTRY_ADD(parent, content, true);
   elm_object_style_set(content, "script");
   btn = elm_button_add(content);
   elm_object_style_set(btn, "elipsis");
   evas_object_smart_callback_add(btn, signals.elm.button.clicked, _btn_script_manager_cb, content);

   elm_object_part_content_set(content, "elm.swallow.elipsis", btn);
   elm_entry_editable_set(content, false);
   evas_object_show(btn);
   evas_object_show(content);

   return content;
}
