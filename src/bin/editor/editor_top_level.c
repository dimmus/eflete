#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "editor.h"
#include "editor_macro.h"

extern int _editor_signals_blocked;

Eina_Bool
editor_image_add(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_image_add(obj, name))
     return false;

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_image_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_image_del(obj, name));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_DELETED, (void *)name);
   return true;
}

Eina_Bool
editor_image_set_add(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_image_set_add(obj, name))
     return false;

   if (!editor_save_all(obj))
     return false;
   _editor_project_changed();

   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_SET_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_image_set_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_image_set_del(obj, name))
     return false;

   if (!editor_save_all(obj))
     return false;
   _editor_project_changed();

   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_SET_DELETED, (void *)name);
   return true;
}

Eina_Bool
editor_image_set_image_add(Evas_Object *obj, const char *image_set_name, const char *image, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(image_set_name != NULL);
   assert(image != NULL);

   Image_Set_Change send;

   if (!edje_edit_image_set_image_add(obj, image_set_name, image))
     return false;

   if (!editor_save_all(obj))
     return false;
   _editor_project_changed();

   if (notify)
     {
        send.set_name = eina_stringshare_add(image_set_name);
        send.image_name = eina_stringshare_add(image);
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_SET_IMAGE_ADD, &send);
        eina_stringshare_del(send.set_name);
        eina_stringshare_del(send.image_name);
     }

   return true;
}

Eina_Bool
editor_image_set_image_del(Evas_Object *obj, const char *image_set_name, const char *image_name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(image_set_name != NULL);

   Image_Set_Change send;
   Eina_List *images = NULL, *l = NULL;
   int pos = 0;
   const char *name = NULL;

   images = edje_edit_image_set_images_list_get(obj, image_set_name);
   EINA_LIST_FOREACH(images, l, name)
     {
        if (!strcmp(image_name, name))
          break;
        pos++;
     }


   if (!edje_edit_image_set_image_del(obj, image_set_name, pos))
     return false;

   if (!editor_save_all(obj))
     return false;
   _editor_project_changed();

   if (notify)
     {
        send.set_name = eina_stringshare_add(image_set_name);
        send.image_position = pos;
        send.image_name = eina_stringshare_add(image_name);
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_SET_IMAGE_DEL, &send);
        eina_stringshare_del(send.set_name);
        eina_stringshare_del(send.image_name);
     }

   return true;
}

inline static Eina_Bool
_image_set_image_attribute_save(Evas_Object *obj, Editor_Attribute_Resource_Change *send)
{
   if (!editor_save_all(obj))
     {
        eina_stringshare_del(send->image_set_name);
        return false;
     }
   _editor_project_changed();

   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, send);
   eina_stringshare_del(send->image_set_name);

   return true;
}

Eina_Bool
editor_image_set_image_border_left_set(Evas_Object *obj, const char *name, int position, int l)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_BORDER_LEFT;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   send.left = l;

   edje_edit_image_set_image_border_get(obj, name, position,
                                        NULL, &send.right, &send.top, &send.bottom);

   CRIT_ON_FAIL(edje_edit_image_set_image_border_set(obj, name, position,
                                                     send.left, send.right, send.top, send.bottom));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_border_right_set(Evas_Object *obj, const char *name, int position, int r)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_BORDER_RIGHT;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   send.right = r;

   edje_edit_image_set_image_border_get(obj, name, position,
                                        &send.left, NULL, &send.top, &send.bottom);

   CRIT_ON_FAIL(edje_edit_image_set_image_border_set(obj, name, position,
                                                     send.left, send.right, send.top, send.bottom));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_border_top_set(Evas_Object *obj, const char *name, int position, int t)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_BORDER_TOP;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   send.top = t;

   edje_edit_image_set_image_border_get(obj, name, position,
                                        &send.left, &send.right, NULL, &send.bottom);

   CRIT_ON_FAIL(edje_edit_image_set_image_border_set(obj, name, position,
                                                     send.left, send.right, send.top, send.bottom));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_border_bottom_set(Evas_Object *obj, const char *name, int position, int b)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_BORDER_BOTTOM;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   send.bottom = b;

   edje_edit_image_set_image_border_get(obj, name, position,
                                        &send.left, &send.right, &send.top, NULL);

   CRIT_ON_FAIL(edje_edit_image_set_image_border_set(obj, name, position,
                                                     send.left, send.right, send.top, send.bottom));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_min_width_set(Evas_Object *obj, const char *name, int position, int min_w)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_MIN_W;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   int min_h = 0;

   edje_edit_image_set_image_min_get(obj, name, position, NULL, &min_h);

   CRIT_ON_FAIL(edje_edit_image_set_image_min_set(obj, name, position, min_w, min_h));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_min_height_set(Evas_Object *obj, const char *name, int position, int min_h)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_MIN_H;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   int min_w = 0;

   edje_edit_image_set_image_min_get(obj, name, position, &min_w, NULL);

   CRIT_ON_FAIL(edje_edit_image_set_image_min_set(obj, name, position, min_w, min_h));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_max_width_set(Evas_Object *obj, const char *name, int position, int max_w)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_MAX_W;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   int max_h = 0;

   edje_edit_image_set_image_max_get(obj, name, position, NULL, &max_h);

   CRIT_ON_FAIL(edje_edit_image_set_image_max_set(obj, name, position, max_w, max_h));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_max_height_set(Evas_Object *obj, const char *name, int position, int max_h)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_MAX_H;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   int max_w = 0;

   edje_edit_image_set_image_max_get(obj, name, position, &max_w, NULL);

   CRIT_ON_FAIL(edje_edit_image_set_image_max_set(obj, name, position, max_w, max_h));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_image_set_image_border_scale_set(Evas_Object *obj, const char *name, int position, double scale)
{
   assert (obj != NULL);
   assert (name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_IMAGE_SET_IMAGE_BORDER_SCALE;
   send.image_set_name = eina_stringshare_add(name);
   send.image_pos = position;

   CRIT_ON_FAIL(edje_edit_image_set_image_border_scale_set(obj, name, position, scale));

   return _image_set_image_attribute_save(obj, &send);
}

Eina_Bool
editor_color_class_add(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_color_class_add(obj, name))
     return false;

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_COLORCLASS_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_color_class_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_color_class_del(obj, name));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_COLORCLASS_DELETED, (void *)name);
   return true;
}

