#include "notify.h"

static void
_notify_close (void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *notify = (Evas_Object *)data;
   evas_object_hide(notify);
}

void
noti_error_show (Evas_Object *obj, const char *message)
{
   Evas_Object *notify, *layout, *image, *entry, *btn;

   //1. setting notify up
   notify = elm_notify_add(obj);
   elm_notify_allow_events_set(notify, EINA_FALSE);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_notify_orient_set(notify, ELM_NOTIFY_ORIENT_CENTER);

   //2. start forming layout
   layout = elm_layout_add(obj);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "notify/content/default");

   //2.1. add an image to the layout
   image = elm_image_add(notify);
   elm_image_file_set(image, TET_IMG_PATH"error.png", NULL);
   elm_object_part_content_set (layout, "image", image);
   evas_object_show(image);

   //2.2 add not editable entry for information viewable
   entry = elm_entry_add(obj);
   elm_entry_editable_set(entry, EINA_FALSE);
   elm_entry_scrollbar_policy_set(entry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set (layout, "information", entry);
   elm_object_text_set(entry, message);
   evas_object_show(entry);

   //2.3. add button for closing the error message
   btn = elm_button_add(obj);
   elm_object_text_set(btn, "Close");
   evas_object_smart_callback_add(btn, "clicked", _notify_close, notify);
   elm_object_part_content_set (layout, "close_button", btn);
   evas_object_show(btn);

   //2.4. show this layout with everything
   evas_object_show (layout);
   elm_object_content_set(notify, layout);
   evas_object_show (notify);
}

void
noti_warning_show (Evas_Object *obj, const char *message)
{
   Evas_Object *notify, *layout, *image, *entry, *btn;

   //1. setting notify up
   notify = elm_notify_add(obj);
   elm_notify_allow_events_set(notify, EINA_FALSE);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_notify_orient_set(notify, ELM_NOTIFY_ORIENT_CENTER);

   //2. start forming layout
   layout = elm_layout_add(obj);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "notify/content/default");

   //2.1. add an image to the layout
   image = elm_image_add(obj);
   elm_image_file_set(image, TET_IMG_PATH"warning.png", NULL);
   elm_object_part_content_set (layout, "image", image);
   evas_object_show(image);

   //2.2 add not editable entry for information viewable
   entry = elm_entry_add(obj);
   elm_entry_editable_set(entry, EINA_FALSE);
   elm_entry_scrollbar_policy_set(entry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set (layout, "information", entry);
   elm_object_text_set(entry, message);
   evas_object_show(entry);

   //2.3. add button for closing the error message
   btn = elm_button_add(obj);
   elm_object_text_set(btn, "Close");
   evas_object_smart_callback_add(btn, "clicked", _notify_close, notify);
   elm_object_part_content_set (layout, "close_button", btn);
   evas_object_show(btn);

   //2.4. show this layout with everything
   evas_object_show (layout);
   elm_object_content_set(notify, layout);
   evas_object_show (notify);
}

void
noti_info_show (Evas_Object *obj, const char *message, double time)
{
   Evas_Object *notify, *layout, *image, *entry;

   //1. setting notify up
   notify = elm_notify_add(obj);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_notify_orient_set(notify, ELM_NOTIFY_ORIENT_BOTTOM);
   elm_notify_timeout_set(notify, time);

   //2. start forming layout
   layout = elm_layout_add(notify);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "notify/content/info");

   //2.1. add an image to the layout
   image = elm_image_add(obj);
   elm_image_file_set(image, TET_IMG_PATH"info.png", NULL);
   elm_object_part_content_set (layout, "image", image);
   evas_object_show(image);

   //2.2 add not editable entry for information viewable
   entry = elm_entry_add(obj);
   elm_entry_editable_set(entry, EINA_FALSE);
   elm_entry_scrollbar_policy_set(entry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set (layout, "information", entry);
   elm_object_text_set(entry, message);
   evas_object_show(entry);

   //2.4. show this layout with everything
   evas_object_show (layout);
   elm_object_content_set(notify, layout);
   evas_object_show (notify);
}
