#include "live_elementary_widgets.h"

static void
_on_scroller_swallow_check(void *data __UNUSED__,
                           Evas_Object *obj,
                           void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *content, *bt;

   if (!strcmp(part->name, "elm.swallow.content"))
     {
        content = elm_table_add(obj);
        int i, j;

        for (j = 0; j < ELEMENTS_MID_COUNT; j++)
          {
             for (i = 0; i < ELEMENTS_MID_COUNT; i++)
               {
                  bt = object_generate(part, content);
                  if (bt)
                    {
                       evas_object_color_set(part->object,
                                             part->r,
                                             part->g,
                                             part->b,
                                             part->a);

                       evas_object_size_hint_min_set(part->object,
                                                     part->min_w,
                                                     part->min_h);
                       evas_object_size_hint_max_set(part->object,
                                                     part->max_w,
                                                     part->max_h);
                    }
                  else
                    return;
                  elm_table_pack(content, bt, i, j, 1, 1);
                  evas_object_show(bt);
               }
          }
        elm_object_content_set(obj, content);
        part->object = content;
        evas_object_show(content);
     }
   else
     {
        bt = object_generate(part, obj);
        if (bt)
          {
             evas_object_color_set(part->object,
                                   part->r,
                                   part->g,
                                   part->b,
                                   part->a);

             evas_object_size_hint_min_set(part->object,
                                           part->min_w,
                                           part->min_h);
             evas_object_size_hint_max_set(part->object,
                                           part->max_w,
                                           part->max_h);
          }
        elm_object_part_content_set(obj, part->name, part->object);
     }
}

Evas_Object *
widget_scroller_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   Evas_Object *object;

   if ((strcmp(group->class, "entry") == 0) || (strcmp(group->class, "entry_single") == 0))
     {
        object = elm_entry_add(parent);
        evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
        elm_entry_scrollable_set(object, true);
        if (strcmp(group->class, "entry_single") == 0)
          elm_entry_single_line_set(object, true);
     }
   else
     {
        object = elm_scroller_add(parent);
        evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, _on_scroller_swallow_check, NULL);
     }
   elm_object_style_set(object, group->style);

   elm_scroller_policy_set(object, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