Eina_Bool
editor_color_class_description_set(Evas_Object *obj, const char *name, const char *description)
{
   assert(obj != NULL);
   assert(name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_COLORCLASS_DESCRIPTION;
   send.color_class_name = eina_stringshare_add(name);
   send.color_class_description = eina_stringshare_add(description);

   CRIT_ON_FAIL(edje_edit_color_class_description_set(obj, name, description));

   if (!editor_save_all(obj))
     {
        eina_stringshare_del(send.color_class_name);
        eina_stringshare_del(send.color_class_description);
        return false; /* i hope it will never happen */
     }
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, &send);
   eina_stringshare_del(send.color_class_name);
   eina_stringshare_del(send.color_class_description);
   return true;
}

Eina_Bool
editor_color_class_colors_set(Evas_Object *obj, const char *name,
                              int r, int g, int b, int a,
                              int r2, int g2, int b2, int a2,
                              int r3, int g3, int b3, int a3)
{
   assert(obj != NULL);
   assert(name != NULL);

   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_COLORCLASS_COLORS;
   send.color_class_name = eina_stringshare_add(name);
   send.r = r; send.r2 = r2; send.r3 = r3;
   send.g = g; send.g2 = g2; send.g3 = g3;
   send.b = b; send.b2 = b2; send.b3 = b3;
   send.a = a; send.a2 = a2; send.a3 = a3;

   CRIT_ON_FAIL(edje_edit_color_class_colors_set(obj, name,
                                                 r, g, b, a,
                                                 r2, g2, b2, a2,
                                                 r3, g3, b3, a3));

   if (!editor_save_all(obj))
     {
        eina_stringshare_del(send.color_class_name);
        return false; /* i hope it will never happen */
     }
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, &send);
   eina_stringshare_del(send.color_class_name);
   return true;
}

Eina_Bool
editor_sound_sample_add(Evas_Object *obj, const char *name, const char *source, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_sound_sample_add(obj, name, source))
     return false;

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_SOUND_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_sound_sample_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_sound_sample_del(obj, name));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_SOUND_DELETED, (void *)name);
   return true;
}

Eina_Bool
editor_sound_tone_add(Evas_Object *obj, const char *name, int frq, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_sound_tone_add(obj, name, frq));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_TONE_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_sound_tone_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_sound_tone_del(obj, name));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_TONE_DELETED, (void *)name);
   return true;
}

Eina_Bool
editor_style_add(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_style_add(obj, name));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_STYLE_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_style_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_style_del(obj, name));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_STYLE_DELETED, (void *)name);
   return true;
}

Eina_Bool
editor_style_tag_add(Evas_Object *obj, const char *name, const char *tag)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_style_tag_add(obj, name, tag));
   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_STYLE_TAG_ADDED;
   send.tag_name = eina_stringshare_add(tag);
   send.style_name = eina_stringshare_add(name);

   if (!editor_save_all(obj))
     {
        eina_stringshare_del(send.style_name);
        eina_stringshare_del(send.tag_name);
        return false; /* i hope it will never happen */
     }
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, &send);
   eina_stringshare_del(send.style_name);
   eina_stringshare_del(send.tag_name);
   return true;
}

Eina_Bool
editor_style_tag_del(Evas_Object *obj, const char *name, const char *tag)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_style_tag_del(obj, name, tag));
   Editor_Attribute_Resource_Change send;
   send.attribute = RM_ATTRIBUTE_RESOURCES_STYLE_TAG_DELETED;
   send.tag_name = eina_stringshare_add(tag);
   send.style_name = eina_stringshare_add(name);

   if (!editor_save_all(obj))
     {
        eina_stringshare_del(send.style_name);
        eina_stringshare_del(send.tag_name);
        return false; /* i hope it will never happen */
     }
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, &send);

   eina_stringshare_del(send.style_name);
   eina_stringshare_del(send.tag_name);
   return true;
}

Eina_Bool
editor_style_tag_value_set(Evas_Object *obj, const char *name, const char *tag, const char *value)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_style_tag_value_set(obj, name, tag, value));

   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   return true;
}
